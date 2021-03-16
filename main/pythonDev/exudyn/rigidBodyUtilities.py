#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN python utility library
#
# Details: 	Advanced utility/mathematical functions for reference frames, rigid body kinematics
#			and dynamics. Useful Euler parameter and Tait-Bryan angle conversion functions
#			are included. A class for rigid body inertia creating and transformation is available.
#
# Author:   Johannes Gerstmayr, Stefan Holzinger (rotation vector and Tait-Bryan angles)
# Date:     2020-03-10 (created)
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#constants and fixed structures:
import numpy as np #LoadSolutionFile
from exudyn.itemInterface import *
import exudyn as exu #do not import! causes troubles with exudynFast, etc.!!
from exudyn.basicUtilities import NormL2


eulerParameters0 = [1.,0.,0.,0.] #Euler parameters for case where rotation angle is zero (rotation axis arbitrary)

#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: compute orthogonal basis vectors (normal1, normal2) for given vector0 (non-unique solution!); if vector0 == [0,0,0], then any normal basis is returned
def ComputeOrthonormalBasis(vector0):
    v = np.array([vector0[0],vector0[1],vector0[2]])

    L0 = np.linalg.norm(v)
    if L0 == 0:
        n1 = np.array([1,0,0])
        n2 = np.array([0,1,0])
    else:
        v = (1. / L0)*v;
    
        if (abs(v[0]) > 0.5) and (abs(v[1]) < 0.1) and (abs(v[2]) < 0.1):
            n1 = np.array([0., 1., 0.])
        else:
            n1 = np.array([1., 0., 0.])
    
        h = np.dot(n1, v);
        n1 -= h * v;
        n1 = (1/np.linalg.norm(n1))*n1;
        n2 = np.cross(v,n1)
    #print("basis=", v,n1,n2)
    return [v, n1, n2]

#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: compute Gram-Schmidt projection of given 3D vector 1 on vector 0 and return normalized triad (vector0, vector1, vector0 x vector1)
def GramSchmidt(vector0, vector1):

    v0 = np.array([vector0[0],vector0[1],vector0[2]])
    L0 = np.linalg.norm(v0)
    v0 = (1. / L0)*v0;
    
    v1 = np.array([vector1[0],vector1[1],vector1[2]])
    L1 = np.linalg.norm(v1)
    v1 = (1. / L1)*v1;
    
    h = np.dot(v1, v0);
    v1 -= h * v0;
    v1 = (1/np.linalg.norm(v1))*v1;
    n2 = np.cross(v0,v1)
    #print("basis=", v,n1,n2)
    return [v0, v1, n2]


#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: compute skew symmetric 3x3-matrix from 3x1- or 1x3-vector
def Skew(vector):
    skewsymmetricMatrix = np.array([[ 0.,       -vector[2], vector[1]], 
                                    [ vector[2], 0.,       -vector[0]],
                                    [-vector[1], vector[0], 0.]])
    return skewsymmetricMatrix

#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: convert skew symmetric matrix m to vector
def Skew2Vec(skew):
    shape = skew.shape
    if shape == (3,3):
        w1 = skew[2][1]
        w2 = skew[0][2]
        w3 = -skew[0][1]
        vec = np.array([w1, w2, w3])
    if shape == (4,4):
        w1 = skew[2][1]
        w2 = skew[0][2]
        w3 = -skew[0][1]
        u1 = skew[0][3]
        u2 = skew[1][3]
        u3 = skew[2][3]
        vec = np.array([u1, u2, u3, w1, w2, w3])       
    return vec


#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: compute (3 x 3*n) skew matrix from (3*n) vector
def ComputeSkewMatrix(v):
    n = int(len(v)/3) #number of nodes
    sm = np.zeros((3*n,3))

    for i in range(n):
        off = 3*i
        x=v[off+0]
        y=v[off+1]
        z=v[off+2]
        mLoc = np.array([[0,-z,y],[z,0,-x],[-y,x,0]])
        sm[off:off+3,:] = mLoc[:,:]
    
    return sm



#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#helper functions for RIGID BODY KINEMATICS:

