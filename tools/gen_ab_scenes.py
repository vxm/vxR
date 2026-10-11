#!/usr/bin/env python3
"""Generates the stress scenes of the acceleration structure A/B tests.

  scenes/ab_spheres.vx   about 1000 random spheres, a floor, a dome and a sun.
                         A scene where a flat per-visible loop does 1000 box
                         tests per ray and a tree does a handful.
  scenes/ab_grid_big.vx  scenes/ab_grid.vx at resolution 474: a thin ground of
                         voxels with a sparse layer of random ones, and a lot of
                         empty space above it. Made to exercise the voxel grid
                         occupancy hierarchy.

Usage (from the repo root; the output is deterministic):

  python3 tools/gen_ab_scenes.py [--spheres 1000] [--seed 7] [--grid-res 474]
"""

import argparse
import random
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent

HEADER = """#vxR scene
#format ascii 1.0

node
{{
name = "time"
type = "vxClock"
active = "true"
    start = 0.0
    end = 1.0
    step = 1.0
}}

node
{{
name = "renderSettings"
type = "vxRenderSettings"
active = "true"
    resolution = 320 180
    pixelSamples = 4
    rayDepth = 2
    giMultiplier = 1.0
    numThreads = -1
}}

node
{{
name = "persp"
type = "vxCamera"
active = "true"
    transform = [1 0 0 0 0 1 0 0 0 0 1 0 {cam} 1]
    focusDistance = 6.0
    lensSize = 0.0
    horizontalAperture = 1.27
    verticalAperture = 1.7786
    focalLength = 4.5
    pixelRadius = 0.0
}}

node
{{
name = "image_1"
type = "vxImage"
active = "true"
    imagePath = "../images/image.0001403.0000.jpg"
    gain = 1.0
    gamma = 1.0
}}
"""

SHADER = """
node
{{
name = "{name}"
type = "vxShader"
active = "true"
    diffuseColor = {color}
    diffuseCoeficent = 0.85
    giCoeficent = 0.2
    giColorMultiplier = 255 255 255
    rayDepth = 1
    reflectionRadius = 0.0
    reflectionCoefficent = 0.08
    reflectionColorMultiplier = 255 255 255
    refractionRadius = 0.0
    refractionSamples = 0
    refractionCoefficent = 0.0
    refractionColorMultiplier = 255 255 255
    sscSamples = 0
    sscRadius = 0.0
    sscCoefficent = 0.0
    sscColorMultiplier = 255 255 255
}}
"""

SPHERE = """
node
{{
name = "sphere_{i}"
type = "vxSphere"
active = "true"
    transform = [1 0 0 0 0 1 0 0 0 0 1 0 {x:.4f} {y:.4f} {z:.4f} 1]
    shader = "{shader}"
    color = {color}
    radius = {r:.4f}
}}
"""

FLOOR = """
node
{
name = "floor"
type = "vxPlane"
active = "true"
    transform = [1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1]
    planeType = "Y"
    x = 0.0
    y = -0.62
    z = 0.0
    pointA = 440.0 -57.0 440.0
    pointB = 450.0 -56.0 440.0
    pointC = 440.0 -56.0 450.0
    color = 180 186 192
}
"""

LIGHTING = """
node
{
name = "dome"
type = "vxDome"
active = "true"
    transform = [1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1]
    imageNode = "image_1"
    lightImageNode = "image_1"
    radius = 1000.0
}

node
{
name = "sun"
type = "vxDirectLight"
active = "true"
    transform = [1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1]
    intensity = 1.4
    color = 255 248 235
    orientation = -0.3 -1.0 0.5
    samples = 1
    castShadows = "true"
}
"""

PALETTE = [
    ("80 140 170", "shader_a"),
    ("190 170 90", "shader_b"),
    ("170 90 90", "shader_c"),
    ("110 170 110", "shader_d"),
]


def spheres_scene(count, seed):
    rng = random.Random(seed)
    out = [HEADER.format(cam="0.0 0.9 -6.5")]
    for color, name in PALETTE:
        out.append(SHADER.format(name=name, color=color))
    for i in range(count):
        color, shader = PALETTE[i % len(PALETTE)]
        r = rng.uniform(0.06, 0.16)
        out.append(
            SPHERE.format(
                i=i + 1,
                x=rng.uniform(-3.2, 3.2),
                y=rng.uniform(-0.62 + r, 1.8),
                z=rng.uniform(-2.5, 4.0),
                r=r,
                shader=shader,
                color=color,
            )
        )
    out.append(FLOOR)
    out.append(LIGHTING)
    return "".join(out)


def grid_big_scene(res):
    text = (REPO / "scenes" / "ab_grid.vx").read_text()
    old = "    resolution = 28\n"
    assert text.count(old) == 1, "unexpected ab_grid.vx"
    return text.replace(old, "    resolution = %d\n" % res)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--spheres", type=int, default=1000)
    ap.add_argument("--seed", type=int, default=7)
    ap.add_argument("--grid-res", type=int, default=474)
    args = ap.parse_args()

    scenes = REPO / "scenes"
    (scenes / "ab_spheres.vx").write_text(spheres_scene(args.spheres, args.seed))
    (scenes / "ab_grid_big.vx").write_text(grid_big_scene(args.grid_res))
    print("wrote scenes/ab_spheres.vx and scenes/ab_grid_big.vx")


if __name__ == "__main__":
    main()
