cmake_minimum_required(VERSION 3.16)

project(vxR LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

# Translation units from the old qmake project, without the unused Qt windows.
set(VXR_SOURCES
	source/main.cpp
	source/ImageProperties.cpp
	source/TimeUtils.cpp
	source/FileUtils.cpp
	source/MathUtils.cpp
	source/renderPresset.cpp
	source/imageData.cpp
	source/sseUtils.cpp
	source/MathUtilsUnitTest.cpp
	source/StringUtils.cpp
	source/VectorUtils.cpp
	source/Matrix33.cpp
	source/Matrix44.cpp
	source/Quaternion.cpp
	source/Light.cpp
	source/Ramp.cpp
	source/BoundingBox.cpp
	source/Box.cpp
	source/BoxCreator.cpp
	source/AccelStructure.cpp
	source/BroadPhase.cpp
	source/Bvh.cpp
	source/MeshBvh.cpp
	source/AccelUnitTest.cpp
	source/BucketList.cpp
	source/Camera.cpp
	source/CirclesMap.cpp
	source/Clock.cpp
	source/Collision.cpp
	source/Color.cpp
	source/ContactBuffer.cpp
	source/Dome.cpp
	source/GeoGrid.cpp
	source/Geometry.cpp
	source/GeometryImporter.cpp
	source/Global.cpp
	source/Grid.cpp
	source/GridUnitTest.cpp
	source/Image.cpp
	source/LegoBlock.cpp
	source/Manager.cpp
	source/Map.cpp
	source/Map2d.cpp
	source/Node.cpp
	source/NonSequentalPool.cpp
	source/PerfStats.cpp
	source/Pixel.cpp
	source/Object.cpp
	source/Plane.cpp
	source/PLYImporter.cpp
	source/Point.cpp
	source/Process.cpp
	source/Ray.cpp
	source/RayEmitter.cpp
	source/RenderProcess.cpp
	source/Sampler.cpp
	source/Scene.cpp
	source/SceneParser.cpp
	source/Shader.cpp
	source/Vector.cpp
	source/Value.cpp
	source/UnitTest.cpp
	source/TriRef.cpp
	source/TriangleMesh.cpp
	source/ThreadPool.cpp
	source/Stopwatch.cpp
	source/Status.cpp
	source/StandardShader.cpp
	source/Cylinder.cpp
	source/visible.cpp
	source/LightShader.cpp
	source/Sphere.cpp
	source/ConvexGeometry.cpp
	source/stb_image_impl.cpp
	source/tinyexr_impl.cpp
)

add_executable(vxR ${VXR_SOURCES})

target_include_directories(vxR PRIVATE
	${CMAKE_SOURCE_DIR}/headers
	${CMAKE_SOURCE_DIR}/source
	${CMAKE_SOURCE_DIR}/third_party/stb
	${CMAKE_SOURCE_DIR}/third_party/tinyexr
)

target_link_libraries(vxR PRIVATE Threads::Threads z)

# Counters and phase timers behind --stats. OFF compiles every call site away.
option(VXR_PERF_STATS "Compile the PerfStats counters" ON)

target_compile_definitions(vxR PRIVATE
	$<$<CONFIG:Debug>:_DEBUG>
	VXR_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
	VXR_BUILD_TYPE="$<CONFIG>"
)
if(NOT VXR_PERF_STATS)
	target_compile_definitions(vxR PRIVATE VXR_NO_PERF_STATS)
endif()

# x86 SIMD flags from the old qmake release config. Apple Silicon rejects them.
set(VXR_ARCH_FLAGS)
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|i686)$")
	list(APPEND VXR_ARCH_FLAGS -msse2 -mssse3 -mfpmath=sse)
endif()

# CMake's Release config already passes -O3. -ffast-math keeps the old
# qmake -Ofast behavior; Apple Clang 21 deprecates -Ofast itself.
set(VXR_RELEASE_FLAGS -fomit-frame-pointer -ffast-math -march=native -mtune=native)
list(APPEND VXR_RELEASE_FLAGS ${VXR_ARCH_FLAGS})

target_compile_options(vxR PRIVATE
	"$<$<CONFIG:Release>:${VXR_RELEASE_FLAGS}>"
	"$<$<CONFIG:RelWithDebInfo>:${VXR_RELEASE_FLAGS}>"
)

# The differential tests look for NaN positions and compare hits, which -ffast-math
# turns into undefined behaviour. The engine code they test is still built with it.
set_source_files_properties(source/AccelUnitTest.cpp PROPERTIES
	COMPILE_OPTIONS "$<$<CONFIG:Release>:-fno-fast-math>;$<$<CONFIG:RelWithDebInfo>:-fno-fast-math>")

include(CheckIPOSupported)
check_ipo_supported(RESULT vxr_ipo_supported OUTPUT vxr_ipo_error)
if(vxr_ipo_supported)
	set_property(TARGET vxR PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()

# Compares two images for the A/B benchmarks, see tools/ab_bench.py.
add_executable(vxr_imgdiff tools/vxr_imgdiff.cpp)
target_include_directories(vxr_imgdiff PRIVATE ${CMAKE_SOURCE_DIR}/third_party/stb)
