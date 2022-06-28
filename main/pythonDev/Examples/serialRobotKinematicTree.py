#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN example
#
# Details:  Example of a serial robot with minimum and redundant coordinates
#
# Author:   Johannes Gerstmayr
# Date:     2022-06-26
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


import exudyn as exu
from exudyn.itemInterface import *
from exudyn.utilities import *
from exudyn.rigidBodyUtilities import *
from exudyn.graphicsDataUtilities import *
from exudyn.robotics import *
from exudyn.robotics.motion import Trajectory, ProfileConstantAcceleration, ProfilePTP

import numpy as np
from numpy import linalg as LA
from math import pi

SC = exu.SystemContainer()
mbs = SC.AddSystem()

sensorWriteToFile = True

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
compensateStaticTorques = False #static torque compensation converges slowly!
useKinematicTree = True
#kinematic tree and redundant mbs agrees for stdDH version up to 1e-10, with compensateStaticTorques = False
# KT:      rotations at tEnd= 1.8464514676503092 , [0.4921990591981066, 0.2718999073958087, 0.818158053005264, -0.0030588904101585936, 0.26831938569719394, -0.0010660472359057434] 
# red. MBS:rotations at tEnd= 1.8464514674961 ,   [ 0.49219906  0.27189991  0.81815805 -0.00305889  0.26831939 -0.00106605]

mode='newDH'
# mode='newModDH' #modified DH parameters #position agrees for all digits: 1.846440411790352


graphicsBaseList = [GraphicsDataOrthoCubePoint([0,0,-0.15], [0.12,0.12,0.1], color4grey)]
graphicsBaseList +=[GraphicsDataCylinder([0,0,0], [0.5,0,0], 0.0025, color4red)]
graphicsBaseList +=[GraphicsDataCylinder([0,0,0], [0,0.5,0], 0.0025, color4green)]
graphicsBaseList +=[GraphicsDataCylinder([0,0,0], [0,0,0.5], 0.0025, color4blue)]
#robot.base.visualization['graphicsData']=graphicsBaseList

ty = 0.03
tz = 0.04
zOff = -0.05
toolSize= [0.05,0.5*ty,0.06]
graphicsToolList = [GraphicsDataCylinder(pAxis=[0,0,zOff], vAxis= [0,0,tz], radius=ty*1.5, color=color4red)]
graphicsToolList+= [GraphicsDataOrthoCubePoint([0,ty,1.5*tz+zOff], toolSize, color4grey)]
graphicsToolList+= [GraphicsDataOrthoCubePoint([0,-ty,1.5*tz+zOff], toolSize, color4grey)]

#control parameters, per joint:
fc=1
Pcontrol = np.array([40000, 40000, 40000, 100, 100, 10])
Dcontrol = np.array([400,   400,   100,   1,   1,   0.1])
Pcontrol = fc*Pcontrol
Dcontrol = fc*Dcontrol


#changed to new robot structure July 2021:
robot = Robot(gravity=[0,0,9.81],
              base = RobotBase(visualization=VRobotBase(graphicsData=graphicsBaseList)),
              tool = RobotTool(HT=HTtranslate([0,0,0.1]), visualization=VRobotTool(graphicsData=graphicsToolList)),
             referenceConfiguration = []) #referenceConfiguration created with 0s automatically

#modKKDH according to Khalil and Kleinfinger, 1986
link0={'stdDH':[0,0,0,pi/2], 
       'modKKDH':[0,0,0,0], 
        'mass':20,  #not needed!
        'inertia':np.diag([1e-8,0.35,1e-8]), #w.r.t. COM! in stdDH link frame
        'COM':[0,0,0]} #in stdDH link frame

link1={'stdDH':[0,0,0.4318,0],
       'modKKDH':[0.5*pi,0,0,0], 
        'mass':17.4, 
        'inertia':np.diag([0.13,0.524,0.539]), #w.r.t. COM! in stdDH link frame
        'COM':[-0.3638, 0.006, 0.2275]} #in stdDH link frame

link2={'stdDH':[0,0.15,0.0203,-pi/2], 
       'modKKDH':[0,0.4318,0,0.15], 
        'mass':4.8, 
        'inertia':np.diag([0.066,0.086,0.0125]), #w.r.t. COM! in stdDH link frame
        'COM':[-0.0203,-0.0141,0.07]} #in stdDH link frame

