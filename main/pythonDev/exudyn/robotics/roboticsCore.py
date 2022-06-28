#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN python utility library
#
# Details:  A library which includes support functions for robotics;
#			the library is built on standard Denavit-Hartenberg Parameters and
#			Homogeneous Transformations (HT) to describe transformations and coordinate systems;
#           import this library e.g. with import exudyn.robotics as robotics
#
# Author:   Johannes Gerstmayr
# Date:     2020-04-14
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
# Example:	New robot model uses the class Robot with class RobotLink; the old dictionary structure is defined in the example in ComputeJointHT for the definition of the 'robot' dictionary.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#constants and fixed structures:
import numpy as np

import exudyn
from exudyn.itemInterface import *
from exudyn.utilities import *
from exudyn.rigidBodyUtilities import *

from copy import copy, deepcopy


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++  Define robot link +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define convert text named joint types to exudyn joint types
dictJointTypeText2Exudyn = {
    'Rx':exudyn.JointType.RevoluteX, #revolute joint for local X axis
    'Ry':exudyn.JointType.RevoluteY, #revolute joint for local Y axis
    'Rz':exudyn.JointType.RevoluteZ, #revolute joint for local Z axis
    'Px':exudyn.JointType.PrismaticX, #prismatic joint for local X axis
    'Py':exudyn.JointType.PrismaticY, #prismatic joint for local Y axis
    'Pz':exudyn.JointType.PrismaticZ, #prismatic joint for local Z axis
    }

#define dictionary for joint transformations as homogeneous transformations, replacement for switch/case
dictJointType2HT = {
    'Rx':HTrotateX, #revolute joint for local X axis
    'Ry':HTrotateY, #revolute joint for local Y axis
    'Rz':HTrotateZ, #revolute joint for local Z axis
    'Px':HTtranslateX, #prismatic joint for local X axis
    'Py':HTtranslateY, #prismatic joint for local Y axis
    'Pz':HTtranslateZ, #prismatic joint for local Z axis
    }

#define dictionary for joint transformations as homogeneous transformations, replacement for switch/case
dictJointType2Axis = {
    'Rx':np.array([1,0,0]), #revolute joint for local X axis
    'Ry':np.array([0,1,0]), #revolute joint for local Y axis
    'Rz':np.array([0,0,1]), #revolute joint for local Z axis
    'Px':np.array([1,0,0]), #prismatic joint for local X axis
    'Py':np.array([0,1,0]), #prismatic joint for local Y axis
    'Pz':np.array([0,0,1]), #prismatic joint for local Z axis
    }

#maps joint type to (constrained) coordinate 0..2: translation, 3..5: rotation
dictJointType2coordinate6D = {
    'Px':0, #revolute joint for local X axis
    'Py':1, #revolute joint for local Y axis
    'Pz':2, #revolute joint for local Z axis
    'Rx':3, #prismatic joint for local X axis
    'Ry':4, #prismatic joint for local Y axis
    'Rz':5, #prismatic joint for local Z axis
}

#RobotLink changes 2021-08-16:
#localHT->localHT (does not include rotation); this joint to this link HT
#add preHT: previous link to this joint HT
#dictJointType2HT[link.jointType](q[i]): defines joint HT
#localHT in MBS modeling (inertia, etc.) is kept same, while localHT = ID4 for ModDH
#preHT must be added for joint axes definitions and for currentHT
#inertia and COM need to be converted by localHT of StdDH into ModDH configuration!!!

#**class: class to define visualization of RobotLink
class VRobotLink:
    #**classFunction: initialize robot link with parameters, being self-explaining
    #**input:
    #  jointRadius: radius of joint to draw
    #  jointWidth: length or width of joint (depending on type of joint)
    #  showMBSjoint: if False, joint is not drawn
    #  linkWidth: width of link for default drawing
    #  linkColor: color of link for default drawing
    #  showCOM: if True, center of mass is marked with cube
    #  graphicsData: list of GraphicsData to represent link; if list is empty, link graphics will be generated from link geometry data; otherwise, drawing will be taken from graphicsData, and only showMBSjoint and showCOM flags will add additional graphics
    def __init__(self, jointRadius = 0.06, jointWidth = 0.12, linkWidth = 0.1, showMBSjoint = True, showCOM = True, 
                 linkColor = [0.4,0.4,0.4,1], graphicsData = [] ):
        self.jointRadius = jointRadius
        self.jointWidth = jointWidth 
        self.showMBSjoint = showMBSjoint
        self.linkWidth = linkWidth 
        self.linkColor = linkColor
        self.showCOM = showCOM
        self.graphicsData = graphicsData

    def __str__(self):
        s = '  jointRadius = ' + str(self.jointRadius)
        s += '\n  jointWidth = ' + str(self.jointWidth)
        s += '\n  showMBSjoint = ' + str(self.showMBSjoint)
        s += '\n  linkWidth = ' + str(self.linkWidth)
        s += '\n  linkColor = ' + str(self.linkColor)
        s += '\n  showCOM = ' + str(self.showCOM)
        gDataStr = ''
        if len(self.graphicsData) != 0:
            gDataStr = '...'
        s += '\n  graphicsData = [' + gDataStr + ']'
            
        return s
    def __repr__(self):
        return str(self)


#**class: class to define one link of a robot
class RobotLink:
    #**classFunction: initialize robot link
    #**input:
    #  mass: mass of robot link
    #  COM: center of mass in link coordinate system
    #  inertia: 3x3 matrix (list of lists / numpy array) containing inertia tensor in link coordinates, with respect to center of mass
    #  localHT: 4x4 matrix (list of lists / numpy array) containing homogeneous transformation from local joint to link coordinates; default = identity; currently, this transformation is not available in KinematicTree, therefore the link inertia and COM must be transformed accordingly
    #  preHT: 4x4 matrix (list of lists / numpy array) containing homogeneous transformation from previous link to this joint; default = identity
    #  jointType: string containing joint type, out of: 'Rx', 'Ry', 'Rz' for revolute joints and 'Px', 'Py', 'Pz' for prismatic joints around/along the respecitive local axes
    #  parent: for building robots as kinematic tree; use '-2' to automatically set parents for serial robot (on fixed base), use '-1' for ground-parent and any other 0-based index for connection to parent link
    #  PDcontrol: tuple of P and D control values, defining position (rotation) proportional value P and velocitiy proportional value D
    #  visualization: VRobotLink structure containing options for drawing of link and joints; see class VRobotLink
    def __init__(self, mass, COM, inertia, localHT=HT0(), jointType='Rz', parent=-2, preHT=HT0(), PDcontrol=(None,None), visualization=VRobotLink()):
        self.mass = mass
        self.COM = np.array(COM)
        self.inertia = np.array(inertia)
        self.localHT = np.array(localHT)
        self.preHT = np.array(preHT)
        self.jointType = jointType
        self.parent = parent
        self.visualization = deepcopy(visualization)
        if PDcontrol[0] != None:
            self.PDcontrol = PDcontrol

    #**classFunction: set PD control values for drive of joint related to link using position-proportional value P and differential value (velocity proportional) D
    def SetPDcontrol(self, Pvalue, Dvalue):
        self.PDcontrol = (Pvalue, Dvalue)

    #**classFunction: check if contrl is available
    def HasPDcontrol(self):
        return hasattr(self, 'PDcontrol')

    #**classFunction: get PD control values
    def GetPDcontrol(self):
        if not hasattr(self, 'PDcontrol'):
            raise ValueError('RobotLink: PDcontrol is not defined for link! Use SetPDcontrol to define parameters before using')
        return self.PDcontrol

    def __str__(self):
        s = '  mass = ' + str(self.mass)
        s += '\n  COM = ' + str(self.COM)
        s += '\n  inertia = ' + str(self.inertia)
        s += '\n  localHT = ' + str(self.localHT)
        s += '\n  preHT = ' + str(self.preHT)
        s += '\n  jointType = ' + str(self.jointType)
        s += '\n  parent = ' + str(self.parent)
        s += '\n  visualization:\n' + str(self.visualization)
        if self.HasPDcontrol():
            s += '\n  PDcontrol = ' + str(self.PDcontrol)
        return s

    def __repr__(self):
        return str(self)
        
#**class: class to define visualization of RobotTool
class VRobotTool:
    #**classFunction: initialize robot tool with parameters; currently only graphicsData, which is a list of GraphicsData same as in mbs Objects
    def __init__(self, graphicsData=[]):
        self.graphicsData = graphicsData

#**class: define tool of robot: containing graphics and HT (may add features in future)
class RobotTool:
    #**classFunction: initialize robot tool
    #**input:
    #  HT: 4x4 matrix (list of lists / numpy array) containing homogeneous transformation to transform from last link to tool
    #  graphicsData: dictionary containing a list of GraphicsData, same as in exudyn Objects
    def __init__(self, HT=HT0(), visualization=VRobotTool()):
        self.HT = np.array(HT)
        self.visualization = visualization
        
    def __str__(self):
        s = '  HT = ' + str(self.HT)
        #s += '\nvisualization = ' + str(self.visualization)
        return s
    def __repr__(self):
        return str(self)

