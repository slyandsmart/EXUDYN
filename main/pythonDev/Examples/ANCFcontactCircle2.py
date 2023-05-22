#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN example
#
# Details:  ANCF Cable2D contact test
#
# Author:   Johannes Gerstmayr
# Date:     2019-10-01
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

import exudyn as exu
from exudyn.utilities import *


SC = exu.SystemContainer()
mbs = SC.AddSystem()

L=2                     # length of ANCF element in m
pCircle = [0.65*L,-0.5,0]
pCircle2 =  [0.25*L,-0.15,0]
circleRadius=0.3
circleRadius2=0.1

#background
rect = [-0.5,-1,2.5,1] #xmin,ymin,xmax,ymax
background1 = {'type':'Line', 'color':[0.1,0.1,0.8,1], 'data':[0,-1,0, 2,-1,0]} #background


background  = [GraphicsDataRectangle(-0.5,-1,2.5,1, color=color4blue)]
background += [GraphicsDataLine([[0,-1,0], [2,-1,0]], color=color4green)]
background += [GraphicsDataCircle(point=pCircle, radius = circleRadius-0.002, color=color4blue)] #not necessary, as it is drawn by connector
background += [GraphicsDataCircle(point=pCircle2, radius = circleRadius2-0.002, color=color4blue)] #not necessary, as it is drawn by connector
background += [GraphicsDataText(point=[0.,0.2,0], text = 'ANCF contact with circle', color=color4black)]

oGround=mbs.AddObject(ObjectGround(referencePosition= [0,0,0], visualization=VObjectGround(graphicsData= background)))


#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#cable:
mypi = 3.141592653589793

#L=mypi                 # length of ANCF element in m
E=2.07e11               # Young's modulus of ANCF element in N/m^2
rho=7800                # density of ANCF element in kg/m^3
b=0.001                 # width of rectangular ANCF element in m
h=0.001                 # height of rectangular ANCF element in m
A=b*h                   # cross sectional area of ANCF element in m^2
I=b*h**3/12             # second moment of area of ANCF element in m^4
f=3*E*I/L**2            # tip load applied to ANCF element in N

print("load f="+str(f))
print("EI="+str(E*I))

nGround = mbs.AddNode(NodePointGround(referenceCoordinates=[0,0,0])) #ground node for coordinate constraint
mGround = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = nGround, coordinate=0)) #Ground node ==> no action

cableList=[]        #for cable elements
nodeList=[]  #for nodes of cable
markerList=[]       #for nodes
nc0 = mbs.AddNode(Point2DS1(referenceCoordinates=[0,0,1,0]))
nodeList+=[nc0]
nElements = 8*4 #8,16,32,64
lElem = L / nElements
for i in range(nElements):
    nLast = mbs.AddNode(Point2DS1(referenceCoordinates=[lElem*(i+1),0,1,0]))
    nodeList+=[nLast]
    elem=mbs.AddObject(Cable2D(physicsLength=lElem, physicsMassPerLength=rho*A, 
                               physicsBendingStiffness=E*I, physicsAxialStiffness=E*A*0.1, 
                               nodeNumbers=[int(nc0)+i,int(nc0)+i+1]))
    cableList+=[elem]

mANCF0 = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = nc0, coordinate=0))
mANCF1 = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = nc0, coordinate=1))
mANCF2 = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = nc0, coordinate=3))
    
mbs.AddObject(CoordinateConstraint(markerNumbers=[mGround,mANCF0]))
mbs.AddObject(CoordinateConstraint(markerNumbers=[mGround,mANCF1]))
mbs.AddObject(CoordinateConstraint(markerNumbers=[mGround,mANCF2]))

#add gravity:
markerList=[]
for i in range(len(nodeList)):
    m = mbs.AddMarker(MarkerNodePosition(nodeNumber=nodeList[i])) 
    markerList+=[m]
    fact = 1 #add (half) weight of two elements to node
    if (i==0) | (i==len(nodeList)-1): fact = 0.5 # first and last node only weighted half
    mbs.AddLoad(Force(markerNumber = m, loadVector = [0, -400*rho*A*fact*lElem, 0])) #will be changed in load steps

#mANCFend = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = nodeList[-1], coordinate=1)) #last marker
#mbs.AddObject(CoordinateConstraint(markerNumbers=[mGround,mANCFend]))

