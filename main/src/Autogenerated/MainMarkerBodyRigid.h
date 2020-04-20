/** ***********************************************************************************************
* @class        MainMarkerBodyRigidParameters
* @brief        Parameter class for MainMarkerBodyRigid
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-03-30  22:45:22 (last modfied)
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
#include "Autogenerated/CMarkerBodyRigid.h"

#include "Autogenerated/VisuMarkerBodyRigid.h"

//! AUTO: Parameters for class MainMarkerBodyRigidParameters
class MainMarkerBodyRigidParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainMarkerBodyRigid
* @brief        A rigid-body (position+orientation) body-marker attached to local position (x,y,z) of the body.
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

//! AUTO: MainMarkerBodyRigid
class MainMarkerBodyRigid: public MainMarker // AUTO: 
{
protected: // AUTO: 
    CMarkerBodyRigid* cMarkerBodyRigid; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationMarkerBodyRigid* visualizationMarkerBodyRigid; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainMarkerBodyRigid()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CMarkerBodyRigid* GetCMarkerBodyRigid() { return cMarkerBodyRigid; }
    //! AUTO: Get const pointer to computational class
    const CMarkerBodyRigid* GetCMarkerBodyRigid() const { return cMarkerBodyRigid; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCMarkerBodyRigid(CMarkerBodyRigid* pCMarkerBodyRigid) { cMarkerBodyRigid = pCMarkerBodyRigid; }

    //! AUTO: Get pointer to visualization class
    VisualizationMarkerBodyRigid* GetVisualizationMarkerBodyRigid() { return visualizationMarkerBodyRigid; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationMarkerBodyRigid* GetVisualizationMarkerBodyRigid() const { return visualizationMarkerBodyRigid; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationMarkerBodyRigid(VisualizationMarkerBodyRigid* pVisualizationMarkerBodyRigid) { visualizationMarkerBodyRigid = pVisualizationMarkerBodyRigid; }

    //! AUTO: Get const pointer to computational base class object
    virtual CMarker* GetCMarker() const { return cMarkerBodyRigid; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCMarker(CMarker* pCMarker) { cMarkerBodyRigid = (CMarkerBodyRigid*)pCMarker; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationMarker* GetVisualizationMarker() const { return visualizationMarkerBodyRigid; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationMarker(VisualizationMarker* pVisualizationMarker) { visualizationMarkerBodyRigid = (VisualizationMarkerBodyRigid*)pVisualizationMarker; }

    //! AUTO:  Get type name of marker (without keyword 'Marker'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "BodyRigid";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cMarkerBodyRigid->GetParameters().bodyNumber = py::cast<Index>(d["bodyNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetVector3DSafely(d, "localPosition", cMarkerBodyRigid->GetParameters().localPosition); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationMarkerBodyRigid->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["markerType"] = (std::string)GetTypeName();
        d["bodyNumber"] = (Index)cMarkerBodyRigid->GetParameters().bodyNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["localPosition"] = (std::vector<Real>)cMarkerBodyRigid->GetParameters().localPosition; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationMarkerBodyRigid->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("bodyNumber") == 0) { return py::cast((Index)cMarkerBodyRigid->GetParameters().bodyNumber);} //! AUTO: get parameter
        else if (parameterName.compare("localPosition") == 0) { return py::cast((std::vector<Real>)cMarkerBodyRigid->GetParameters().localPosition);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationMarkerBodyRigid->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("MarkerBodyRigid::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("bodyNumber") == 0) { cMarkerBodyRigid->GetParameters().bodyNumber = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("localPosition") == 0) { EPyUtils::SetVector3DSafely(value, cMarkerBodyRigid->GetParameters().localPosition); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationMarkerBodyRigid->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("MarkerBodyRigid::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


