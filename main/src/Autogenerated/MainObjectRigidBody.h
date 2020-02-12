/** ***********************************************************************************************
* @class        MainObjectRigidBodyParameters
* @brief        Parameter class for MainObjectRigidBody
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-02-04  14:23:17 (last modfied)
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
#include "Autogenerated/CObjectRigidBody.h"

#include "Autogenerated/VisuObjectRigidBody.h"

//! AUTO: Parameters for class MainObjectRigidBodyParameters
class MainObjectRigidBodyParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectRigidBody
* @brief        A 3D rigid body which is attached to a 3D rigid body node. Equations of motion with the displacements \f$[u_x\;\; u_y\;\; u_z]^T\f$ of the center of mass and the rotation parameters (Euler parameters) \f$\mathbf{q}\f$, the mass \f$m\f$, inertia \f$\mathbf{J} = [J_{xx}, J_{xy}, J_{xz}; J_{yx}, J_{yy}, J_{yz}; J_{zx}, J_{zy}, J_{zz}]\f$ and the residual of all forces and moments \f$[R_x\;\; R_y\;\; R_z\;\; R_{q0}\;\; R_{q1}\;\; R_{q2}\;\; R_{q3}]^T\f$ are given as ...
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

//! AUTO: MainObjectRigidBody
class MainObjectRigidBody: public MainObjectBody // AUTO: 
{
protected: // AUTO: 
    CObjectRigidBody* cObjectRigidBody; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectRigidBody* visualizationObjectRigidBody; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectRigidBody()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectRigidBody* GetCObjectRigidBody() { return cObjectRigidBody; }
    //! AUTO: Get const pointer to computational class
    const CObjectRigidBody* GetCObjectRigidBody() const { return cObjectRigidBody; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectRigidBody(CObjectRigidBody* pCObjectRigidBody) { cObjectRigidBody = pCObjectRigidBody; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectRigidBody* GetVisualizationObjectRigidBody() { return visualizationObjectRigidBody; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectRigidBody* GetVisualizationObjectRigidBody() const { return visualizationObjectRigidBody; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectRigidBody(VisualizationObjectRigidBody* pVisualizationObjectRigidBody) { visualizationObjectRigidBody = pVisualizationObjectRigidBody; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectRigidBody; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectRigidBody = (CObjectRigidBody*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectRigidBody; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectRigidBody = (VisualizationObjectRigidBody*)pVisualizationObject; }

    //! AUTO:  Get type name of object; could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "RigidBody";
    }

    //! AUTO:  provide requested nodeType for objects; used for automatic checks in CheckSystemIntegrity()
    virtual Node::Type GetRequestedNodeType() const override
    {
        return (Node::Type)(Node::Position + Node::Orientation + Node::RigidBody);
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectRigidBody->GetParameters().physicsMass = py::cast<Real>(d["physicsMass"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetVector6DSafely(d, "physicsInertia", cObjectRigidBody->GetParameters().physicsInertia); /*! AUTO:  safely cast to C++ type*/
        cObjectRigidBody->GetParameters().nodeNumber = py::cast<Index>(d["nodeNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectRigidBody->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VgraphicsData")) { PyWriteBodyGraphicsData(d, "VgraphicsData", visualizationObjectRigidBody->GetGraphicsData()); /*! AUTO: convert dict to BodyGraphicsData*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["physicsMass"] = (Real)cObjectRigidBody->GetParameters().physicsMass; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsInertia"] = (std::vector<Real>)cObjectRigidBody->GetParameters().physicsInertia; //! AUTO: cast variables into python (not needed for standard types) 
        d["nodeNumber"] = (Index)cObjectRigidBody->GetParameters().nodeNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectRigidBody->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VgraphicsData"] = PyGetBodyGraphicsDataDictionary(visualizationObjectRigidBody->GetGraphicsData()); //! AUTO: generate dictionary with special function
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("physicsMass") == 0) { return py::cast((Real)cObjectRigidBody->GetParameters().physicsMass);} //! AUTO: get parameter
        else if (parameterName.compare("physicsInertia") == 0) { return py::cast((std::vector<Real>)cObjectRigidBody->GetParameters().physicsInertia);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { return py::cast((Index)cObjectRigidBody->GetParameters().nodeNumber);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectRigidBody->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectRigidBody::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsMass") == 0) { cObjectRigidBody->GetParameters().physicsMass = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsInertia") == 0) { EPyUtils::SetVector6DSafely(value, cObjectRigidBody->GetParameters().physicsInertia); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { cObjectRigidBody->GetParameters().nodeNumber = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectRigidBody->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectRigidBody::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