#**class: class to define visualization of RobotBase
class VRobotBase:
    #**classFunction: initialize robot base with parameters; currently only graphicsData, which is a list of GraphicsData same as in mbs Objects
    def __init__(self, graphicsData=[]):
        self.graphicsData = graphicsData

    
#**class: define base of robot: containing graphics and HT (may add features in future)
class RobotBase:
    #**classFunction: initialize robot base
    #**input:
    #  HT: 4x4 matrix (list of lists / numpy array) containing homogeneous transformation to transform from world coordinates to base coordinates (changes orientation and position of robot)
    #  graphicsData: dictionary containing a list of GraphicsData, same as in exudyn Objects
    def __init__(self, HT=HT0(), visualization=VRobotBase()):
        self.HT = np.array(HT)
        self.visualization = visualization
    
    def __str__(self):
        s = '  HT = ' + str(self.HT)
        #s += '\nvisualization = ' + str(self.visualization)
        return s

    def __repr__(self):
        return str(self)


buildFromDictionaryWarned = False #do not use this variable, it is for deprecation warnings!
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++  Define robot ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**class: class to define a robot
class Robot:
    #**classFunction: initialize robot class
    #**input:
    #  base: definition of base using RobotBase() class
    #  tool: definition of tool using RobotTool() class
    #  gravity: a list or 3D numpy array defining gravity
    #  referenceConfiguration: a list of scalar quantities defining the parameters for reference configuration
    def __init__(self, 
                 gravity=[0,0,-9.81],
                 base = RobotBase(),
                 tool = RobotTool(),
                 referenceConfiguration = []
                 ):
        self.gravity = np.array(gravity)
        self.base = base
        self.tool = tool
        self.referenceConfiguration = np.array(referenceConfiguration)
        self.links = [] #initialize list of link data
        self.isSerialRobot = True #this is true as long as parent = link index - 1

    def __str__(self):
        s = 'gravity = ' + str(self.gravity)
        s += '\nreferenceConfiguration = ' + str(self.referenceConfiguration)
        s += '\nbase: \n' + str(self.base)
        s += '\ntool: \n' + str(self.tool)
        s += '\nlinks: \n' + str(self.links)
        return s
    
    def __repr__(self):
        return str(self)
        

    #**classFunction: add a link to serial robot
    def AddLink(self, robotLink):
        i = len(self.links) #current index
        if len(self.referenceConfiguration) == i: #extend reference configuration, if not specified by user during initialization
            self.referenceConfiguration = np.hstack((self.referenceConfiguration,[0]))

        self.links += [deepcopy(robotLink)]
        if self.links[i].parent == -2: #in this case, automatically set parents for serial robot (chain)
            self.links[i].parent = i-1
        elif self.links[i].parent >= i:
            raise ValueError('Robot.AddLink(...): link parent index must be always lower than link index')

        if  self.links[i].parent != i-1:
            self.isSerialRobot = False
        
        if not self.isSerialRobot and (np.linalg.norm(self.tool.HT - HT0()) >= 1e-15
            or self.tool.visualization.graphicsData != []):
            exudyn.Print('Warning: class Robot: tool defined in kinematic tree; currently tool is only allowed for serial robots')

        return i #return index of link

    #**classFunction: return True, if robot is a serial robot
    def IsSerialRobot(self):
        return self.isSerialRobot

    #**classFunction: return Link object of link i
    def GetLink(self, i):
        return self.links[i]

    #**classFunction: True if link has parent, False if not
    def HasParent(self, i):
        return self.links[i].parent >= 0

    #**classFunction: Get index of parent link; for serial robot this is simple, but for general trees, there is a index list
    def GetParentIndex(self, i):
        return self.links[i].parent

    
    #**classFunction: return number of links
    def NumberOfLinks(self):
        return len(self.links)

    #**classFunction: return base as homogeneous transformation
    def GetBaseHT(self):
        return self.base.HT

    #**classFunction: return base as homogeneous transformation
    def GetToolHT(self):
        return self.tool.HT
    
    #**classFunction: compute list of homogeneous transformations for every link, using current joint coordinates q; leads to different results for standard and modified DH parameters because link coordinates are different!
    def LinkHT(self, q):
        HT = []

        # #only for serial robots:
        # Tcurrent = self.base.HT
        # for i in range(len(self.links)):
        #     link = self.links[i]

        #     #call function to compute HT for joint rotation/translation:
        #     T01 = link.preHT @ dictJointType2HT[link.jointType](q[i]) @ link.localHT

        #     Tcurrent = Tcurrent @ T01
        #     HT += [copy(Tcurrent)]


        for i in range(len(self.links)):
            link = self.links[i]

            T01 = link.preHT @ dictJointType2HT[link.jointType](q[i]) @ link.localHT
            if self.HasParent(i):
                pIndex = self.GetParentIndex(i)
                Tcurrent = HT[pIndex] @ T01
            else:
                Tcurrent = self.base.HT @ T01
            HT += [copy(Tcurrent)]
        
        return HT    

    #**classFunction: compute list of homogeneous transformations for every joint (after rotation), using current joint coordinates q
    def JointHT(self, q):
        HT = []

        # #only for serial robots:
        # Tcurrent = self.base.HT
        # for i in range(len(self.links)):
        #     link = self.links[i]

        #     T01 = link.preHT @ dictJointType2HT[link.jointType](q[i])
        #     Tcurrent = Tcurrent @ T01
        #     HT += [copy(Tcurrent)]
            
        #     Tcurrent = Tcurrent @ link.localHT

        for i in range(len(self.links)):
            link = self.links[i]

            T01 = link.preHT @ dictJointType2HT[link.jointType](q[i])
            if self.HasParent(i):
                pIndex = self.GetParentIndex(i)
                Tcurrent = HT[pIndex] @ self.links[pIndex].localHT @ T01
            else:
                Tcurrent = self.base.HT @ T01
            HT += [copy(Tcurrent)]
        return HT

    #**classFunction: compute list of  homogeneous transformations HT from base to every COM using HT list from Robot.JointHT(...)
    def COMHT(self, HT):
        HTCOM = []
        
        for i in range(len(self.links)):
            HTCOM += [HT[i] @ self.links[i].localHT @ HTtranslate(self.links[i].COM)]
        
        return HTCOM
    
    #**classFunction: compute list of joint torques for serial robot due to gravity (gravity and mass as given in robot), taking HT from Robot.JointHT()
    def StaticTorques(self,HT):
        jointTorques = np.zeros(np.size(self.links))
    
        #compute HTs for COM
        HTcom=self.COMHT(HT)
        
        #sum up the torques of all gravity loads:
        for i in range(len(HTcom)):
            p = HT2translation(HTcom[i])
            Jcom = self.Jacobian(HT[0:i+1],toolPosition=p,mode='trans')
            fG = self.links[i].mass * self.gravity
            tau = Jcom.T @ fG
            jointTorques[0:i+1] += tau
        return jointTorques
    
    
    #**classFunction: compute jacobian for translation and rotation at toolPosition using joint HT; this is using the Robot functions, but is inefficient for simulation purposes
    #**input:
    #  HT: list of homogeneous transformations per joint , as computed by Robot.JointHT(...)
    #  toolPosition: global position at which the jacobian is evaluated (e.g., COM); if empty [], it uses the origin of the last link
    #  mode: 'all'...translation and rotation jacobian, 'trans'...only translation part, 'rot': only rotation part
    #  linkIndex: link index for which the jacobian is evaluated; if linkIndex==None, it uses the last link provided in HT
    #**output: returns jacobian with translation and rotation parts in rows (3 or 6) according to mode, and one column per HT; in the kinematic tree the columns not related to linkIndex remain zero
    def Jacobian(self, HT, toolPosition=[], mode='all', linkIndex=None):
        n = len(HT)
        if n > len(self.links):
            print("ERROR: number of homogeneous transformations (HT) greater than number of links")

        #link index is usually the last link contained in HT (all subsequent columns in jacobian would be anyway zero):
        if linkIndex == None:
            linkIndex = n-1
    
        Jomega = np.zeros((3,n))#rotation part of jacobian
        Jvel = np.zeros((3,n))  #translation part of jacobian
        #A = HT2rotationMatrix(self.GetBaseHT())
        #vPrevious = HT2translation(self.GetBaseHT())
        
        vn = toolPosition
        if len(vn) == 0:
            vn = HT2translation(HT[linkIndex] @ self.links[linkIndex].localHT) #last link coordinates


        #for tree, we may only consider links in the chain from link to base!
        i = linkIndex
        endReached = False
        while not endReached:
            #if i > 0:
            A = HT2rotationMatrix(HT[i]) #rotation of joint i

            # localAxis = HT2rotationMatrix(self.links[i].preHT) @ dictJointType2Axis[self.links[i].jointType]
            localAxis = dictJointType2Axis[self.links[i].jointType]

            axis = A @ localAxis #axis in global coordinates
            #OLD DH parameter based: Jomega[0:3,i] = robot['jointType'][i] * axis #only considered, if revolute joint
            if self.links[i].jointType[0] == 'R':
                Jomega[0:3,i] = axis #only considered, if revolute joint
            
            #vPrevious = HT2translation(HT[i] @ self.links[i].preHT)
            vPrevious = HT2translation(HT[i])
             
            #revolute joint:
            if self.links[i].jointType[0] == 'R': #revolute joint
                Jvel[0:3,i]   = Skew(axis) @ (vn - vPrevious) #only considered, if revolute joint
            elif self.links[i].jointType[0] == 'P': #prismatic joint
                Jvel[0:3,i]   = axis
            else:
                raise ValueError('Robot.Jacobian(...): illegal jointType')
                
            if not self.HasParent(i):
                endReached = True
            else:
                i = self.GetParentIndex(i)
        
        if mode == 'rot':
            J = Jomega
        elif mode == 'trans':
            J = Jvel
        elif mode == 'all':
            J = np.zeros((6,n))
            J[0:3,0:n] = Jvel
            J[3:6,0:n] = Jomega
    
        return J


    #%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    #**classFunction: Add a ObjectKinematicTree to existing mbs from the robot structure inside this robot class;
    #                 Joints defined by the kinematics as well as links (and inertia) are transferred to the kinematic tree object;
    #                 Current implementation only works for serial robots;
    #                 Control can be realized simply by adding PDcontrol to RobotLink structures, then modifying jointPositionOffsetVector and jointVelocityOffsetVector in ObjectKinematicTree; force offsets (e.g., static or dynamic torque compensation) can be added to KinematicTree jointForceVector; more general control can be added by using KinematicTree forceUserFunction;
    #                 The coordinates in KinematicTree (as well as jointPositionOffsetVector, etc.) are sorted in the order as the RobotLinks are added to the Robot class;
    #                 Note that the ObjectKinematicTree is still under development and interfaces may change.
    #**input: 
    #   mbs: the multibody system, which will be extended
    #   name: object name in KinematicTree; transferred to KinematicTree, default = ''
    #   forceUserFunction: defines the user function for computation of joint forces in KinematicTree; transferred to KinematicTree, default = 0
    #**output: the function returns a dictionary containing 'nodeGeneric': generic ODE2 node number ,'objectKinematicTree': the kinematic tree object, 'baseObject': the base object if created, otherwise None; further values will be added in future
    def CreateKinematicTree(self, mbs, name = '', forceUserFunction = 0):
        #def CreateKinematicTree(self, mbs, jointSpringDamperUserFunctionList=[]):

        #add graphics for base:
        baseObject = None #if it does not exist
        baseOffset = HT2translation(self.base.HT)

        if self.base.visualization.graphicsData != []:
            #add a ground object at base position
            graphicsDataBase = []
            pOff = baseOffset
            Aoff = HT2rotationMatrix(self.base.HT)
            for data in self.base.visualization.graphicsData:
                graphicsDataBase += [MoveGraphicsData(data, [0,0,0], Aoff)] #only rotated, translation is in ground

            baseObject = mbs.AddObject(ObjectGround(referencePosition=pOff, 
                                                    visualization=VObjectGround(graphicsData=graphicsDataBase)))

        #+++++++++++++++++++++++
        Tcurrent = self.GetBaseHT()
        qRef = self.referenceConfiguration
        
        nLinks = len(self.links)
        graphicsDataList = []   #list of graphicsData per link
        jointTypesList = []     #exudyn joint types
        linkParents = []

        jointTransformations=[]
        jointOffsets=[]
        linkInertiasCOM=[] 
        linkCOMs=[]
        linkMasses=[]

        #if PD control exists, this vector is kept, otherwise erased:
        jointPControlVector = [0]*nLinks
        jointDControlVector = [0]*nLinks
        jointPositionOffsetVector = [0]*nLinks
        jointVelocityOffsetVector = [0]*nLinks
        hasPDcontrol = False #True, if any link has control
        
        #create robot tree:
        for i in range(nLinks):
            link = self.links[i]
            linkParents += [self.GetParentIndex(i)]

            # if np.linalg.norm(link.localHT - HT0()) >= 1e-14: #now implemented
            #     raise ValueError('CreateKinematicTree: can only convert robots with localHT = identity')
            if link.jointType not in dictJointType2Axis:
                raise ValueError('CreateKinematicTree: found invalid joint type in link '+str(i)+':'+link.jointType)


            axis = dictJointType2Axis[link.jointType]
            
            if link.HasPDcontrol():
                jointPControlVector[i] = link.GetPDcontrol()[0]
                jointDControlVector[i] = link.GetPDcontrol()[1]
                hasPDcontrol = True
                
            # com = link.COM
            #++++++++++++++++++++++++++++++++++
            jointTypesList += [dictJointTypeText2Exudyn[link.jointType]]
            # jointTransformations += [HT2rotationMatrix(link.preHT)] 
            # jointOffsets += [HT2translation(link.preHT)]
            
            # 
            parentLinkLocalHT = HT0()
            if self.HasParent(i):
                parentLinkLocalHT = self.links[self.GetParentIndex(i)].localHT
            jointTransformations += [HT2rotationMatrix(parentLinkLocalHT @ link.preHT)] 
            jointOffsets += [HT2translation(parentLinkLocalHT @ link.preHT)]

            #inertia is defined in link coordinates; but KinematicTree needs inertia w.r.t. joint coordinates:
            rbi = RigidBodyInertia()
            rbi.SetWithCOMinertia(link.mass, link.inertia, link.COM)
    
            rbi = rbi.Transformed((link.localHT)) #inertia parameters need to be transformed to joint frame
            
            linkInertiasCOM += [rbi.InertiaCOM()] #KinematicTree needs inertia w.r.t. COM
            linkCOMs += [rbi.COM()] 
            linkMasses += [rbi.Mass()]
            # linkInertiasCOM += [link.inertia] #is already w.r.t. COM
            # linkCOMs += [link.COM]
            # linkMasses += [link.mass]

            
            #++++++++++++++++++++++++++++++++++
            #visualization:
            linkVisualization = link.visualization
            showMBSjoint = linkVisualization.showMBSjoint
            r = linkVisualization.jointRadius
            wJ = linkVisualization.jointWidth
            wL = linkVisualization.linkWidth
            color = linkVisualization.linkColor
            showCOM = linkVisualization.showCOM

            graphicsDataLink = []
            #draw COM:
            if linkVisualization.showCOM:
                dd = r*0.2
                colorCOM = copy(color)
                colorCOM[0] *= 0.7 #make COM a little darker
                colorCOM[1] *= 0.7
                colorCOM[2] *= 0.7
                graphicsDataLink += [GraphicsDataOrthoCubePoint(rbi.COM(), [dd,dd,dd], colorCOM)]


            #draw joint in this link
            if showMBSjoint:
                gJoint = GraphicsDataCylinder(-0.5*wJ*axis, 0.5*wJ*axis, radius=r, color=color4grey)
                graphicsDataLink += [gJoint]
                
            #draw link from parent link origin to this link origin, defined by preHT of this link
            if self.HasParent(i):
                iParent = self.GetParentIndex(i)
                parentLink = self.GetLink(iParent)
                vParent = HT2translation(parentLinkLocalHT@link.preHT)
                if len(linkVisualization.graphicsData) == 0:
                    gLink = GraphicsDataCylinder([0.,0.,0.], vParent, radius=0.5*wL, color=color)
                    graphicsDataList[iParent] += [gLink]

            if len(linkVisualization.graphicsData) != 0:
                graphicsDataLink += linkVisualization.graphicsData

            #add transformed graphicsData of tool to link graphics
            if (i==len(self.links)-1 and #tool
                self.tool.visualization.graphicsData != []):
                pOff = HT2translation(self.tool.HT)
                Aoff = HT2rotationMatrix(self.tool.HT)
                for data in self.tool.visualization.graphicsData:
                    graphicsDataLink += [MoveGraphicsData(data, pOff, Aoff)] 

            graphicsDataList += [graphicsDataLink]
        
        #create node for unknowns of KinematicTree
        nGeneric = mbs.AddNode(NodeGenericODE2(referenceCoordinates=qRef,
                                               initialCoordinates=[0.]*nLinks,
                                               initialCoordinates_t=[0.]*nLinks,
                                               numberOfODE2Coordinates=nLinks))

        if not hasPDcontrol: #use empty lists, makes model more efficient
            jointPControlVector = []
            jointDControlVector = []
            jointPositionOffsetVector = [] #size must agree with jointPControlVector
            jointVelocityOffsetVector = [] #size must agree with jointDControlVector

        jointForceVector = [] #[0]*nLinks #not used right now


        #create KinematicTree
        oKT = mbs.AddObject(ObjectKinematicTree(nodeNumber=nGeneric, jointTypes=jointTypesList, linkParents=linkParents,
                                          jointTransformations=exudyn.Matrix3DList(jointTransformations), 
                                          jointOffsets=exudyn.Vector3DList(jointOffsets), 
                                          linkInertiasCOM=exudyn.Matrix3DList(linkInertiasCOM), 
                                          linkCOMs=exudyn.Vector3DList(linkCOMs), linkMasses=linkMasses, 
                                          baseOffset = baseOffset, gravity=self.gravity, 
                                          jointForceVector=jointForceVector, forceUserFunction=forceUserFunction,
                                          jointPositionOffsetVector = jointPositionOffsetVector, jointPControlVector = jointPControlVector,
                                          jointVelocityOffsetVector = jointVelocityOffsetVector, jointDControlVector = jointDControlVector,
                                          visualization=VObjectKinematicTree(graphicsDataList = graphicsDataList)))

        #return some needed variables for further use        
        d = {'nodeGeneric': nGeneric, 'objectKinematicTree': oKT, 'baseObject': baseObject,
             }
        return d

                      

    #%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    #**classFunction: Add items to existing mbs from the robot structure inside this robot class; robot is attached to baseMarker (can be ground object or moving/deformable body);
    #                 The (serial) robot is built as rigid bodies (containing rigid body nodes), where bodies represent the links which are connected by joints; 
    #                 Add optional jointSpringDamperUserFunctionList for individual control of joints; otherwise use PDcontrol in RobotLink structure; additional joint torques/forces can be added via spring damper, using mbs.SetObjectParameter(...) function;
    #                 See several Python examples, e.g., \texttt{serialRobotTestTSD.py}, in Examples or TestModels;
    #                 For more efficient models, use CreateKinematicTree(...) function!
    #**input: 
    #   mbs: the multibody system, which will be extended
    #   baseMarker: a rigid body marker, at which the robot will be placed (usually ground); note that the local coordinate system of the base must be in accordance with the DH-parameters, i.e., the z-axis must be the first rotation axis. For correction of the base coordinate system, use rotationMarkerBase
    #   jointSpringDamperUserFunctionList: a list of user functions for actuation of joints with more efficient spring-damper based connector (spring-damper directly emulates PD-controller); uses torsional spring damper for revolute joints and linear spring damper for prismatic joints; can be empty list (no spring dampers); if entry of list is 0, no user function is created, just pure spring damper; parameters are taken from RobotLink PDcontrol structure, which MUST be defined using SetPDcontrol(...) in RobotLink
    #   jointLoadUserFunctionList: DEPRECATED: a list of user functions for actuation of joints according to a LoadTorqueVector userFunction, see serialRobotTest.py as an example; can be empty list
    #   createJointTorqueLoads: DEPRECATED: if True, independently of jointLoadUserFunctionList, joint loads are created; the load numbers are stored in lists jointTorque0List/ jointTorque1List; the loads contain zero torques and need to be updated in every computation step, e.g., using a preStepUserFunction; unitTorque0List/ unitTorque1List contain the unit torque vector for the according body(link) which needs to be applied on both bodies attached to the joint
    #   rotationMarkerBase: add a numpy 3x3 matrix for rotation of the base, in order that the robot can be attached to any rotated base marker; the rotationMarkerBase is according to the definition in GenericJoint; note, that for moving base, the static compensation does not work (base rotation must be updated)
    #**output: the function returns a dictionary containing per link nodes and object (body) numbers, 'nodeList', 'bodyList', the object numbers for joints, 'jointList', list of load numbers for joint torques (jointTorque0List, jointTorque1List); unit torque vectors in local coordinates of the bodies to which the torques are applied (unitTorque0List, unitTorque1List); springDamperList contains the spring dampers if defined by PDcontrol of links
    def CreateRedundantCoordinateMBS(self, mbs, baseMarker, jointSpringDamperUserFunctionList= [], 
                                     jointLoadUserFunctionList=[], 
                                     createJointTorqueLoads=True, rotationMarkerBase=None): 
        #build robot model:
        nodeList = []           #node number or rigid node for link
        bodyList = []           #body number or rigid body for link
        jointList = []          #joint which links to previous link or base
        markerList0 = []        #contains n marker numbers per link (=body) which connect to previous/left link
        markerList1 = []        #contains n marker numbers per link (=body) which connect to next/right link
        jointTorque0List = []   #load number of joint torque at previous/left link (negative); 
        jointTorque1List = []   #load number of joint torque at next/right link (positive)
        unitTorque0List = []    #contains unit torque0 (previous/left link) for joint i, should be multiplied with according factor to represent joint torque
        unitTorque1List = []    #contains unit torque1 (next/right link) for joint i, should be multiplied with according factor to represent joint torque
        springDamperList = []   #contains torsional or linear spring dampers for control of joint axes
        
        HTlist = [] #list of homogeneous transformations for links stored (for parent link relation in tree)

        #delete: lastMarker = baseMarker
        lastMarkerRotation = np.identity(3) #base rotation included in marker
        if rotationMarkerBase != None:
            lastMarkerRotation = rotationMarkerBase
            
        qRef = self.referenceConfiguration
        
        baseObject = -1 #if it does not exist
        if self.base.visualization.graphicsData != []:
            #add a ground object at base position
            graphicsDataBase = []
            pOff = HT2translation(self.base.HT)
            Aoff = HT2rotationMatrix(self.base.HT)
            for data in self.base.visualization.graphicsData:
                graphicsDataBase += [MoveGraphicsData(data, [0,0,0], Aoff)] #only rotated, translation is in ground

            baseObject = mbs.AddObject(ObjectGround(referencePosition=pOff, 
                                                    visualization=VObjectGround(graphicsData=graphicsDataBase)))

        #create list of indices to next links
        nextLinks = [None]*self.NumberOfLinks()
        for i in range(self.NumberOfLinks()):
            nextLinks[i] = []
            link = self.links[i]
            if self.GetParentIndex(i) != -1:
               nextLinks[self.GetParentIndex(i)] += [i]
                
        #create robot nodes and bodies:
        for i in range(len(self.links)):
            link = self.links[i]
            if link.jointType not in dictJointType2Axis:
                raise ValueError('CreateRedundantCoordinateMBS: found invalid joint type in link '+str(i)+':'+link.jointType)
        
        
            # T01 = DH2HT(DHparam) #transformation from last link to this link; it defines the orientation of the body
            T01 = link.preHT @ dictJointType2HT[link.jointType](qRef[i]) @ link.localHT #new bodies are placed at origin of link frame
            if not self.HasParent(i):
                Tcurrent = self.GetBaseHT()
            else:
                Tcurrent = HTlist[self.GetParentIndex(i)]

            Tcurrent = Tcurrent @ T01
            HTlist += [copy(Tcurrent)]

            #++++++++++++++++++++++++++++++++++++++++++++++
        
            localHTinv = InverseHT(link.localHT)
            AthisT = HT2rotationMatrix(localHTinv) #transforms back to joint0
            pThis =  HT2translation(localHTinv) #AthisT @ np.array([-a,0,-d]) #needed for marker of next link
            
            #compute axis of previous link, for std DH, this is transformed back to previous joint
            jointAxis = dictJointType2Axis[link.jointType]
            axis0 = AthisT @ jointAxis #for drawing
            #++++++++++++++++++++++++++++++++++++++++++++++
            
            
            #rigid body parameters:
            com = link.COM #is defined within link==body frame
        
            inertiaLink = RigidBodyInertia(mass=link.mass, inertiaTensor=link.inertia)
            inertiaLink = inertiaLink.Translated(com) #needs to be recomputed, because inertia in Robot is w.r.t. COM, but ObjectRigidBody needs inertia for reference point
            
            #++++++++++++++++++++++++
            #compute axis1 related to next link (for std DH, this is local z-axis in link coordinates)
            graphicsList = []
            for nextLinkIndex in nextLinks[i]:
                # if i == len(self.links)-1:
                #     pNext = np.array([0,0,0.]) #use local position for final link
                #     axisNext=np.array([0,0,0]) #no axis to draw for last link
                nextLink = self.links[nextLinkIndex]
                pNext = HT2translation(nextLink.preHT) #this defines the position for the local of the axis for next link
                axisNext = HT2rotationMatrix(nextLink.preHT) @ dictJointType2Axis[nextLink.jointType] 

                if len(link.visualization.graphicsData) == 0:
                    graphicsList += self.GetLinkGraphicsData(i, pThis, pNext, axis0, axisNext, link.visualization)

            if len(link.visualization.graphicsData) != 0:
                graphicsList += link.visualization.graphicsData

            #add transformed graphicsData of tool to link graphics
            if (i==len(self.links)-1 and #tool
                self.tool.visualization.graphicsData != []):
                pOff = HT2translation(self.tool.HT)
                Aoff = HT2rotationMatrix(self.tool.HT)
                for data in self.tool.visualization.graphicsData:
                    graphicsList += [MoveGraphicsData(data, pOff, Aoff)] 

            
            #++++++++++++++++++++++++
            #now add body for link:
            [nLink,bLink]=AddRigidBody(mainSys = mbs, inertia=inertiaLink, 
                                nodeType='NodeType.RotationEulerParameters', 
                                position=HT2translation(Tcurrent), 
                                rotationMatrix = HT2rotationMatrix(Tcurrent),
                                gravity=self.gravity, 
                                graphicsDataList=graphicsList)
            nodeList+=[nLink]
            bodyList+=[bLink]
            #print(mbs.GetObject(bLink))
        
            #++++++++++++++++++++++++
            #add markers and joints
            mLink1 = mbs.AddMarker(MarkerBodyRigid(bodyNumber=bLink, localPosition=pThis))
            # markerPositionLink1next = [0,0,0] #not used in last link!
            # if i < len(self.links)-1:
            #     markerPositionLink1next = HT2translation(self.links[i+1].preHT) #this is defined in the next link!
            # mlink1next = mbs.AddMarker(MarkerBodyRigid(bodyNumber=bLink, 
            #                                            localPosition=markerPositionLink1next))

            if i == 0:
                lastMarkerRotation = HT2rotationMatrix(link.preHT)@lastMarkerRotation #is rotationMarkerBase
                mLink0LastBody = baseMarker
            else:
                lastMarkerRotation = HT2rotationMatrix(link.preHT)
                marker0Position = HT2translation(link.preHT) #this is defined in the parent link!
                parentBody = bodyList[self.GetParentIndex(i)]
                mLink0LastBody = mbs.AddMarker(MarkerBodyRigid(bodyNumber=parentBody, 
                                                               localPosition=marker0Position))

            markerList0+=[mLink0LastBody]
            markerList1+=[mLink1]

            constrainedAxes = [1,1,1,1,1,1] #all axes constrained
            constrainedAxes[dictJointType2coordinate6D[link.jointType]] = 0 #this is the free axis

            r = link.visualization.jointRadius
            wJ = link.visualization.jointWidth
            showMBSjoint = link.visualization.showMBSjoint

            loadSize = 1
            
            marker0 = mLink0LastBody
            marker1 = mLink1
            rotationMarker0 = lastMarkerRotation
            rotationMarker1 = AthisT

            jointLink = mbs.AddObject(GenericJoint(markerNumbers=[marker0, marker1],
                                                    constrainedAxes=constrainedAxes,
                                                    rotationMarker0=rotationMarker0,
                                                    rotationMarker1=rotationMarker1,
                                                    visualization=VObjectJointGeneric(show=showMBSjoint, axesRadius = r*0.25, 
                                                                  axesLength=wJ*1.1, color=color4grey)))

            jointList+=[jointLink]
            objectSD = None #if not added

            if link.jointType[0] == 'R':
                #load on previous body, negative sign
                torque0 = rotationMarker0 @ (-loadSize*jointAxis) #np.array([0,0, -loadSize])
                torque1 = rotationMarker1 @ (loadSize*jointAxis) #rotated negative torque vector for current link, it is not the z-axis
                unitTorque0List += [torque0]
                unitTorque1List += [torque1]
    
                if i < len(jointLoadUserFunctionList):
                    load0 = mbs.AddLoad(LoadTorqueVector(markerNumber=marker0, loadVector=torque0,
                                                                    bodyFixed=True, loadVectorUserFunction=jointLoadUserFunctionList[i]))
                    load1 = mbs.AddLoad(LoadTorqueVector(markerNumber=marker1, loadVector=torque1, 
                                                                    bodyFixed=True, loadVectorUserFunction=jointLoadUserFunctionList[i]))
                    jointTorque0List += [load0]
                    jointTorque1List += [load1]
                elif createJointTorqueLoads: #loads then must be updated in, e.g., mbs.SetPreStepUserFunction(...)
                    load0 = mbs.AddLoad(LoadTorqueVector(markerNumber=marker0, loadVector=[0,0,0], 
                                                                    bodyFixed=True))
                    load1 = mbs.AddLoad(LoadTorqueVector(markerNumber=marker1, loadVector=[0,0,0],
                                                                    bodyFixed=True))
                    jointTorque0List += [load0]
                    jointTorque1List += [load1]
                else:
                    jointTorque0List += [None]
                    jointTorque1List += [None]
                
                if i < len(jointSpringDamperUserFunctionList) or link.HasPDcontrol():
                    PDcontrol = link.GetPDcontrol()
                    #generic node for infinite revolutions:
                    nGeneric=mbs.AddNode(NodeGenericData(initialCoordinates=[0], 
                                                         numberOfDataCoordinates=1)) #for infinite rotations
                    #torsional spring-damper allows control of rotation
                    objectSD = mbs.AddObject(TorsionalSpringDamper(markerNumbers=[marker0, marker1],
                                                        nodeNumber=nGeneric,
                                                        rotationMarker0=rotationMarker0,
                                                        rotationMarker1=rotationMarker1,                                            
                                                        stiffness=PDcontrol[0],
                                                        damping=PDcontrol[1],
                                                        visualization=VTorsionalSpringDamper(show=False)
                                                        ))
            else:
                if (createJointTorqueLoads):
                    raise ValueError('CreateRedundantCoordinateMBS: createJointTorqueLoads only valid for Revolute joint')

                if i < len(jointSpringDamperUserFunctionList) or link.HasPDcontrol():
                    PDcontrol = link.GetPDcontrol()
                    #linear spring-damper allows control in translational direction
                    objectSD = mbs.AddObject(LinearSpringDamper(markerNumbers=[marker0, marker1],
                                                                axisMarker0 = jointAxis,
                                                                stiffness=PDcontrol[0],
                                                                damping=PDcontrol[1],
                                                                visualization=VLinearSpringDamper(show=False)
                                                                ))
                    
            springDamperList += [objectSD]
            
            #markerList0 += [mlink1next]
            # if i < len(self.links)-1: #not suitable for kinematicTree
            #     lastMarkerRotation = HT2rotationMatrix(self.links[i+1].preHT) #needed for modified DH parameters
            
            #end loop over links
            #++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        #return some needed variables for further use        
        d = {'nodeList': nodeList,'bodyList': bodyList,'jointList': jointList,
             'markerList0': markerList0,
             'markerList1': markerList1,
             'springDamperList': springDamperList,
             'jointTorque0List': jointTorque0List,
             'jointTorque1List': jointTorque1List,
             'unitTorque0List': unitTorque0List,
             'unitTorque1List': unitTorque1List,
             'baseObject':baseObject}
        return d
    
        
    

    #%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    #**classFunction: export kinematicTree
    def GetKinematicTree66(self):
        from exudyn.kinematicTree import KinematicTree66, Inertia2T66
        
        jointTypes = []
        transformations = []
        inertias = []
        n = len(self.links)
        for i in range(n):
            link = self.links[i]
            jointTypes += [link.jointType]
            
            preHT = link.preHT
            Amat = HT2rotationMatrix(preHT) 
            vVec = HT2translation(preHT)
            X=RotationTranslation2T66Inverse(A=Amat, v=vVec)
            if np.linalg.norm(link.localHT - HT0()) > 1e-15:
                raise ValueError('GetKinematicTree66(): not implemented for links with localHT != HT0()')
            
            transformations += [X] #defines transformation to joint in parent link
            J = RigidBodyInertia(mass=link.mass, inertiaTensor=link.inertia) #link.inertia around COM
            J = J.Translated(link.COM)
            inertias += [Inertia2T66(J)]
        
        KT=KinematicTree66(listOfJointTypes=jointTypes, 
                           listOfTransformations=transformations, 
                           listOfInertias=inertias, 
                           gravity=self.gravity)
        return KT
    
    #**classFunction: create link GraphicsData (list) for link i; internally used in CreateRedundantCoordinateMBS(...); linkVisualization contains visualization dict of link
    def GetLinkGraphicsData(self, i, p0, p1, axis0, axis1, linkVisualization):
        #bodyAlpha = 1 #default value; no transparency
        
        com = self.links[i].COM
        graphicsList = []

        r = linkVisualization.jointRadius
        wJ = linkVisualization.jointWidth
        wL = linkVisualization.linkWidth
        color = linkVisualization.linkColor
        showCOM = linkVisualization.showCOM

        #draw COM:
        if linkVisualization.showCOM:
            dd = r*0.2
            colorCOM = copy(color)
            colorCOM[0] *= 0.8 #make COM a little darker
            colorCOM[1] *= 0.8
            colorCOM[2] *= 0.8
            graphicsList += [GraphicsDataOrthoCubePoint(com, [dd,dd,dd], colorCOM)]

        #draw link:
        if r != 0:
            h0 = 0.5*wJ   #height of half axis, first joint
            h1 = 0.5*wJ   #height of half axis, second joint
            
            if i == 0: #draw full cylinder for first joint
                h0 = wJ*2
            
            graphicsList += [GraphicsDataCylinder(pAxis=p0, vAxis=-h0*axis0, 
                                                  radius=r, color=color)]
            
            graphicsList += [GraphicsDataCylinder(pAxis=p1, vAxis= h1*axis1, 
                                                      radius=r, color=color)]

            #draw body as cylinder:
            if NormL2(VSub(p1,p0)) > 1e-15:
                graphicsList += [GraphicsDataCylinder(pAxis=p1, vAxis=VSub(p0,p1), 
                                                      radius=0.5*wL, color=color)]
        
        return graphicsList

    
    #**classFunction: build robot structre from dictionary; this is a DEPRECATED function, which is used in older models; DO NOT USE
    def BuildFromDictionary(self, robotDict):
        global buildFromDictionaryWarned
        if not buildFromDictionaryWarned:
            buildFromDictionaryWarned = True
            print("WARNING: function BuildFromDictionary in class Robot is DEPRECATED; DO NOT USE")

        if 'base' in robotDict:
            self.base.HT = np.array(robotDict['base']['HT'])
        if 'tool' in robotDict:
            self.tool.HT = np.array(robotDict['tool']['HT'])
        if 'gravity' in robotDict:
            self.gravity = np.array(robotDict['gravity'])
        if 'referenceConfiguration' in robotDict:
            self.referenceConfiguration = np.array(robotDict['referenceConfiguration'])
        
        warnedModDH = False
        for i in range(len(robotDict['links'])):
            link = robotDict['links'][i]
            # qRef = 0
            # if len(robotDict['referenceConfiguration']) > i:
            #     qRef = robotDict['referenceConfiguration'][i]
            
            if 'stdDH' in link:
                localHT = StdDH2HT(link['stdDH']) #using theta=0
            #modified DH parameters are re-interpreted as standard DH parameters, which does not always work / needs additional adjustments!
            elif 'modDHcraig' in link: #according to Craig (1986) and modified DH parameters in Corke's toolbox (2017)
                if not warnedModDH:
                    warnedModDH = True
                    print('WARNING: BuildFromDictionary untested for modDH; missing transformation for inertia and COM?')
                [theta, d, a, alpha] = link['modDHcraig']
                a1 = 0 #if no succeeding link follows
                alpha1 = 0
                if i < len(robotDict['links'])-1: #there exists a next link, which we use as additional standard DH-parameters for this link
                    [theta1, d1, a1, alpha1] = link['modDHcraig']
                if i == 0: #put first two DH parameters to base
                    base.HT = base.HT @ HTrotateX(alpha) @ HTtranslate([a,0,0]) 
                
                #local HT re-interpreted as standard DH-parameters, as suggested by Corke 2017, page 219:
                localHT = HTrotateZ(theta) @ HTtranslate([0,0,d]) @ HTtranslate([a1,0,0]) @ HTrotateX(alpha1)
            else:
                raise ValueError('BuildFromDictionary in class Robot: only supports links with stdDH parameters')

            robotLink = RobotLink(mass=link['mass'], COM=link['COM'], inertia=link['inertia'], localHT=localHT)
            self.AddLink(robotLink)

        

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++  DH-PARAMETERS  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#**function: compute homogeneous transformation matrix HT from standard DHparameters=[theta, d, a, alpha]
def StdDH2HT(DHparameters):
#    [theta, d, a, alpha] = DHparameters
#    return HTrotateZ(theta) @ HTtranslate([0,0,d]) @ HTtranslate([a,0,0]) @ HTrotateX(alpha)
    #optimized version:
    [theta, d, a, alpha] = DHparameters
    ct = np.cos(theta)
    st = np.sin(theta)
    ca = np.cos(alpha)
    sa = np.sin(alpha)
    return np.array([[ct,-st*ca, st*sa, a*ct],
                     [st, ct*ca,-ct*sa, a*st],
                     [ 0, sa   , ca   , d   ],
                     [ 0, 0    , 0    , 1   ]])

