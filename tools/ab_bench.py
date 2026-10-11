#!/usr/bin/env python3
"""A/B performance runner for vxR acceleration structures.

For every scene it renders with backend A and backend B (vxR --accel <name>),
with the same seed and thread count, collects the --stats-out JSON of each run,
compares the images with vxr_imgdiff and prints one comparison block per scene.
Python 3 standard library only.

Examples (from the repo root, after a Release build):

  python3 tools/ab_bench.py                         # broadphase vs broadphase, default scenes
  python3 tools/ab_bench.py --quick                 # small images, a few seconds per scene
  python3 tools/ab_bench.py --a broadphase --b bvh --runs 5 --res 640x360 --samples 16
  python3 tools/ab_bench.py --scenes scenes/ab_grid.vx --bin-b /path/to/other/vxR

Runs are interleaved (A B, B A, A B ...) so slow drift of the machine does not
favour one side. The first --warmup rounds are rendered and thrown away.
Timing comes from the renderer itself: render_wall is the time from the start of
the render to the end of its last thread. It does not include the 1 second
polling sleep of RenderProcess::execute, scene parsing or image encoding.

Exit status is non-zero when a run fails, a run's own sanity checks fail, the
same backend gives different images or counters for the same seed, or the A and
B images differ by more than --max-mae.
"""

import argparse
import csv
import datetime
import itertools
import json
import math
import os
import re
import shlex
import statistics
import subprocess
import sys
import time
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent

DEFAULT_SCENES = [
    "scenes/ab_grid.vx",
    "scenes/ab_prims.vx",
    "scenes/bg_dof.vx",
    "scenes/bg_statue.vx",
    "scenes/bg_panther.vx",
    "scenes/juan_kloofendal.vx",
]

# Counters shown side by side, in this order, when non zero on either side.
COUNTER_ORDER = [
    "ray_primary", "ray_reflection", "ray_gi", "ray_shadow", "ray_env_light",
    "accel_casts", "cast_hits", "cast_misses",
    "accel_node_visits", "accel_light_skips", "accel_box_tests",
    "accel_box_rejects", "accel_prim_tests", "accel_prim_hits",
    "tri_tests", "tri_backface_rejects", "sphere_tests", "cylinder_tests",
    "plane_tests", "grid_casts", "grid_voxel_steps", "grid_voxel_box_tests",
    "grid_skips_l1", "grid_skips_l2",
    "mesh_casts", "mesh_bucket_lookups", "mesh_bvh_node_visits", "dome_lookups",
]

DERIVED_ORDER = [
    "tests_per_ray", "box_tests_per_ray", "prim_tests_per_ray",
    "node_visits_per_ray", "voxel_steps_per_ray", "bucket_lookups_per_ray",
    "hit_ratio", "rays_per_sec",
]

TIMING_ORDER = ["scene_build", "accel_build", "render_wall", "image_encode"]

MIN_RELIABLE_RENDER_SEC = 0.2


def quote(cmd):
    return " ".join(shlex.quote(str(c)) for c in cmd)


def fmt(value, digits=4):
    if value is None:
        return "-"
    if isinstance(value, int):
        return f"{value:,}"
    if abs(value) >= 1000:
        return f"{value:,.0f}"
    return f"{value:.{digits}f}"


def scene_inputs(scene):
    """Files a scene refers to, resolved like FileUtils::resolve does."""
    text = scene.read_text(errors="replace")
    inputs = []
    for key, value in re.findall(r'\b(imagePath|filePath)\s*=\s*"([^"]*)"', text):
        path = Path(value) if value.startswith("/") else Path(str(scene.parent) + "/" + value)
        inputs.append(path)
    return inputs


