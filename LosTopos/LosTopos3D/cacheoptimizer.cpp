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

// ---------------------------------------------------------
// Includes
// ---------------------------------------------------------

#include <cacheoptimizer.h>
#include <surftrack.h>
#include <iostream>
#include <OpenCCL.h>

// ---------------------------------------------------------
// Member function definitions
// ---------------------------------------------------------

namespace LosTopos {

  void CacheOptimizer::sortTriangles() {
    auto & triangles = m_surf.m_mesh.get_triangles();
    auto &labels = m_surf.m_mesh.get_triangle_labels();
    // find out graph's biggest node index
    size_t graphSize = 0;
    for (size_t i = 0; i < triangles.size(); i++) {
      if (!m_surf.m_mesh.triangle_is_deleted(i)) {
        graphSize = std::max(graphSize, triangles[i][0]);
        graphSize = std::max(graphSize, triangles[i][1]);
        graphSize = std::max(graphSize, triangles[i][2]);
      }
    }
    graphSize++;
    // compute new vertex order
    OpenCCL::CLayoutGraph graph(graphSize);
    for (size_t i = 0; i < triangles.size(); i++) {
      if (!m_surf.m_mesh.triangle_is_deleted(i)) {
        graph.AddEdge(triangles[i][0], triangles[i][1]);
        graph.AddEdge(triangles[i][1], triangles[i][2]);
        graph.AddEdge(triangles[i][2], triangles[i][0]);
      }
    }
    std::vector<int> order(graphSize, 0), newVertexIndex(graphSize, 0);
    graph.ComputeOrdering(&order[0]);
    std::cerr << "GRAPH SIZE " << order.size() << std::endl;
    std::cerr << m_surf.m_velocities.size() << " " << m_surf.pm_positions.size() << std::endl;
    // swap vertices positions
    // Order [i] has an original vertex id in ith position in the computed vertex ordering.
    std::vector<Vec3d> newPositions(order.size()), newNewPositions(order.size()), newMasses(order.size()), newVelocities(order.size());
    for (size_t i = 0; i < order.size(); i++) {
      newPositions[i] = m_surf.pm_positions[order[i]];
      newNewPositions[i] = m_surf.pm_newpositions[order[i]];
      newMasses[i] = m_surf.m_masses[order[i]];
      //newVelocities[i] = m_surf.m_velocities[order[i]];
      newVertexIndex[order[i]] = i;
    }
    assert(newPositions.size() == newNewPositions.size());
    m_surf.m_mesh.set_num_vertices(newPositions.size());
    m_surf.set_all_positions(newPositions);
    m_surf.set_all_newpositions(newNewPositions);
    //m_surf.set_all_remesh_velocities(newVelocities);
    for (size_t i = 0; i < order.size(); i++) 
      m_surf.m_masses[i] = newMasses[i];
    // now renumber triangle vertices
    std::vector<Vec3st> newTriangles(triangles.size());
    std::vector<Vec2i> newLabels(triangles.size());
    for (size_t i = 0; i < triangles.size(); i++) {
        newLabels[i] = labels[i];
        newTriangles[i] =
          Vec3st(newVertexIndex[triangles[i][0]],
            newVertexIndex[triangles[i][1]],
            newVertexIndex[triangles[i][2]]);

    }
   m_surf.m_mesh.replace_all_triangles(newTriangles, newLabels);
   size_t nv = m_surf.m_mesh.m_vertex_to_triangle_map.size();
   m_surf.pm_positions.resize(nv);
   m_surf.pm_newpositions.resize(nv);
   m_surf.pm_velocities.resize(nv);
   m_surf.m_velocities.resize(nv);

    
  }

}