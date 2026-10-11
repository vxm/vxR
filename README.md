# vxR

vxR is an offline CPU ray tracer written in C++17. Geometry is traversed through a uniform voxel grid, and the same grid can hold solid voxels or meshes voxelized from PLY files. A scene is a text file of nodes: camera, lights, shaders, primitives, and render settings. The renderer shades each pixel with multiple samples and writes a JPEG.

An example render with depth of field: [sample image](https://drive.google.com/file/d/1jfJt2YT7y5JY8lYSBpWuf-h_S-4QZdX-/view?usp=sharing).

## What it renders

- **Voxel grid.** A uniform grid of active voxels is the main acceleration structure and a geometry type of its own. Resolution and size come from the scene.
- **Primitives.** Spheres, cylinders, and planes.
- **Environment dome.** An image mapped on a sphere. `imageNode` is the visible background. `lightImageNode` is what missed rays and global-illumination samples read. `intensity` scales that light image only. `lightClamp` caps the brightest channel of one light sample so an HDR sun disk does not burn a few random hits; `0` leaves the sample unclamped.
- **Meshes.** ASCII PLY files, either as triangle meshes or voxelized into a grid (`vxGridGeometry`).
- **Camera.** A perspective camera with focal length, film apertures, focus distance, and lens size for thin-lens depth of field.
- **Lights.** Area, point, sphere, sun, directional, ambient, and image-based lights, with shadow rays. A dome can light the scene on its own.
- **Shading.** Diffuse response, glossy reflection, and cosine-weighted hemisphere global illumination. Ray depth and samples per pixel are set in the scene. Shading is evaluated across a thread per column stride.
- **Output.** 8-bit JPEG (`image.0000001.jpg` in the working directory).

## Build

Requires CMake 3.16 or newer, a C++17 compiler, and zlib (OpenEXR is read with TinyEXR).

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Release builds compile with `-O3`, `-ffast-math`, and `-march=native`. On x86_64 they also enable SSE2 and SSSE3. Link-time optimization is turned on when the compiler supports it.

## Usage

```
./build/vxR -r -scene scenes/demo.vx
```

| Argument | Effect |
| --- | --- |
| `-r -scene <file.vx>` | Render the scene. `-r` must come first. |
| `-at`, `-allTests` | Run the grid and math unit tests. |
| `-help`, `--help` | Print the short help text. |

`scenes/demo.vx` is the fuller example. `scenes/single.vx` is a smaller one. `scenes/juan_kloofendal.vx` lights a mesh from an OpenEXR sky, with `intensity` and `lightClamp` on the dome. `scenes/bg_kloofendal.vx`, `scenes/bg_dof.vx`, `scenes/bg_panther.vx`, and `scenes/bg_statue.vx` are smaller setups of the same kind. The Kloofendal scenes point at a local `.exr` outside this repository; change `imagePath` before rendering them. Other paths inside a scene, including PLY and image files, are resolved relative to the scene file.

## Scene file

A `.vx` file is an ASCII list of `node` blocks. Each node has a `name`, a `type`, an `active` flag, and type-specific attributes. Inactive nodes are ignored. Several `vxRenderSettings` nodes can sit in one file; only the active ones are rendered.

Common types:

| Type | Role |
| --- | --- |
| `vxClock` | Frame range (`start`, `end`, `step`). |
| `vxRenderSettings` | Resolution, `pixelSamples`, `rayDepth`, `giMultiplier`. |
| `vxCamera` | Transform, focus, lens, and apertures. |
| `vxShader` | Diffuse, reflection, and GI coefficients. |
| `vxGrid` | Voxel resolution, size, and shader. |
| `vxGridGeometry` | PLY file voxelized into the grid. |
| `vxGeometry` | PLY triangle mesh. |
| `vxSphere`, `vxCylinder`, `vxPlane` | Analytic primitives. |
| `vxDome` | Environment. `imageNode` is the background, `lightImageNode` is the light. Optional `intensity` and `lightClamp`. |
| `vxAreaLight`, `vxPointLight`, `vxSphereLight`, `vxSunLight`, `vxDirectLight`, `vxAmbientLight`, `vxIBLight` | Lights. |
| `vxImage` | Image for a dome or an image-based light. `.exr` is read as linear HDR. JPEG, PNG, and the other formats stb_image supports are read as 8-bit. `gamma` and `gain` are applied when the image is sampled; use `gamma = 1.0` for an EXR sky. |

## Background

vxR started in 2002 as a personal renderer, after an attempt to implement subsurface scattering inside a shader network that did not expose enough render data. It has stayed a study project for C++ and rendering math, built around the voxel grid rather than as a production path tracer.