class Runner:
    def __init__(self, args, out_dir):
        self.args = args
        self.out_dir = out_dir
        self.commands = []

    def render(self, binary, scene, label, backend, index, warmup):
        """One render. Returns a dict with the parsed JSON or an error."""
        a = self.args
        tag = f"{scene.stem}__{label}__{'warmup' if warmup else 'run'}{index}"
        run_dir = self.out_dir / "runs" / scene.stem / f"{label}_{'warmup' if warmup else 'run'}{index}"
        run_dir.mkdir(parents=True, exist_ok=True)
        json_path = self.out_dir / "json" / f"{tag}.json"
        image = run_dir / "image.jpg"
        log = run_dir / "run.log"

        cmd = [str(binary), "-r", "-scene", str(scene), "--accel", backend,
               "--seed", str(a.seed), "--threads", str(a.threads),
               "--stats-out", str(json_path), "--out", str(image)]
        if not a.no_quiet:
            cmd.append("--quiet")
        if a.res:
            cmd += ["--res", a.res]
        if a.samples:
            cmd += ["--samples", str(a.samples)]

        extra_env = a.env_a if label == "A" else a.env_b
        env = os.environ.copy()
        env_prefix = []
        for item in extra_env:
            key, _, value = item.partition("=")
            if not key:
                continue
            env[key] = value
            env_prefix.append(f"{key}={value}")

        shown = ((" ".join(env_prefix) + " ") if env_prefix else "") + quote(cmd)
        line = f"(cd {shlex.quote(str(run_dir))} && {shown} > run.log 2>&1)"
        self.commands.append(line)
        print(f"    $ {line}", flush=True)

        started = time.monotonic()
        try:
            with open(log, "w") as fh:
                proc = subprocess.run(cmd, cwd=run_dir, stdout=fh, stderr=subprocess.STDOUT,
                                      timeout=a.timeout, env=env)
            rc = proc.returncode
        except subprocess.TimeoutExpired:
            rc = -1
        wall = time.monotonic() - started

        result = {"tag": tag, "label": label, "backend": backend, "index": index,
                  "warmup": warmup, "rc": rc, "process_wall": wall,
                  "image": image, "json_path": json_path, "log": log, "stats": None,
                  "error": None}
        if rc != 0:
            result["error"] = f"exit code {rc}, see {log}"
            return result
        try:
            result["stats"] = json.loads(json_path.read_text())
        except (OSError, ValueError) as exc:
            result["error"] = f"no valid stats JSON ({exc}), see {log}"
        return result

    def imgdiff(self, a_image, b_image):
        cmd = [str(self.args.imgdiff), str(a_image), str(b_image), "--json",
               "--max-mae", str(self.args.max_mae)]
        self.commands.append(quote(cmd))
        print(f"    $ {quote(cmd)}", flush=True)
        proc = subprocess.run(cmd, capture_output=True, text=True)
        try:
            return json.loads(proc.stdout), proc.returncode
        except ValueError:
            return {"error": (proc.stderr or proc.stdout).strip()}, proc.returncode


def summarize(values):
    return {"median": statistics.median(values), "min": min(values), "max": max(values),
            "spread": (max(values) - min(values)) / statistics.median(values)
            if statistics.median(values) > 0 else 0.0}


def mann_whitney_p(a, b):
    """Two sided Mann-Whitney U test, exact for up to 20 runs in total.

    With n runs per side the smallest p it can give is 2 / C(2n, n): 0.1 for
    3 runs, 0.029 for 4. Fewer runs than that cannot show a difference.
    """
    n, m = len(a), len(b)
    pooled = sorted(a + b)
    # Average rank of each value, so ties are handled.
    rank = {v: sum(i + 1 for i, x in enumerate(pooled) if x == v) / pooled.count(v)
            for v in set(pooled)}
    ranks = [rank[v] for v in a + b]
    mean_u = n * m / 2.0
    u_obs = sum(ranks[:n]) - n * (n + 1) / 2.0
    if n + m <= 20:
        extreme = total = 0
        for idx in itertools.combinations(range(n + m), n):
            u = sum(ranks[i] for i in idx) - n * (n + 1) / 2.0
            total += 1
            extreme += abs(u - mean_u) >= abs(u_obs - mean_u) - 1e-9
        return extreme / total
    sigma = math.sqrt(n * m * (n + m + 1) / 12.0)
    z = (abs(u_obs - mean_u) - 0.5) / sigma
    return math.erfc(max(z, 0.0) / math.sqrt(2.0))


def counters_equal(runs):
    first = runs[0]["stats"]["counters"]
    return all(r["stats"]["counters"] == first for r in runs[1:])


def hashes_equal(runs):
    first = runs[0]["stats"]["image"]["jpeg_fnv1a64"]
    return all(r["stats"]["image"]["jpeg_fnv1a64"] == first for r in runs[1:])


def ratio_text(a, b, digits=3):
    """a/b, or - when either side is too small to divide meaningfully."""
    return f"{a / b:.{digits}f}x" if min(a, b) > 1e-4 else "-"


def side_row(title, a_text, b_text, ratio_text="", width=(34, 22, 22)):
    return f"  {title:<{width[0]}}{a_text:>{width[1]}}{b_text:>{width[2]}}  {ratio_text}"


