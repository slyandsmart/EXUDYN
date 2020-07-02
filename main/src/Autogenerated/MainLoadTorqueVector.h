/** ***********************************************************************************************
* @class        MainLoadTorqueVectorParameters
* @brief        Parameter class for MainLoadTorqueVector
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-06-24  22:25:49 (last modfied)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */
#pragma once

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include <pybind11/functional.h> //! AUTO: for function handling ... otherwise gives a python error (no compilation error in C++ !)
#include "Autogenerated/CLoadTorqueVector.h"

#include "Autogenerated/VisuLoadTorqueVector.h"

//! AUTO: Parameters for class MainLoadTorqueVectorParameters
class MainLoadTorqueVectorParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainLoadTorqueVector
* @brief        Load with (3D) torque vector; attached to rigidbody-based marker.
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */
#pragma once

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

//! AUTO: MainLoadTorqueVector
class MainLoadTorqueVector: public MainLoad // AUTO: 
{
protected: // AUTO: 
    CLoadTorqueVector* cLoadTorqueVector; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationLoadTorqueVector* visualizationLoadTorqueVector; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainLoadTorqueVector()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CLoadTorqueVector* GetCLoadTorqueVector() { return cLoadTorqueVector; }
    //! AUTO: Get const pointer to computational class
    const CLoadTorqueVector* GetCLoadTorqueVector() const { return cLoadTorqueVector; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCLoadTorqueVector(CLoadTorqueVector* pCLoadTorqueVector) { cLoadTorqueVector = pCLoadTorqueVector; }

    //! AUTO: Get pointer to visualization class
    VisualizationLoadTorqueVector* GetVisualizationLoadTorqueVector() { return visualizationLoadTorqueVector; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationLoadTorqueVector* GetVisualizationLoadTorqueVector() const { return visualizationLoadTorqueVector; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationLoadTorqueVector(VisualizationLoadTorqueVector* pVisualizationLoadTorqueVector) { visualizationLoadTorqueVector = pVisualizationLoadTorqueVector; }

    //! AUTO: Get const pointer to computational base class object
    virtual CLoad* GetCLoad() const { return cLoadTorqueVector; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCLoad(CLoad* pCLoad) { cLoadTorqueVector = (CLoadTorqueVector*)pCLoad; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationLoad* GetVisualizationLoad() const { return visualizationLoadTorqueVector; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationLoad(VisualizationLoad* pVisualizationLoad) { visualizationLoadTorqueVector = (VisualizationLoadTorqueVector*)pVisualizationLoad; }

    //! AUTO:  Get type name of load (without keyword 'Load'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "TorqueVector";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cLoadTorqueVector->GetParameters().markerNumber = py::cast<Index>(d["markerNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetVector3DSafely(d, "loadVector", cLoadTorqueVector->GetParameters().loadVector); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "bodyFixed")) { cLoadTorqueVector->GetParameters().bodyFixed = py::cast<bool>(d["bodyFixed"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "loadVectorUserFunction")) { if (EPyUtils::CheckForValidFunction(d["loadVectorUserFunction"])) { cLoadTorqueVector->GetParameters().loadVectorUserFunction = py::cast<std::function<StdVector(Real,StdVector3D)>>((py::function)d["loadVectorUserFunction"]); /* AUTO:  read out dictionary and cast to C++ type*/}} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationLoadTorqueVector->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["loadType"] = (std::string)GetTypeName();
        d["markerNumber"] = (Index)cLoadTorqueVector->GetParameters().markerNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["loadVector"] = (std::vector<Real>)cLoadTorqueVector->GetParameters().loadVector; //! AUTO: cast variables into python (not needed for standard types) 
        d["bodyFixed"] = (bool)cLoadTorqueVector->GetParameters().bodyFixed; //! AUTO: cast variables into python (not needed for standard types) 
        if (cLoadTorqueVector->GetParameters().loadVectorUserFunction)
            {d["loadVectorUserFunction"] = (std::function<StdVector(Real,StdVector3D)>)cLoadTorqueVector->GetParameters().loadVectorUserFunction;}
        else
            {d["loadVectorUserFunction"] = 0;}
 //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationLoadTorqueVector->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumber") == 0) { return py::cast((Index)cLoadTorqueVector->GetParameters().markerNumber);} //! AUTO: get parameter
        else if (parameterName.compare("loadVector") == 0) { return py::cast((std::vector<Real>)cLoadTorqueVector->GetParameters().loadVector);} //! AUTO: get parameter
        else if (parameterName.compare("bodyFixed") == 0) { return py::cast((bool)cLoadTorqueVector->GetParameters().bodyFixed);} //! AUTO: get parameter
        else if (parameterName.compare("loadVectorUserFunction") == 0) { return py::cast((std::function<StdVector(Real,StdVector3D)>)cLoadTorqueVector->GetParameters().loadVectorUserFunction);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationLoadTorqueVector->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("LoadTorqueVector::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumber") == 0) { cLoadTorqueVector->GetParameters().markerNumber = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("loadVector") == 0) { EPyUtils::SetVector3DSafely(value, cLoadTorqueVector->GetParameters().loadVector); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("bodyFixed") == 0) { cLoadTorqueVector->GetParameters().bodyFixed = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("loadVectorUserFunction") == 0) { cLoadTorqueVector->GetParameters().loadVectorUserFunction = py::cast<std::function<StdVector(Real,StdVector3D)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationLoadTorqueVector->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("LoadTorqueVector::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