#\mfour{\cos \theta_j &-\sin \theta_j \cos \alpha_j & \sin \theta_j \sin \alpha_j & a_j \cos \theta_j}
#														{\sin \theta_j & \cos \theta_j \cos \alpha_j &-\cos \theta_j \sin \alpha_j & a_j \sin \theta_j}
#														{0             & \sin \alpha_j               & \cos \alpha_j               & d_j }
#														{0 & 0 & 0 & 1}
#Test (compared with Robotcs, Vision and Control book of P. Corke:
#print("std. DH =\n", DH2HT([0.5, 0.1, 0.2, np.pi/2]).round(4))

#**function: compute pre- and post- homogeneous transformation matrices from modified Denavit-Hartenberg DHparameters=[alpha, d, theta, r]; returns [HTpre, HTpost]; HTpre is transformation before axis rotation, HTpost includes axis rotation and everything hereafter; modified DH-Parameters according to Khalil and Kleinfinger, 1986
def ModDHKK2HT(DHparameters):
    [alpha, d, theta, r] = DHparameters
    return [HTrotateX(alpha) @ HTtranslate([d,0,0]) , HTrotateZ(theta) @ HTtranslate([0,0,r]) ] 




#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++  MOTION PLANNING and TRAJECTORIES  +++++++++++++++++++++++++++++++++++++++++++++
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: Compute parameters for optimal trajectory using given duration and distance
#**notes: DEPRECATED, DO NOT USE - moved to robotics.motion
#**input: duration in seconds and distance in meters or radians
#**output: returns [vMax, accMax] with maximum velocity and maximum acceleration to achieve given trajectory
def ConstantAccelerationParameters(duration, distance):
    accMax = 4*distance/duration**2
    vMax = (accMax * distance)**0.5
    return [vMax, accMax]