link3={'stdDH':[0,0.4318,0,pi/2], 
       'modKKDH':[-0.5*pi,0.0203,0,0.4318], 
        'mass':0.82, 
        'inertia':np.diag([0.0018,0.0013,0.0018]), #w.r.t. COM! in stdDH link frame
        'COM':[0,0.019,0]} #in stdDH link frame

link4={'stdDH':[0,0,0,-pi/2], 
       'modKKDH':[0.5*pi,0,0,0], 
        'mass':0.34, 
        'inertia':np.diag([0.0003,0.0004,0.0003]), #w.r.t. COM! in stdDH link frame
        'COM':[0,0,0]} #in stdDH link frame

link5={'stdDH':[0,0,0,0], 
       'modKKDH':[-0.5*pi,0,0,0], 
        'mass':0.09, 
        'inertia':np.diag([0.00015,0.00015,4e-5]), #w.r.t. COM! in stdDH link frame
        'COM':[0,0,0.032]} #in stdDH link frame
linkList=[link0, link1, link2, link3, link4, link5]

#++++++++++++++++++++++++++
#test example for graphicsData in VRobotLink
s0=0.08
wj = 0.12
rj = 0.06
gLink0 = GraphicsDataOrthoCubePoint(centerPoint=[0,0,0], size=[s0,s0,1.2*s0], color=color4list[0], addEdges=True)
gLink0 += GraphicsDataCylinder(pAxis=[0,-1*wj,0], vAxis=[0,wj,0], radius=rj, color=color4list[0], addEdges=True) #base joint
gLink0 += GraphicsDataCylinder(pAxis=[0,0,-0.5*wj], vAxis=[0,0,wj], radius=rj, color=color4list[0], addEdges=True)
gLink1 = GraphicsDataOrthoCubePoint(centerPoint=[-0.5*0.4318,0,0], size=[0.4318,s0,s0], color=color4list[1], addEdges=True)
gLink1 += GraphicsDataCylinder(pAxis=[0,0,-0.5*wj], vAxis=[0,0,wj], radius=rj, color=color4list[1], addEdges=True)
gLink2 = GraphicsDataOrthoCubePoint(centerPoint=[0, 0.5*0.15,0], size=[s0,0.15,s0], color=color4list[2], addEdges=True)
gLink2 += GraphicsDataCylinder(pAxis=[0,0,-0.5*wj], vAxis=[0,0,wj], radius=rj, color=color4list[2], addEdges=True)

gLink3 = GraphicsDataOrthoCubePoint(centerPoint=[0,-0.5*0.4318, 0], size=[s0,0.4318,s0], color=color4list[3], addEdges=True)
gLink3 += GraphicsDataCylinder(pAxis=[0,0,-0.5*wj], vAxis=[0,0,wj], radius=rj, color=color4list[3], addEdges=True)
gLink4 = GraphicsDataOrthoCubePoint(centerPoint=[0,0,0], size=[s0,s0,s0], color=color4list[4], addEdges=True)
gLink4 += GraphicsDataCylinder(pAxis=[0,0,-0.5*wj], vAxis=[0,0,wj], radius=rj, color=color4list[4], addEdges=True)
gLink5 = GraphicsDataOrthoCubePoint(centerPoint=[0,0,0], size=[s0,s0,s0], color=color4list[5], addEdges=True)

gLinkList=[gLink0, gLink1, gLink2, gLink3, gLink4, gLink5]

if mode=='newDH':
    for cnt, link in enumerate(linkList):
        vLink = VRobotLink(showCOM=True, showMBSjoint=False, graphicsData=gLinkList[cnt])
        robot.AddLink(RobotLink(mass=link['mass'], 
                                   COM=link['COM'], 
                                   inertia=link['inertia'], 
                                   localHT=StdDH2HT(link['stdDH']),
                                   PDcontrol=(Pcontrol[cnt], Dcontrol[cnt]),
                                   visualization=VRobotLink(linkColor=color4list[cnt])
                                   #visualization=VRobotLink(showCOM=True, showMBSjoint=False, graphicsData=gLinkList[cnt])
                                   ))
