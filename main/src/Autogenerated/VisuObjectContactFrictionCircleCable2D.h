/** ***********************************************************************************************
* @class        VisualizationObjectContactFrictionCircleCable2D
* @brief        A very specialized penalty-based contact/friction condition between a 2D circle in the local x/y plane (=marker0, a Rigid-Body Marker) on a body and an ANCFCable2DShape (=marker1, Marker: BodyCable2DShape), in xy-plane; a node NodeGenericData is required with 3\f$\times\f$(number of contact segments) -- containing per segment: [contact gap, stick/slip (stick=1), last friction position]; the contact gap \f$g\f$ is integrated (piecewise linear) along the cable and circle; the contact force \f$f_c\f$ is zero for \f$gap>0\f$ and otherwise computed from \f$f_c = g*contactStiffness + \dot g*contactDamping\f$; during Newton iterations, the contact force is actived only, if \f$dataCoordinate[0] <= 0\f$; dataCoordinate is set equal to gap in nonlinear iterations, but not modified in Newton iterations.
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

#ifndef VISUALIZATIONOBJECTCONTACTFRICTIONCIRCLECABLE2D__H
#define VISUALIZATIONOBJECTCONTACTFRICTIONCIRCLECABLE2D__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

class VisualizationObjectContactFrictionCircleCable2D: public VisualizationObject // AUTO: 
{
protected: // AUTO: 
    float drawSize;                               //!< AUTO: drawing size = diameter of spring; size == -1.f means that default connector size is used
    Float4 color;                                 //!< AUTO: RGBA connector color; if R==-1, use default color

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    VisualizationObjectContactFrictionCircleCable2D()
    {
        show = true;
        drawSize = -1.f;
        color = Float4({-1.f,-1.f,-1.f,-1.f});
    };

    // AUTO: access functions
    //! AUTO:  Update visualizationSystem -> graphicsData for item; index shows item Number in CData
    virtual void UpdateGraphics(const VisualizationSettings& visualizationSettings, VisualizationSystem* vSystem, Index itemNumber) override;

    //! AUTO:  this function is needed to distinguish connector objects from body objects
    virtual bool IsConnector() const override
    {
        return true;
    }

    //! AUTO:  Write (Reference) access to:drawing size = diameter of spring; size == -1.f means that default connector size is used
    void SetDrawSize(const float& value) { drawSize = value; }
    //! AUTO:  Read (Reference) access to:drawing size = diameter of spring; size == -1.f means that default connector size is used
    const float& GetDrawSize() const { return drawSize; }
    //! AUTO:  Read (Reference) access to:drawing size = diameter of spring; size == -1.f means that default connector size is used
    float& GetDrawSize() { return drawSize; }

    //! AUTO:  Write (Reference) access to:RGBA connector color; if R==-1, use default color
    void SetColor(const Float4& value) { color = value; }
    //! AUTO:  Read (Reference) access to:RGBA connector color; if R==-1, use default color
    const Float4& GetColor() const { return color; }
    //! AUTO:  Read (Reference) access to:RGBA connector color; if R==-1, use default color
    Float4& GetColor() { return color; }

};



#endif //#ifdef include once...
