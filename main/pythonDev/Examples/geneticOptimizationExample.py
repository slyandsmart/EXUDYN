#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN example
#
# Details:  This example performs a genetic algorithm to optimization using a simple
#           mass-spring-damper system; varying mass, spring, ...
#           The objective function is the error compared to 
#           a reference solution using reference/nominal values (which are known here, but could originate from a measurement)
#
# Author:   Johannes Gerstmayr
# Date:     2020-11-18
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

import exudyn as exu
from exudyn.itemInterface import *
from exudyn.processing import GeneticOptimization, PlotOptimizationResults2D

import numpy as np #for postprocessing
import os
from time import sleep

#this is the function which is repeatedly called from ParameterVariation
#parameterSet contains dictinary with varied parameters
def ParameterFunction(parameterSet):
    SC = exu.SystemContainer()
    mbs = SC.AddSystem()

    #default values
    mass = 1.6          #mass in kg
    spring = 4000       #stiffness of spring-damper in N/m
    damper = 8          #damping constant in N/(m/s)
    u0=-0.08            #initial displacement
    v0=1                #initial velocity
    force =80               #force applied to mass

    #process parameters
    if 'mass' in parameterSet:
        mass = parameterSet['mass']
        
    if 'spring' in parameterSet:
        spring = parameterSet['spring']

    if 'force' in parameterSet:
        force = parameterSet['force']

    iCalc = 'Ref' #needed for parallel computation ==> output files are different for every computation
    if 'computationIndex' in parameterSet:
        iCalc = str(parameterSet['computationIndex'])
        #print("computation index=",iCalc, flush=True)

    #mass-spring-damper system
    L=0.5               #spring length (for drawing)
    
    # x0=force/spring         #static displacement
    
    # print('resonance frequency = '+str(np.sqrt(spring/mass)))
    # print('static displacement = '+str(x0))
    
    #node for 3D mass point:
    n1=mbs.AddNode(Point(referenceCoordinates = [L,0,0], 
                         initialCoordinates = [u0,0,0], 
                         initialVelocities= [v0,0,0]))
    
    #ground node
    nGround=mbs.AddNode(NodePointGround(referenceCoordinates = [0,0,0]))
    
    #add mass point (this is a 3D object with 3 coordinates):
    massPoint = mbs.AddObject(MassPoint(physicsMass = mass, nodeNumber = n1))
    
    #marker for ground (=fixed):
    groundMarker=mbs.AddMarker(MarkerNodeCoordinate(nodeNumber= nGround, coordinate = 0))
    #marker for springDamper for first (x-)coordinate:
    nodeMarker  =mbs.AddMarker(MarkerNodeCoordinate(nodeNumber= n1, coordinate = 0))
    
    #spring-damper between two marker coordinates
    nC = mbs.AddObject(CoordinateSpringDamper(markerNumbers = [groundMarker, nodeMarker], 
                                              stiffness = spring, damping = damper)) 
    
    #add load:
    mbs.AddLoad(LoadCoordinate(markerNumber = nodeMarker, 
                                             load = force))
    #add sensor:
    sensorFileName = 'solution/paramVarDisplacement'+iCalc+'.txt'
    mbs.AddSensor(SensorObject(objectNumber=nC, fileName=sensorFileName, 
                               outputVariableType=exu.OutputVariableType.Displacement))
    #print("sensorFileName",sensorFileName)
    
    #print(mbs)
    mbs.Assemble()
    
    steps = 1000  #number of steps to show solution
    tEnd = 1     #end time of simulation
    
    simulationSettings = exu.SimulationSettings()
    #simulationSettings.solutionSettings.solutionWritePeriod = 5e-3  #output interval general
    simulationSettings.solutionSettings.writeSolutionToFile = False
    simulationSettings.solutionSettings.sensorsWritePeriod = 2e-3  #output interval of sensors
    simulationSettings.timeIntegration.numberOfSteps = steps
    simulationSettings.timeIntegration.endTime = tEnd
    
    simulationSettings.timeIntegration.generalizedAlpha.spectralRadius = 1 #no damping
    
    #exu.StartRenderer()              #start graphics visualization
    #mbs.WaitForUserToContinue()    #wait for pressing SPACE bar to continue
    
    #start solver:
    exu.SolveDynamic(mbs, simulationSettings)
    
    #SC.WaitForRenderEngineStopFlag()#wait for pressing 'Q' to quit
    #exu.StopRenderer()               #safely close rendering window!
    
    # #evaluate final (=current) output values
    # u = mbs.GetNodeOutput(n1, exu.OutputVariableType.Position)
    # print('displacement=',u)

    #+++++++++++++++++++++++++++++++++++++++++++++++++++++
    #evaluate difference between reference and optimized solution
    #reference solution:
    dataRef = np.loadtxt('solution/paramVarDisplacementRef.txt', comments='#', delimiter=',')
    data = np.loadtxt(sensorFileName, comments='#', delimiter=',')
    diff = data[:,1]-dataRef[:,1]
    
    errorNorm = np.sqrt(np.dot(diff,diff))/steps*tEnd
    
    #+++++++++++++++++++++++++++++++++++++++++++++++++++++
    #draw solution (not during optimization!):
    if False:
        from matplotlib import plt
        
        plt.close('all')
        plt.plot(data[:,0], data[:,1], 'b-', label='displacement (m)')
                
        ax=plt.gca() # get current axes
        ax.grid(True, 'major', 'both')
        ax.xaxis.set_major_locator(ticker.MaxNLocator(10)) 
        ax.yaxis.set_major_locator(ticker.MaxNLocator(10)) 
        plt.legend() #show labels as legend
        plt.tight_layout()
        plt.show() 

    if True: #delete files; does not work for parallel, consecutive operation
        if iCalc != 'Ref':
            os.remove(sensorFileName) #remove files in order to clean up
            while(os.path.exists(sensorFileName)): #wait until file is really deleted -> usually some delay
                sleep(0.001) #not nice, but there is no other way than that
        
    del mbs
    del SC
    
    return errorNorm


