#include "PlaneComparator.hpp"

// OCC
#include <GeomAdaptor_Surface.hxx>
#include <BRep_Tool.hxx>

// C++
#include <array>

McCAD::Tools::PlaneComparator::PlaneComparator(
        Standard_Real parameterTolerance,
        Standard_Real angularTolerance,
        Standard_Real distanceTolerance)
    : parameterTolerance{parameterTolerance}
    , angularTolerance{angularTolerance}
    , distanceTolerance{distanceTolerance}{
}

bool
McCAD::Tools::PlaneComparator::operator()(
        const TopoDS_Face& firstFace,
        const TopoDS_Face& secondFace) const{
    // Compare orientations
    if(firstFace.Orientation() != secondFace.Orientation())
        return false;

    // Compare locations
    TopLoc_Location firstLocation;
    GeomAdaptor_Surface firstAdaptor{
        BRep_Tool::Surface(firstFace, firstLocation)};

    TopLoc_Location secondLocation;
    GeomAdaptor_Surface secondAdaptor{
        BRep_Tool::Surface(secondFace, secondLocation)};

    if(firstLocation != secondLocation)
        return false;

    try{
        auto firstPlane = firstAdaptor.Plane();             // also checks if surface type is GeomAbs_Plane
        auto secondPlane = secondAdaptor.Plane();           // also checks if surface type is GeomAbs_Plane
        secondPlane.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);
        firstPlane.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);

        // Compare plane parameters
        return equivalentPlaneParameters(firstPlane, secondPlane);
    }catch(const Standard_NoSuchObject&){
        // GeomAdaptor_Surface::Plane() failed: one surface is not a plane
        return false;
    }
}

std::array<Standard_Real, 4>
McCAD::Tools::PlaneComparator::planeParameters(
        const gp_Pln& plane) const{
    std::array<Standard_Real, 4> planeParameters;
    plane.Coefficients(
                planeParameters[0],
                planeParameters[1],
                planeParameters[2],
                planeParameters[3]);
    for(auto& parameter : planeParameters){
        if(parameter <= parameterTolerance)
            parameter = 0.0;
    }

    return planeParameters;
}

bool
McCAD::Tools::PlaneComparator::equivalentPlaneParameters(
        const gp_Pln& first,
        const gp_Pln& second) const{

    try{
        auto firstPlaneParameters = planeParameters(first);

        gp_Dir firstPlaneDirection{
            firstPlaneParameters[0],
            firstPlaneParameters[1],
            firstPlaneParameters[2]
        };

        auto secondPlaneParameters = planeParameters(second);

        gp_Dir secondPlaneDirection{
            secondPlaneParameters[0],
            secondPlaneParameters[1],
            secondPlaneParameters[2]
        };

        return (firstPlaneDirection.IsEqual(secondPlaneDirection, angularTolerance)
                && std::abs(firstPlaneParameters[3] - secondPlaneParameters[3])
                    <= distanceTolerance)
                ||
               (firstPlaneDirection.IsOpposite(secondPlaneDirection, angularTolerance)
                && std::abs(firstPlaneParameters[3] + secondPlaneParameters[3])
                    <= distanceTolerance);
    }catch(const Standard_ConstructionError&){
        // Plane parameters cannot form a normed direction vector;
        // not distinguishable from zero vector
        return false;
    }
}