#**function: Compute angle / displacement s, velocity v and acceleration a
#**input: 
#  t: current time to compute values
#  tStart: start time of profile
#  sStart: start offset of path
#  duration: duration of profile
#  distance: total distance (of path) of profile
#**notes: DEPRECATED, DO NOT USE - moved to robotics.motion
#**output: [s, v, a] with path s, velocity v and acceleration a for constant acceleration profile; before tStart, solution is [0,0,0] while after duration, solution is [sStart+distance, 0, 0]
def ConstantAccelerationProfile(t, tStart, sStart, duration, distance):
    [vMax, accMax] = ConstantAccelerationParameters(duration, distance)
    
    s = sStart
    v = 0
    a = 0
    
    x = t-tStart
    if x < 0:
        s=0
    elif x < 0.5*duration:
        s = sStart + 0.5*accMax*x**2
        v = x*accMax
        a = accMax
    elif x < duration:
        s = sStart + distance - 0.5*accMax * (duration-x)**2
        v = (duration - x)*accMax
        a = -accMax
    else:
        s = sStart + distance
    
    return [s, v, a]

motionInterpolatorWarned = False
#**function: Compute joint value, velocity and acceleration for given robotTrajectory['PTP'] of point-to-point type, evaluated for current time t and joint number
#**input:
#  t: time to evaluate trajectory
#  robotTrajectory: dictionary to describe trajectory; in PTP case, either use 'time' points, or 'time' and 'duration', or 'time' and 'maxVelocity' and 'maxAccelerations' in all consecutive points; 'maxVelocities' and 'maxAccelerations' must be positive nonzero values that limit velocities and accelerations; 
#  joint: joint number for which the trajectory shall be evaluated
#**output: for current time t it returns [s, v, a] with path s, velocity v and acceleration a for current acceleration profile; outside of profile, it returns [0,0,0] !
#**notes: DEPRECATED, DO NOT USE - moved to robotics.motion
#**example:
# q0 = [0,0,0,0,0,0] #initial configuration
# q1 = [8,5,2,0,2,1] #other configuration
# PTP =[]
# PTP+=[{'q':q0, 
#        'time':0}]
# PTP+=[{'q':q1,
#        'time':0.5}]
# PTP+=[{'q':q1, 
#        'time':1e6}] #forever
# RT={'PTP':PTP}
# [u,v,a] = MotionInterpolator(t=0.5, robotTrajectory=RT, joint=1)
def MotionInterpolator(t, robotTrajectory, joint):
    global motionInterpolatorWarned
    if not motionInterpolatorWarned:
        motionInterpolatorWarned = True
        print('MotionInterpolator: deprecated - use Trajectory class from robotics.trajectory instead')
    n = len(robotTrajectory['PTP'])
    if n < 2:
        print("ERROR in MotionInterpolator: trajectory must have at least 2 points!")
    
    i = 0
    while (i < n) and (t >= robotTrajectory['PTP'][i]['time']):
        i += 1

    if (i==0) or (i==n):
        return [0,0,0] #outside of trajectory
    
    #i must be > 0 and < n now!
    q0 = robotTrajectory['PTP'][i-1] #must always exist
    q1 = robotTrajectory['PTP'][i] #must always exist
    
    return ConstantAccelerationProfile(t, q0['time'], q0['q'][joint], 
                                       q1['time'] - q0['time'], 
                                       q1['q'][joint] - q0['q'][joint])