#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: convert Euler parameters (ep) to G-matrix (=$\partial \tomega  / \partial \pv_t$)
#**input: vector of 4 eulerParameters as list or np.array
#**output: 3x4 matrix G as np.array
def EulerParameters2G(eulerParameters):
    ep = eulerParameters
    return np.array([[-2.*ep[1], 2.*ep[0],-2.*ep[3], 2.*ep[2]],
                     [-2.*ep[2], 2.*ep[3], 2.*ep[0],-2.*ep[1]],
                     [-2.*ep[3],-2.*ep[2], 2.*ep[1], 2.*ep[0]] ])

#**function: convert Euler parameters (ep) to local G-matrix (=$\partial \LU{b}{\tomega} / \partial \pv_t$)
#**input: vector of 4 eulerParameters as list or np.array
#**output: 3x4 matrix G as np.array
def EulerParameters2GLocal(eulerParameters):
    ep = eulerParameters
    return np.array([[-2.*ep[1], 2.*ep[0], 2.*ep[3],-2.*ep[2]],
                     [-2.*ep[2],-2.*ep[3], 2.*ep[0], 2.*ep[1]],
                     [-2.*ep[3], 2.*ep[2],-2.*ep[1], 2.*ep[0]] ])

#**function: compute rotation matrix from eulerParameters    
#**input: vector of 4 eulerParameters as list or np.array
#**output: 3x3 rotation matrix as np.array
def EulerParameters2RotationMatrix(eulerParameters):
    ep = eulerParameters
    return np.array([[-2.0*ep[3]*ep[3] - 2.0*ep[2]*ep[2] + 1.0, -2.0*ep[3]*ep[0] + 2.0*ep[2]*ep[1], 2.0*ep[3]*ep[1] + 2.0*ep[2]*ep[0]],
                     [ 2.0*ep[3]*ep[0] + 2.0*ep[2]*ep[1], -2.0*ep[3]*ep[3] - 2.0*ep[1]*ep[1] + 1.0, 2.0*ep[3]*ep[2] - 2.0*ep[1]*ep[0]],
                     [-2.0*ep[2]*ep[0] + 2.0*ep[3]*ep[1], 2.0*ep[3]*ep[2] + 2.0*ep[1]*ep[0], -2.0*ep[2]*ep[2] - 2.0*ep[1]*ep[1] + 1.0] ])

#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: compute Euler parameters from given rotation matrix
#**input: 3x3 rotation matrix as list of lists or as np.array
#**output: vector of 4 eulerParameters as np.array
def RotationMatrix2EulerParameters(rotationMatrix):
    A=rotationMatrix
    trace = A[0][0] + A[1][1] + A[2][2] + 1.0
    M_EPSILON = 1e-15 #small number to avoid division by zero

    if (abs(trace) > M_EPSILON):
        s = 0.5 / np.sqrt(abs(trace))
        ep0 = 0.25 / s
        ep1 = (A[2][1] - A[1][2]) * s
        ep2 = (A[0][2] - A[2][0]) * s
        ep3 = (A[1][0] - A[0][1]) * s
    else:
        if (A[0][0] > A[1][1]) and (A[0][0] > A[2][2]):
            s = 2.0 * np.sqrt(abs(1.0 + A[0][0] - A[1][1] - A[2][2]))
            ep1 = 0.25 * s
            ep2 = (A[0][1] + A[1][0]) / s
            ep3 = (A[0][2] + A[2][0]) / s
            ep0 = (A[1][2] - A[2][1]) / s
        elif A[1][1] > A[2][2]:
            s = 2.0 * np.sqrt(abs(1.0 + A[1][1] - A[0][0] - A[2][2]))
            ep1 = (A[0][1] + A[1][0]) / s
            ep2 = 0.25 * s
            ep3 = (A[1][2] + A[2][1]) / s
            ep0 = (A[0][2] - A[2][0]) / s
        else:
            s = 2.0 * np.sqrt(abs(1.0 + A[2][2] - A[0][0] - A[1][1]));
            ep1 = (A[0][2] + A[2][0]) / s
            ep2 = (A[1][2] + A[2][1]) / s
            ep3 = 0.25 * s
            ep0 = (A[0][1] - A[1][0]) / s

    return np.array([ep0,ep1,ep2,ep3])