#now perform parameter variation
if __name__ == '__main__': #include this to enable parallel processing
    import time

    refval = ParameterFunction({}) # compute reference solution
    print("refval =", refval)
    
    start_time = time.time()
    [pOpt, vOpt, pList, values] = GeneticOptimization(objectiveFunction = ParameterFunction, 
                                         parameters = {'mass':(1,10), 'spring':(100,10000), 'force':(1,1000)}, #parameters provide search range
                                         numberOfGenerations = 20,
                                         initialPopulationSize = 100,
                                         rangeReductionFactor = 0.7,
                                         numberOfChildren = 8,
                                         addComputationIndex=True,
                                         randomizerInitialization=0, #for reproducible results
                                         distanceFactor = 0., #for this example only one significant minimum
                                         debugMode=False,
                                         useMultiProcessing=True,
                                         showProgress=True,
                                         )
    print("--- %s seconds ---" % (time.time() - start_time))

    print("optimum parameters=", pOpt)
    print("minimum value=", vOpt)

    from mpl_toolkits.mplot3d import Axes3D  # noqa: F401 unused import
    import matplotlib.pyplot as plt
    #from matplotlib import cm
    #from matplotlib.ticker import LinearLocator, FormatStrFormatter
    import numpy as np
    colorMap = plt.cm.get_cmap('jet') #finite element colors
    
    plt.close('all')
    [figList, axList] = PlotOptimizationResults2D(pList, values, yLogScale=True)
    
    #add 3D visualization of searched parameters
    if False:
        fig = plt.figure()
        ax = fig.gca(projection='3d') #not compatible with log scale!
        
        #plt.scatter(pDict['mass'], pDict['spring'], values, c='b', marker='o')
        #ps = ax.scatter(pList['mass'], pList['spring'], pList['force'], c=values, marker='o', cmap = colorMap)
        ps = ax.scatter(pList['mass'], pList['spring'], pList['force'], c=np.log(values), marker='o', cmap = colorMap)

        ax.set_xlabel('mass')
        ax.set_ylabel('spring')
        ax.set_zlabel('force')

        plt.colorbar(ps)
        plt.tight_layout()
        plt.show()
