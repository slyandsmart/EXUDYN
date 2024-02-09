/** ***********************************************************************************************
* @class        MainObjectConnectorCartesianSpringDamperParameters
* @brief        Parameter class for MainObjectConnectorCartesianSpringDamper
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2024-02-03  15:37:35 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINOBJECTCONNECTORCARTESIANSPRINGDAMPERPARAMETERS__H
#define MAINOBJECTCONNECTORCARTESIANSPRINGDAMPERPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include <pybind11/functional.h> //! AUTO: for function handling ... otherwise gives a python error (no compilation error in C++ !)
#include "Autogenerated/CObjectConnectorCartesianSpringDamper.h"

#include "Autogenerated/VisuObjectConnectorCartesianSpringDamper.h"

//! AUTO: Parameters for class MainObjectConnectorCartesianSpringDamperParameters
class MainObjectConnectorCartesianSpringDamperParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectConnectorCartesianSpringDamper
* @brief        An 3D spring-damper element, providing springs and dampers in three (global) directions (x,y,z); the connector can be attached to position-based markers.
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

//! AUTO: MainObjectConnectorCartesianSpringDamper
class MainObjectConnectorCartesianSpringDamper: public MainObjectConnector // AUTO: 
{
protected: // AUTO: 
    CObjectConnectorCartesianSpringDamper* cObjectConnectorCartesianSpringDamper; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectConnectorCartesianSpringDamper* visualizationObjectConnectorCartesianSpringDamper; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectConnectorCartesianSpringDamper()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectConnectorCartesianSpringDamper* GetCObjectConnectorCartesianSpringDamper() { return cObjectConnectorCartesianSpringDamper; }
    //! AUTO: Get const pointer to computational class
    const CObjectConnectorCartesianSpringDamper* GetCObjectConnectorCartesianSpringDamper() const { return cObjectConnectorCartesianSpringDamper; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectConnectorCartesianSpringDamper(CObjectConnectorCartesianSpringDamper* pCObjectConnectorCartesianSpringDamper) { cObjectConnectorCartesianSpringDamper = pCObjectConnectorCartesianSpringDamper; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectConnectorCartesianSpringDamper* GetVisualizationObjectConnectorCartesianSpringDamper() { return visualizationObjectConnectorCartesianSpringDamper; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectConnectorCartesianSpringDamper* GetVisualizationObjectConnectorCartesianSpringDamper() const { return visualizationObjectConnectorCartesianSpringDamper; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectConnectorCartesianSpringDamper(VisualizationObjectConnectorCartesianSpringDamper* pVisualizationObjectConnectorCartesianSpringDamper) { visualizationObjectConnectorCartesianSpringDamper = pVisualizationObjectConnectorCartesianSpringDamper; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectConnectorCartesianSpringDamper; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectConnectorCartesianSpringDamper = (CObjectConnectorCartesianSpringDamper*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectConnectorCartesianSpringDamper; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectConnectorCartesianSpringDamper = (VisualizationObjectConnectorCartesianSpringDamper*)pVisualizationObject; }

    //! AUTO:  Get type name of node (without keyword 'Object'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "ConnectorCartesianSpringDamper";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectConnectorCartesianSpringDamper->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(d["markerNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "stiffness", cObjectConnectorCartesianSpringDamper->GetParameters().stiffness); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "damping")) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "damping", cObjectConnectorCartesianSpringDamper->GetParameters().damping); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "offset")) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "offset", cObjectConnectorCartesianSpringDamper->GetParameters().offset); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "springForceUserFunction")) { cObjectConnectorCartesianSpringDamper->GetParameters().springForceUserFunction = d["springForceUserFunction"]; /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "activeConnector")) { cObjectConnectorCartesianSpringDamper->GetParameters().activeConnector = py::cast<bool>(d["activeConnector"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectConnectorCartesianSpringDamper->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawSize")) { visualizationObjectConnectorCartesianSpringDamper->GetDrawSize() = py::cast<float>(d["VdrawSize"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectConnectorCartesianSpringDamper->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary(bool addGraphicsData=false) const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["markerNumbers"] = EPyUtils::GetArrayMarkerIndex(cObjectConnectorCartesianSpringDamper->GetParameters().markerNumbers); //! AUTO: cast variables into python (not needed for standard types) 
        d["stiffness"] = EPyUtils::SlimVector2NumPy(cObjectConnectorCartesianSpringDamper->GetParameters().stiffness); //! AUTO: cast variables into python (not needed for standard types) 
        d["damping"] = EPyUtils::SlimVector2NumPy(cObjectConnectorCartesianSpringDamper->GetParameters().damping); //! AUTO: cast variables into python (not needed for standard types) 
        d["offset"] = EPyUtils::SlimVector2NumPy(cObjectConnectorCartesianSpringDamper->GetParameters().offset); //! AUTO: cast variables into python (not needed for standard types) 
        d["springForceUserFunction"] = (py::object)cObjectConnectorCartesianSpringDamper->GetParameters().springForceUserFunction; //! AUTO: cast variables into python (not needed for standard types) 
        d["activeConnector"] = (bool)cObjectConnectorCartesianSpringDamper->GetParameters().activeConnector; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectConnectorCartesianSpringDamper->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawSize"] = (float)visualizationObjectConnectorCartesianSpringDamper->GetDrawSize(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectConnectorCartesianSpringDamper->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { return py::cast(EPyUtils::GetArrayMarkerIndex(cObjectConnectorCartesianSpringDamper->GetParameters().markerNumbers));} //! AUTO: get parameter
        else if (parameterName.compare("stiffness") == 0) { return EPyUtils::SlimVector2NumPy(cObjectConnectorCartesianSpringDamper->GetParameters().stiffness);} //! AUTO: get parameter
        else if (parameterName.compare("damping") == 0) { return EPyUtils::SlimVector2NumPy(cObjectConnectorCartesianSpringDamper->GetParameters().damping);} //! AUTO: get parameter
        else if (parameterName.compare("offset") == 0) { return EPyUtils::SlimVector2NumPy(cObjectConnectorCartesianSpringDamper->GetParameters().offset);} //! AUTO: get parameter
        else if (parameterName.compare("springForceUserFunction") == 0) { return cObjectConnectorCartesianSpringDamper->GetParameters().springForceUserFunction.GetPythonDictionary();;} //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { return py::cast((bool)cObjectConnectorCartesianSpringDamper->GetParameters().activeConnector);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectConnectorCartesianSpringDamper->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { return py::cast((float)visualizationObjectConnectorCartesianSpringDamper->GetDrawSize());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectConnectorCartesianSpringDamper->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectConnectorCartesianSpringDamper::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { cObjectConnectorCartesianSpringDamper->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("stiffness") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, cObjectConnectorCartesianSpringDamper->GetParameters().stiffness); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("damping") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, cObjectConnectorCartesianSpringDamper->GetParameters().damping); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("offset") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, cObjectConnectorCartesianSpringDamper->GetParameters().offset); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("springForceUserFunction") == 0) { cObjectConnectorCartesianSpringDamper->GetParameters().springForceUserFunction = value; } //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { cObjectConnectorCartesianSpringDamper->GetParameters().activeConnector = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectConnectorCartesianSpringDamper->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { visualizationObjectConnectorCartesianSpringDamper->GetDrawSize() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectConnectorCartesianSpringDamper->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectConnectorCartesianSpringDamper::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...