#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: compute time derivative of Euler parameters from (global) angular velocity vector
#note that for Euler parameters $\pv$, we have $\tomega=\Gm \pv_t$ ==> $\Gm^T \tomega = \Gm^T\cdot \Gm\cdot \pv_t$ ==> $\Gm^T \Gm=4(\Im_{4x4} - \pv\cdot \pv^T)\pv_t = 4 (\Im_{4x4}) \pv_t$
#**input: 
#  angularVelocity: 3D vector of angular velocity in global frame, as lists or as np.array
#  eulerParameters: vector of 4 eulerParameters as np.array or list
#**output: vector of time derivatives of 4 eulerParameters as np.array
def AngularVelocity2EulerParameters_t(angularVelocity, eulerParameters):
    
    GT = np.transpose(EulerParameters2G(eulerParameters))
    return 0.25*(GT.dot(angularVelocity))


#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#            ROTATION VECTOR
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: rotaton matrix from rotation vector, see appendix B in \cite{Simo1988}
#**input: 3D rotation vector as list or np.array
#**output: 3x3 rotation matrix as np.array
def RotationVector2RotationMatrix(rotationVector):
    phi = np.linalg.norm(rotationVector)
    if phi == 0.:
        R = np.eye(3)
    else:
        OmegaSkew = Skew(rotationVector)
        alpha = np.sin(phi)/phi
        beta = 2*(1-np.cos(phi))/phi**2
        R = np.eye(3) + alpha*OmegaSkew + 0.5*beta*np.matmul(OmegaSkew, OmegaSkew)

    
    return R  


#**function: compute rotation vector from rotation matrix
#**input: 3x3 rotation matrix as list of lists or as np.array
#**output: vector of 3 components of rotation vector as np.array
def RotationMatrix2RotationVector(rotationMatrix):
    # compute a  rotation vector from given rotation matrix according to 
    # 2015 - Sonneville - A geometrical local frame approach for flexible multibody systems, p45
    if np.linalg.norm(rotationMatrix - np.eye(3)) == 0.:
        rotationVector = np.zeros(3)
    else:
        theta = np.arccos(0.5*(np.trace(rotationMatrix)-1))
        if abs(theta) < np.pi and abs(theta) > 0:
            logR = (theta/(2*np.sin(theta)))*(rotationMatrix - np.transpose(rotationMatrix))
            rotationVector = Skew2Vec(logR)
        else:
            rotationVector = np.zeros(3)

    return rotationVector


#**function: compute rotation axis from given rotation vector
#**input: 3D rotation vector as np.array
#**output: 3D vector as np.array representing the rotation axis
def ComputeRotationAxisFromRotationVector(rotationVector):
    
    # compute rotation angle
    rotationAngle = np.linalg.norm(rotationVector)
    
    # compute rotation axis
    if rotationAngle == 0.0:
        rotationAxis = np.zeros(3)
    else:
        rotationAxis = rotationVector/rotationAngle
    
    # return rotation axis 
    return rotationAxis


#**function: convert rotation vector (parameters) (v) to G-matrix (=$\partial \tomega  / \partial \vv_t$)
#**input: vector of rotation vector (len=3) as list or np.array
#**output: 3x3 matrix G as np.array
def RotationVector2G(rotationVector):
    v = eulerParameters
    return RotationVector2RotationMatrix(rotationVector)

#**function: convert rotation vector (parameters) (v) to local G-matrix (=$\partial \LU{b}{\tomega}   / \partial \vv_t$)
#**input: vector of rotation vector (len=3) as list or np.array
#**output: 3x3 matrix G as np.array
def RotationVector2GLocal(eulerParameters):
    ep = eulerParameters
    return np.eye(3)



#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#            TAIT BRYAN ANGLES
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#**function: compute rotation matrix from consecutive xyz rotations (Tait-Bryan angles); A=Ax*Ay*Az; rot=[rotX, rotY, rotZ]
#**input: 3D vector of Tait-Bryan rotation parameters [X,Y,Z] in radiant
#**output: 3x3 rotation matrix as np.array
def RotXYZ2RotationMatrix(rot):
    c0 = np.cos(rot[0])
    s0 = np.sin(rot[0])
    c1 = np.cos(rot[1])
    s1 = np.sin(rot[1])
    c2 = np.cos(rot[2])
    s2 = np.sin(rot[2])
    
    return np.array([[c1*c2,-c1 * s2,s1],
                  [s0*s1*c2 + c0 * s2, -s0 * s1*s2 + c0 * c2,-s0 * c1],
                  [-c0 * s1*c2 + s0 * s2,c0*s1*s2 + s0 * c2,c0*c1 ]]);