elif mode=='newModDH': #computes preHT and localHT, but ALSO converts inertia parameters from stdDH to modDHKK (NEEDED!)
    for cnt, link in enumerate(linkList): 
        [preHT, localHT] =  ModDHKK2HT(link['modKKDH'])
        stdLocalHT =  StdDH2HT(link['stdDH'])
        HT = InverseHT(stdLocalHT) @ (localHT) #from stdHT back and forward in localHT of ModDHKK
        
        rbi = RigidBodyInertia()
        rbi.SetWithCOMinertia(link['mass'], link['inertia'], link['COM'])

        rbi = rbi.Transformed(InverseHT(HT)) #inertia parameters need to be transformed to new modKKDH link frame
        
        vLink = VRobotLink(showCOM=True, showMBSjoint=False, graphicsData=gLinkList[cnt])
        robot.AddLink(RobotLink(mass=rbi.mass,
                                   COM=rbi.COM(), 
                                   inertia=rbi.InertiaCOM(),
                                   preHT = preHT,
                                   localHT=localHT,
                                   PDcontrol=(Pcontrol[cnt], Dcontrol[cnt]),
                                   visualization=VRobotLink(linkColor=color4list[cnt])
                                   #visualization=VRobotLink(showCOM=True, showMBSjoint=False, graphicsData=gLinkList[cnt])
                                   ))
else:
    raise ValueError('invalid mode!')

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#configurations and trajectory
q0 = [0,0,0,0,0,0] #zero angle configuration

#this set of coordinates only works with TSD, not with old fashion load control:
# q1 = [0, pi/8, pi*0.75, 0,pi/8,0] #configuration 1
# q2 = [pi,-pi, -pi*0.5,1.5*pi,-pi*2,pi*2] #configuration 2
# q3 = [3*pi,0,-0.25*pi,0,0,0] #zero angle configuration

#this set also works with load control:
q1 = [0, pi/8, pi*0.5, 0,pi/8,0] #configuration 1
q2 = [0.8*pi,-0.8*pi, -pi*0.5,0.75*pi,-pi*0.4,pi*0.4] #configuration 2
q3 = [0.5*pi,0,-0.25*pi,0,0,0] #zero angle configuration

#trajectory generated with optimal acceleration profiles:
trajectory = Trajectory(initialCoordinates=q0, initialTime=0)
trajectory.Add(ProfileConstantAcceleration(q3,0.25))
trajectory.Add(ProfileConstantAcceleration(q1,0.25))
trajectory.Add(ProfileConstantAcceleration(q2,0.25))
trajectory.Add(ProfileConstantAcceleration(q0,0.25))
#traj.Add(ProfilePTP([1,1],syncAccTimes=False, maxVelocities=[1,1], maxAccelerations=[5,5]))

# x = traj.EvaluateCoordinate(t,0)


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#test robot model
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


jointList = [0]*robot.NumberOfLinks() #this list must be filled afterwards with the joint numbers in the mbs!

def ComputeMBSstaticRobotTorques(robot):
    
    if not useKinematicTree:
        q=[]
        for joint in jointList:
            q += [mbs.GetObjectOutput(joint, exu.OutputVariableType.Rotation)[2]] #z-rotation
    else:
        q = mbs.GetObjectOutputBody(oKT, exu.OutputVariableType.Coordinates, localPosition=[0,0,0])

    HT=robot.JointHT(q)
    return robot.StaticTorques(HT)

#++++++++++++++++++++++++++++++++++++++++++++++++
#base, graphics, object and marker:

objectGround = mbs.AddObject(ObjectGround(referencePosition=HT2translation(robot.GetBaseHT()), 
                                      #visualization=VObjectGround(graphicsData=graphicsBaseList)
                                          ))


#baseMarker; could also be a moving base!
baseMarker = mbs.AddMarker(MarkerBodyRigid(bodyNumber=objectGround, localPosition=[0,0,0]))



#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#build mbs robot model:
if not useKinematicTree:
    robotDict = robot.CreateRedundantCoordinateMBS(mbs, baseMarker=baseMarker, 
                                                      createJointTorqueLoads=False,
                                                      )
        
    jointList = robotDict['jointList'] #must be stored there for the load user function
else:
    robotDict = robot.CreateKinematicTree(mbs)
    oKT = robotDict['objectKinematicTree']
    
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#control robot
# torsionalSDlist = []

# for i in range(len(jointList)):
#     joint = jointList[i]
#     rot0 = mbs.GetObject(joint)['rotationMarker0']
#     rot1 = mbs.GetObject(joint)['rotationMarker1']
#     markers = mbs.GetObject(joint)['markerNumbers']
#     nGeneric=mbs.AddNode(NodeGenericData(initialCoordinates=[0], 
#                                          numberOfDataCoordinates=1)) #for infinite rotations
#     tsd = mbs.AddObject(TorsionalSpringDamper(markerNumbers=markers,
#                                         nodeNumber=nGeneric,
#                                         rotationMarker0=rot0,
#                                         rotationMarker1=rot1,                                            
#                                         stiffness=Pcontrol[i],
#                                         damping=Dcontrol[i],
#                                         visualization=VTorsionalSpringDamper(drawSize=0.1)
#                                         ))
#     torsionalSDlist += [tsd]
    

