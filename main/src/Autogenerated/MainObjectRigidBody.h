/** ***********************************************************************************************
* @class        MainObjectRigidBodyParameters
* @brief        Parameter class for MainObjectRigidBody
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2023-12-03  23:26:45 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINOBJECTRIGIDBODYPARAMETERS__H
#define MAINOBJECTRIGIDBODYPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include <pybind11/functional.h> //! AUTO: for function handling ... otherwise gives a python error (no compilation error in C++ !)
#include "Autogenerated/CObjectRigidBody.h"

#include "Autogenerated/VisuObjectRigidBody.h"

//! AUTO: Parameters for class MainObjectRigidBodyParameters
class MainObjectRigidBodyParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectRigidBody
* @brief        A 3D rigid body which is attached to a 3D rigid body node. The rotation parametrization of the rigid body follows the rotation parametrization of the node. Use Euler parameters in the general case (no singularities) in combination with implicit solvers (GeneralizedAlpha or TrapezoidalIndex2), Tait-Bryan angles for special cases, e.g., rotors where no singularities occur if you rotate about \f$x\f$ or \f$z\f$ axis, or use Lie-group formulation with rotation vector together with explicit solvers. REMARK: Use the class \texttt{RigidBodyInertia}, see \refSection{sec:rigidBodyUtilities:RigidBodyInertia:__init__} and \texttt{AddRigidBody(...)}, see \refSection{sec:rigidBodyUtilities:AddRigidBody}, of \texttt{exudyn.rigidBodyUtilities} to handle inertia, \hac{COM} and mass. \addExampleImage{ObjectRigidBody}
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
        EPyUtils::SetSlimVectorTemplateSafely<Real, 6>(d, "physicsInertia", cObjectRigidBody->GetParameters().physicsInertia); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "physicsCenterOfMass")) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "physicsCenterOfMass", cObjectRigidBody->GetParameters().physicsCenterOfMass); /*! AUTO:  safely cast to C++ type*/} 
        cObjectRigidBody->GetParameters().nodeNumber = EPyUtils::GetNodeIndexSafely(d["nodeNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectRigidBody->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VgraphicsDataUserFunction")) { if (EPyUtils::CheckForValidFunction(d["VgraphicsDataUserFunction"])) 
            { visualizationObjectRigidBody->GetGraphicsDataUserFunction() = py::cast<std::function<py::object(const MainSystem&,Index)>>((py::function)d["VgraphicsDataUserFunction"]); /* AUTO:  read out dictionary and cast to C++ type*/}
            else {visualizationObjectRigidBody->GetGraphicsDataUserFunction() = 0;  /*AUTO: otherwise assign with zero!*/ }} 
        if (EPyUtils::DictItemExists(d, "VgraphicsData")) { PyWriteBodyGraphicsDataList(d, "VgraphicsData", visualizationObjectRigidBody->GetGraphicsData()); /*! AUTO: convert dict to BodyGraphicsData*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary(bool addGraphicsData=false) const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["physicsMass"] = (Real)cObjectRigidBody->GetParameters().physicsMass; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsInertia"] = EPyUtils::SlimVector2NumPy(cObjectRigidBody->GetParameters().physicsInertia); //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsCenterOfMass"] = EPyUtils::SlimVector2NumPy(cObjectRigidBody->GetParameters().physicsCenterOfMass); //! AUTO: cast variables into python (not needed for standard types) 
        d["nodeNumber"] = (NodeIndex)cObjectRigidBody->GetParameters().nodeNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectRigidBody->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        if (visualizationObjectRigidBody->GetGraphicsDataUserFunction())
            {d["VgraphicsDataUserFunction"] = (std::function<py::object(const MainSystem&,Index)>)visualizationObjectRigidBody->GetGraphicsDataUserFunction();}
        else
            {d["VgraphicsDataUserFunction"] = 0;}
 //! AUTO: cast variables into python (not needed for standard types) 
        d["VgraphicsData"] = PyGetBodyGraphicsDataList(visualizationObjectRigidBody->GetGraphicsData(), addGraphicsData); //! AUTO: generate dictionary with special function
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("physicsMass") == 0) { return py::cast((Real)cObjectRigidBody->GetParameters().physicsMass);} //! AUTO: get parameter
        else if (parameterName.compare("physicsInertia") == 0) { return EPyUtils::SlimVector2NumPy(cObjectRigidBody->GetParameters().physicsInertia);} //! AUTO: get parameter
        else if (parameterName.compare("physicsCenterOfMass") == 0) { return EPyUtils::SlimVector2NumPy(cObjectRigidBody->GetParameters().physicsCenterOfMass);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { return py::cast((NodeIndex)cObjectRigidBody->GetParameters().nodeNumber);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectRigidBody->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VgraphicsDataUserFunction") == 0) { return py::cast((std::function<py::object(const MainSystem&,Index)>)visualizationObjectRigidBody->GetGraphicsDataUserFunction());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectRigidBody::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsMass") == 0) { cObjectRigidBody->GetParameters().physicsMass = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsInertia") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 6>(value, cObjectRigidBody->GetParameters().physicsInertia); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsCenterOfMass") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, cObjectRigidBody->GetParameters().physicsCenterOfMass); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { cObjectRigidBody->GetParameters().nodeNumber = EPyUtils::GetNodeIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectRigidBody->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VgraphicsDataUserFunction") == 0) { if (py::isinstance<py::function>(value)) {visualizationObjectRigidBody->GetGraphicsDataUserFunction() = py::cast<std::function<py::object(const MainSystem&,Index)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/} else
            if (!EPyUtils::IsPyTypeInteger(value) || (py::cast<int>(value) != 0)) {PyError(STDstring("Failed to convert PyFunction: must be either valid python function or 0, but got ")+EXUstd::ToString(value)); }; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectRigidBody::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...
