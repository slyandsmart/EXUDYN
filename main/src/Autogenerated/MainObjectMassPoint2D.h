/** ***********************************************************************************************
* @class        MainObjectMassPoint2DParameters
* @brief        Parameter class for MainObjectMassPoint2D
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-01-28  08:47:25 (last modfied)
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
#include "Autogenerated/CObjectMassPoint2D.h"

#include "Autogenerated/VisuObjectMassPoint2D.h"

//! AUTO: Parameters for class MainObjectMassPoint2DParameters
class MainObjectMassPoint2DParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectMassPoint2D
* @brief        A 2D mass point which is attached to a position-based 2D node. Equations of motion with the displacements \f$[u_x\;\; u_y]^T\f$, the mass \f$m\f$ and the residual of all forces \f$[R_x\;\; R_y]^T\f$ are given as \f[ \vp{m \cdot \ddot u_x}{m \cdot \ddot u_y} = \vp{R_x}{R_y}.\f]
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

//! AUTO: MainObjectMassPoint2D
class MainObjectMassPoint2D: public MainObjectBody // AUTO: 
{
protected: // AUTO: 
    CObjectMassPoint2D* cObjectMassPoint2D; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectMassPoint2D* visualizationObjectMassPoint2D; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectMassPoint2D()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectMassPoint2D* GetCObjectMassPoint2D() { return cObjectMassPoint2D; }
    //! AUTO: Get const pointer to computational class
    const CObjectMassPoint2D* GetCObjectMassPoint2D() const { return cObjectMassPoint2D; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectMassPoint2D(CObjectMassPoint2D* pCObjectMassPoint2D) { cObjectMassPoint2D = pCObjectMassPoint2D; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectMassPoint2D* GetVisualizationObjectMassPoint2D() { return visualizationObjectMassPoint2D; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectMassPoint2D* GetVisualizationObjectMassPoint2D() const { return visualizationObjectMassPoint2D; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectMassPoint2D(VisualizationObjectMassPoint2D* pVisualizationObjectMassPoint2D) { visualizationObjectMassPoint2D = pVisualizationObjectMassPoint2D; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectMassPoint2D; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectMassPoint2D = (CObjectMassPoint2D*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectMassPoint2D; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectMassPoint2D = (VisualizationObjectMassPoint2D*)pVisualizationObject; }

    //! AUTO:  Get type name of object; could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "MassPoint2D";
    }

    //! AUTO:  provide requested nodeType for objects; used for automatic checks in CheckSystemIntegrity()
    virtual Node::Type GetRequestedNodeType() const override
    {
        return Node::Position2D;
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectMassPoint2D->GetParameters().physicsMass = py::cast<Real>(d["physicsMass"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectMassPoint2D->GetParameters().nodeNumber = py::cast<Index>(d["nodeNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectMassPoint2D->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VgraphicsData")) { PyWriteBodyGraphicsData(d, "VgraphicsData", visualizationObjectMassPoint2D->GetGraphicsData()); /*! AUTO: convert dict to BodyGraphicsData*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["physicsMass"] = (Real)cObjectMassPoint2D->GetParameters().physicsMass; //! AUTO: cast variables into python (not needed for standard types) 
        d["nodeNumber"] = (Index)cObjectMassPoint2D->GetParameters().nodeNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectMassPoint2D->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VgraphicsData"] = PyGetBodyGraphicsDataDictionary(visualizationObjectMassPoint2D->GetGraphicsData()); //! AUTO: generate dictionary with special function
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("physicsMass") == 0) { return py::cast((Real)cObjectMassPoint2D->GetParameters().physicsMass);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { return py::cast((Index)cObjectMassPoint2D->GetParameters().nodeNumber);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectMassPoint2D->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectMassPoint2D::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsMass") == 0) { cObjectMassPoint2D->GetParameters().physicsMass = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { cObjectMassPoint2D->GetParameters().nodeNumber = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectMassPoint2D->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectMassPoint2D::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


