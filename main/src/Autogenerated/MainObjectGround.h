/** ***********************************************************************************************
* @class        MainObjectGroundParameters
* @brief        Parameter class for MainObjectGround
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2023-12-14  08:22:32 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINOBJECTGROUNDPARAMETERS__H
#define MAINOBJECTGROUNDPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include <pybind11/functional.h> //! AUTO: for function handling ... otherwise gives a python error (no compilation error in C++ !)
#include "Autogenerated/CObjectGround.h"

#include "Autogenerated/VisuObjectGround.h"

//! AUTO: Parameters for class MainObjectGroundParameters
class MainObjectGroundParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectGround
* @brief        A ground object behaving like a rigid body, but having no degrees of freedom; used to attach body-connectors without an action. For examples see spring dampers and joints.
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

//! AUTO: MainObjectGround
class MainObjectGround: public MainObjectBody // AUTO: 
{
protected: // AUTO: 
    CObjectGround* cObjectGround; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectGround* visualizationObjectGround; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectGround()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectGround* GetCObjectGround() { return cObjectGround; }
    //! AUTO: Get const pointer to computational class
    const CObjectGround* GetCObjectGround() const { return cObjectGround; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectGround(CObjectGround* pCObjectGround) { cObjectGround = pCObjectGround; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectGround* GetVisualizationObjectGround() { return visualizationObjectGround; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectGround* GetVisualizationObjectGround() const { return visualizationObjectGround; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectGround(VisualizationObjectGround* pVisualizationObjectGround) { visualizationObjectGround = pVisualizationObjectGround; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectGround; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectGround = (CObjectGround*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectGround; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectGround = (VisualizationObjectGround*)pVisualizationObject; }

    //! AUTO:  Get type name of object; could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "Ground";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "referencePosition", cObjectGround->GetParameters().referencePosition); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "referenceRotation")) { EPyUtils::SetConstMatrixTemplateSafely<3,3>(d, "referenceRotation", cObjectGround->GetParameters().referenceRotation); /*! AUTO:  safely cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectGround->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VgraphicsDataUserFunction")) { if (EPyUtils::CheckForValidFunction(d["VgraphicsDataUserFunction"])) 
            { visualizationObjectGround->GetGraphicsDataUserFunction() = py::cast<std::function<py::object(const MainSystem&,Index)>>((py::function)d["VgraphicsDataUserFunction"]); /* AUTO:  read out dictionary and cast to C++ type*/}
            else {visualizationObjectGround->GetGraphicsDataUserFunction() = 0;  /*AUTO: otherwise assign with zero!*/ }} 
        if (EPyUtils::DictItemExists(d, "VgraphicsData")) { PyWriteBodyGraphicsDataList(d, "VgraphicsData", visualizationObjectGround->GetGraphicsData()); /*! AUTO: convert dict to BodyGraphicsData*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary(bool addGraphicsData=false) const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["referencePosition"] = EPyUtils::SlimVector2NumPy(cObjectGround->GetParameters().referencePosition); //! AUTO: cast variables into python (not needed for standard types) 
        d["referenceRotation"] = EPyUtils::Matrix2NumPyTemplate(cObjectGround->GetParameters().referenceRotation); //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectGround->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        if (visualizationObjectGround->GetGraphicsDataUserFunction())
            {d["VgraphicsDataUserFunction"] = (std::function<py::object(const MainSystem&,Index)>)visualizationObjectGround->GetGraphicsDataUserFunction();}
        else
            {d["VgraphicsDataUserFunction"] = 0;}
 //! AUTO: cast variables into python (not needed for standard types) 
        d["VgraphicsData"] = PyGetBodyGraphicsDataList(visualizationObjectGround->GetGraphicsData(), addGraphicsData); //! AUTO: generate dictionary with special function
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("referencePosition") == 0) { return EPyUtils::SlimVector2NumPy(cObjectGround->GetParameters().referencePosition);} //! AUTO: get parameter
        else if (parameterName.compare("referenceRotation") == 0) { return EPyUtils::Matrix2NumPyTemplate(cObjectGround->GetParameters().referenceRotation);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectGround->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VgraphicsDataUserFunction") == 0) { return py::cast((std::function<py::object(const MainSystem&,Index)>)visualizationObjectGround->GetGraphicsDataUserFunction());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectGround::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("referencePosition") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, cObjectGround->GetParameters().referencePosition); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("referenceRotation") == 0) { EPyUtils::SetConstMatrixTemplateSafely<3,3>(value, cObjectGround->GetParameters().referenceRotation); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectGround->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VgraphicsDataUserFunction") == 0) { if (py::isinstance<py::function>(value)) {visualizationObjectGround->GetGraphicsDataUserFunction() = py::cast<std::function<py::object(const MainSystem&,Index)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/} else
            if (!EPyUtils::IsPyTypeInteger(value) || (py::cast<int>(value) != 0)) {PyError(STDstring("Failed to convert PyFunction: must be either valid python function or 0, but got ")+EXUstd::ToString(value)); }; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectGround::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...