#**function: convert rotation matrix to xyz Euler angles (Tait-Bryan angles);  A=Ax*Ay*Az; 
#**input:  3x3 rotation matrix as list of lists or np.array
#**output: vector of Tait-Bryan rotation parameters [X,Y,Z] (in radiant) as np.array
def RotationMatrix2RotXYZ(rotationMatrix):
    R=rotationMatrix
    #rot=np.array([0,0,0])
    rot=[0,0,0]
    rot[0] = np.arctan2(-R[1][2], R[2][2])
    rot[1] = np.arctan2(R[0][2], np.sqrt(abs(1. - R[0][2] * R[0][2]))) #fabs for safety, if small round up error in rotation matrix ...
    rot[2] = np.arctan2(-R[0][1], R[0][0])
    return np.array(rot);


#**function: compute time derivatives of angles RotXYZ from (global) angular velocity vector and given rotation
#**input:  
#  angularVelocity: global angular velocity vector as list or np.array
#  rotation: 3D vector of Tait-Bryan rotation parameters [X,Y,Z] in radiant
#**output: time derivative of vector of Tait-Bryan rotation parameters [X,Y,Z] (in radiant) as np.array
def AngularVelocity2RotXYZ_t(angularVelocity, rotation):
    psi = rotation[0]
    theta = rotation[1]
    #phi = rotation[2] #not needed
    cTheta = np.cos(theta)
    if cTheta == 0:
        print('AngularVelocity2RotXYZ_t: not possible for rotation[1] == pi/2, 3*pi/2, ...')

    GInv = (1/cTheta)*np.array([[np.cos(theta), np.sin(psi)*np.sin(theta),-np.cos(psi)*np.sin(theta)],
                                [0            , np.cos(psi)*np.cos(theta)   , np.sin(psi)*np.cos(theta)],
                                [0            ,-np.sin(psi)              , np.cos(psi)]])
    return np.dot(GInv,angularVelocity)
  
    
#**function: compute four Euler parameters from given RotXYZ angles, see \cite{Henderson1977}
#**input: 
#   alpha: 3D vector as np.array containing RotXYZ angles
#**output: 4D vector as np.array containing four Euler parameters 
#          entry zero of output represent the scalar part of Euler parameters
def RotXYZ2EulerParameters(alpha):
    psi   = alpha[0]
    theta = alpha[1]
    phi   = alpha[2]   
    u = 0.5*psi
    v = 0.5*theta
    w = 0.5*phi    
    cPsi   = np.cos(u)
    cTheta = np.cos(v)
    cPhi   = np.cos(w)    
    sPsi   = np.sin(u)
    sTheta = np.sin(v)
    sPhi   = np.sin(w)    
    q0 = -sPsi*sTheta*sPhi + cPsi*cTheta*cPhi
    q1 =  sPsi*cTheta*cPhi + sTheta*sPhi*cPsi    
    q2 = -sPsi*sPhi*cTheta + sTheta*cPsi*cPhi 
    q3 =  sPsi*sTheta*cPhi + sPhi*cPsi*cTheta 
    return np.array([q0, q1, q2, q3])


#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: compute rotation matrix w.r.t. X-axis (first axis)
#**input: angle around X-axis in radiant
#**output: 3x3 rotation matrix as np.array
def RotationMatrixX(angleRad):
    return np.array([[1, 0, 0],
                     [0, np.cos(angleRad),-np.sin(angleRad)],
                     [0, np.sin(angleRad), np.cos(angleRad)] ])

#**function: compute rotation matrix w.r.t. Y-axis (second axis)
#**input: angle around Y-axis in radiant
#**output: 3x3 rotation matrix as np.array
def RotationMatrixY(angleRad):
    return np.array([ [ np.cos(angleRad), 0, np.sin(angleRad)],
                      [0,        1, 0],
                      [-np.sin(angleRad),0, np.cos(angleRad)] ])

#**function: compute rotation matrix w.r.t. Z-axis (third axis)
#**input: angle around Z-axis in radiant
#**output: 3x3 rotation matrix as np.array
def RotationMatrixZ(angleRad):
    return np.array([ [np.cos(angleRad),-np.sin(angleRad), 0],
                      [np.sin(angleRad), np.cos(angleRad), 0],
                      [0,	    0,        1] ]);

    