serialRobot2MBSwarned=False
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++  create a SERIAL ROBOT from DH-parameters in the mbs +++++++++++++++++++++++++++
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#**function: DEPRECATED function, use Robot.CreateRedundantCoordinateMBS(...); add items to existing mbs from the robot structure, a baseMarker (can be ground object or body)
#            and the user function list for the joints; there are options that can be passed as args / kwargs, which can contains options as described below. For details, see the python file and \texttt{serialRobotTest.py} in TestModels
#**input: 
#   mbs: the multibody system, which will be extended
#   robot: the robot model as dictionary, described in function ComputeJointHT
#   jointLoadUserFunctionList: a list of user functions for actuation of joints according to a LoadTorqueVector userFunction, see serialRobotTest.py as an example; can be empty list
#   baseMarker: a rigid body marker, at which the robot will be placed (usually ground); note that the local coordinate system of the base must be in accordance with the DH-parameters, i.e., the z-axis must be the first rotation axis. For correction of the base coordinate system, use rotationMarkerBase
#   rotationMarkerBase: used in Generic joint between first joint and base; note, that for moving base, the static compensation does not work (base rotation must be updated)
#   showCOM: a scalar d, which if nonzero it causes to draw the center of mass (COM) as rectangular block with size [d,d,d]
#   bodyAlpha: a float value in range [0..1], adds transparency to links if value < 1
#   toolGraphicsSize: list of 3 floats [sx,sy,sz], giving the size of the tool for graphics representation; set sx=0 to disable tool drawing or do not provide this optional variable
#   drawLinkSize: draw parameters for links as list of 3 floats [r,w,0], r=radius of joint, w=radius of link, set r=0 to disable link drawing
#   rotationMarkerBase: add a numpy 3x3 matrix for rotation of the base, in order that the robot can be attached to any rotated base marker; the rotationMarkerBase is according to the definition in GenericJoint
#**output: the function returns a dictionary containing information on nodes, bodies, joints, markers, torques, for every joint
def SerialRobot2MBS(mbs, robot, jointLoadUserFunctionList, baseMarker, *args, **kwargs):
    global serialRobot2MBSwarned
    if not serialRobot2MBSwarned:
        serialRobot2MBSwarned = True
        print('function SerialRobot2MBS(...) is deprecated, use Robot.CreateRedundantCoordinateMBS(...) of class Robot instead')
    #build robot model:
    nodeList = []           #node number or rigid node for link
    bodyList = []           #body number or rigid body for link
    jointList = []          #joint which links to previous link or base
    markerList0 = [] #contains n marker numbers per link which connect to previous body
    markerList1 = [] #contains n marker numbers per link which connect to next body
    jointTorque0List = []  #load number of joint torque at previous link (negative)
    jointTorque1List = []  #load number of joint torque at next link (positive)
    
    Tcurrent = robot['base']['HT']
    
    lastMarker = baseMarker
    
    bodyAlpha = 1 #default value; no transparency
    if 'bodyAlpha' in kwargs:
        bodyAlpha = kwargs['bodyAlpha']


    toolSize = [0.05,0.02,0.06] #default values
    if 'toolGraphicsSize' in kwargs:
        toolSize = kwargs['toolGraphicsSize']

        
    drawLinkSize=[0.06,0.05] #default values
    if 'drawLinkSize' in kwargs:
        drawLinkSize = kwargs['drawLinkSize']

    #create robot nodes and bodies:
    for i in range(len(robot['links'])):
        link = robot['links'][i]
    
        DHparam = np.zeros(4)
        DHparam[0:4] = link['stdDH'][0:4] #copy content!
        if robot['jointType'][i] == 1: #1==revolute, 0==prismatic
            DHparam[0] = robot['referenceConfiguration'][i] #add reference angle
        else:
            DHparam[1] = robot['referenceConfiguration'][i] #add reference displacement
            
        T01 = StdDH2HT(DHparam) #transformation from last link to this link; it defines the orientation of the body
    
        Tcurrent = Tcurrent @ T01
        
        #the next (distal) joint (joint1) is aligned with the z-axis of the body's frame:
        p1 = np.array([0,0,0.])
        axis1 = np.array([0,0,1.])
    
        #the previous joint (joint0) axis is rotated back with alpha and translated along -x with a
        d = link['stdDH'][1]
        a = link['stdDH'][2]
        alpha = link['stdDH'][3]
        A0T = RotationMatrixX(-alpha) #rotation matrix transforms back to joint0
        p0 = A0T @ np.array([-a,0,-d])
        axis0 = A0T @ np.array([0,0,1.])
        
        #rigid body parameters:
        com = link['COM']
        com4 = np.array(com+[1])
    
        inertiaLink = RigidBodyInertia(mass=link['mass'], 
                                       inertiaTensor=link['inertia'])
        inertiaLink = inertiaLink.Translated(com)#needs to be recomputed, because inertia is w.r.t. COM
        
        color = list(np.array(color4list[i]))
        color[3] = bodyAlpha #transparency of bodies
        graphicsList = []

        #draw COM:
        if 'showCOM' in args:
            dd=args['showCOM']
            graphicsList += [GraphicsDataOrthoCubePoint(com, [dd,dd,dd], color4list[i])]

        #draw links:
        r = drawLinkSize[0]
        w = drawLinkSize[1]
        if r != 0:
            h0 = w   #height of half axis, first joint
            h1 = w   #height of half axis, second joint
            
            if i == 0: #draw full cylinder for first joint
                h0 = w*2
            
            graphicsList += [GraphicsDataCylinder(pAxis=p0, vAxis=-h0*axis0, 
                                                 radius=r, color=color)]
            graphicsList += [GraphicsDataCylinder(pAxis=p1, vAxis= h1*axis1, 
                                                      radius=r, color=color)]

            #draw body as cylinder:
            if NormL2(VSub(p1,p0)) > 1e-15:
                graphicsList += [GraphicsDataCylinder(pAxis=p1, vAxis=VSub(p0,p1), 
                                                      radius=w, color=color)]
        
        if i==len(robot['links']): #tool
            pTool = HT2translation(robot['tool']['HT'])
            if toolSize[0] != 0:
                colorTool = color4steelblue
                colorTool[3] = bodyAlpha #transparency of bodies
                    
                if NormL2(pTool) != 0:
                    graphicsList += [GraphicsDataCylinder(pAxis=p1, vAxis= VSub(pTool,p1), 
                                                          radius=r*0.75, color=colorTool)]
                graphicsList += [GraphicsDataOrthoCubePoint(pTool+[0,ty,0.5*tz], toolSize, colorTool)]
                graphicsList += [GraphicsDataOrthoCubePoint(pTool+[0,-ty,0.5*tz], toolSize, colorTool)]
                
        
        #++++++++++++++++++++++++
        #now add body for link:
        [nLink,bLink]=AddRigidBody(mainSys = mbs, inertia=inertiaLink, 
                            nodeType='NodeType.RotationEulerParameters', 
                            position=HT2translation(Tcurrent), 
                            rotationMatrix = HT2rotationMatrix(Tcurrent),
                            gravity=robot['gravity'], 
                            graphicsDataList=graphicsList)
        nodeList+=[nLink]
        bodyList+=[bLink]
        #print(mbs.GetObject(bLink))
    
        #++++++++++++++++++++++++
        #add markers and joints
        mLink0 = mbs.AddMarker(MarkerBodyRigid(bodyNumber=bLink, localPosition=p0))
        mLink1 = mbs.AddMarker(MarkerBodyRigid(bodyNumber=bLink, localPosition=[0,0,0]))
        markerList0+=[mLink0]
        markerList1+=[mLink1]
        
        rotation1 = np.identity(3) #only used for base rotation
        if i == 0: #only for base we can add a transformation
            if 'rotationMarkerBase' in kwargs:
                rotation1 = kwargs['rotationMarkerBase']
            
        #this configuration is less optimal for larger joint values:
    #    jointLink = mbs.AddObject(GenericJoint(markerNumbers=[lastMarker, mLink0],
    #                                           constrainedAxes=[1,1,1,1,1,0],
    #                                           rotationMarker1=A0T,
    #                                           visualization=VObjectJointGeneric(axesRadius = 0.01,axesLength=0.1, color=color4red)))
        jointLink = mbs.AddObject(GenericJoint(markerNumbers=[mLink0, lastMarker],
                                               constrainedAxes=[1,1,1,1,1,0],
                                               rotationMarker0=A0T,
                                               rotationMarker1=rotation1,
                                               visualization=VObjectJointGeneric(axesRadius = 0.01,axesLength=0.1, color=color4red)))
                
        #load on previous body, negative sign
        loadSize = 1
        torque1 = A0T @ np.array([0,0, loadSize]) #rotated torque vector for current link, it is not the z-axis
        #print("torque1=", torque1)
        if i < len(jointLoadUserFunctionList):
            load0 = mbs.AddLoad(LoadTorqueVector(markerNumber=lastMarker, loadVector=[0,0,-loadSize], 
                                                            bodyFixed=True, loadVectorUserFunction=jointLoadUserFunctionList[i]))
            load1 = mbs.AddLoad(LoadTorqueVector(markerNumber=mLink0, loadVector=torque1, 
                                                            bodyFixed=True, loadVectorUserFunction=jointLoadUserFunctionList[i]))
    
            jointTorque0List += [load0]
            jointTorque1List += [load1]
    
        jointList+=[jointLink]

        lastMarker = mLink1
        #end loop over links
        #++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    d = {'nodeList': nodeList,'bodyList': bodyList,'jointList': jointList,
         'markerList0': markerList0,'markerList1': markerList1,
         'jointTorque0List': jointTorque0List,'jointTorque1List': jointTorque1List}
    return d




