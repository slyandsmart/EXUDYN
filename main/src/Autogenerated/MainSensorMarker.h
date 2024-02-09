/** ***********************************************************************************************
* @class        MainSensorMarkerParameters
* @brief        Parameter class for MainSensorMarker
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2024-02-03  15:35:24 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINSENSORMARKERPARAMETERS__H
#define MAINSENSORMARKERPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CSensorMarker.h"

#include "Autogenerated/VisuSensorMarker.h"

//! AUTO: Parameters for class MainSensorMarkerParameters
class MainSensorMarkerParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainSensorMarker
* @brief        A sensor attached to a marker. The sensor measures the selected marker values and outputs values into a file, showing per line [time, sensorValue[0], sensorValue[1], ...]. Depending on markers, it can measure Coordinates (MarkerNodeCoordinate), Position and Velocity (MarkerXXXPosition), Position, Velocity, Rotation and AngularVelocityLocal (MarkerXXXRigid). Note that marker values are only available for the current configuration. Use SensorUserFunction to modify sensor results (e.g., transforming to other coordinates) and writing to file
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

//! AUTO: MainSensorMarker
class MainSensorMarker: public MainSensor // AUTO: 
{
protected: // AUTO: 
    CSensorMarker* cSensorMarker; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationSensorMarker* visualizationSensorMarker; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainSensorMarker()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CSensorMarker* GetCSensorMarker() { return cSensorMarker; }
    //! AUTO: Get const pointer to computational class
    const CSensorMarker* GetCSensorMarker() const { return cSensorMarker; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCSensorMarker(CSensorMarker* pCSensorMarker) { cSensorMarker = pCSensorMarker; }

    //! AUTO: Get pointer to visualization class
    VisualizationSensorMarker* GetVisualizationSensorMarker() { return visualizationSensorMarker; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationSensorMarker* GetVisualizationSensorMarker() const { return visualizationSensorMarker; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationSensorMarker(VisualizationSensorMarker* pVisualizationSensorMarker) { visualizationSensorMarker = pVisualizationSensorMarker; }

    //! AUTO: Get const pointer to computational base class object
    virtual CSensor* GetCSensor() const { return cSensorMarker; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCSensor(CSensor* pCSensor) { cSensorMarker = (CSensorMarker*)pCSensor; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationSensor* GetVisualizationSensor() const { return visualizationSensorMarker; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationSensor(VisualizationSensor* pVisualizationSensor) { visualizationSensorMarker = (VisualizationSensorMarker*)pVisualizationSensor; }

    //! AUTO:  Get type name of sensor (without keyword 'Sensor'...!)
    virtual const char* GetTypeName() const override
    {
        return "Marker";
    }

    //! AUTO:  Check consistency prior to CSystem::Assemble(); needs to find all possible violations such that Assemble() would fail
    virtual bool CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const override;


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cSensorMarker->GetParameters().markerNumber = EPyUtils::GetMarkerIndexSafely(d["markerNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cSensorMarker->GetParameters().writeToFile = py::cast<bool>(d["writeToFile"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "fileName", cSensorMarker->GetParameters().fileName); /*! AUTO:  safely cast to C++ type*/
        cSensorMarker->GetParameters().outputVariableType = (OutputVariableType)py::cast<Index>(d["outputVariableType"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cSensorMarker->GetParameters().storeInternal = py::cast<bool>(d["storeInternal"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationSensorMarker->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["sensorType"] = (std::string)GetTypeName();
        d["markerNumber"] = (MarkerIndex)cSensorMarker->GetParameters().markerNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["writeToFile"] = (bool)cSensorMarker->GetParameters().writeToFile; //! AUTO: cast variables into python (not needed for standard types) 
        d["fileName"] = (std::string)cSensorMarker->GetParameters().fileName; //! AUTO: cast variables into python (not needed for standard types) 
        d["outputVariableType"] = (OutputVariableType)cSensorMarker->GetParameters().outputVariableType; //! AUTO: cast variables into python (not needed for standard types) 
        d["storeInternal"] = (bool)cSensorMarker->GetParameters().storeInternal; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationSensorMarker->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumber") == 0) { return py::cast((MarkerIndex)cSensorMarker->GetParameters().markerNumber);} //! AUTO: get parameter
        else if (parameterName.compare("writeToFile") == 0) { return py::cast((bool)cSensorMarker->GetParameters().writeToFile);} //! AUTO: get parameter
        else if (parameterName.compare("fileName") == 0) { return py::cast((std::string)cSensorMarker->GetParameters().fileName);} //! AUTO: get parameter
        else if (parameterName.compare("outputVariableType") == 0) { return py::cast((OutputVariableType)cSensorMarker->GetParameters().outputVariableType);} //! AUTO: get parameter
        else if (parameterName.compare("storeInternal") == 0) { return py::cast((bool)cSensorMarker->GetParameters().storeInternal);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationSensorMarker->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("SensorMarker::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumber") == 0) { cSensorMarker->GetParameters().markerNumber = EPyUtils::GetMarkerIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("writeToFile") == 0) { cSensorMarker->GetParameters().writeToFile = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("fileName") == 0) { EPyUtils::SetStringSafely(value, cSensorMarker->GetParameters().fileName); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("outputVariableType") == 0) { cSensorMarker->GetParameters().outputVariableType = py::cast<OutputVariableType>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("storeInternal") == 0) { cSensorMarker->GetParameters().storeInternal = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationSensorMarker->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("SensorMarker::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};



#endif //#ifdef include once...