#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: compute homogeneous transformation matrix from rotation matrix A and translation vector r
def HomogeneousTransformation(A, r):
    T = np.zeros((4,4))
    T[0:3,0:3] = A
    T[0:3,3] = r
    T[3,3] = 1
    return T

HT = HomogeneousTransformation #shortcut

#**function: homogeneous transformation for translation with vector r
def HTtranslate(r):
    T = np.eye(4)
    T[0:3,3] = r
    return T

#**function: identity homogeneous transformation:
def HT0():
    return np.eye(4)

#**function: homogeneous transformation for rotation around axis X (first axis)
def HTrotateX(angle):
    T = np.eye(4)
    T[0:3,0:3] = RotationMatrixX(angle)
    return T
    
#**function: homogeneous transformation for rotation around axis X (first axis)
def HTrotateY(angle):
    T = np.eye(4)
    T[0:3,0:3] = RotationMatrixY(angle)
    return T
    
#**function: homogeneous transformation for rotation around axis X (first axis)
def HTrotateZ(angle):
    T = np.eye(4)
    T[0:3,0:3] = RotationMatrixZ(angle)
    return T

#**function: return translation part of homogeneous transformation
def HT2translation(T):
    return T[0:3,3]

#**function: return rotation matrix of homogeneous transformation
def HT2rotationMatrix(T):
    return T[0:3,0:3]

#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: return inverse homogeneous transformation such that inv(T)*T = np.eye(4)
def InverseHT(T):
    Tinv = np.eye(4)
    Ainv = T[0:3,0:3].T #inverse rotation part
    Tinv[0:3,0:3] = Ainv
    r = T[0:3,3]        #translation part
    Tinv[0:3,3]  = -Ainv @ r       #inverse translation part
    return Tinv

################################################################################
#Test (compared with Robotcs, Vision and Control book of P. Corke:
#T=HTtranslate([1,0,0]) @ HTrotateX(np.pi/2) @ HTtranslate([0,1,0])
#print("T=",T.round(8))
#
#R = RotationMatrixZ(0.1) @ RotationMatrixY(0.2) @ RotationMatrixZ(0.3) 
#print("R=",R.round(4))

################################################################################

#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**class: helper class for rigid body inertia (see also derived classes Inertia...).
#Provides a structure to define mass, inertia and center of mass (com) of a rigid body.
#The inertia tensor and center of mass must correspond when initializing the body!
#**notes:
#   It is recommended to start with com=[0,0,0] and then to use Translated(...) and Rotated(...) to get transformed inertia parameters.
#**example:
#i0 = RigidBodyInertia(10,np.diag([1,2,3]))
#i1 = i0.Rotated(RotationMatrixX(np.pi/2))
#i2 = i1.Translated([1,0,0])
class RigidBodyInertia:
    #**classFunction: initialize RigidBodyInertia with scalar mass, 3x3 inertiaTensor and center of mass com
    def __init__(self, mass=0, inertiaTensor=np.zeros([3,3]), com=np.zeros(3)):
        
        if inertiaTensor.shape != (3,3): #shape is a tuple
            raise ValueError('RigidBodyInertia: must have dimensions (3,3), but received'+str(inertiaTensor.shape))
        self.mass = mass
        self.inertiaTensor = np.array(inertiaTensor)
        self.com = np.array(com)
        
    #**classFunction: add (+) operator allows adding another inertia information with SAME local coordinate system
    #only inertias with same center of rotation can be added!
    #**example: 
    #J = InertiaSphere(2,0.1) + InertiaRodX(1,2)
    def __add__(self, otherBodyInertia):
        sumMass = self.mass + otherBodyInertia.mass
        return RigidBodyInertia(mass=sumMass,
                                inertiaTensor = self.inertiaTensor + otherBodyInertia.inertiaTensor,
                                com=1./sumMass*(self.mass*self.com + otherBodyInertia.mass*otherBodyInertia.com))
        
    #**classFunction: returns a RigidBodyInertia with center of mass com shifted by vec; $\ra$ transforms the returned inertiaTensor to the new center of rotation
    def Translated(self, vec):
        #transform inertia to com=[0,0,0]
        inertia = self.inertiaTensor - self.mass*np.dot(Skew(self.com).transpose(),Skew(self.com))
        try:
            newCOM = self.com + vec
        except:
            raise ValueError("ERROR in RigidBodyInertia.Translated(vec): vec must be a vector with 3 components")
        inertia += self.mass*np.dot(Skew(newCOM).transpose(),Skew(newCOM))
        return RigidBodyInertia(mass=self.mass, 
                                inertiaTensor=inertia,
                                com=newCOM)

    #**classFunction: returns a RigidBodyInertia rotated by 3x3 rotation matrix rot, such that for a given J, the new inertia tensor reads Jnew = rot*J*rot.T
    #**notes: only allowed if COM=0 !
    def Rotated(self, rot):
        if NormL2(self.com) != 0:
            print("ERROR: RigidBodyInertia.Rotated only allowed in case of com=0")
            return 0
        try:
            inertia = np.dot(np.array(rot),np.dot(self.inertiaTensor,rot.transpose()))
        except:
            raise ValueError("ERROR in RigidBodyInertia.Rotated(rot): rot must be a 3x3 rotation matrix")
        return RigidBodyInertia(mass=self.mass, 
                                inertiaTensor=inertia,
                                com=self.com)

    #**classFunction: get vector with 6 inertia components (Jxx, Jyy, Jzz, Jyz, Jxz, Jxy) as needed in ObjectRigidBody
    def GetInertia6D(self):
        J = self.inertiaTensor
        return [J[0][0], J[1][1], J[2][2],  J[1][2], J[0][2], J[0][1]]

    def __str__(self):
        s = 'mass = ' + str(self.mass)
        s += '\nCOM = ' + str(self.com)
        s += '\ninertiaTensorAtOrigin = \n' + str(self.inertiaTensor)
        return s
    def __repr__(self):
        return str(self)


