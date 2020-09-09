/** ***********************************************************************************************
* @class        MainObjectConnectorCoordinateParameters
* @brief        Parameter class for MainObjectConnectorCoordinate
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-09-08  18:14:40 (last modfied)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINOBJECTCONNECTORCOORDINATEPARAMETERS__H
#define MAINOBJECTCONNECTORCOORDINATEPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include <pybind11/functional.h> //! AUTO: for function handling ... otherwise gives a python error (no compilation error in C++ !)
#include "Autogenerated/CObjectConnectorCoordinate.h"

#include "Autogenerated/VisuObjectConnectorCoordinate.h"

//! AUTO: Parameters for class MainObjectConnectorCoordinateParameters
class MainObjectConnectorCoordinateParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectConnectorCoordinate
* @brief        A coordinate constraint which constrains two (scalar) coordinates of Marker[Node|Body]Coordinates attached to nodes or bodies. The constraint acts directly on coordinates, but does not include reference values, e.g., of nodal values.
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

//! AUTO: MainObjectConnectorCoordinate
class MainObjectConnectorCoordinate: public MainObjectConnector // AUTO: 
{
protected: // AUTO: 
    CObjectConnectorCoordinate* cObjectConnectorCoordinate; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectConnectorCoordinate* visualizationObjectConnectorCoordinate; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectConnectorCoordinate()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectConnectorCoordinate* GetCObjectConnectorCoordinate() { return cObjectConnectorCoordinate; }
    //! AUTO: Get const pointer to computational class
    const CObjectConnectorCoordinate* GetCObjectConnectorCoordinate() const { return cObjectConnectorCoordinate; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectConnectorCoordinate(CObjectConnectorCoordinate* pCObjectConnectorCoordinate) { cObjectConnectorCoordinate = pCObjectConnectorCoordinate; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectConnectorCoordinate* GetVisualizationObjectConnectorCoordinate() { return visualizationObjectConnectorCoordinate; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectConnectorCoordinate* GetVisualizationObjectConnectorCoordinate() const { return visualizationObjectConnectorCoordinate; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectConnectorCoordinate(VisualizationObjectConnectorCoordinate* pVisualizationObjectConnectorCoordinate) { visualizationObjectConnectorCoordinate = pVisualizationObjectConnectorCoordinate; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectConnectorCoordinate; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectConnectorCoordinate = (CObjectConnectorCoordinate*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectConnectorCoordinate; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectConnectorCoordinate = (VisualizationObjectConnectorCoordinate*)pVisualizationObject; }

    //! AUTO:  Get type name of object (without keyword 'Object'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "ConnectorCoordinate";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectConnectorCoordinate->GetParameters().markerNumbers = py::cast<std::vector<Index>>(d["markerNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectConnectorCoordinate->GetParameters().offset = py::cast<Real>(d["offset"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectConnectorCoordinate->GetParameters().factorValue1 = py::cast<Real>(d["factorValue1"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectConnectorCoordinate->GetParameters().velocityLevel = py::cast<bool>(d["velocityLevel"]); /* AUTO:  read out dictionary and cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "offsetUserFunction")) { if (EPyUtils::CheckForValidFunction(d["offsetUserFunction"])) { cObjectConnectorCoordinate->GetParameters().offsetUserFunction = py::cast<std::function<Real(Real,Real)>>((py::function)d["offsetUserFunction"]); /* AUTO:  read out dictionary and cast to C++ type*/}} 
        if (EPyUtils::DictItemExists(d, "offsetUserFunction_t")) { if (EPyUtils::CheckForValidFunction(d["offsetUserFunction_t"])) { cObjectConnectorCoordinate->GetParameters().offsetUserFunction_t = py::cast<std::function<Real(Real,Real)>>((py::function)d["offsetUserFunction_t"]); /* AUTO:  read out dictionary and cast to C++ type*/}} 
        if (EPyUtils::DictItemExists(d, "activeConnector")) { cObjectConnectorCoordinate->GetParameters().activeConnector = py::cast<bool>(d["activeConnector"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectConnectorCoordinate->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawSize")) { visualizationObjectConnectorCoordinate->GetDrawSize() = py::cast<float>(d["VdrawSize"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectConnectorCoordinate->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["markerNumbers"] = (std::vector<Index>)cObjectConnectorCoordinate->GetParameters().markerNumbers; //! AUTO: cast variables into python (not needed for standard types) 
        d["offset"] = (Real)cObjectConnectorCoordinate->GetParameters().offset; //! AUTO: cast variables into python (not needed for standard types) 
        d["factorValue1"] = (Real)cObjectConnectorCoordinate->GetParameters().factorValue1; //! AUTO: cast variables into python (not needed for standard types) 
        d["velocityLevel"] = (bool)cObjectConnectorCoordinate->GetParameters().velocityLevel; //! AUTO: cast variables into python (not needed for standard types) 
        if (cObjectConnectorCoordinate->GetParameters().offsetUserFunction)
            {d["offsetUserFunction"] = (std::function<Real(Real,Real)>)cObjectConnectorCoordinate->GetParameters().offsetUserFunction;}
        else
            {d["offsetUserFunction"] = 0;}
 //! AUTO: cast variables into python (not needed for standard types) 
        if (cObjectConnectorCoordinate->GetParameters().offsetUserFunction_t)
            {d["offsetUserFunction_t"] = (std::function<Real(Real,Real)>)cObjectConnectorCoordinate->GetParameters().offsetUserFunction_t;}
        else
            {d["offsetUserFunction_t"] = 0;}
 //! AUTO: cast variables into python (not needed for standard types) 
        d["activeConnector"] = (bool)cObjectConnectorCoordinate->GetParameters().activeConnector; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectConnectorCoordinate->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawSize"] = (float)visualizationObjectConnectorCoordinate->GetDrawSize(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectConnectorCoordinate->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { return py::cast((std::vector<Index>)cObjectConnectorCoordinate->GetParameters().markerNumbers);} //! AUTO: get parameter
        else if (parameterName.compare("offset") == 0) { return py::cast((Real)cObjectConnectorCoordinate->GetParameters().offset);} //! AUTO: get parameter
        else if (parameterName.compare("factorValue1") == 0) { return py::cast((Real)cObjectConnectorCoordinate->GetParameters().factorValue1);} //! AUTO: get parameter
        else if (parameterName.compare("velocityLevel") == 0) { return py::cast((bool)cObjectConnectorCoordinate->GetParameters().velocityLevel);} //! AUTO: get parameter
        else if (parameterName.compare("offsetUserFunction") == 0) { return py::cast((std::function<Real(Real,Real)>)cObjectConnectorCoordinate->GetParameters().offsetUserFunction);} //! AUTO: get parameter
        else if (parameterName.compare("offsetUserFunction_t") == 0) { return py::cast((std::function<Real(Real,Real)>)cObjectConnectorCoordinate->GetParameters().offsetUserFunction_t);} //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { return py::cast((bool)cObjectConnectorCoordinate->GetParameters().activeConnector);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectConnectorCoordinate->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { return py::cast((float)visualizationObjectConnectorCoordinate->GetDrawSize());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectConnectorCoordinate->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectConnectorCoordinate::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { cObjectConnectorCoordinate->GetParameters().markerNumbers = py::cast<std::vector<Index>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("offset") == 0) { cObjectConnectorCoordinate->GetParameters().offset = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("factorValue1") == 0) { cObjectConnectorCoordinate->GetParameters().factorValue1 = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("velocityLevel") == 0) { cObjectConnectorCoordinate->GetParameters().velocityLevel = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("offsetUserFunction") == 0) { cObjectConnectorCoordinate->GetParameters().offsetUserFunction = py::cast<std::function<Real(Real,Real)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("offsetUserFunction_t") == 0) { cObjectConnectorCoordinate->GetParameters().offsetUserFunction_t = py::cast<std::function<Real(Real,Real)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { cObjectConnectorCoordinate->GetParameters().activeConnector = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectConnectorCoordinate->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { visualizationObjectConnectorCoordinate->GetDrawSize() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectConnectorCoordinate->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectConnectorCoordinate::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...
