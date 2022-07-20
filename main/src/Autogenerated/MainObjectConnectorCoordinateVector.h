/** ***********************************************************************************************
* @class        MainObjectConnectorCoordinateVectorParameters
* @brief        Parameter class for MainObjectConnectorCoordinateVector
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2022-07-04  22:03:18 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINOBJECTCONNECTORCOORDINATEVECTORPARAMETERS__H
#define MAINOBJECTCONNECTORCOORDINATEVECTORPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include <pybind11/functional.h> //! AUTO: for function handling ... otherwise gives a python error (no compilation error in C++ !)
#include "Autogenerated/CObjectConnectorCoordinateVector.h"

#include "Autogenerated/VisuObjectConnectorCoordinateVector.h"

//! AUTO: Parameters for class MainObjectConnectorCoordinateVectorParameters
class MainObjectConnectorCoordinateVectorParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectConnectorCoordinateVector
* @brief        A constraint which constrains the coordinate vectors of two markers Marker[Node|Object|Body]Coordinates attached to nodes or bodies. The marker uses the objects \ac{LTG}-lists to build the according coordinate mappings.
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

//! AUTO: MainObjectConnectorCoordinateVector
class MainObjectConnectorCoordinateVector: public MainObjectConnector // AUTO: 
{
protected: // AUTO: 
    CObjectConnectorCoordinateVector* cObjectConnectorCoordinateVector; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectConnectorCoordinateVector* visualizationObjectConnectorCoordinateVector; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectConnectorCoordinateVector()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectConnectorCoordinateVector* GetCObjectConnectorCoordinateVector() { return cObjectConnectorCoordinateVector; }
    //! AUTO: Get const pointer to computational class
    const CObjectConnectorCoordinateVector* GetCObjectConnectorCoordinateVector() const { return cObjectConnectorCoordinateVector; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectConnectorCoordinateVector(CObjectConnectorCoordinateVector* pCObjectConnectorCoordinateVector) { cObjectConnectorCoordinateVector = pCObjectConnectorCoordinateVector; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectConnectorCoordinateVector* GetVisualizationObjectConnectorCoordinateVector() { return visualizationObjectConnectorCoordinateVector; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectConnectorCoordinateVector* GetVisualizationObjectConnectorCoordinateVector() const { return visualizationObjectConnectorCoordinateVector; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectConnectorCoordinateVector(VisualizationObjectConnectorCoordinateVector* pVisualizationObjectConnectorCoordinateVector) { visualizationObjectConnectorCoordinateVector = pVisualizationObjectConnectorCoordinateVector; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectConnectorCoordinateVector; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectConnectorCoordinateVector = (CObjectConnectorCoordinateVector*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectConnectorCoordinateVector; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectConnectorCoordinateVector = (VisualizationObjectConnectorCoordinateVector*)pVisualizationObject; }

    //! AUTO:  Get type name of object (without keyword 'Object'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "ConnectorCoordinateVector";
    }

    //! AUTO:  Check consistency prior to CSystem::Assemble(); needs to find all possible violations such that Assemble() would fail
    virtual bool CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const override;


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectConnectorCoordinateVector->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(d["markerNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetNumpyMatrixSafely(d, "scalingMarker0", cObjectConnectorCoordinateVector->GetParameters().scalingMarker0); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetNumpyMatrixSafely(d, "scalingMarker1", cObjectConnectorCoordinateVector->GetParameters().scalingMarker1); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetNumpyMatrixSafely(d, "quadraticTermMarker0", cObjectConnectorCoordinateVector->GetParameters().quadraticTermMarker0); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetNumpyMatrixSafely(d, "quadraticTermMarker1", cObjectConnectorCoordinateVector->GetParameters().quadraticTermMarker1); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetNumpyVectorSafely(d, "offset", cObjectConnectorCoordinateVector->GetParameters().offset); /*! AUTO:  safely cast to C++ type*/
        cObjectConnectorCoordinateVector->GetParameters().velocityLevel = py::cast<bool>(d["velocityLevel"]); /* AUTO:  read out dictionary and cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "constraintUserFunction")) { if (EPyUtils::CheckForValidFunction(d["constraintUserFunction"])) 
            { cObjectConnectorCoordinateVector->GetParameters().constraintUserFunction = py::cast<std::function<StdVector(const MainSystem&,Real,Index,StdVector,StdVector,bool)>>((py::function)d["constraintUserFunction"]); /* AUTO:  read out dictionary and cast to C++ type*/}
            else {cObjectConnectorCoordinateVector->GetParameters().constraintUserFunction = 0;  /*AUTO: otherwise assign with zero!*/ }} 
        if (EPyUtils::DictItemExists(d, "jacobianUserFunction")) { if (EPyUtils::CheckForValidFunction(d["jacobianUserFunction"])) 
            { cObjectConnectorCoordinateVector->GetParameters().jacobianUserFunction = py::cast<std::function<py::object(const MainSystem&,Real,Index,StdVector,StdVector,bool)>>((py::function)d["jacobianUserFunction"]); /* AUTO:  read out dictionary and cast to C++ type*/}
            else {cObjectConnectorCoordinateVector->GetParameters().jacobianUserFunction = 0;  /*AUTO: otherwise assign with zero!*/ }} 
        if (EPyUtils::DictItemExists(d, "activeConnector")) { cObjectConnectorCoordinateVector->GetParameters().activeConnector = py::cast<bool>(d["activeConnector"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectConnectorCoordinateVector->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectConnectorCoordinateVector->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary(bool addGraphicsData=false) const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["markerNumbers"] = EPyUtils::GetArrayMarkerIndex(cObjectConnectorCoordinateVector->GetParameters().markerNumbers); //! AUTO: cast variables into python (not needed for standard types) 
        d["scalingMarker0"] = EPyUtils::Matrix2NumPy(cObjectConnectorCoordinateVector->GetParameters().scalingMarker0); //! AUTO: cast variables into python (not needed for standard types) 
        d["scalingMarker1"] = EPyUtils::Matrix2NumPy(cObjectConnectorCoordinateVector->GetParameters().scalingMarker1); //! AUTO: cast variables into python (not needed for standard types) 
        d["quadraticTermMarker0"] = EPyUtils::Matrix2NumPy(cObjectConnectorCoordinateVector->GetParameters().quadraticTermMarker0); //! AUTO: cast variables into python (not needed for standard types) 
        d["quadraticTermMarker1"] = EPyUtils::Matrix2NumPy(cObjectConnectorCoordinateVector->GetParameters().quadraticTermMarker1); //! AUTO: cast variables into python (not needed for standard types) 
        d["offset"] = EPyUtils::Vector2NumPy(cObjectConnectorCoordinateVector->GetParameters().offset); //! AUTO: cast variables into python (not needed for standard types) 
        d["velocityLevel"] = (bool)cObjectConnectorCoordinateVector->GetParameters().velocityLevel; //! AUTO: cast variables into python (not needed for standard types) 
        if (cObjectConnectorCoordinateVector->GetParameters().constraintUserFunction)
            {d["constraintUserFunction"] = (std::function<StdVector(const MainSystem&,Real,Index,StdVector,StdVector,bool)>)cObjectConnectorCoordinateVector->GetParameters().constraintUserFunction;}
        else
            {d["constraintUserFunction"] = 0;}
 //! AUTO: cast variables into python (not needed for standard types) 
        if (cObjectConnectorCoordinateVector->GetParameters().jacobianUserFunction)
            {d["jacobianUserFunction"] = (std::function<py::object(const MainSystem&,Real,Index,StdVector,StdVector,bool)>)cObjectConnectorCoordinateVector->GetParameters().jacobianUserFunction;}
        else
            {d["jacobianUserFunction"] = 0;}
 //! AUTO: cast variables into python (not needed for standard types) 
        d["activeConnector"] = (bool)cObjectConnectorCoordinateVector->GetParameters().activeConnector; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectConnectorCoordinateVector->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectConnectorCoordinateVector->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { return py::cast(EPyUtils::GetArrayMarkerIndex(cObjectConnectorCoordinateVector->GetParameters().markerNumbers));} //! AUTO: get parameter
        else if (parameterName.compare("scalingMarker0") == 0) { return EPyUtils::Matrix2NumPy(cObjectConnectorCoordinateVector->GetParameters().scalingMarker0);} //! AUTO: get parameter
        else if (parameterName.compare("scalingMarker1") == 0) { return EPyUtils::Matrix2NumPy(cObjectConnectorCoordinateVector->GetParameters().scalingMarker1);} //! AUTO: get parameter
        else if (parameterName.compare("quadraticTermMarker0") == 0) { return EPyUtils::Matrix2NumPy(cObjectConnectorCoordinateVector->GetParameters().quadraticTermMarker0);} //! AUTO: get parameter
        else if (parameterName.compare("quadraticTermMarker1") == 0) { return EPyUtils::Matrix2NumPy(cObjectConnectorCoordinateVector->GetParameters().quadraticTermMarker1);} //! AUTO: get parameter
        else if (parameterName.compare("offset") == 0) { return EPyUtils::Vector2NumPy(cObjectConnectorCoordinateVector->GetParameters().offset);} //! AUTO: get parameter
        else if (parameterName.compare("velocityLevel") == 0) { return py::cast((bool)cObjectConnectorCoordinateVector->GetParameters().velocityLevel);} //! AUTO: get parameter
        else if (parameterName.compare("constraintUserFunction") == 0) { return py::cast((std::function<StdVector(const MainSystem&,Real,Index,StdVector,StdVector,bool)>)cObjectConnectorCoordinateVector->GetParameters().constraintUserFunction);} //! AUTO: get parameter
        else if (parameterName.compare("jacobianUserFunction") == 0) { return py::cast((std::function<py::object(const MainSystem&,Real,Index,StdVector,StdVector,bool)>)cObjectConnectorCoordinateVector->GetParameters().jacobianUserFunction);} //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { return py::cast((bool)cObjectConnectorCoordinateVector->GetParameters().activeConnector);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectConnectorCoordinateVector->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectConnectorCoordinateVector->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectConnectorCoordinateVector::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { cObjectConnectorCoordinateVector->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("scalingMarker0") == 0) { EPyUtils::SetNumpyMatrixSafely(value, cObjectConnectorCoordinateVector->GetParameters().scalingMarker0); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("scalingMarker1") == 0) { EPyUtils::SetNumpyMatrixSafely(value, cObjectConnectorCoordinateVector->GetParameters().scalingMarker1); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("quadraticTermMarker0") == 0) { EPyUtils::SetNumpyMatrixSafely(value, cObjectConnectorCoordinateVector->GetParameters().quadraticTermMarker0); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("quadraticTermMarker1") == 0) { EPyUtils::SetNumpyMatrixSafely(value, cObjectConnectorCoordinateVector->GetParameters().quadraticTermMarker1); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("offset") == 0) { EPyUtils::SetNumpyVectorSafely(value, cObjectConnectorCoordinateVector->GetParameters().offset); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("velocityLevel") == 0) { cObjectConnectorCoordinateVector->GetParameters().velocityLevel = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("constraintUserFunction") == 0) { if (py::isinstance<py::function>(value)) {cObjectConnectorCoordinateVector->GetParameters().constraintUserFunction = py::cast<std::function<StdVector(const MainSystem&,Real,Index,StdVector,StdVector,bool)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/} else
            if (!EPyUtils::IsPyTypeInteger(value) || (py::cast<int>(value) != 0)) {PyError(STDstring("Failed to convert PyFunction: must be either valid python function or 0, but got ")+EXUstd::ToString(value)); }; } //! AUTO: get parameter
        else if (parameterName.compare("jacobianUserFunction") == 0) { if (py::isinstance<py::function>(value)) {cObjectConnectorCoordinateVector->GetParameters().jacobianUserFunction = py::cast<std::function<py::object(const MainSystem&,Real,Index,StdVector,StdVector,bool)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/} else
            if (!EPyUtils::IsPyTypeInteger(value) || (py::cast<int>(value) != 0)) {PyError(STDstring("Failed to convert PyFunction: must be either valid python function or 0, but got ")+EXUstd::ToString(value)); }; } //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { cObjectConnectorCoordinateVector->GetParameters().activeConnector = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectConnectorCoordinateVector->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectConnectorCoordinateVector->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectConnectorCoordinateVector::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...
