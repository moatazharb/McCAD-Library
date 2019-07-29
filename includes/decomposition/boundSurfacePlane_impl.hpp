#ifndef BOUNDSURFACEPLANE_IMPL_HPP
#define BOUNDSURFACEPLANE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "boundSurfacePlane.hpp"
// OCC

namespace McCAD::Decomposition{
  class BoundSurfacePlane::Impl {
  public:
    Impl(McCAD::Decomposition::BoundSurfacePlane* backReference);
    ~Impl();

    McCAD::Decomposition::BoundSurfacePlane* boundSurfacePlane;

    std::string surfaceType = "Plane";
    void generateExtPlane(const Standard_Real& boxSquareLength);
    
  private:

  };
}

#endif //BOUNDSURFACEPLANE_IMPL_HPP
