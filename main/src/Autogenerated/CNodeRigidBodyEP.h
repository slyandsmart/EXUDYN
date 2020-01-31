/** ***********************************************************************************************
* @class        CNodeRigidBodyEPParameters
* @brief        Parameter class for CNodeRigidBodyEP
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-01-24  17:03:37 (last modfied)
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


//! AUTO: Parameters for class CNodeRigidBodyEPParameters
class CNodeRigidBodyEPParameters // AUTO: 
{
public: // AUTO: 
    Vector7D referenceCoordinates;                //!< AUTO: reference coordinates (x-pos,y-pos,z-pos and 4 Euler parameters) of node ==> e.g. ref. coordinates for finite elements or reference position of rigid body (e.g. for definition of joints)
    //! AUTO: default constructor with parameter initialization
    CNodeRigidBodyEPParameters()
    {
        referenceCoordinates = Vector7D({0.,0.,0., 0.,0.,0.,0.});
    };
};


/** ***********************************************************************************************
* @class        CNodeRigidBodyEP
* @brief        A 3D rigid body node based on Euler parameters for rigid bodies or beams; the node has 3 displacement coordinates (displacements of center of mass - COM: ux,uy,uz) and four rotation coordinates (Euler parameters = quaternions); all coordinates lead to second order differential equations; additionally there is one constraint equation for quaternions; The rotation matrix \f$\Am\f$, transforming local (body-fixed) 3D positions \f$\pv_{loc} = [p^x_{loc}\;\;p^y_{loc}\;\;p^z_{loc}]^T\f$ to global 3D positions \f$\pv_{glob} = [p^x_{glob}\;\;p^y_{glob}\;\;p^z_{glob}]^T\f$, \f[ \pv_{glob} = \Am \pv_{loc}, \f] is defined according to the book of Shabana, same with the transformation matrix \f$\mathbf{G}\f$ between time derivatives of Euler parameters and angular velocities.
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

//! AUTO: CNodeRigidBodyEP
class CNodeRigidBodyEP: public CNodeODE2AE // AUTO: 
{
protected: // AUTO: 
    static const Index nRotationCoordinates = 4;
    static const Index nDisplacementCoordinates = 3;
    CNodeRigidBodyEPParameters parameters; //! AUTO: contains all parameters for CNodeRigidBodyEP

public: // AUTO: 

    // AUTO: access functions
    //! AUTO: Write (Reference) access to parameters
    virtual CNodeRigidBodyEPParameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const CNodeRigidBodyEPParameters& GetParameters() const { return parameters; }

    //! AUTO:  return number of second order diff. eq. coordinates
    virtual Index GetNumberOfODE2Coordinates() const override
    {
        return 7;
    }

    //! AUTO:  return number of (internal) algebraic eq. coordinates
    virtual Index GetNumberOfAECoordinates() const override
    {
        return 1;
    }

    //! AUTO:  return node type (for node treatment in computation)
    virtual Node::Type GetType() const override
    {
        return (Node::Type)(Node::Position + Node::Orientation + Node::RotationEulerParameters);
    }

    //! AUTO:  return configuration dependent position of node; returns always a 3D Vector
    virtual Vector3D GetPosition(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent velocity of node; returns always a 3D Vector
    virtual Vector3D GetVelocity(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent rotation matrix of node; returns always a 3D Vector
    virtual Matrix3D GetRotationMatrix(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent angular velocity of node; returns always a 3D Vector
    virtual Vector3D GetAngularVelocity(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent local (=body fixed) angular velocity of node; returns always a 3D Vector
    virtual Vector3D GetAngularVelocityLocal(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  provide position jacobian of node; derivative of 3D Position with respect to 7 coordinates ux,uy,uz,ep0,...,ep3
    virtual void GetPositionJacobian(Matrix& value) const override;

    //! AUTO:  provide "rotation" jacobian \f$\Jm_R\f$ of node; derivative of 3D angular velocity vector with respect to all velocity coordinates ("G-matrix"); action of torque \f$\mv\f$: \f$\Qm_m = \Jm_R^T \mv\f$
    virtual void GetRotationJacobian(Matrix& value) const override;

    //! AUTO:  return internally stored reference coordinates of node
    virtual LinkedDataVector GetReferenceCoordinateVector() const override
    {
        return parameters.referenceCoordinates;
    }

    //! AUTO:  provide according output variable in "value"; used e.g. for postprocessing and sensors
    virtual void GetOutputVariable(OutputVariableType variableType, ConfigurationType configuration, Vector& value) const override;

    //! AUTO:  Compute vector to of 4 Euler Parameters from reference and configuration coordinates
    ConstSizeVector<nRotationCoordinates> GetEulerParameters(ConfigurationType configuration = ConfigurationType::Current) const;

    //! AUTO:  Compute vector to time derivative of 4 Euler Parameters in given configuration
    ConstSizeVector<nRotationCoordinates> GetEulerParameters_t(ConfigurationType configuration = ConfigurationType::Current) const;

    virtual OutputVariableType GetOutputVariableTypes() const override
    {
        return (OutputVariableType)(
            (Index)OutputVariableType::Position +
            (Index)OutputVariableType::Displacement +
            (Index)OutputVariableType::Velocity +
            (Index)OutputVariableType::AngularVelocity +
            (Index)OutputVariableType::Coordinates +
            (Index)OutputVariableType::Coordinates_t );
    }

};


