//C++
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <ctime>
// McCAD
#include "inputconfig.hpp"

McCAD::IO::InputConfig::InputConfig(){}

McCAD::IO::InputConfig::InputConfig(const std::filesystem::path& currentPath)
    : currentPath{currentPath}{
}

McCAD::IO::InputConfig::~InputConfig(){
}

/** ********************************************************************
* @brief   Writes a template file of run parameters to disk.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::IO::InputConfig::writeTemplate(){
    auto timeStart{std::chrono::system_clock::now()};
    std::time_t timeStart_t = std::chrono::system_clock::to_time_t(timeStart);
    std::ofstream inputConfig;
    std::filesystem::path templateName = currentPath / "McCADInputConfig.i";
    inputConfig.open(templateName.string());
    inputConfig << "# McCAD Run Parameters    " << std::ctime(&timeStart_t) <<
                   "# ====================\n" << std::endl;
    inputConfig << "# Input\n"
                   "# =====\n"
                   "# > Debug level: 0, 1, 2, 3. [0] provides no debugging outputs.\n"
                   "debugLevel = 0\n"
                   "# > The unit used for the input STEP file(s).\n"
                   "units = cm\n"
                   "# > Path to the input STEP file;\n"
                   "inputFileName = input.stp\n" << std::endl;
    inputConfig << "# Decomposition\n"
                   "# =============\n"
                   "decompose = true\n"
                   "recurrenceDepth = 20\n"
                   "minSolidVolume = 1.0e-3 [cm3]\n"
                   "minFaceArea = 1.0e-4 [cm2]\n"
                   "scalingFactor = 100.0\n"
                   "precision = 1.0e-6\n"
                   "faceTolerance = 1.0e-8 [cm]\n"
                   "edgeTolerance = 1.0e-8 [cm]\n"
                   "parameterTolerance = 1.0e-8 [cm]\n"
                   "angularTolerance = 1.0e-4 [radian/PI]\n"
                   "distanceTolerance = 1.0e-6 [cm]\n"
                   "simplifyTori = false\n"
                   "simplifyAllTori = false\n"
                   "torusSplitAngle = 30.0 [degrees]\n" << std::endl;
    inputConfig << "# Conversion\n"
                   "# ==========\n"
                   "convert = false\n"
                   "# > Choose whether or not to generate void cells;\n"
                   "voidGeneration = false\n"
                   "# > Condition to treat a component as a single cell or a group of cells.\n"
                   "componentIsSingleCell = false\n"
                   "# > Minimum acceptable void volume shouldn;t be less than minSolidVolume;\n"
                   "minVoidVolume = 1.0 [cm3]\n"
                   "# > A larger number will result in fewer void cells but longer cell expressions;\n"
                   "maxSolidsPerVoidCell = 40\n"
                   "# > Choose whether or not to generate Bound Volume Heirarchy void cells;\n"
                   "BVHVoid = false\n"
                   "# > Choose the desired MC code for conversion;\n"
                   "MCcode = mcnp\n"
                   "startCellNum = 1\n"
                   "startSurfNum = 1\n"
                   "startMatNum = 1\n"
                   "maxLineWidth = 80\n"
                   "MCOutputFileName = MCFile.i\n"
                   "volumesFileName = volumes.i\n" << std::endl;
    inputConfig.close();
}

/** ********************************************************************
* @brief   Reads the config file of run parameters.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::IO::InputConfig::readTemplate(){
    std::ifstream inputConfig("McCADInputConfig.i");
    if (!inputConfig){
        inputFileNames.push_back("input.stp");
        std::cout << "[WARNING] McCADInputConfig.i is missing!, proceeding with default parameters:"
                     "\nInput  = input.stp" <<
                     "\nResult = inputDecomposed.stp" <<
                     "\nReject = inputRejected.stp" << std::endl;
    } else {
        // Read the file and populate the parameters
        while (!inputConfig.eof()){
            std::string line;
            getline(inputConfig, line);
           std::vector<std::string> lineSplit = splitLine(line, ' ');
           if (lineSplit.size() == 0 || lineSplit[0] == "#") continue;
           else {
               // General input.
               if (lineSplit[0] == "debugLevel")
                   debugLevel = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "units"){
                   units = stringToLowerCase(lineSplit[2]);
                   if (units == "cm") conversionFactor = 10.0;
                   else if (units == "m") conversionFactor = 1000.0;
               } else if (lineSplit[0] == "inputFileName"){
                   for(int i = 2; i < lineSplit.size(); ++i){
                       inputFileNames.push_back(lineSplit[i]);
                   }
               // Decompositions
               } else if (lineSplit[0] == "decompose")
                   decompose = stringToLowerCase(lineSplit[2]) == "true" ? true : false;
               else if (lineSplit[0] == "recurrenceDepth")
                   recurrenceDepth = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "minSolidVolume")
                   minSolidVolume = std::stof(lineSplit[2]) * std::pow(conversionFactor, 3);
               else if (lineSplit[0] == "minFaceArea")
                   minFaceArea = std::stof(lineSplit[2]) * std::pow(conversionFactor, 2);
               else if (lineSplit[0] == "scalingFactor")
                   scalingFactor = std::stof(lineSplit[2]);
               else if (lineSplit[0] == "precision")
                   precision = std::stof(lineSplit[2]);
               else if (lineSplit[0] == "faceTolerance")
                   faceTolerance = std::stof(lineSplit[2]) * conversionFactor;
               else if (lineSplit[0] == "edgeTolerance")
                   edgeTolerance = std::stof(lineSplit[2]) * conversionFactor;
               else if (lineSplit[0] == "parameterTolerance")
                   parameterTolerance = std::stof(lineSplit[2]) * conversionFactor;
               else if (lineSplit[0] == "angularTolerance")
                   angularTolerance = std::stof(lineSplit[2]) * PI;
               else if (lineSplit[0] == "distanceTolerance")
                   distanceTolerance = std::stof(lineSplit[2]) * conversionFactor;
               else if (lineSplit[0] == "simplifyTori")
                   simplifyTori = stringToLowerCase(lineSplit[2]) == "true" ? true : false;
               else if (lineSplit[0] == "simplifyAllTori")
                   simplifyAllTori = stringToLowerCase(lineSplit[2]) == "true" ? true : false;
               else if (lineSplit[0] == "torusSplitAngle")
                   torusSplitAngle = std::stof(lineSplit[2]) * PI / 180.0;
               // Conversion
               else if (lineSplit[0] == "convert")
                   convert = stringToLowerCase(lineSplit[2]) == "false" ? false : true;
               else if (lineSplit[0] == "voidGeneration")
                   voidGeneration = stringToLowerCase(lineSplit[2]) == "true" ? true : false;
               else if (lineSplit[0] == "componentIsSingleCell")
                   componentIsSingleCell = stringToLowerCase(lineSplit[2]) == "true" ? true : false;
               else if (lineSplit[0] == "minVoidVolume")
                   minVoidVolume = std::stof(lineSplit[2]) * std::pow(conversionFactor, 3);
               else if (lineSplit[0] == "maxSolidsPerVoidCell")
                   maxSolidsPerVoidCell = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "BVHVoid")
                   BVHVoid = stringToLowerCase(lineSplit[2]) == "true" ? true : false;
               else if (lineSplit[0] == "MCcode")
                   MCcode = stringToLowerCase(lineSplit[2]);
               else if (lineSplit[0] == "startCellNum")
                   startCellNum = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "startSurfNum")
                   startSurfNum = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "startMatNum")
                   startMatNum = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "maxLineWidth")
                   maxLineWidth = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "MCOutputFileName")
                   MCOutputFileName = lineSplit[2];
               else if (lineSplit[0] == "volumesFileName")
                   volumesFileName = lineSplit[2];
               else continue;
           }
        }
    }
    if (decompose) populateNamesLists();
    if(convert) populateMatList();
}

/** ********************************************************************
* @brief   Splits a line with a specified delimiter.
* @param   A string to split.
* @param   A character delimiter.
* @return  A vector of strings.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
std::vector<std::string>
McCAD::IO::InputConfig::splitLine(const std::string& line, char delimiter){
    std::istringstream ss(line);
    std::vector<std::string> lineSplit;
    std::string s;
    while (std::getline(ss, s, delimiter)){
        lineSplit.push_back(s);
    }
    return lineSplit;
}

/** ********************************************************************
* @brief   Converts a string to lower case.
* @param   A string to convert to lower case.
* @return  A lower case string.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
std::string
McCAD::IO::InputConfig::stringToLowerCase(std::string& string){
    std::transform(string.begin(), string.end(), string.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return string;
}

/** ********************************************************************
* @brief   Populates a list of file names for writing output STEP files.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::IO::InputConfig::populateNamesLists(){
    for(int i = 0; i < inputFileNames.size(); ++i){
        std::string splitName = splitLine(inputFileNames[i], '.')[0];
        decomposedFileNames.push_back(splitName + std::string("Decomposed.stp"));
        rejectedFileNames.push_back(splitName + std::string("Rejected.stp"));
    }
}

/** ********************************************************************
* @brief   Populates materials list with material info extracted from STEP input files names.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::IO::InputConfig::populateMatList(){
    for(int i = 0; i < inputFileNames.size(); ++i){
        std::string splitName = splitLine(inputFileNames[i], '.')[0];
        rejectedConvFileNames.push_back(splitName + std::string("RejectedConv.stp"));
        std::string matName = stringToLowerCase(splitLine(splitName, '_')[0]);
        double matDensity{0.0};
        if(matName != "void" && splitLine(splitName, '_').size() > 1)
            matDensity = std::stof(splitLine(splitName, '_')[1]);
        materialsInfo.push_back(std::make_tuple(matName, matDensity));
    }
}
