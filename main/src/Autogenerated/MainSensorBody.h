/** ***********************************************************************************************
* @class        MainSensorBodyParameters
* @brief        Parameter class for MainSensorBody
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2022-05-26  21:50:03 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINSENSORBODYPARAMETERS__H
#define MAINSENSORBODYPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CSensorBody.h"

#include "Autogenerated/VisuSensorBody.h"

//! AUTO: Parameters for class MainSensorBodyParameters
class MainSensorBodyParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainSensorBody
* @brief        A sensor attached to a body-object with local position \f$\pLocB\f$. As a difference to SensorObject, the body sensor needs a local position at which the sensor is attached to. The sensor measures OutputVariableBody and outputs values into a file, showing per line [time, sensorValue[0], sensorValue[1], ...]. Use SensorUserFunction to modify sensor results (e.g., transforming to other coordinates) and writing to file.
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

//! AUTO: MainSensorBody
class MainSensorBody: public MainSensor // AUTO: 
{
protected: // AUTO: 
    CSensorBody* cSensorBody; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationSensorBody* visualizationSensorBody; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainSensorBody()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CSensorBody* GetCSensorBody() { return cSensorBody; }
    //! AUTO: Get const pointer to computational class
    const CSensorBody* GetCSensorBody() const { return cSensorBody; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCSensorBody(CSensorBody* pCSensorBody) { cSensorBody = pCSensorBody; }

    //! AUTO: Get pointer to visualization class
    VisualizationSensorBody* GetVisualizationSensorBody() { return visualizationSensorBody; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationSensorBody* GetVisualizationSensorBody() const { return visualizationSensorBody; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationSensorBody(VisualizationSensorBody* pVisualizationSensorBody) { visualizationSensorBody = pVisualizationSensorBody; }

    //! AUTO: Get const pointer to computational base class object
    virtual CSensor* GetCSensor() const { return cSensorBody; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCSensor(CSensor* pCSensor) { cSensorBody = (CSensorBody*)pCSensor; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationSensor* GetVisualizationSensor() const { return visualizationSensorBody; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationSensor(VisualizationSensor* pVisualizationSensor) { visualizationSensorBody = (VisualizationSensorBody*)pVisualizationSensor; }

    //! AUTO:  Get type name of sensor (without keyword 'Sensor'...!)
    virtual const char* GetTypeName() const override
    {
        return "Body";
    }

    //! AUTO:  Check consistency prior to CSystem::Assemble(); needs to find all possible violations such that Assemble() would fail
    virtual bool CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const override;


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cSensorBody->GetParameters().bodyNumber = EPyUtils::GetObjectIndexSafely(d["bodyNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "localPosition", cSensorBody->GetParameters().localPosition); /*! AUTO:  safely cast to C++ type*/
        cSensorBody->GetParameters().writeToFile = py::cast<bool>(d["writeToFile"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "fileName", cSensorBody->GetParameters().fileName); /*! AUTO:  safely cast to C++ type*/
        cSensorBody->GetParameters().outputVariableType = (OutputVariableType)py::cast<Index>(d["outputVariableType"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cSensorBody->GetParameters().storeInternal = py::cast<bool>(d["storeInternal"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationSensorBody->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["sensorType"] = (std::string)GetTypeName();
        d["bodyNumber"] = (ObjectIndex)cSensorBody->GetParameters().bodyNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["localPosition"] = (std::vector<Real>)cSensorBody->GetParameters().localPosition; //! AUTO: cast variables into python (not needed for standard types) 
        d["writeToFile"] = (bool)cSensorBody->GetParameters().writeToFile; //! AUTO: cast variables into python (not needed for standard types) 
        d["fileName"] = (std::string)cSensorBody->GetParameters().fileName; //! AUTO: cast variables into python (not needed for standard types) 
        d["outputVariableType"] = (OutputVariableType)cSensorBody->GetParameters().outputVariableType; //! AUTO: cast variables into python (not needed for standard types) 
        d["storeInternal"] = (bool)cSensorBody->GetParameters().storeInternal; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationSensorBody->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("bodyNumber") == 0) { return py::cast((ObjectIndex)cSensorBody->GetParameters().bodyNumber);} //! AUTO: get parameter
        else if (parameterName.compare("localPosition") == 0) { return py::cast((std::vector<Real>)cSensorBody->GetParameters().localPosition);} //! AUTO: get parameter
        else if (parameterName.compare("writeToFile") == 0) { return py::cast((bool)cSensorBody->GetParameters().writeToFile);} //! AUTO: get parameter
        else if (parameterName.compare("fileName") == 0) { return py::cast((std::string)cSensorBody->GetParameters().fileName);} //! AUTO: get parameter
        else if (parameterName.compare("outputVariableType") == 0) { return py::cast((OutputVariableType)cSensorBody->GetParameters().outputVariableType);} //! AUTO: get parameter
        else if (parameterName.compare("storeInternal") == 0) { return py::cast((bool)cSensorBody->GetParameters().storeInternal);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationSensorBody->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("SensorBody::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("bodyNumber") == 0) { cSensorBody->GetParameters().bodyNumber = EPyUtils::GetObjectIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("localPosition") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, cSensorBody->GetParameters().localPosition); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("writeToFile") == 0) { cSensorBody->GetParameters().writeToFile = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("fileName") == 0) { EPyUtils::SetStringSafely(value, cSensorBody->GetParameters().fileName); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("outputVariableType") == 0) { cSensorBody->GetParameters().outputVariableType = py::cast<OutputVariableType>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("storeInternal") == 0) { cSensorBody->GetParameters().storeInternal = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationSensorBody->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("SensorBody::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};



#endif //#ifdef include once...