#%%+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#DEPRECATED functionality:
dh2HTwarned = False
def DH2HT(DHparameters):
    global dh2HTwarned 
    if not dh2HTwarned:
        dh2HTwarned = True
        print('function DH2HT(...) is deprecated, use StdDH2HT instead')
    return StdDH2HT(DHparameters)

computeJointHTwarned = False
#compute HT for every joint, using given configuration
#**function: DEPRECATED: compute list of  homogeneous transformations HT from base to every joint (more precisely of every link!) for given configuration
#**example:
#link0={'stdDH':[0,0,0,np.pi/2], 
#         'mass':20,  #not needed!
#         'inertia':np.diag([1e-8,0.35,1e-8]), #w.r.t. COM!
#         'COM':[0,0,0]}
#link1={'stdDH':[0,0,0.4318,0],
#         'mass':17.4, 
#         'inertia':np.diag([0.13,0.524,0.539]), #w.r.t. COM!
#         'COM':[-0.3638, 0.006, 0.2275]}
#robot={'links':[link0, link1],
#         'jointType':[1,1], #1=revolute, 0=prismatic
#         'base':{'HT':HT0()},
#         'tool':{'HT':HTtranslate([0,0,0.1])},
#         'gravity':[0,0,9.81],
#         'referenceConfiguration':[0]*2 #reference configuration for bodies; at which the robot is built
#         } 
#HTlist = ComputeJointHT(robot, [np.pi/8]*2)
def ComputeJointHT(robot, configuration):
    global computeJointHTwarned
    if not computeJointHTwarned:
        computeJointHTwarned = True
        print('function ComputeJointHT(robot, configuration) is deprecated, use Robot.JointHT(...) of class Robot instead')

    Tcurrent = robot['base']['HT']
    HT = []
    
    for i in range(len(robot['links'])):
        link = robot['links'][i]
        DHparam = np.zeros(4)
        DHparam[0:4] = link['stdDH'][0:4] #copys content!
        if robot['jointType'][i] == 1: #1==revolute, 0==prismatic
            DHparam[0] = configuration[i] #add current angle
        else:
            DHparam[1] = configuration[i] #add current displacement
            
        T01 = DH2HT(DHparam) #transformation from last link to this link; it defines the orientation of the body
        Tcurrent = Tcurrent @ T01
        HT += [Tcurrent]
    
    return HT


