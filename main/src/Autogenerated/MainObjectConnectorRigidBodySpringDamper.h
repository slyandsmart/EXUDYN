/** ***********************************************************************************************
* @class        MainObjectConnectorRigidBodySpringDamperParameters
* @brief        Parameter class for MainObjectConnectorRigidBodySpringDamper
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-04-08  14:46:12 (last modfied)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */
#pragma once

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CObjectConnectorRigidBodySpringDamper.h"

#include "Autogenerated/VisuObjectConnectorRigidBodySpringDamper.h"

//! AUTO: Parameters for class MainObjectConnectorRigidBodySpringDamperParameters
class MainObjectConnectorRigidBodySpringDamperParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectConnectorRigidBodySpringDamper
* @brief        An 3D spring-damper element acting on relative displacements and relative rotations of two rigid body (position+orientation) markers; connects to (position+orientation)-based markers and represents a penalty-based rigid joint (or prismatic, revolute, etc.)
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */
#pragma once

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

//! AUTO: MainObjectConnectorRigidBodySpringDamper
class MainObjectConnectorRigidBodySpringDamper: public MainObjectConnector // AUTO: 
{
protected: // AUTO: 
    CObjectConnectorRigidBodySpringDamper* cObjectConnectorRigidBodySpringDamper; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectConnectorRigidBodySpringDamper* visualizationObjectConnectorRigidBodySpringDamper; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectConnectorRigidBodySpringDamper()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectConnectorRigidBodySpringDamper* GetCObjectConnectorRigidBodySpringDamper() { return cObjectConnectorRigidBodySpringDamper; }
    //! AUTO: Get const pointer to computational class
    const CObjectConnectorRigidBodySpringDamper* GetCObjectConnectorRigidBodySpringDamper() const { return cObjectConnectorRigidBodySpringDamper; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectConnectorRigidBodySpringDamper(CObjectConnectorRigidBodySpringDamper* pCObjectConnectorRigidBodySpringDamper) { cObjectConnectorRigidBodySpringDamper = pCObjectConnectorRigidBodySpringDamper; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectConnectorRigidBodySpringDamper* GetVisualizationObjectConnectorRigidBodySpringDamper() { return visualizationObjectConnectorRigidBodySpringDamper; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectConnectorRigidBodySpringDamper* GetVisualizationObjectConnectorRigidBodySpringDamper() const { return visualizationObjectConnectorRigidBodySpringDamper; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectConnectorRigidBodySpringDamper(VisualizationObjectConnectorRigidBodySpringDamper* pVisualizationObjectConnectorRigidBodySpringDamper) { visualizationObjectConnectorRigidBodySpringDamper = pVisualizationObjectConnectorRigidBodySpringDamper; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectConnectorRigidBodySpringDamper; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectConnectorRigidBodySpringDamper = (CObjectConnectorRigidBodySpringDamper*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectConnectorRigidBodySpringDamper; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectConnectorRigidBodySpringDamper = (VisualizationObjectConnectorRigidBodySpringDamper*)pVisualizationObject; }

    //! AUTO:  Get type name of node (without keyword 'Object'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "ConnectorRigidBodySpringDamper";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectConnectorRigidBodySpringDamper->GetParameters().markerNumbers = py::cast<std::vector<Index>>(d["markerNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetMatrix6DSafely(d, "stiffness", cObjectConnectorRigidBodySpringDamper->GetParameters().stiffness); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetMatrix6DSafely(d, "damping", cObjectConnectorRigidBodySpringDamper->GetParameters().damping); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetMatrix3DSafely(d, "rotationMarker0", cObjectConnectorRigidBodySpringDamper->GetParameters().rotationMarker0); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetMatrix3DSafely(d, "rotationMarker1", cObjectConnectorRigidBodySpringDamper->GetParameters().rotationMarker1); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "offset")) { EPyUtils::SetVector6DSafely(d, "offset", cObjectConnectorRigidBodySpringDamper->GetParameters().offset); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "activeConnector")) { cObjectConnectorRigidBodySpringDamper->GetParameters().activeConnector = py::cast<bool>(d["activeConnector"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectConnectorRigidBodySpringDamper->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawSize")) { visualizationObjectConnectorRigidBodySpringDamper->GetDrawSize() = py::cast<float>(d["VdrawSize"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectConnectorRigidBodySpringDamper->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["markerNumbers"] = (std::vector<Index>)cObjectConnectorRigidBodySpringDamper->GetParameters().markerNumbers; //! AUTO: cast variables into python (not needed for standard types) 
        d["stiffness"] = EXUmath::Matrix6DToStdArray66(cObjectConnectorRigidBodySpringDamper->GetParameters().stiffness); //! AUTO: cast variables into python (not needed for standard types) 
        d["damping"] = EXUmath::Matrix6DToStdArray66(cObjectConnectorRigidBodySpringDamper->GetParameters().damping); //! AUTO: cast variables into python (not needed for standard types) 
        d["rotationMarker0"] = EXUmath::Matrix3DToStdArray33(cObjectConnectorRigidBodySpringDamper->GetParameters().rotationMarker0); //! AUTO: cast variables into python (not needed for standard types) 
        d["rotationMarker1"] = EXUmath::Matrix3DToStdArray33(cObjectConnectorRigidBodySpringDamper->GetParameters().rotationMarker1); //! AUTO: cast variables into python (not needed for standard types) 
        d["offset"] = (std::vector<Real>)cObjectConnectorRigidBodySpringDamper->GetParameters().offset; //! AUTO: cast variables into python (not needed for standard types) 
        d["activeConnector"] = (bool)cObjectConnectorRigidBodySpringDamper->GetParameters().activeConnector; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectConnectorRigidBodySpringDamper->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawSize"] = (float)visualizationObjectConnectorRigidBodySpringDamper->GetDrawSize(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectConnectorRigidBodySpringDamper->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { return py::cast((std::vector<Index>)cObjectConnectorRigidBodySpringDamper->GetParameters().markerNumbers);} //! AUTO: get parameter
        else if (parameterName.compare("stiffness") == 0) { return py::cast(EXUmath::Matrix6DToStdArray66(cObjectConnectorRigidBodySpringDamper->GetParameters().stiffness));} //! AUTO: get parameter
        else if (parameterName.compare("damping") == 0) { return py::cast(EXUmath::Matrix6DToStdArray66(cObjectConnectorRigidBodySpringDamper->GetParameters().damping));} //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker0") == 0) { return py::cast(EXUmath::Matrix3DToStdArray33(cObjectConnectorRigidBodySpringDamper->GetParameters().rotationMarker0));} //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker1") == 0) { return py::cast(EXUmath::Matrix3DToStdArray33(cObjectConnectorRigidBodySpringDamper->GetParameters().rotationMarker1));} //! AUTO: get parameter
        else if (parameterName.compare("offset") == 0) { return py::cast((std::vector<Real>)cObjectConnectorRigidBodySpringDamper->GetParameters().offset);} //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { return py::cast((bool)cObjectConnectorRigidBodySpringDamper->GetParameters().activeConnector);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectConnectorRigidBodySpringDamper->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { return py::cast((float)visualizationObjectConnectorRigidBodySpringDamper->GetDrawSize());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectConnectorRigidBodySpringDamper->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectConnectorRigidBodySpringDamper::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { cObjectConnectorRigidBodySpringDamper->GetParameters().markerNumbers = py::cast<std::vector<Index>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("stiffness") == 0) { EPyUtils::SetMatrix6DSafely(value, cObjectConnectorRigidBodySpringDamper->GetParameters().stiffness); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("damping") == 0) { EPyUtils::SetMatrix6DSafely(value, cObjectConnectorRigidBodySpringDamper->GetParameters().damping); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker0") == 0) { EPyUtils::SetMatrix3DSafely(value, cObjectConnectorRigidBodySpringDamper->GetParameters().rotationMarker0); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker1") == 0) { EPyUtils::SetMatrix3DSafely(value, cObjectConnectorRigidBodySpringDamper->GetParameters().rotationMarker1); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("offset") == 0) { EPyUtils::SetVector6DSafely(value, cObjectConnectorRigidBodySpringDamper->GetParameters().offset); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { cObjectConnectorRigidBodySpringDamper->GetParameters().activeConnector = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectConnectorRigidBodySpringDamper->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { visualizationObjectConnectorRigidBodySpringDamper->GetDrawSize() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectConnectorRigidBodySpringDamper->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectConnectorRigidBodySpringDamper::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};


