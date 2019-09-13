// McCAD
#include "meshtriangle_impl.hpp"

void
McCAD::Decomposition::MeshTriangle::Impl::initiate(const TopoDS_Face& face){
  Bnd_Box boundingBox;
  BRepBndLib::Add(face, boundingBox);
  boundingBox.SetGap(0.0);
}