def compare_scene(runner, scene, binary_a, binary_b, problems, csv_rows, summary_rows):
    a = runner.args
    print(f"\n=== {scene.relative_to(REPO) if scene.is_relative_to(REPO) else scene}")
    runs = {"A": [], "B": []}
    names = {"A": a.a, "B": a.b}
    binaries = {"A": binary_a, "B": binary_b}

    for r in range(a.warmup + a.runs):
        warmup = r < a.warmup
        index = r + 1 if warmup else r - a.warmup + 1
        order = ["A", "B"] if r % 2 == 0 else ["B", "A"]
        for label in order:
            res = runner.render(binaries[label], scene, label, names[label], index, warmup)
            if res["error"]:
                problems.append(f"{scene.name} {label}: {res['error']}")
                print(f"    ! {res['error']}")
            if not warmup:
                runs[label].append(res)
            csv_rows.append(res)

    for label in "AB":
        runs[label] = [r for r in runs[label] if r["stats"]]
    if not runs["A"] or not runs["B"]:
        problems.append(f"{scene.name}: no successful runs on one side")
        return

    # What each side must give for a fixed seed.
    for label in "AB":
        if not hashes_equal(runs[label]):
            problems.append(f"{scene.name} {label}: same seed gave different images")
        if not counters_equal(runs[label]):
            problems.append(f"{scene.name} {label}: same seed gave different counters")
        for r in runs[label]:
            checks = r["stats"]["checks"]
            if not checks["all_ok"]:
                problems.append(f"{scene.name} {label} run{r['index']}: sanity checks failed: "
                                f"{checks['failures']}")

    first_a, first_b = runs["A"][0], runs["B"][0]
    cfg = first_a["stats"]["config"]
    print(f"\n  {cfg['width']}x{cfg['height']}  samples {cfg['pixel_samples']}  depth {cfg['ray_depth']}"
          f"  seed {cfg['seed']}  threads {cfg['threads']}  {len(runs['A'])} runs + {a.warmup} warm-up")
    for label in "AB":
        git = runs[label][0]["stats"]["git"]
        build = runs[label][0]["stats"]["build"]
        print(f"  {label}: {names[label]}  build {build['type']}  git "
              f"{git['commit'][:10] or '?'}{'+dirty' if git['dirty'] else ''}")

    # Timings.
    times = {l: [r["stats"]["timings_sec"]["render_wall"] for r in runs[l]] for l in "AB"}
    sa, sb = summarize(times["A"]), summarize(times["B"])
    speedup = sa["median"] / sb["median"] if sb["median"] > 0 else float("inf")
    # Run-to-run time is not normal (macOS puts threads on fast or slow cores,
    # so it can be bimodal), so use a rank test and also ask for 2%.
    p_value = mann_whitney_p(times["A"], times["B"])
    n_runs = len(times["A"])
    if p_value < 0.05 and abs(speedup - 1.0) > 0.02:
        who, pct = ("B", speedup - 1.0) if speedup > 1.0 else ("A", 1.0 / speedup - 1.0)
        verdict = f"{who} faster by {pct * 100:.1f}% (Mann-Whitney p={p_value:.3f})"
    else:
        verdict = (f"not distinguishable: median ratio {speedup:.3f}x, "
                   f"Mann-Whitney p={p_value:.3f} with {n_runs} runs per side")
        if n_runs < 4:
            verdict += " (3 runs can never reach p<0.05, use --runs 5)"
    if a.a == a.b and binary_a == binary_b:
        verdict += "  (identical backends: this is run-to-run noise)"

    print()
    print(side_row("", f"A {names['A']}", f"B {names['B']}", "A/B"))
    print(side_row("render_wall median (s)", fmt(sa["median"]), fmt(sb["median"]),
                   f"speedup {speedup:.3f}x"))
    print(side_row("render_wall min (s)", fmt(sa["min"]), fmt(sb["min"]),
                   ratio_text(sa["min"], sb["min"])))
    print(side_row("render_wall max (s)", fmt(sa["max"]), fmt(sb["max"]),
                   ratio_text(sa["max"], sb["max"])))
    print(side_row("spread (max-min)/median", f"{sa['spread'] * 100:.1f}%", f"{sb['spread'] * 100:.1f}%"))
    for label in ("A", "B"):
        print(f"  render_wall every run, {label} (s): " + " ".join(f"{t:.4f}" for t in times[label]))
    print(f"  verdict: {verdict}")
    if min(sa["median"], sb["median"]) < MIN_RELIABLE_RENDER_SEC:
        print(f"  ! render under {MIN_RELIABLE_RENDER_SEC}s: too short to time reliably, "
              f"use --res / --samples (see --quick)")

    for key in TIMING_ORDER:
        if key == "render_wall":
            continue
        va = statistics.median(r["stats"]["timings_sec"][key] for r in runs["A"])
        vb = statistics.median(r["stats"]["timings_sec"][key] for r in runs["B"])
        print(side_row(f"{key} median (s)", fmt(va), fmt(vb), ratio_text(va, vb)))
    rss = {l: max(r["stats"]["process"]["peak_rss_bytes"] for r in runs[l]) / 2**20 for l in "AB"}
    print(side_row("peak RSS (MiB, whole process)", f"{rss['A']:.0f}", f"{rss['B']:.0f}"))

    # Counters (identical across runs of one side, checked above).
    ca, cb = first_a["stats"]["counters"], first_b["stats"]["counters"]
    print()
    print(side_row("counters", "A", "B", "B/A"))
    for key in COUNTER_ORDER:
        if ca.get(key, 0) or cb.get(key, 0):
            va, vb = ca.get(key, 0), cb.get(key, 0)
            ratio = f"{vb / va:.3f}" if va else ("-" if not vb else "inf")
            print(side_row(key, fmt(va), fmt(vb), ratio))
    da, db = first_a["stats"]["derived"], first_b["stats"]["derived"]
    print()
    print(side_row("derived", "A", "B", "B/A"))
    for key in DERIVED_ORDER:
        va, vb = da[key], db[key]
        ratio = f"{vb / va:.3f}" if va else "-"
        print(side_row(key, fmt(va, 3), fmt(vb, 3), ratio))

    # Images.
    print()
    diff, rc = runner.imgdiff(first_a["image"], first_b["image"])
    if "error" in diff:
        problems.append(f"{scene.name}: image diff failed: {diff['error']}")
        print(f"  image diff: ERROR {diff['error']}")
    else:
        psnr = "inf" if diff["psnr"] is None else f"{diff['psnr']:.2f} dB"
        print(f"  image diff A vs B: {'IDENTICAL' if diff['identical'] else 'DIFFERENT'}  "
              f"mae {diff['mae']:.6f}  rmse {diff['rmse']:.6f}  psnr {psnr}  "
              f"max channel diff {diff['max_diff']}  differing pixels {diff['differing_pixels']}"
              f" of {diff['pixels']}")
        if rc != 0:
            problems.append(f"{scene.name}: A and B images differ (mae {diff['mae']:.6f} "
                            f"> --max-mae {a.max_mae})")
    for label in "AB":
        print(f"  jpeg fnv1a64 {label}: {runs[label][0]['stats']['image']['jpeg_fnv1a64']}"
              f"  same on all {len(runs[label])} runs: {hashes_equal(runs[label])}")

    for label in "AB":
        s = summarize(times[label])
        st = runs[label][0]["stats"]
        summary_rows.append({
            "scene": scene.stem, "label": label, "backend": names[label],
            "runs": len(runs[label]), "render_wall_median": s["median"],
            "render_wall_min": s["min"], "render_wall_max": s["max"],
            "spread": s["spread"], "tests_per_ray": st["derived"]["tests_per_ray"],
            "rays_total": st["derived"]["rays_total"],
            "rays_per_sec_median": statistics.median(
                r["stats"]["derived"]["rays_per_sec"] for r in runs[label]),
            "speedup_a_over_b_median": speedup,
            "image_mae_a_vs_b": diff.get("mae"), "image_identical_a_vs_b": diff.get("identical"),
        })


