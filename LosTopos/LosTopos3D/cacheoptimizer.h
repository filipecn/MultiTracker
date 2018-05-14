// ---------------------------------------------------------
//
//  meshsmoother.h
//  Tyson Brochu 2011
//  Christopher Batty, Fang Da 2014
//  Filipe CN, Christopher Batty 2018
//
//  Functions related to mesh elements sorting for cache miss minimization.
//
// ---------------------------------------------------------


#ifndef LOSTOPOS_CACHEOPTIMIZER_H
#define LOSTOPOS_CACHEOPTIMIZER_H

// ---------------------------------------------------------
//  Nested includes
// ---------------------------------------------------------

#include <cstddef>
#include <vector>
#include <cmath>
// ---------------------------------------------------------
//  Forwards and typedefs
// ---------------------------------------------------------

namespace LosTopos {

  class SurfTrack;
  template<unsigned int N, class T> struct Vec;
  typedef Vec<3, double> Vec3d;
  typedef Vec<2, size_t> Vec2st;
  typedef Vec<3, size_t> Vec3st;

  // ---------------------------------------------------------
  //  Class definitions
  // ---------------------------------------------------------

  // ---------------------------------------------------------
  ///
  /// Mesh smoother object.  Performs NULL-space constrained Laplacian smoothing of mesh vertices.
  ///
  // ---------------------------------------------------------

  class CacheOptimizer
  {

  public:

    /// Constructor
    ///
    CacheOptimizer(SurfTrack& surf) :
      m_surf(surf)
    {}

    void sortTriangles();

  private:
    friend class SurfTrack;

    /// The mesh this object operates on
    /// 
    SurfTrack& m_surf;
  };

}

#endif

