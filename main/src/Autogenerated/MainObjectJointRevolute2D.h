/** ***********************************************************************************************
* @class        MainObjectJointRevolute2DParameters
* @brief        Parameter class for MainObjectJointRevolute2D
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2024-02-03  15:35:23 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINOBJECTJOINTREVOLUTE2DPARAMETERS__H
#define MAINOBJECTJOINTREVOLUTE2DPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CObjectJointRevolute2D.h"

#include "Autogenerated/VisuObjectJointRevolute2D.h"

//! AUTO: Parameters for class MainObjectJointRevolute2DParameters
class MainObjectJointRevolute2DParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectJointRevolute2D
* @brief        A revolute joint in 2D; constrains the absolute 2D position of two points given by PointMarkers or RigidMarkers
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

//! AUTO: MainObjectJointRevolute2D
class MainObjectJointRevolute2D: public MainObjectConnector // AUTO: 
{
protected: // AUTO: 
    CObjectJointRevolute2D* cObjectJointRevolute2D; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectJointRevolute2D* visualizationObjectJointRevolute2D; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectJointRevolute2D()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectJointRevolute2D* GetCObjectJointRevolute2D() { return cObjectJointRevolute2D; }
    //! AUTO: Get const pointer to computational class
    const CObjectJointRevolute2D* GetCObjectJointRevolute2D() const { return cObjectJointRevolute2D; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectJointRevolute2D(CObjectJointRevolute2D* pCObjectJointRevolute2D) { cObjectJointRevolute2D = pCObjectJointRevolute2D; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectJointRevolute2D* GetVisualizationObjectJointRevolute2D() { return visualizationObjectJointRevolute2D; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectJointRevolute2D* GetVisualizationObjectJointRevolute2D() const { return visualizationObjectJointRevolute2D; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectJointRevolute2D(VisualizationObjectJointRevolute2D* pVisualizationObjectJointRevolute2D) { visualizationObjectJointRevolute2D = pVisualizationObjectJointRevolute2D; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectJointRevolute2D; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectJointRevolute2D = (CObjectJointRevolute2D*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectJointRevolute2D; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectJointRevolute2D = (VisualizationObjectJointRevolute2D*)pVisualizationObject; }

    //! AUTO:  Get type name of object (without keyword 'Object'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "JointRevolute2D";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectJointRevolute2D->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(d["markerNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "activeConnector")) { cObjectJointRevolute2D->GetParameters().activeConnector = py::cast<bool>(d["activeConnector"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectJointRevolute2D->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawSize")) { visualizationObjectJointRevolute2D->GetDrawSize() = py::cast<float>(d["VdrawSize"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectJointRevolute2D->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary(bool addGraphicsData=false) const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["markerNumbers"] = EPyUtils::GetArrayMarkerIndex(cObjectJointRevolute2D->GetParameters().markerNumbers); //! AUTO: cast variables into python (not needed for standard types) 
        d["activeConnector"] = (bool)cObjectJointRevolute2D->GetParameters().activeConnector; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectJointRevolute2D->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawSize"] = (float)visualizationObjectJointRevolute2D->GetDrawSize(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectJointRevolute2D->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { return py::cast(EPyUtils::GetArrayMarkerIndex(cObjectJointRevolute2D->GetParameters().markerNumbers));} //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { return py::cast((bool)cObjectJointRevolute2D->GetParameters().activeConnector);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectJointRevolute2D->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { return py::cast((float)visualizationObjectJointRevolute2D->GetDrawSize());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectJointRevolute2D->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectJointRevolute2D::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { cObjectJointRevolute2D->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { cObjectJointRevolute2D->GetParameters().activeConnector = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectJointRevolute2D->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { visualizationObjectJointRevolute2D->GetDrawSize() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectJointRevolute2D->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectJointRevolute2D::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...
