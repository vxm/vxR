#ifndef VXACCELUNITTEST_H
#define VXACCELUNITTEST_H

namespace vxCore
{

///
/// Differential tests of the scene acceleration structures and of the voxel
/// grid traversal. They run with `vxR -at` (also `-allTests`).
///
///  - broadphase against bvh on fixed seed rays, on a synthetic scene and on
///    scenes/juan_kloofendal.vx, ab_grid.vx, ab_prims.vx, bg_statue.vx (a scene
///    whose input files are missing is skipped and said so);
///  - Grid::throwRayReference against the hierarchical Grid::throwRay at the
///    resolutions 28, 100 (not a multiple of 8), 256 and 474 (sparse);
///  - GeoGrid CSR store and CellMarcher against the reference unique_ptr store
///    (VXR_MESH=ref), including the single-cell SearchResult::index == 0 bug;
///  - per-mesh triangle BVH (VXR_MESH=bvh) against a linear closest-hit oracle
///    and against CSR. Closest-hit vs first-occupied-cell diffs are reported
///    and do not fail -at by themselves.
///
/// Set VXR_AT_RAYS to change the number of random rays per case (default
/// 20000), VXR_AT_SKIP_SCENES=1 to leave out the scene files.
///
class AccelUnitTest
{
public:
	/// Returns the number of failed checks, 0 when everything passed.
	static int run();
};

} // namespace vxCore
#endif // VXACCELUNITTEST_H
