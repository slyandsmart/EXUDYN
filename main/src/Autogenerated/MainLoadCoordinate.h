/** ***********************************************************************************************
* @class        MainLoadCoordinateParameters
* @brief        Parameter class for MainLoadCoordinate
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
#include <pybind11/functional.h> //! AUTO: for function handling ... otherwise gives a python error (no compilation error in C++ !)
#include "Autogenerated/CLoadCoordinate.h"

#include "Autogenerated/VisuLoadCoordinate.h"

//! AUTO: Parameters for class MainLoadCoordinateParameters
class MainLoadCoordinateParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainLoadCoordinate
* @brief        Load with scalar value, which is attached to a coordinate-based marker; the load can be used e.g. to apply a force to a single axis of a body, a nodal coordinate of a finite element  or a torque to the rotatory DOF of a rigid body.
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

//! AUTO: MainLoadCoordinate
class MainLoadCoordinate: public MainLoad // AUTO: 
{
protected: // AUTO: 
    CLoadCoordinate* cLoadCoordinate; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationLoadCoordinate* visualizationLoadCoordinate; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainLoadCoordinate()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CLoadCoordinate* GetCLoadCoordinate() { return cLoadCoordinate; }
    //! AUTO: Get const pointer to computational class
    const CLoadCoordinate* GetCLoadCoordinate() const { return cLoadCoordinate; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCLoadCoordinate(CLoadCoordinate* pCLoadCoordinate) { cLoadCoordinate = pCLoadCoordinate; }

    //! AUTO: Get pointer to visualization class
    VisualizationLoadCoordinate* GetVisualizationLoadCoordinate() { return visualizationLoadCoordinate; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationLoadCoordinate* GetVisualizationLoadCoordinate() const { return visualizationLoadCoordinate; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationLoadCoordinate(VisualizationLoadCoordinate* pVisualizationLoadCoordinate) { visualizationLoadCoordinate = pVisualizationLoadCoordinate; }

    //! AUTO: Get const pointer to computational base class object
    virtual CLoad* GetCLoad() const { return cLoadCoordinate; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCLoad(CLoad* pCLoad) { cLoadCoordinate = (CLoadCoordinate*)pCLoad; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationLoad* GetVisualizationLoad() const { return visualizationLoadCoordinate; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationLoad(VisualizationLoad* pVisualizationLoad) { visualizationLoadCoordinate = (VisualizationLoadCoordinate*)pVisualizationLoad; }

    //! AUTO:  Get type name of load (without keyword 'Load'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "Coordinate";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cLoadCoordinate->GetParameters().markerNumber = py::cast<Index>(d["markerNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cLoadCoordinate->GetParameters().load = py::cast<Real>(d["load"]); /* AUTO:  read out dictionary and cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "loadUserFunction")) { if (EPyUtils::CheckForValidFunction(d["loadUserFunction"])) { cLoadCoordinate->GetParameters().loadUserFunction = py::cast<std::function<Real(Real,Real)>>((py::function)d["loadUserFunction"]); /* AUTO:  read out dictionary and cast to C++ type*/}} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationLoadCoordinate->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["loadType"] = (std::string)GetTypeName();
        d["markerNumber"] = (Index)cLoadCoordinate->GetParameters().markerNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["load"] = (Real)cLoadCoordinate->GetParameters().load; //! AUTO: cast variables into python (not needed for standard types) 
        d["loadUserFunction"] = (std::function<Real(Real,Real)>)cLoadCoordinate->GetParameters().loadUserFunction; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationLoadCoordinate->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumber") == 0) { return py::cast((Index)cLoadCoordinate->GetParameters().markerNumber);} //! AUTO: get parameter
        else if (parameterName.compare("load") == 0) { return py::cast((Real)cLoadCoordinate->GetParameters().load);} //! AUTO: get parameter
        else if (parameterName.compare("loadUserFunction") == 0) { return py::cast((std::function<Real(Real,Real)>)cLoadCoordinate->GetParameters().loadUserFunction);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationLoadCoordinate->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("LoadCoordinate::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumber") == 0) { cLoadCoordinate->GetParameters().markerNumber = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("load") == 0) { cLoadCoordinate->GetParameters().load = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("loadUserFunction") == 0) { cLoadCoordinate->GetParameters().loadUserFunction = py::cast<std::function<Real(Real,Real)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationLoadCoordinate->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("LoadCoordinate::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