computeCOMHTwarned = False
#compute HT for every link's COM; takes current jointHT as input
#**function: DEPRECATED: compute list of  homogeneous transformations HT from base to every COM using HT list from ComputeJointHT
def ComputeCOMHT(robot, HT):
    global computeCOMHTwarned
    if not computeCOMHTwarned:
        computeCOMHTwarned = True
        print('function ComputeCOMHT(robot, HT) is deprecated, use Robot.COMHT(...) of class Robot instead')
    HTCOM = []
    
    #HTCOM += [robot['base']['HT']]
    for i in range(len(robot['links'])):
        link = robot['links'][i]
        HTCOM += [HT[i] @ HTtranslate(link['COM'])]
    
    return HTCOM

computeStaticTorqueswarned=False
#compute static torques for robot defined by DH-parameters and for given HT
#**function: DEPRECATED: compute list joint torques for serial robot under gravity (gravity and mass as given in robot)
def ComputeStaticTorques(robot,HT):
    global computeStaticTorqueswarned
    if not computeStaticTorqueswarned:
        computeStaticTorqueswarned = True
        print('function ComputeStaticTorques(robot, HT) is deprecated, use Robot.StaticTorques(...) of class Robot instead')
    jointTorques = np.zeros(np.size(robot['links']))
    #old, limited to 6 joints: jointTorques = np.zeros(6)

    #compute HTs for COM
    HTcom=ComputeCOMHT(robot, HT)
    grav = np.array(robot['gravity'])
    
    #sum up the torques of all gravity loads:
    for i in range(len(HTcom)):
        p = HT2translation(HTcom[i])
        Jcom=Jacobian(robot,HT[0:i+1],toolPosition=p,mode='trans')
        #print(HT2translation(HTcom[i]))
        fG = robot['links'][i]['mass'] * grav
        #print(fG)
        tau = Jcom.T @ fG
        jointTorques[0:i+1] += tau
    return jointTorques