#**class: create RigidBodyInertia with moment of inertia and mass of a cuboid with density and side lengths sideLengths along local axes 1, 2, 3; inertia w.r.t. center of mass, com=[0,0,0]
#**example: InertiaCuboid(density=1000,sideLengths=[1,0.1,0.1])
class InertiaCuboid(RigidBodyInertia):
    def __init__(self, density, sideLengths):
        L1=sideLengths[0]
        L2=sideLengths[1]
        L3=sideLengths[2]
        newMass=density*L1*L2*L3
        RigidBodyInertia.__init__(self, mass=newMass,
                                  inertiaTensor=newMass/12.*np.diag([(L2**2 + L3**2),(L1**2 + L3**2),(L1**2 + L2**2)]),
                                  com=np.zeros(3))

#**class: create RigidBodyInertia with moment of inertia and mass of a rod with mass m and length L in local 1-direction (x-direction); inertia w.r.t. center of mass, com=[0,0,0]
class InertiaRodX(RigidBodyInertia):
    def __init__(self, mass, length):
        RigidBodyInertia.__init__(self, mass=mass,
                                  inertiaTensor=mass/12.*np.diag([0.,length**2,length**2]),
                                  com=np.zeros(3))
        
#**class: create RigidBodyInertia with moment of inertia and mass of mass point with 'mass'; inertia w.r.t. center of mass, com=[0,0,0]
class InertiaMassPoint(RigidBodyInertia):
    def __init__(self, mass):
        RigidBodyInertia.__init__(self, mass=mass,
                                  inertiaTensor=np.zeros([3,3]),
                                  com=np.zeros(3))

#**class: create RigidBodyInertia with moment of inertia and mass of sphere with mass and radius; inertia w.r.t. center of mass, com=[0,0,0]
class InertiaSphere(RigidBodyInertia):
    def __init__(self, mass, radius):
        J = 2.*mass/5.*radius**2
        RigidBodyInertia.__init__(self, mass=mass,
                                  inertiaTensor=np.diag([J,J,J]),
                                  com=np.zeros(3))
        
#**class: create RigidBodyInertia with moment of inertia and mass of hollow sphere with mass (concentrated at circumference) and radius; inertia w.r.t. center of mass, com=0
class InertiaHollowSphere(RigidBodyInertia):
    def __init__(self, mass, radius):
        J = 2.*mass/3.*radius**2
        RigidBodyInertia.__init__(self, mass=mass,
                                  inertiaTensor=np.diag([J,J,J]),
                                  com=np.zeros(3))

