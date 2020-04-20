/** ***********************************************************************************************
* @class        MainNodePointGroundParameters
* @brief        Parameter class for MainNodePointGround
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-03-30  22:45:21 (last modfied)
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
#include "Autogenerated/CNodePointGround.h"

#include "Autogenerated/VisuNodePointGround.h"

//! AUTO: Parameters for class MainNodePointGroundParameters
class MainNodePointGroundParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainNodePointGround
* @brief        A 3D point node fixed to ground. The node can be used as NodePoint, but it does not generate coordinates. Applied or reaction forces do not have any effect.
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

//! AUTO: MainNodePointGround
class MainNodePointGround: public MainNode // AUTO: 
{
protected: // AUTO: 
    CNodePointGround* cNodePointGround; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationNodePointGround* visualizationNodePointGround; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainNodePointGround()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CNodePointGround* GetCNodePointGround() { return cNodePointGround; }
    //! AUTO: Get const pointer to computational class
    const CNodePointGround* GetCNodePointGround() const { return cNodePointGround; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCNodePointGround(CNodePointGround* pCNodePointGround) { cNodePointGround = pCNodePointGround; }

    //! AUTO: Get pointer to visualization class
    VisualizationNodePointGround* GetVisualizationNodePointGround() { return visualizationNodePointGround; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationNodePointGround* GetVisualizationNodePointGround() const { return visualizationNodePointGround; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationNodePointGround(VisualizationNodePointGround* pVisualizationNodePointGround) { visualizationNodePointGround = pVisualizationNodePointGround; }

    //! AUTO: Get const pointer to computational base class object
    virtual CNode* GetCNode() const { return cNodePointGround; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCNode(CNode* pCNode) { cNodePointGround = (CNodePointGround*)pCNode; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationNode* GetVisualizationNode() const { return visualizationNodePointGround; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationNode(VisualizationNode* pVisualizationNode) { visualizationNodePointGround = (VisualizationNodePointGround*)pVisualizationNode; }

    //! AUTO:  Get type name of node (without keyword 'Node'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "PointGround";
    }

    //! AUTO:  Call a specific node function ==> automatically generated in future
    virtual py::object CallFunction(STDstring functionName, py::dict args) const override;

    //! AUTO:  return empty vector, as there are no initial coordinates
    virtual LinkedDataVector GetInitialVector() const override
    {
        return LinkedDataVector();
    }

    //! AUTO:  return empty vector, as there are no initial velocity coordinates
    virtual LinkedDataVector GetInitialVector_t() const override
    {
        return LinkedDataVector();
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        EPyUtils::SetVector3DSafely(d, "referenceCoordinates", cNodePointGround->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationNodePointGround->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawSize")) { visualizationNodePointGround->GetDrawSize() = py::cast<float>(d["VdrawSize"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationNodePointGround->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["nodeType"] = (std::string)GetTypeName();
        d["referenceCoordinates"] = (std::vector<Real>)cNodePointGround->GetParameters().referenceCoordinates; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationNodePointGround->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawSize"] = (float)visualizationNodePointGround->GetDrawSize(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationNodePointGround->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { return py::cast((std::vector<Real>)cNodePointGround->GetParameters().referenceCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationNodePointGround->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { return py::cast((float)visualizationNodePointGround->GetDrawSize());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationNodePointGround->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("NodePointGround::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { EPyUtils::SetVector3DSafely(value, cNodePointGround->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationNodePointGround->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { visualizationNodePointGround->GetDrawSize() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationNodePointGround->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("NodePointGround::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


