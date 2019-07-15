#ifndef DECOMPOSITION_HPP
#define DECOMPOSITION_HPP

// C++
#include <memory>
// McCAD
#include "inputdata.hpp"
#include <Standard.hxx>

namespace McCAD::Decomposition {
    class Decompose {
    private:
      class Impl;
      
    public:
      Decompose(const McCAD::General::InputData& inputData);
      ~Decompose();
      
      Impl* accessDImpl() const;

    private:
      std::unique_ptr<Impl> pDImpl;
    };
}
#endif //DECOMPOSITION_HPP
