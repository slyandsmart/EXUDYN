/** ***********************************************************************************************
* @class        MainObjectANCFBeamParameters
* @brief        Parameter class for MainObjectANCFBeam
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2023-04-05  13:57:34 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINOBJECTANCFBEAMPARAMETERS__H
#define MAINOBJECTANCFBEAMPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CObjectANCFBeam.h"

#include "Autogenerated/VisuObjectANCFBeam.h"
#include "Autogenerated/PyStructuralElementsDataStructures.h"

//! AUTO: Parameters for class MainObjectANCFBeamParameters
class MainObjectANCFBeamParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectANCFBeam
* @brief        OBJECT UNDER CONSTRUCTION: A 3D beam finite element based on the absolute nodal coordinate formulation, using two . The localPosition \f$x\f$ of the beam ranges from \f$-L/2\f$ (at node 0) to \f$L/2\f$ (at node 1). The axial coordinate is \f$x\f$ (first coordinate) and the cross section is spanned by local \f$y\f$/\f$z\f$ axes; assuming dimensions \f$w_y\f$ and \f$w_z\f$ in cross section, the local position range is \f$\in [[-L/2,L/2],\, [-wy/2,wy/2],\, [-wz/2,wz/2] ]\f$.
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

//! AUTO: MainObjectANCFBeam
class MainObjectANCFBeam: public MainObjectBody // AUTO: 
{
protected: // AUTO: 
    CObjectANCFBeam* cObjectANCFBeam; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectANCFBeam* visualizationObjectANCFBeam; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectANCFBeam()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectANCFBeam* GetCObjectANCFBeam() { return cObjectANCFBeam; }
    //! AUTO: Get const pointer to computational class
    const CObjectANCFBeam* GetCObjectANCFBeam() const { return cObjectANCFBeam; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectANCFBeam(CObjectANCFBeam* pCObjectANCFBeam) { cObjectANCFBeam = pCObjectANCFBeam; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectANCFBeam* GetVisualizationObjectANCFBeam() { return visualizationObjectANCFBeam; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectANCFBeam* GetVisualizationObjectANCFBeam() const { return visualizationObjectANCFBeam; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectANCFBeam(VisualizationObjectANCFBeam* pVisualizationObjectANCFBeam) { visualizationObjectANCFBeam = pVisualizationObjectANCFBeam; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectANCFBeam; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectANCFBeam = (CObjectANCFBeam*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectANCFBeam; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectANCFBeam = (VisualizationObjectANCFBeam*)pVisualizationObject; }

    void SetInternalBeamSection(const py::object& pyObject); //! AUTO: special function which writes pyObject into local data
    PyBeamSection GetInternalBeamSection() const; //! AUTO: special function which returns BeamSection converted from local data
    //! AUTO:  Get type name of object; could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "ObjectANCFBeam3D";
    }

    //! AUTO:  provide requested nodeType for objects; used for automatic checks in CheckSystemIntegrity()
    virtual Node::Type GetRequestedNodeType() const override
    {
        return (Node::Type)(Node::Position + Node::Orientation + Node::Point3DSlope23);
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectANCFBeam->GetParameters().nodeNumbers = EPyUtils::GetNodeIndex2Safely(d["nodeNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectANCFBeam->GetParameters().physicsLength = py::cast<Real>(d["physicsLength"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "crossSectionPenaltyFactor", cObjectANCFBeam->GetParameters().crossSectionPenaltyFactor); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        SetInternalBeamSection(d["sectionData"]); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectANCFBeam->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VsectionGeometry")) { visualizationObjectANCFBeam->GetSectionGeometry() = py::cast<BeamSectionGeometry>(d["VsectionGeometry"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectANCFBeam->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary(bool addGraphicsData=false) const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["nodeNumbers"] = EPyUtils::GetArrayNodeIndex(ArrayIndex(cObjectANCFBeam->GetParameters().nodeNumbers)); //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsLength"] = (Real)cObjectANCFBeam->GetParameters().physicsLength; //! AUTO: cast variables into python (not needed for standard types) 
        d["crossSectionPenaltyFactor"] = EPyUtils::SlimVector2NumPy(cObjectANCFBeam->GetParameters().crossSectionPenaltyFactor); //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["sectionData"] = GetInternalBeamSection(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectANCFBeam->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VsectionGeometry"] = (BeamSectionGeometry)visualizationObjectANCFBeam->GetSectionGeometry(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectANCFBeam->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { return py::cast(EPyUtils::GetArrayNodeIndex(ArrayIndex(cObjectANCFBeam->GetParameters().nodeNumbers)));} //! AUTO: get parameter
        else if (parameterName.compare("physicsLength") == 0) { return py::cast((Real)cObjectANCFBeam->GetParameters().physicsLength);} //! AUTO: get parameter
        else if (parameterName.compare("sectionData") == 0) { return py::cast(GetInternalBeamSection());} //! AUTO: get parameter
        else if (parameterName.compare("crossSectionPenaltyFactor") == 0) { return EPyUtils::SlimVector2NumPy(cObjectANCFBeam->GetParameters().crossSectionPenaltyFactor);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectANCFBeam->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VsectionGeometry") == 0) { return py::cast((BeamSectionGeometry)visualizationObjectANCFBeam->GetSectionGeometry());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectANCFBeam->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectANCFBeam::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { cObjectANCFBeam->GetParameters().nodeNumbers = EPyUtils::GetNodeIndex2Safely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsLength") == 0) { cObjectANCFBeam->GetParameters().physicsLength = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("sectionData") == 0) { SetInternalBeamSection(value); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("crossSectionPenaltyFactor") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, cObjectANCFBeam->GetParameters().crossSectionPenaltyFactor); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectANCFBeam->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VsectionGeometry") == 0) { visualizationObjectANCFBeam->GetSectionGeometry() = py::cast<BeamSectionGeometry>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectANCFBeam->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectANCFBeam::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...