def write_csv(out_dir, csv_rows, summary_rows):
    counters = COUNTER_ORDER
    with open(out_dir / "results.csv", "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["scene", "label", "backend", "warmup", "run", "rc", "process_wall_s"] +
                   [f"t_{k}" for k in ["scene_parse", "scene_build", "accel_build",
                                       "render_wall", "render_execute", "image_convert",
                                       "image_encode", "total"]] +
                   counters + [f"d_{k}" for k in DERIVED_ORDER] +
                   ["checks_ok", "jpeg_fnv1a64", "rgba8_fnv1a64", "json"])
        for r in csv_rows:
            st = r["stats"]
            row = [r["tag"].split("__")[0], r["label"], r["backend"], int(r["warmup"]),
                   r["index"], r["rc"], f"{r['process_wall']:.4f}"]
            if st:
                row += [st["timings_sec"][k] for k in ["scene_parse", "scene_build", "accel_build",
                                                       "render_wall", "render_execute",
                                                       "image_convert", "image_encode", "total"]]
                row += [st["counters"][k] for k in counters]
                row += [st["derived"][k] for k in DERIVED_ORDER]
                row += [int(st["checks"]["all_ok"]), st["image"]["jpeg_fnv1a64"],
                        st["image"]["rgba8_fnv1a64"], r["json_path"]]
            w.writerow(row)
    if summary_rows:
        with open(out_dir / "summary.csv", "w", newline="") as fh:
            w = csv.DictWriter(fh, fieldnames=list(summary_rows[0].keys()))
            w.writeheader()
            w.writerows(summary_rows)