#**class: create RigidBodyInertia with moment of inertia and mass of cylinder with density, length and outerRadius; axis defines the orientation of the cylinder axis (0=x-axis, 1=y-axis, 2=z-axis); for hollow cylinder use innerRadius != 0; inertia w.r.t. center of mass, com=[0,0,0]
class InertiaCylinder(RigidBodyInertia):
    def __init__(self, density, length, outerRadius, axis, innerRadius=0):
        m = density*length*np.pi*(outerRadius**2-innerRadius**2)
        Jaxis = 0.5*m*(outerRadius**2+innerRadius**2)
        Jtt = 1./12.*m*(3*(outerRadius**2+innerRadius**2)+length**2)

        if axis==0:
            RigidBodyInertia.__init__(self, mass=m,
                                      inertiaTensor=np.diag([Jaxis,Jtt,Jtt]),
                                      com=np.zeros(3))
        elif axis==1:
            RigidBodyInertia.__init__(self, mass=m,
                                      inertiaTensor=np.diag([Jtt,Jaxis,Jtt]),
                                      com=np.zeros(3))
        elif axis==2:
            RigidBodyInertia.__init__(self, mass=m,
                                      inertiaTensor=np.diag([Jtt,Jtt,Jaxis]),
                                      com=np.zeros(3))
        else:
            raise ValueError("InertiaCylinder: axis must be 0, 1 or 2!")
        
        
    
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: get node item interface according to nodeType, using initialization with position, velocity, angularVelocity and rotationMatrix
#**input:
#   position: reference position as list or numpy array with 3 components
#   velocity: initial translational velocity as list or numpy array with 3 components
#   rotationMatrix: 3x3 list or numpy matrix to define reference rotation; use EITHER rotationMatrix=[[...],[...],[...]] (while rotationParameters=[]) or rotationParameters=[...] (while rotationMatrix=[]) 
#   rotationParameters: reference rotation parameters; use EITHER rotationMatrix=[[...],[...],[...]] (while rotationParameters=[]) or rotationParameters=[...] (while rotationMatrix=[]) 
#   angularVelocity: initial angular velocity as list or numpy array with 3 components
#**output: returns list containing node number and body number: [nodeNumber, bodyNumber]
def GetRigidBodyNode(nodeType, 
                 position=[0,0,0], 
                 velocity=[0,0,0], 
                 rotationMatrix= [],
                 rotationParameters = [],
                 angularVelocity=[0,0,0]):

    if len(rotationMatrix) != 0 and len(rotationParameters) != 0:
        raise ValueError('GetRigidBodyNode: either rotationMatrix or rotationParameters must empty!')
    if len(rotationMatrix) == 0 and len(rotationParameters) == 0:
        rotationMatrix=np.eye(3)

    strNodeType = str(nodeType) #works both for nodeType and for strings (if exudyn not available)

    nodeItem = []
    if strNodeType == 'NodeType.RotationEulerParameters':
        if len(rotationParameters) == 0:
            ep0 = RotationMatrix2EulerParameters(rotationMatrix)
        else:
            ep0 = rotationParameters
           
        ep_t0 = AngularVelocity2EulerParameters_t(angularVelocity, ep0)
        nodeItem = NodeRigidBodyEP(referenceCoordinates=list(position)+list(ep0),
                                   initialVelocities=list(velocity)+list(ep_t0))       
    elif strNodeType == 'NodeType.RotationRxyz':
        if len(rotationParameters) == 0:
            rot0 = RotationMatrix2RotXYZ(rotationMatrix)
        else:
            rot0 = rotationParameters

        rot_t0 = AngularVelocity2RotXYZ_t(angularVelocity, rot0)
        nodeItem = NodeRigidBodyRxyz(referenceCoordinates=list(position)+list(rot0),
                                     initialVelocities=list(velocity)+list(rot_t0))
    elif strNodeType == 'NodeType.RotationRotationVector':
        if len(rotationParameters) == 0:
            #raise ValueError('NodeType.RotationRotationVector not implemented!')
            rot0 = RotationMatrix2RotationVector(rotationMatrix)
        else:
            rot0 = rotationParameters
        
        rotMatrix = RotationVector2RotationMatrix(rot0) #rotationMatrix needed!
        angularVelocityLocal = np.dot(rotMatrix.transpose(),angularVelocity)
            
        nodeItem = NodeRigidBodyRotVecLG(referenceCoordinates=list(position) + list(rot0), 
                                         initialVelocities=list(velocity)+list(angularVelocityLocal))
    else:
        raise ValueError("GetRigidBodyNode: invalid node type:"+strNodeType)

    return nodeItem

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: adds a node (with str(exu.NodeType. ...)) and body for a given rigid body; all quantities (esp. velocity and angular velocity) are given in global coordinates!
#**input:
#   position: reference position as list or numpy array with 3 components
#   velocity: initial translational velocity as list or numpy array with 3 components
#   rotationMatrix: 3x3 list or numpy matrix to define reference rotation; use EITHER rotationMatrix=[[...],[...],[...]] (while rotationParameters=[]) or rotationParameters=[...] (while rotationMatrix=[]) 
#   rotationParameters: reference rotation parameters; use EITHER rotationMatrix=[[...],[...],[...]] (while rotationParameters=[]) or rotationParameters=[...] (while rotationMatrix=[]) 
#   angularVelocity: initial angular velocity as list or numpy array with 3 components
#   gravity: if provided as list or numpy array with 3 components, it adds gravity force to the body at the COM, i.e., fAdd = m*gravity
#   graphicsDataList: list of graphicsData objects to define appearance of body
#**output: returns list containing node number and body number: [nodeNumber, bodyNumber]
def AddRigidBody(mainSys, inertia, nodeType, 
                 position=[0,0,0], velocity=[0,0,0], 
                 rotationMatrix= [],
                 rotationParameters = [],
                 angularVelocity=[0,0,0],
                 gravity=[0,0,0],
                 graphicsDataList=[]):

    if len(rotationMatrix) != 0 and len(rotationParameters) != 0:
        raise ValueError('AddRigidBody: either rotationMatrix or rotationParameters must empty!')
    if len(rotationMatrix) == 0 and len(rotationParameters) == 0:
        rotationMatrix=np.eye(3)
        
    nodeItem = GetRigidBodyNode(nodeType, position, velocity, rotationMatrix, rotationParameters, angularVelocity)
    nodeNumber = mainSys.AddNode(nodeItem)
    
