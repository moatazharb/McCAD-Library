// C++
#include <iostream>
#include <string>
// McCAD
#include "inputdata.hpp"
#include "stepreader.hpp"
#include "stepwriter.hpp"
#include "decomposition.hpp"

int main (){
  std::string inputFileName, successFileName, failFileName;
    std::cout << "Specify the path to the input step file: " << std::endl;
    getline(std::cin, inputFileName);
    std::cout << "Specify the path to save the resulting step file: " << std::endl;
    getline(std::cin, successFileName);
    std::cout << "Specify the path to save the rejected solids'sstep file: " << std::endl;
    getline(std::cin, failFileName);
    
    // Load the input file.
    std::cout << "***************************" << std::endl;
    std::cout << "** Loading the STEP file **" << std::endl;
    std::cout << "***************************" << std::endl;
    McCAD::IO::STEPReader reader{inputFileName};
    auto inputData = reader.getInputData();

    // Start decomposition.
    std::cout << "****************************" << std::endl;
    std::cout << "** Starting decomposition **" << std::endl;
    std::cout << "****************************" << std::endl;
    McCAD::Decomposition::Decompose decompose{inputData};
    auto outputData_success = decompose.getResultSolids();
    auto outputData_fail = decompose.getRejectedsubSolids();

    // Write output STEP files.
    std::cout << "****************************" << std::endl;
    std::cout << "**  Saving to STEP file   **" << std::endl;
    std::cout << "****************************" << std::endl;
    McCAD::IO::STEPWriter successWriter{successFileName, outputData_success};
    McCAD::IO::STEPWriter failWriter{failFileName, outputData_fail};
    
    return 0;
}