#mGroundTip = mbs.AddMarker(MarkerBodyPosition(bodyNumber = oGround, localPosition=[L,0,0])) 
#mbs.AddObject(CartesianSpringDamper(markerNumbers=[mGroundTip,markerList[-1]], stiffness=[10,10,10], damping=[0.1,0.1,0.1]))

#mGroundTip2 = mbs.AddMarker(MarkerBodyPosition(bodyNumber = oGround, localPosition=[L,0.2,0])) 
#mbs.AddObject(SpringDamper(markerNumbers=[mGroundTip2,markerList[-1]], stiffness=0.1, referenceLength=0.2))

#mANCFLast = mbs.AddMarker(MarkerNodePosition(nodeNumber=nLast)) #force
#mbs.AddLoad(Force(markerNumber = mANCFLast, loadVector = [0, -1e8, 0])) #will be changed in load steps

#mANCFrigid = mbs.AddMarker(MarkerBodyRigid(bodyNumber=elem, localPosition=[lElem,0,0])) #local position L = beam tip
#mbs.AddLoad(Torque(markerNumber = mANCFrigid, loadVector = [0, 0, E*I*1*mypi]))

#mANCFnode = mbs.AddMarker(MarkerNodeRigid(nodeNumber=nLast)) #local position L = beam tip
#mbs.AddLoad(Torque(markerNumber = mANCFnode, loadVector = [0, 0, 3*E*I*1*mypi]))

cStiffness = 1e3
cDamping = 0.02*cStiffness*0
useContact = False
if useContact:
    tipContact = False
    if tipContact:
        nodeData = mbs.AddNode(NodeGenericData(initialCoordinates=[0],numberOfDataCoordinates=1))
        mbs.AddObject(ObjectContactCoordinate(markerNumbers=[mGround, mANCFend],nodeNumber = nodeData, contactStiffness = cStiffness, contactDamping=cDamping, offset = -0.8))
    else:
        for i in range(len(nodeList)):
            mNC = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = nodeList[i], coordinate=1))
            nodeData = mbs.AddNode(NodeGenericData(initialCoordinates=[1],numberOfDataCoordinates=1)) #start with gap!
            mbs.AddObject(ObjectContactCoordinate(markerNumbers=[mGround, mNC], nodeNumber = nodeData, contactStiffness = cStiffness, contactDamping=cDamping, offset = -1))

useCircleContact = True
if useCircleContact:
    nSegments = 8 #4; number of contact segments; must be consistent between nodedata and contact element
    initialGapList = [0.1]*nSegments #initial gap of 0.1
    mGroundCircle = mbs.AddMarker(MarkerBodyPosition(bodyNumber = oGround, localPosition=pCircle)) 
    mGroundCircle2 = mbs.AddMarker(MarkerBodyPosition(bodyNumber = oGround, localPosition=pCircle2)) 

    #mCable = mbs.AddMarker(MarkerBodyCable2DShape(bodyNumber=elem, numberOfSegments = nSegments))
    #nodeDataContactCable = mbs.AddNode(NodeGenericData(initialCoordinates=initialGapList,numberOfDataCoordinates=nSegments))
    #mbs.AddObject(ObjectContactCircleCable2D(markerNumbers=[mGroundCircle, mCable], nodeNumber = nodeDataContactCable, 
    #                                         numberOfContactSegments=nSegments, contactStiffness = cStiffness, contactDamping=cDamping, 
    #                                         circleRadius = 0.4, offset = 0))
    for i in range(len(cableList)):
        #print("cable="+str(cableList[i]))
        mCable = mbs.AddMarker(MarkerBodyCable2DShape(bodyNumber=cableList[i], numberOfSegments = nSegments))
        #print("mCable="+str(mCable))
        nodeDataContactCable = mbs.AddNode(NodeGenericData(initialCoordinates=initialGapList,numberOfDataCoordinates=nSegments))
        mbs.AddObject(ObjectContactCircleCable2D(markerNumbers=[mGroundCircle, mCable], nodeNumber = nodeDataContactCable, 
                                                 numberOfContactSegments=nSegments, contactStiffness = cStiffness, contactDamping=cDamping, 
                                                 circleRadius = circleRadius, offset = 0))
        nodeDataContactCable = mbs.AddNode(NodeGenericData(initialCoordinates=initialGapList,numberOfDataCoordinates=nSegments))
        mbs.AddObject(ObjectContactCircleCable2D(markerNumbers=[mGroundCircle2, mCable], nodeNumber = nodeDataContactCable, 
                                                 numberOfContactSegments=nSegments, contactStiffness = cStiffness, contactDamping=cDamping, 
                                                 circleRadius = circleRadius2, offset = 0))


