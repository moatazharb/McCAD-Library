// McCAD
#include "tools_impl.hpp"

void
McCAD::Tools::Preprocessor::Impl::removeSmallFaces(TopoDS_Shape& solidShape, const Standard_Real precision, const Standard_Real maxTolerance){
  TopoDS_Shape fixedSolidShape;
  Handle_ShapeFix_FixSmallFace smallFaceFix = new ShapeFix_FixSmallFace;
  smallFaceFix->Init(solidShape);
  smallFaceFix->SetPrecision(precision);
  smallFaceFix->SetMaxTolerance(maxTolerance);
  smallFaceFix->Perform();
  solidShape = smallFaceFix->FixShape();
}

void
McCAD::Tools::Preprocessor::Impl::repairSolid(TopoDS_Solid& solid){
  TopoDS_Solid fixedSolid;
  Handle_ShapeFix_Solid solidFix = new ShapeFix_Solid;
  solidFix->Init(solid);
  solidFix->Perform();
  solid = TopoDS::Solid(solidFix->Solid());
}

void
McCAD::Tools::Preprocessor::Impl::genericFix(TopoDS_Solid& solid){
  TopoDS_Solid finalSolid;
  Handle_ShapeFix_Solid genericFix = new ShapeFix_Solid;
  genericFix->Init(solid);
  genericFix->Perform();
  solid = TopoDS::Solid(genericFix->Solid());
}

const Standard_Real
McCAD::Tools::Preprocessor::Impl::calcMeshDeflection(TopoDS_Solid& solid, const Standard_Real bndBoxGap, const Standard_Real converting){
  Standard_Real deflection;
  /** Calculate the bounding box of face **/
  Bnd_Box boundingBox;
  BRepBndLib::Add(solid,boundingBox);
  boundingBox.SetGap(bndBoxGap);
  Standard_Real Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;
  boundingBox.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
  Standard_Real tempdeflection = std::max((Xmax-Xmin), (Ymax-Ymin));
  deflection = std::max(tempdeflection, (Zmax-Zmin)) / converting;
  return deflection;
}

const Standard_Boolean
McCAD::Tools::Preprocessor::Impl::checkBndSurfaces(TopoDS_Solid& solid){
  Standard_Boolean spline_torus_found = Standard_False;
  // Find the faces with small areas, they will not be added into face list.
  TopExp_Explorer explorer(solid, TopAbs_FACE);
  for (; explorer.More(); explorer.Next())
    {
      const TopoDS_Face& face = TopoDS::Face(explorer.Current());
      TopLoc_Location location;
      Handle_Geom_Surface geomSurface = BRep_Tool::Surface(face, location);
      GeomAdaptor_Surface surfAdaptor(geomSurface);

      if(surfAdaptor.GetType() == GeomAbs_Torus)
	{
	  std::cout << "    -- The current verion doesn't support processing of Tori. Ignoring solid!" << std::endl;
	  spline_torus_found = Standard_True;
	  break;
	}
      else if (surfAdaptor.GetType() == GeomAbs_BSplineSurface)
	{
	  std::cout << "    -- The current verion doesn't support processing of splines. Ignoring solid!" << std::endl;
	  spline_torus_found = Standard_True;
	  break;
	}
    }
  return spline_torus_found;
}

const Standard_Boolean
McCAD::Tools::Preprocessor::Impl::checkFace(const TopoDS_Face& face, const Standard_Real tolerance){
  ShapeAnalysis_CheckSmallFace shapeAnalysis;
  Standard_Boolean isSmallFace = Standard_False;
  TopoDS_Edge edge1, edge2;
  if( shapeAnalysis.CheckSpotFace(face, tolerance) || shapeAnalysis.CheckStripFace(face, edge1, edge2, tolerance))
    {
      Standard_Boolean isSmallFace = Standard_True;
    }
  return isSmallFace;
}

void
McCAD::Tools::Preprocessor::Impl::fixFace(TopoDS_Face& face, const Standard_Real precision, const Standard_Real maxTolerance){
  Handle_ShapeFix_Shape shapeFixer = new ShapeFix_Shape(face);
  shapeFixer->SetPrecision(precision);
  shapeFixer->SetMaxTolerance(maxTolerance);
  shapeFixer->FixWireTool()->FixRemovePCurveMode() = 1;
  shapeFixer->FixWireTool()->FixConnected();
  shapeFixer->Perform();
  face = TopoDS::Face(shapeFixer->Shape());
}