#     strNodeType = str(nodeType)
#     if strNodeType == 'NodeType.RotationEulerParameters':
#         if len(rotationParameters) == 0:
#             ep0 = RotationMatrix2EulerParameters(rotationMatrix)
#         else:
#             ep0 = rotationParameters
           
#         ep_t0 = AngularVelocity2EulerParameters_t(angularVelocity, ep0)
#         nodeNumber = mainSys.AddNode(NodeRigidBodyEP(referenceCoordinates=list(position)+list(ep0), 
#                                                      initialVelocities=list(velocity)+list(ep_t0)))
#     elif strNodeType == 'NodeType.RotationRxyz':
#         if len(rotationParameters) == 0:
#             rot0 = RotationMatrix2RotXYZ(rotationMatrix)
#         else:
#             rot0 = rotationParameters

#         rot_t0 = AngularVelocity2RotXYZ_t(angularVelocity, rot0)
#         nodeNumber = mainSys.AddNode(NodeRigidBodyRxyz(referenceCoordinates=list(position)+list(rot0), 
#                                                        initialVelocities=list(velocity)+list(rot_t0)))
#     elif strNodeType == 'NodeType.RotationRotationVector':
#         if len(rotationParameters) == 0:
#             #raise ValueError('NodeType.RotationRotationVector not implemented!')
#             rot0 = RotationMatrix2RotationVector(rotationMatrix)
#         else:
#             rot0 = rotationParameters
        
#         rotMatrix = RotationVector2RotationMatrix(rot0) #rotationMatrix needed!
#         angularVelocityLocal = np.dot(rotMatrix.transpose(),angularVelocity)
            
#         nodeNumber = mainSys.AddNode(NodeRigidBodyRotVecLG(referenceCoordinates=list(position) + list(rot0), 
#                                                            initialVelocities=list(velocity)+list(angularVelocityLocal)))

    bodyNumber = mainSys.AddObject(ObjectRigidBody(physicsMass=inertia.mass, physicsInertia=inertia.GetInertia6D(), 
                                                   physicsCenterOfMass=inertia.com,
                                                   nodeNumber=nodeNumber, 
                                                   visualization=VObjectRigidBody(graphicsData=graphicsDataList)))
    
    if NormL2(gravity) != 0.:
        markerNumber = mainSys.AddMarker(MarkerBodyMass(bodyNumber=bodyNumber))
        mainSys.AddLoad(LoadMassProportional(markerNumber=markerNumber, loadVector=gravity))
    
    return [nodeNumber, bodyNumber]





    