#mbs.systemData.Info()

mbs.Assemble()
print(mbs)

simulationSettings = exu.SimulationSettings() #takes currently set values or default values
#simulationSettings.solutionSettings.coordinatesSolutionFileName = 'ANCFCable2Dbending' + str(nElements) + '.txt'

fact = 10000
simulationSettings.timeIntegration.numberOfSteps = 1*fact
simulationSettings.timeIntegration.endTime = 0.001*fact
simulationSettings.solutionSettings.writeSolutionToFile = True
simulationSettings.solutionSettings.solutionWritePeriod = simulationSettings.timeIntegration.endTime/fact
#simulationSettings.solutionSettings.outputPrecision = 4
simulationSettings.displayComputationTime = True
simulationSettings.timeIntegration.verboseMode = 1

simulationSettings.timeIntegration.newton.relativeTolerance = 1e-8*10 #10000
simulationSettings.timeIntegration.newton.absoluteTolerance = 1e-10*100

simulationSettings.timeIntegration.newton.useModifiedNewton = False
simulationSettings.timeIntegration.newton.maxModifiedNewtonIterations = 5
simulationSettings.timeIntegration.newton.numericalDifferentiation.minimumCoordinateSize = 1
simulationSettings.timeIntegration.newton.numericalDifferentiation.relativeEpsilon = 6.055454452393343e-06*0.1 #eps^(1/3)
simulationSettings.timeIntegration.newton.modifiedNewtonContractivity = 1e8
simulationSettings.timeIntegration.generalizedAlpha.useIndex2Constraints = False
simulationSettings.timeIntegration.generalizedAlpha.useNewmark = False
simulationSettings.timeIntegration.generalizedAlpha.spectralRadius = 0.6 #0.6 works well 
simulationSettings.displayStatistics = True

#SC.visualizationSettings.nodes.showNumbers = True
SC.visualizationSettings.bodies.showNumbers = False
#SC.visualizationSettings.connectors.showNumbers = True
SC.visualizationSettings.nodes.defaultSize = 0.01
SC.visualizationSettings.markers.defaultSize = 0.01
SC.visualizationSettings.connectors.defaultSize = 0.01
SC.visualizationSettings.contact.contactPointsDefaultSize = 0.005
SC.visualizationSettings.connectors.showContact = 1
SC.visualizationSettings.general.circleTiling = 64

simulationSettings.solutionSettings.solutionInformation = "ANCF cable with imposed curvature or applied tip force/torque"

solveDynamic = False
if solveDynamic: 
    exu.StartRenderer()

    mbs.SolveDynamic(simulationSettings)

    SC.WaitForRenderEngineStopFlag()
    exu.StopRenderer() #safely close rendering window!

else:
    simulationSettings.staticSolver.newton.numericalDifferentiation.relativeEpsilon = 1e-10*100 #can be quite small; WHY?
    simulationSettings.staticSolver.newton.numericalDifferentiation.doSystemWideDifferentiation = False
    simulationSettings.staticSolver.verboseMode = 1
    simulationSettings.staticSolver.numberOfLoadSteps  = 20*2
    simulationSettings.staticSolver.loadStepGeometric = True;
    simulationSettings.staticSolver.loadStepGeometricRange = 5e3;

    simulationSettings.staticSolver.newton.relativeTolerance = 1e-5*100 #10000
    simulationSettings.staticSolver.newton.absoluteTolerance = 1e-10
    simulationSettings.staticSolver.newton.maxIterations = 30 #50 for bending into circle

    simulationSettings.staticSolver.discontinuous.iterationTolerance = 0.1
    #simulationSettings.staticSolver.discontinuous.maxIterations = 5
    simulationSettings.pauseAfterEachStep = False
    simulationSettings.staticSolver.stabilizerODE2term = 50 

    exu.StartRenderer()

    mbs.WaitForUserToContinue()
    mbs.SolveStatic(simulationSettings)

    #sol = mbs.systemData.GetODE2Coordinates()
    #n = len(sol)
    #print('tip displacement: x='+str(sol[n-4])+', y='+str(sol[n-3])) 

    SC.WaitForRenderEngineStopFlag()
    exu.StopRenderer() #safely close rendering window!

# exu.InfoStat();
