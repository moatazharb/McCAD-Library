// McCAD
#include "boundSurface.hpp"
#include "boundSurface_impl.hpp"

McCAD::Decomposition::BoundSurface::BoundSurface()
  : pImpl{std::make_unique<Impl>()}{
}

McCAD::Decomposition::BoundSurface::~BoundSurface(){
}

McCAD::Decomposition::BoundSurface::Impl*
McCAD::Decomposition::BoundSurface::accessImpl() const{
  return pImpl.get();
}