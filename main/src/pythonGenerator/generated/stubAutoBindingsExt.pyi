
class MainSystem:
    @overload
    def SolutionViewer(self, solution=None, rowIncrement=1, timeout=0.04, runOnStart=True, runMode=2, fontSize=12, title='', checkRenderEngineStopFlag=True) -> None: ...

    @overload
    def CreateGround(self, name='', referencePosition=[0.,0.,0.], referenceRotationMatrix=np.eye(3), graphicsDataList=[], graphicsDataUserFunction=0, show=True) -> ObjectIndex: ...

    @overload
    def CreateMassPoint(self, name='', referencePosition=[0.,0.,0.], initialDisplacement=[0.,0.,0.], initialVelocity=[0.,0.,0.], physicsMass=0, gravity=[0.,0.,0.], graphicsDataList=[], drawSize=-1, color=[-1.,-1.,-1.,-1.], show=True, create2D=False, returnDict=False) -> Union[dict, ObjectIndex]: ...

    @overload
    def CreateRigidBody(self, name='', referencePosition=[0.,0.,0.], referenceRotationMatrix=np.eye(3), initialVelocity=[0.,0.,0.], initialAngularVelocity=[0.,0.,0.], initialDisplacement=None, initialRotationMatrix=None, inertia=None, gravity=[0.,0.,0.], nodeType=exudyn.NodeType.RotationEulerParameters, graphicsDataList=[], graphicsDataUserFunction=0, drawSize=-1, color=[-1.,-1.,-1.,-1.], show=True, create2D=False, returnDict=False) -> Union[dict, ObjectIndex]: ...

    @overload
    def CreateSpringDamper(self, name='', bodyList=[None, None], localPosition0=[0.,0.,0.], localPosition1=[0.,0.,0.], referenceLength=None, stiffness=0., damping=0., force=0., velocityOffset=0., springForceUserFunction=0, bodyOrNodeList=[None, None], show=True, drawSize=-1, color=color4default) -> ObjectIndex: ...

    @overload
    def CreateCartesianSpringDamper(self, name='', bodyList=[None, None], localPosition0=[0.,0.,0.], localPosition1=[0.,0.,0.], stiffness=[0.,0.,0.], damping=[0.,0.,0.], offset=[0.,0.,0.], springForceUserFunction=0, bodyOrNodeList=[None, None], show=True, drawSize=-1, color=color4default) -> ObjectIndex: ...

    @overload
    def CreateRigidBodySpringDamper(self, name='', bodyList=[None, None], localPosition0=[0.,0.,0.], localPosition1=[0.,0.,0.], stiffness=np.zeros((6,6)), damping=np.zeros((6,6)), offset=[0.,0.,0.,0.,0.,0.], rotationMatrixJoint=np.eye(3), useGlobalFrame=True, intrinsicFormulation=True, springForceTorqueUserFunction=0, postNewtonStepUserFunction=0, bodyOrNodeList=[None, None], show=True, drawSize=-1, color=color4default) -> ObjectIndex: ...

    @overload
    def CreateRevoluteJoint(self, name='', bodyNumbers=[None, None], position=[], axis=[], useGlobalFrame=True, show=True, axisRadius=0.1, axisLength=0.4, color=color4default) -> [ObjectIndex, MarkerIndex, MarkerIndex]: ...

    @overload
    def CreatePrismaticJoint(self, name='', bodyNumbers=[None, None], position=[], axis=[], useGlobalFrame=True, show=True, axisRadius=0.1, axisLength=0.4, color=color4default) -> [ObjectIndex, MarkerIndex, MarkerIndex]: ...

    @overload
    def CreateSphericalJoint(self, name='', bodyNumbers=[None, None], position=[], constrainedAxes=[1,1,1], useGlobalFrame=True, show=True, jointRadius=0.1, color=color4default) -> [ObjectIndex, MarkerIndex, MarkerIndex]: ...

    @overload
    def CreateGenericJoint(self, name='', bodyNumbers=[None, None], position=[], rotationMatrixAxes=np.eye(3), constrainedAxes=[1,1,1, 1,1,1], useGlobalFrame=True, offsetUserFunction=0, offsetUserFunction\_t=0, show=True, axesRadius=0.1, axesLength=0.4, color=color4default) -> [ObjectIndex, MarkerIndex, MarkerIndex]: ...

    @overload
    def CreateDistanceConstraint(self, name='', bodyList=[None, None], localPosition0=[0.,0.,0.], localPosition1=[0.,0.,0.], distance=None, bodyOrNodeList=[None, None], show=True, drawSize=-1., color=color4default) -> [ObjectIndex, MarkerIndex, MarkerIndex]: ...

    @overload
    def CreateForce(self, name='', bodyNumber=None, loadVector=[0.,0.,0.], localPosition=[0.,0.,0.], bodyFixed=False, loadVectorUserFunction=0, show=True) -> LoadIndex: ...

    @overload
    def CreateTorque(self, name='', bodyNumber=None, loadVector=[0.,0.,0.], localPosition=[0.,0.,0.], bodyFixed=False, loadVectorUserFunction=0, show=True) -> LoadIndex: ...

    @overload
    def PlotSensor(self, sensorNumbers=[], components=0, xLabel='time (s)', yLabel=None, labels=[], colorCodeOffset=0, newFigure=True, closeAll=False, componentsX=[], title='', figureName='', fontSize=16, colors=[], lineStyles=[], lineWidths=[], markerStyles=[], markerSizes=[], markerDensity=0.08, rangeX=[], rangeY=[], majorTicksX=10, majorTicksY=10, offsets=[], factors=[], subPlot=[], sizeInches=[6.4,4.8], fileName='', useXYZcomponents=True, **kwargs) -> [Any, Any, Any, Any]: ...

    @overload
    def SolveStatic(self, simulationSettings=exudyn.SimulationSettings(), updateInitialValues=False, storeSolver=True, showHints=False, showCausingItems=True) -> bool: ...

    @overload
    def SolveDynamic(self, simulationSettings=exudyn.SimulationSettings(), solverType=exudyn.DynamicSolverType.GeneralizedAlpha, updateInitialValues=False, storeSolver=True, showHints=False, showCausingItems=True) -> bool: ...

    @overload
    def ComputeLinearizedSystem(self, simulationSettings=exudyn.SimulationSettings(), useSparseSolver=False) -> [ArrayLike, ArrayLike, ArrayLike]: ...

    @overload
    def ComputeODE2Eigenvalues(self, simulationSettings=exudyn.SimulationSettings(), useSparseSolver=False, numberOfEigenvalues=0, constrainedCoordinates=[], convert2Frequencies=False, useAbsoluteValues=True, ignoreAlgebraicEquations=False, singularValuesTolerance=1e-12) -> [ArrayLike, ArrayLike]: ...

    @overload
    def ComputeSystemDegreeOfFreedom(self, simulationSettings=exudyn.SimulationSettings(), threshold=1e-12, verbose=False, useSVD=False) -> dict: ...

    @overload
    def CreateDistanceSensorGeometry(self, meshPoints, meshTrigs, rigidBodyMarkerIndex, searchTreeCellSize=[8,8,8]) -> int: ...

    @overload
    def CreateDistanceSensor(self, generalContactIndex, positionOrMarker, dirSensor, minDistance=-1e7, maxDistance=1e7, cylinderRadius=0, selectedTypeIndex=exudyn.ContactTypeIndex.IndexEndOfEnumList, storeInternal=False, fileName='', measureVelocity=False, addGraphicsObject=False, drawDisplaced=True, color=color4red) -> SensorIndex: ...

    @overload
    def DrawSystemGraph(self, showLoads=True, showSensors=True, useItemNames=False, useItemTypes=False, addItemTypeNames=True, multiLine=True, fontSizeFactor=1., layoutDistanceFactor=3., layoutIterations=100, showLegend=True, tightLayout=True) -> [Any, Any, Any]: ...