def main():
    cpu = os.cpu_count() or 1
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--bin", type=Path, default=REPO / "build/release/vxR", help="renderer for A (and B)")
    p.add_argument("--bin-b", type=Path, default=None, help="renderer for B when it is another build")
    p.add_argument("--imgdiff", type=Path, default=REPO / "build/release/vxr_imgdiff")
    p.add_argument("--scenes", nargs="+", type=Path, default=[Path(s) for s in DEFAULT_SCENES],
                   help="scene files, relative paths are taken from the repo root")
    p.add_argument("--a", default="broadphase", help="backend A (default broadphase)")
    p.add_argument("--b", default="broadphase", help="backend B (default broadphase)")
    p.add_argument("--runs", type=int, default=3, help="measured runs per backend (default 3)")
    p.add_argument("--warmup", type=int, default=1, help="discarded rounds per backend (default 1)")
    p.add_argument("--seed", type=int, default=12345)
    p.add_argument("--threads", type=int, default=min(6, cpu), help="default min(6, cores)")
    p.add_argument("--res", help="override resolution, WxH")
    p.add_argument("--samples", type=int, help="override pixelSamples")
    p.add_argument("--quick", action="store_true", help="shorthand for --res 320x180 --samples 8")
    p.add_argument("--max-mae", type=float, default=0.0,
                   help="largest mean abs error accepted between A and B images (default 0)")
    p.add_argument("--timeout", type=float, default=3600, help="seconds per render")
    p.add_argument("--no-quiet", action="store_true",
                   help="do not pass --quiet (the per ray debug print then dominates the time)")
    p.add_argument("--env-a", action="append", default=[], metavar="KEY=VAL",
                   help="extra environment for backend A (repeatable, e.g. VXR_MESH=ref)")
    p.add_argument("--env-b", action="append", default=[], metavar="KEY=VAL",
                   help="extra environment for backend B (repeatable, e.g. VXR_MESH=csr)")
    p.add_argument("--out-dir", type=Path, default=None,
                   help="default /tmp/vxr-out/ab/<timestamp>")
    args = p.parse_args()

    if args.quick:
        args.res = args.res or "320x180"
        args.samples = args.samples or 8
    stamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    out_dir = args.out_dir or Path("/tmp/vxr-out/ab") / stamp
    (out_dir / "json").mkdir(parents=True, exist_ok=True)

    binary_a = args.bin
    binary_b = args.bin_b or args.bin
    for b in {binary_a, binary_b, args.imgdiff}:
        if not b.exists():
            sys.exit(f"missing {b}\nbuild it with: cmake -S {REPO} -B {REPO}/build/release -G Ninja "
                     f"-DCMAKE_BUILD_TYPE=Release && cmake --build {REPO}/build/release")

    runner = Runner(args, out_dir)
    problems, csv_rows, summary_rows, skipped = [], [], [], []
    print(f"output directory: {out_dir}")
    print(f"A = {args.a} ({binary_a})   B = {args.b} ({binary_b})")
    if args.env_a:
        print(f"env A: {' '.join(args.env_a)}")
    if args.env_b:
        print(f"env B: {' '.join(args.env_b)}")

    for scene in args.scenes:
        scene = scene if scene.is_absolute() else REPO / scene
        if not scene.exists():
            skipped.append((scene, "scene file not found"))
            print(f"\n=== SKIP {scene}: scene file not found")
            continue
        missing = [str(f) for f in scene_inputs(scene) if not f.exists()]
        if missing:
            skipped.append((scene, "missing input " + ", ".join(missing)))
            print(f"\n=== SKIP {scene.name}: missing input {', '.join(missing)}")
            continue
        compare_scene(runner, scene, binary_a, binary_b, problems, csv_rows, summary_rows)

    (out_dir / "commands.txt").write_text("\n".join(runner.commands) + "\n")
    write_csv(out_dir, csv_rows, summary_rows)

    print(f"\nCSV: {out_dir / 'results.csv'}  {out_dir / 'summary.csv'}")
    print(f"raw JSON: {out_dir / 'json'}   commands: {out_dir / 'commands.txt'}")
    for scene, why in skipped:
        print(f"skipped {scene.name}: {why}")
    if not summary_rows:
        problems.append("no scene was rendered")
    if problems:
        print("\nPROBLEMS:")
        for line in problems:
            print(f"  ! {line}")
        return 1
    print("\nall runs OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