computeJacobianwarned=False
#compute jacobian, needs per-link HT in current configuration
#runs over number of HTs given in HT (may be less than number of links)
#modes are: 'all', 'trans'...only translation part, 'rot': only rotation part
#**function: DEPRECATED: compute jacobian for translation and rotation at toolPosition using joint HT
def Jacobian(robot,HT,toolPosition=[],mode='all'):
    global computeJacobianwarned
    if not computeJacobianwarned:
        computeJacobianwarned = True
        print('function Jacobian(robot, HT,...) is deprecated, use Robot.Jacobian(...) of class Robot instead')
    n = len(HT)
    if n > len(robot['links']):
        print("ERROR: number of homogeneous transformations (HT) greater than number of links")

    Jomega = np.zeros((3,n))#rotation part of jacobian
    Jvel = np.zeros((3,n))  #translation part of jacobian
    A = HT2rotationMatrix(robot['base']['HT'])
    rotAxis = np.array([0,0,1]) #robot axis in local coordinates
    vPrevious = HT2translation(robot['base']['HT'])
    vn = toolPosition
    if len(vn) == 0:
        vn = HT2translation(HT[-1]) #tool position, for jacobian (could include tool itself)
    
    #create robot nodes and bodies:
    for i in range(n):
        
        if i > 0:
            A = HT2rotationMatrix(HT[i-1]) #rotation of joint i
        axis = A @ rotAxis #axis in global coordinates
        Jomega[0:3,i] = robot['jointType'][i] * axis #only considered, if revolute joint
        
        if i > 0:
            vPrevious = HT2translation(HT[i-1])
         
        #revolute joint:
        if robot['jointType'][i] == 1: #revolute joint
            Jvel[0:3,i]   = Skew(axis) @ (vn - vPrevious) #only considered, if revolute joint
        else: #prismatic joint
            Jvel[0:3,i]   = axis #NOT TESTED!!!
    
    if mode == 'all':
        J = np.zeros((6,n))
    else:
        J = np.zeros((3,n))
    
    if mode == 'rot':
        J[0:3,0:n] = Jomega
    elif mode == 'trans':
        J[0:3,0:n] = Jvel
    elif mode == 'all':
        J[0:3,0:n] = Jvel
        J[3:6,0:n] = Jomega

    return J



#the following functions use a structure for the description of the robot according to:
# link0={'stdDH':[0,0,0,np.pi/2], #theta, d, a, alpha
       # 'mass':20,  #not needed!
       # 'inertia':np.diag([1e-8,0.35,1e-8]), #w.r.t. COM!
       # 'COM':[0,0,0]}

# link1={'stdDH':[0,0,0.4318,0],
       # 'mass':17.4, 
       # 'inertia':np.diag([0.13,0.524,0.539]), #w.r.t. COM!
       # 'COM':[-0.3638, 0.006, 0.2275]}

# link2={'stdDH':[0,0.15,0.0203,-np.pi/2], 
       # 'mass':4.8, 
       # 'inertia':np.diag([0.066,0.086,0.0125]), #w.r.t. COM!
       # 'COM':[-0.0203,-0.0141,0.07]}

# link3={'stdDH':[0,0.4318,0,np.pi/2], 
       # 'mass':0.82, 
       # 'inertia':np.diag([0.0018,0.0013,0.0018]), #w.r.t. COM!
       # 'COM':[0,0.019,0]}

# link4={'stdDH':[0,0,0,-np.pi/2], 
       # 'mass':0.34, 
       # 'inertia':np.diag([0.0003,0.0004,0.0003]), #w.r.t. COM!
       # 'COM':[0,0,0]}

# link5={'stdDH':[0,0,0,0], 
       # 'mass':0.09, 
       # 'inertia':np.diag([0.00015,0.00015,4e-5]), #w.r.t. COM!
       # 'COM':[0,0,0.032]}

# #this is the global robot structure
# robot={'links':[link0, link1, link2, link3, link4, link5],
       # 'jointType':[1,1,1,1,1,1], #1=revolute, 0=prismatic
       # 'base':{'HT':HT0()},
       # 'tool':{'HT':HTtranslate([0,0,0.1])},
       # 'gravity':[0,0,9.81],
       # 'referenceConfiguration':[0]*6 #reference configuration for bodies; at which the robot is built
       # } 