#user function which is called only once per step, speeds up simulation drastically
def PreStepUF(mbs, t):
    if compensateStaticTorques:
        staticTorques = ComputeMBSstaticRobotTorques(robot)
        #print("tau=", staticTorques)
    else:
        staticTorques = np.zeros(len(jointList))
        
    [u,v,a] = trajectory.Evaluate(t)

    #compute load for joint number
    if not useKinematicTree:
        for i in range(len(robot.links)):
            joint = jointList[i]
            phi = mbs.GetObjectOutput(joint, exu.OutputVariableType.Rotation)[2] #z-rotation
            omega = mbs.GetObjectOutput(joint, exu.OutputVariableType.AngularVelocityLocal)[2] #z-angular velocity
            tsd = torsionalSDlist[i]
            mbs.SetObjectParameter(tsd, 'offset', u[i])
            mbs.SetObjectParameter(tsd, 'velocityOffset', v[i])
            mbs.SetObjectParameter(tsd, 'torque', staticTorques[i]) #additional torque from given velocity 
    else:
        #in case of kinematic tree, very simple operations!
        mbs.SetObjectParameter(oKT, 'jointPositionOffsetVector', u)
        mbs.SetObjectParameter(oKT, 'jointVelocityOffsetVector', v)
        mbs.SetObjectParameter(oKT, 'jointForceVector', staticTorques)
    
    return True

mbs.SetPreStepUserFunction(PreStepUF)

if not useKinematicTree:
    torsionalSDlist = robotDict['springDamperList']
    
    #add sensors:
    sJointRotList = []
    cnt = 0
    for i in range(len(jointList)):
        jointLink = jointList[i]
        tsd = torsionalSDlist[i]
        #using TSD:
        sJointRot = mbs.AddSensor(SensorObject(objectNumber=tsd, 
                                   fileName="solution/joint" + str(i) + "Rot.txt",
                                   outputVariableType=exu.OutputVariableType.Rotation,
                                   writeToFile = sensorWriteToFile))
        sJointRotList += [sJointRot]
        sJointAngVel = mbs.AddSensor(SensorObject(objectNumber=jointLink, 
                                   fileName="solution/joint" + str(i) + "AngVel.txt",
                                   outputVariableType=exu.OutputVariableType.AngularVelocityLocal,
                                   writeToFile = sensorWriteToFile))

    cnt = 0
    sTorques = []
    for iSD in robotDict['springDamperList']:
        # sTorque = mbs.AddSensor(SensorLoad(loadNumber=load0, fileName="solution/jointTorque" + str(cnt) + ".txt", 
        #                                    writeToFile = sensorWriteToFile))
        sTorque = mbs.AddSensor(SensorObject(objectNumber=iSD, fileName="solution/jointTorque" + str(cnt) + ".txt", 
                                             outputVariableType=exu.OutputVariableType.TorqueLocal,
                                             writeToFile = sensorWriteToFile))
        sTorques += [sTorque]
        
        cnt+=1



mbs.Assemble()
#mbs.systemData.Info()

SC.visualizationSettings.connectors.showJointAxes = True
SC.visualizationSettings.connectors.jointAxesLength = 0.02
SC.visualizationSettings.connectors.jointAxesRadius = 0.002

SC.visualizationSettings.nodes.showBasis = True
SC.visualizationSettings.nodes.basisSize = 0.1
SC.visualizationSettings.loads.show = False

SC.visualizationSettings.openGL.multiSampling=4
    
# tEnd = 1.25
# h = 0.002
tEnd = 0.4
h = 0.001#*0.1*0.01

#mbs.WaitForUserToContinue()
simulationSettings = exu.SimulationSettings() #takes currently set values or default values

simulationSettings.timeIntegration.numberOfSteps = int(tEnd/h)
simulationSettings.timeIntegration.endTime = tEnd
simulationSettings.solutionSettings.solutionWritePeriod = 0.005
simulationSettings.solutionSettings.sensorsWritePeriod = 0.005
simulationSettings.solutionSettings.binarySolutionFile = True
#simulationSettings.solutionSettings.writeSolutionToFile = False
# simulationSettings.timeIntegration.simulateInRealtime = True
# simulationSettings.timeIntegration.realtimeFactor = 0.25

