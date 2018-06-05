#include <vec.h>

namespace LosTopos {
  namespace rootparity {
    bool IntersectVFRobust(
          const Vec3d &a0, const Vec3d &b0, const Vec3d &c0, const Vec3d &d0,
          const Vec3d &a1, const Vec3d &b1, const Vec3d &c1, const Vec3d &d1);

    bool IntersectEERobust(
          const Vec3d &a0, const Vec3d &b0, const Vec3d &c0, const Vec3d &d0,
          const Vec3d &a1, const Vec3d &b1, const Vec3d &c1, const Vec3d &d1);
  }
}