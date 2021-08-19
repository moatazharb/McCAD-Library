#ifndef CYLINDERCOMPARATOR_HPP
#define CYLINDERCOMPARATOR_HPP

// C++
#include <array>
#include <optional>
// OCC
#include <Standard.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <gp_Cylinder.hxx>

namespace McCAD::Tools{
  class CylinderComparator{
  public:
      CylinderComparator();
      CylinderComparator(const Standard_Real& precision, const Standard_Real& angularTolerance,
                         const Standard_Real& distanceTolerance);
      ~CylinderComparator();
  private:
      Standard_Real precision{1.0e-7};
      Standard_Real angularTolerance{1.0e-4};
      Standard_Real distanceTolerance{1.0e-5};
  public:
      std::optional<Standard_Boolean> operator()(const GeomAdaptor_Surface& firstAdaptor,
                                                 const GeomAdaptor_Surface& secondAdaptor);
      std::array<Standard_Real, 10> cylParameters(const gp_Cylinder& cylinder) const;
      std::optional<Standard_Boolean> equivalentCylParameters(const gp_Cylinder& first,
                                                              const gp_Cylinder& second) const;
    };
}

#endif //CYLINDERCOMPARATOR_HPP
