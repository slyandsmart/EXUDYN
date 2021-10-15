/** ***********************************************************************************************
* @class        MainObjectJointRevoluteZParameters
* @brief        Parameter class for MainObjectJointRevoluteZ
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2021-08-11  16:20:59 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINOBJECTJOINTREVOLUTEZPARAMETERS__H
#define MAINOBJECTJOINTREVOLUTEZPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CObjectJointRevoluteZ.h"

#include "Autogenerated/VisuObjectJointRevoluteZ.h"

//! AUTO: Parameters for class MainObjectJointRevoluteZParameters
class MainObjectJointRevoluteZParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectJointRevoluteZ
* @brief        A revolute joint in 3D; constrains the position of two rigid body markers and the rotation about two axes, while the joint \f$z\f$-rotation axis (defined in local coordinates of marker 0 / joint J0 coordinates) can freely rotate. An additional local rotation (rotationMarker) can be used to transform the markers' coordinate systems into the joint coordinate system. For easier definition of the joint, use the exudyn.rigidbodyUtilities function AddRevoluteJoint(...), \refSection{sec:rigidBodyUtilities:AddRevoluteJoint}, for two rigid bodies (or ground). \addExampleImage{RevoluteJointZ}
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

//! AUTO: MainObjectJointRevoluteZ
class MainObjectJointRevoluteZ: public MainObjectConnector // AUTO: 
{
protected: // AUTO: 
    CObjectJointRevoluteZ* cObjectJointRevoluteZ; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectJointRevoluteZ* visualizationObjectJointRevoluteZ; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectJointRevoluteZ()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectJointRevoluteZ* GetCObjectJointRevoluteZ() { return cObjectJointRevoluteZ; }
    //! AUTO: Get const pointer to computational class
    const CObjectJointRevoluteZ* GetCObjectJointRevoluteZ() const { return cObjectJointRevoluteZ; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectJointRevoluteZ(CObjectJointRevoluteZ* pCObjectJointRevoluteZ) { cObjectJointRevoluteZ = pCObjectJointRevoluteZ; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectJointRevoluteZ* GetVisualizationObjectJointRevoluteZ() { return visualizationObjectJointRevoluteZ; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectJointRevoluteZ* GetVisualizationObjectJointRevoluteZ() const { return visualizationObjectJointRevoluteZ; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectJointRevoluteZ(VisualizationObjectJointRevoluteZ* pVisualizationObjectJointRevoluteZ) { visualizationObjectJointRevoluteZ = pVisualizationObjectJointRevoluteZ; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectJointRevoluteZ; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectJointRevoluteZ = (CObjectJointRevoluteZ*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectJointRevoluteZ; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectJointRevoluteZ = (VisualizationObjectJointRevoluteZ*)pVisualizationObject; }

    //! AUTO:  Get type name of object (without keyword 'Object'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "JointRevolute";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectJointRevoluteZ->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(d["markerNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetMatrix3DSafely(d, "rotationMarker0", cObjectJointRevoluteZ->GetParameters().rotationMarker0); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetMatrix3DSafely(d, "rotationMarker1", cObjectJointRevoluteZ->GetParameters().rotationMarker1); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "activeConnector")) { cObjectJointRevoluteZ->GetParameters().activeConnector = py::cast<bool>(d["activeConnector"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectJointRevoluteZ->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VaxisRadius")) { visualizationObjectJointRevoluteZ->GetAxisRadius() = py::cast<float>(d["VaxisRadius"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VaxisLength")) { visualizationObjectJointRevoluteZ->GetAxisLength() = py::cast<float>(d["VaxisLength"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectJointRevoluteZ->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["markerNumbers"] = EPyUtils::GetArrayMarkerIndex(cObjectJointRevoluteZ->GetParameters().markerNumbers); //! AUTO: cast variables into python (not needed for standard types) 
        d["rotationMarker0"] = EXUmath::Matrix3DToStdArray33(cObjectJointRevoluteZ->GetParameters().rotationMarker0); //! AUTO: cast variables into python (not needed for standard types) 
        d["rotationMarker1"] = EXUmath::Matrix3DToStdArray33(cObjectJointRevoluteZ->GetParameters().rotationMarker1); //! AUTO: cast variables into python (not needed for standard types) 
        d["activeConnector"] = (bool)cObjectJointRevoluteZ->GetParameters().activeConnector; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectJointRevoluteZ->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VaxisRadius"] = (float)visualizationObjectJointRevoluteZ->GetAxisRadius(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VaxisLength"] = (float)visualizationObjectJointRevoluteZ->GetAxisLength(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectJointRevoluteZ->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { return py::cast(EPyUtils::GetArrayMarkerIndex(cObjectJointRevoluteZ->GetParameters().markerNumbers));} //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker0") == 0) { return py::cast(EXUmath::Matrix3DToStdArray33(cObjectJointRevoluteZ->GetParameters().rotationMarker0));} //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker1") == 0) { return py::cast(EXUmath::Matrix3DToStdArray33(cObjectJointRevoluteZ->GetParameters().rotationMarker1));} //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { return py::cast((bool)cObjectJointRevoluteZ->GetParameters().activeConnector);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectJointRevoluteZ->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VaxisRadius") == 0) { return py::cast((float)visualizationObjectJointRevoluteZ->GetAxisRadius());} //! AUTO: get parameter
        else if (parameterName.compare("VaxisLength") == 0) { return py::cast((float)visualizationObjectJointRevoluteZ->GetAxisLength());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectJointRevoluteZ->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectJointRevoluteZ::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { cObjectJointRevoluteZ->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker0") == 0) { EPyUtils::SetMatrix3DSafely(value, cObjectJointRevoluteZ->GetParameters().rotationMarker0); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker1") == 0) { EPyUtils::SetMatrix3DSafely(value, cObjectJointRevoluteZ->GetParameters().rotationMarker1); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { cObjectJointRevoluteZ->GetParameters().activeConnector = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectJointRevoluteZ->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VaxisRadius") == 0) { visualizationObjectJointRevoluteZ->GetAxisRadius() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VaxisLength") == 0) { visualizationObjectJointRevoluteZ->GetAxisLength() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectJointRevoluteZ->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectJointRevoluteZ::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...