simulationSettings.timeIntegration.verboseMode = 1
simulationSettings.displayComputationTime = True
simulationSettings.displayStatistics = True
#simulationSettings.linearSolverType = exu.LinearSolverType.EigenSparse

#simulationSettings.timeIntegration.newton.useModifiedNewton = True
simulationSettings.timeIntegration.generalizedAlpha.useIndex2Constraints = True
simulationSettings.timeIntegration.generalizedAlpha.useNewmark = simulationSettings.timeIntegration.generalizedAlpha.useIndex2Constraints
simulationSettings.timeIntegration.newton.useModifiedNewton = True

simulationSettings.timeIntegration.generalizedAlpha.computeInitialAccelerations=True
SC.visualizationSettings.general.autoFitScene=False
SC.visualizationSettings.window.renderWindowSize=[1920,1200]
useGraphics = False

if useGraphics:
    exu.StartRenderer()
    if 'renderState' in exu.sys:
        SC.SetRenderState(exu.sys['renderState'])
    mbs.WaitForUserToContinue()
    
exu.SolveDynamic(mbs, simulationSettings, showHints=True)
# exu.SolveDynamic(mbs, simulationSettings, 
#                  solverType=exu.DynamicSolverType.RK33,
#                  showHints=True)


if useGraphics:
    SC.visualizationSettings.general.autoFitScene = False
    exu.StopRenderer()

from exudyn.interactive import SolutionViewer
SolutionViewer(mbs)


if not useKinematicTree:
    #compute final torques:
    measuredTorques=[]
    for sensorNumber in sTorques:
        measuredTorques += [abs(mbs.GetSensorValues(sensorNumber)) ]
    exu.Print("torques at tEnd=", VSum(measuredTorques))

    measuredRot = []
    for sensorNumber in sJointRotList :
        measuredRot += [(mbs.GetSensorValues(sensorNumber)) ]
    exu.Print("rotations at tEnd=", VSum(measuredRot), ',', measuredRot)

else:
    q = mbs.GetObjectOutputBody(oKT, exu.OutputVariableType.Coordinates, localPosition=[0,0,0])
    exu.Print("rotations at tEnd=", VSum(q), ',', q)
    


#add larger test tolerance for 32/64bits difference
# exudynTestGlobals.testError = 1e-2*(VSum(measuredTorques) - 76.80031232091771 )  #old controller: 77.12176106978085) #OLDER results: up to 2021-06-28: 0.7712176106955341; 2020-08-25: 77.13193176752571 (32bits),   2020-08-24: (64bits)77.13193176846507
# exudynTestGlobals.testResult = 1e-2*VSum(measuredTorques)   


if False:
    import matplotlib.pyplot as plt
    import matplotlib.ticker as ticker
    plt.rcParams.update({'font.size': 14})
    plt.close("all")

    doJointTorques = False
    if doJointTorques:
        for i in range(6):
            data = np.loadtxt("solution/jointTorque" + str(i) + ".txt", comments='#', delimiter=',')
            plt.plot(data[:,0], data[:,3], PlotLineCode(i), label="joint torque"+str(i)) #z-rotation
    
        plt.xlabel("time (s)")
        plt.ylabel("joint torque (Nm)")
        ax=plt.gca() # get current axes
        ax.grid(True, 'major', 'both')
        ax.xaxis.set_major_locator(ticker.MaxNLocator(10)) 
        ax.yaxis.set_major_locator(ticker.MaxNLocator(10)) 
        plt.tight_layout()
        ax.legend(loc='center right')
        plt.show() 
        # plt.savefig("solution/robotJointTorques.pdf")

    doJointAngles = True
    if doJointAngles:
        plt.close("all")
        
        for i in range(6):
            data = np.loadtxt("solution/joint" + str(i) + "Rot.txt", comments='#', delimiter=',')
            # data = np.loadtxt("solution/joint" + str(i) + "AngVel.txt", comments='#', delimiter=',')
            plt.plot(data[:,0], data[:,1], PlotLineCode(i), label="joint"+str(i)) #z-rotation
            
        plt.xlabel("time (s)")
        plt.ylabel("joint angle (rad)")
        ax=plt.gca() 
        ax.grid(True, 'major', 'both')
        ax.xaxis.set_major_locator(ticker.MaxNLocator(10)) 
        ax.yaxis.set_major_locator(ticker.MaxNLocator(10)) 
        plt.tight_layout()
        ax.legend()
        plt.rcParams.update({'font.size': 16})
        plt.show() 
        # plt.savefig("solution/robotJointAngles.pdf")

