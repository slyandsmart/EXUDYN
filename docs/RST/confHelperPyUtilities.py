#this is a helper file to define additional keywords for examples
#Created: 2023-03-17, Johannes Gerstmayr

listPyFunctionNames=['CreateMBS', 'SetupSpaces', 'MapAction2MBS', 'Output2StateAndDone', 'State2InitialValues', 'TestModel', 'SetSolver', 'PreInitializeSolver', 'IntegrateStep', 'step', 'reset', 'render', 'close', 'ClearWorkspace', 'DiagonalMatrix', 'NormL2', 'VSum', 'VAdd', 'VSub', 'VMult', 'ScalarMult', 'Normalize', 'Vec2Tilde', 'Tilde2Vec', 'GaussIntegrate', 'LobattoIntegrate', 'GenerateStraightLineANCFCable2D', 'GenerateCircularArcANCFCable2D', 'CreateReevingCurve', 'PointsAndSlopes2ANCFCable2D', 'GenerateSlidingJoint', 'GenerateAleSlidingJoint', 'CompressedRowSparseToDenseMatrix', 'MapSparseMatrixIndices', 'VectorDiadicUnitMatrix3D', 'CyclicCompareReversed', 'AddEntryToCompressedRowSparseArray', 'CSRtoRowsAndColumns', 'CSRtoScipySparseCSR', 'ScipySparseCSRtoCSR', 'ResortIndicesOfCSRmatrix', 'ResortIndicesOfNGvector', 'ResortIndicesExudyn2NGvector', 'ConvertHexToTrigs', 'ConvertDenseToCompressedRowMatrix', 'ReadMatrixFromAnsysMMF', 'ReadMatrixDOFmappingVectorFromAnsysTxt', 'ReadNodalCoordinatesFromAnsysTxt', 'ReadElementsFromAnsysTxt', 'CMSObjectComputeNorm', 'Strain2Stress', 'StrainVector2StressVector', 'StrainVector2StressVector2D', 'LameParameters', 'AddObjectFFRF', 'UFforce', 'UFmassGenericODE2', 'AddObjectFFRFreducedOrderWithUserFunctions', 'UFmassFFRFreducedOrder', 'UFforceFFRFreducedOrder', 'AddObjectFFRFreducedOrder', 'SaveToFile', 'LoadFromFile', 'ImportFromAbaqusInputFile', 'ReadMassMatrixFromAbaqus', 'ReadStiffnessMatrixFromAbaqus', 'ImportMeshFromNGsolve', 'ComputeEigenmodesNGsolve', 'ComputeHurtyCraigBamptonModesNGsolve', 'ComputePostProcessingModesNGsolve', 'GetMassMatrix', 'GetStiffnessMatrix', 'NumberOfNodes', 'GetNodePositionsAsArray', 'GetNodePositionsMean', 'NumberOfCoordinates', 'GetNodeAtPoint', 'GetNodesInPlane', 'GetNodesInCube', 'GetNodesOnLine', 'GetNodesOnCylinder', 'GetNodesOnCircle', 'GetNodeWeightsFromSurfaceAreas', 'GetSurfaceTriangles', 'VolumeToSurfaceElements', 'GetGyroscopicMatrix', 'ScaleMassMatrix', 'ScaleStiffnessMatrix', 'AddElasticSupportAtNode', 'AddNodeMass', 'CreateLinearFEMObjectGenericODE2', 'CreateNonlinearFEMObjectGenericODE2NGsolve', 'ComputeEigenmodes', 'ComputeEigenModesWithBoundaryNodes', 'ComputeHurtyCraigBamptonModes', 'GetEigenFrequenciesHz', 'ComputePostProcessingModes', 'ComputeCampbellDiagram', 'CheckConsistency', 'ReadMassMatrixFromAnsys', 'ReadStiffnessMatrixFromAnsys', 'ReadNodalCoordinatesFromAnsys', 'ReadElementsFromAnsys', 'SwitchTripletOrder', 'ComputeTriangleNormal', 'ComputeTriangleArea', 'GraphicsData2PointsAndTrigs', 'GraphicsDataFromPointsAndTrigs', 'RefineMesh', 'ShrinkMeshNormalToSurface', 'MoveGraphicsData', 'MergeGraphicsDataTriangleList', 'GraphicsDataLine', 'GraphicsDataCircle', 'GraphicsDataText', 'GraphicsDataRectangle', 'GraphicsDataOrthoCubeLines', 'GraphicsDataOrthoCube', 'GraphicsDataOrthoCubePoint', 'GraphicsDataCube', 'GraphicsDataSphere', 'GraphicsDataCylinder', 'GraphicsDataRigidLink', 'GraphicsDataFromSTLfileTxt', 'GraphicsDataFromSTLfile', 'AddEdgesAndSmoothenNormals', 'ExportGraphicsData2STL', 'GraphicsDataSolidOfRevolution', 'GraphicsDataArrow', 'GraphicsDataBasis', 'GraphicsDataFrame', 'GraphicsDataQuad', 'GraphicsDataCheckerBoard', 'ComputeTriangularMesh', 'SegmentsFromPoints', 'CirclePointsAndSegments', 'GraphicsDataSolidExtrusion', 'GetTkRootAndNewWindow', 'TkRootExists', 'EditDictionaryWithTypeInfo', 'AnimateModes', 'SolutionViewer', 'OnQuit', 'StartSimulation', 'ProcessWidgetStates', 'ContinuousRunFunction', 'InitializePlots', 'UpdatePlots', 'InitializeSolver', 'FinalizeSolver', 'RunSimulationPeriod', 'MassCOMinertia2T66', 'Inertia2T66', 'Inertia66toMassCOMinertia', 'JointTransformMotionSubspace66', 'JointTransformMotionSubspace', 'CRM', 'CRF', 'Size', 'XL', 'ForwardDynamicsCRB', 'ComputeMassMatrixAndForceTerms', 'AddExternalForces', 'Sinc', 'Cot', 'R3xSO3Matrix2RotationMatrix', 'R3xSO3Matrix2Translation', 'R3xSO3Matrix', 'ExpSO3', 'ExpS3', 'LogSO3', 'TExpSO3', 'TExpSO3Inv', 'ExpSE3', 'LogSE3', 'TExpSE3', 'TExpSE3Inv', 'ExpR3xSO3', 'TExpR3xSO3', 'TExpR3xSO3Inv', 'CompositionRuleDirectProductR3AndS3', 'CompositionRuleSemiDirectProductR3AndS3', 'CompositionRuleDirectProductR3AndR3RotVec', 'CompositionRuleSemiDirectProductR3AndR3RotVec', 'CompositionRuleDirectProductR3AndR3RotXYZAngles', 'CompositionRuleSemiDirectProductR3AndR3RotXYZAngles', 'CompositionRuleForEulerParameters', 'CompositionRuleForRotationVectors', 'CompositionRuleRotXYZAnglesRotationVector', 'StribeckFunction', 'RegularizedFrictionStep', 'RegularizedFriction', 'VonMisesStress', 'UFvonMisesStress', 'ParseOutputFileHeader', 'PlotSensorDefaults', 'PlotSensor', 'PlotFFT', 'FileStripSpaces', 'DataArrayFromSensorList', 'LoadImage', 'PlotImage', 'GetVersionPlatformString', 'ProcessParameterList', 'ParameterVariation', 'GeneticOptimization', 'Minimize', 'ComputeSensitivities', 'PlotOptimizationResults2D', 'PlotSensitivityResults', 'ComputeOrthonormalBasisVectors', 'ComputeOrthonormalBasis', 'GramSchmidt', 'Skew', 'Skew2Vec', 'ComputeSkewMatrix', 'EulerParameters2G', 'EulerParameters2GLocal', 'EulerParameters2RotationMatrix', 'RotationMatrix2EulerParameters', 'AngularVelocity2EulerParameters_t', 'RotationVector2RotationMatrix', 'RotationMatrix2RotationVector', 'ComputeRotationAxisFromRotationVector', 'RotationVector2G', 'RotationVector2GLocal', 'RotXYZ2RotationMatrix', 'RotationMatrix2RotXYZ', 'RotXYZ2G', 'RotXYZ2G_t', 'RotXYZ2GLocal', 'RotXYZ2GLocal_t', 'AngularVelocity2RotXYZ_t', 'RotXYZ2EulerParameters', 'RotationMatrix2RotZYZ', 'RotationMatrixX', 'RotationMatrixY', 'RotationMatrixZ', 'HomogeneousTransformation', 'HTtranslate', 'HTtranslateX', 'HTtranslateY', 'HTtranslateZ', 'HT0', 'HTrotateX', 'HTrotateY', 'HTrotateZ', 'HT2translation', 'HT2rotationMatrix', 'InverseHT', 'RotationX2T66', 'RotationY2T66', 'RotationZ2T66', 'Translation2T66', 'TranslationX2T66', 'TranslationY2T66', 'TranslationZ2T66', 'T66toRotationTranslation', 'InverseT66toRotationTranslation', 'RotationTranslation2T66', 'RotationTranslation2T66Inverse', 'T66toHT', 'HT2T66Inverse', 'InertiaTensor2Inertia6D', 'Inertia6D2InertiaTensor', 'GetRigidBodyNode', 'AddRigidBody', 'AddRevoluteJoint', 'AddPrismaticJoint', 'SetWithCOMinertia', 'Inertia', 'InertiaCOM', 'COM', 'Mass', 'Translated', 'Rotated', 'Transformed', 'GetInertia6D', 'StdDH2HT', 'ModDHKK2HT', 'projectAngleToPMPi', 'SetPDcontrol', 'HasPDcontrol', 'GetPDcontrol', 'AddLink', 'IsSerialRobot', 'GetLink', 'HasParent', 'GetParentIndex', 'NumberOfLinks', 'GetBaseHT', 'GetToolHT', 'LinkHT', 'JointHT', 'COMHT', 'StaticTorques', 'Jacobian', 'CreateKinematicTree', 'CreateRedundantCoordinateMBS', 'GetKinematicTree66', 'GetLinkGraphicsData', 'BuildFromDictionary', 'GetCurrentRobotHT', 'InterpolateHTs', 'SolveSafe', 'Solve', 'MakeCorkeRobot', 'ComputeIK3R', 'ComputeIKPuma560', 'ComputeIKUR', 'Manipulator4Rsimple', 'Manipulator3RSimple', 'ManipulatorPANDA', 'ManipulatorUR5', 'ManipulatorPuma560', 'LinkDict2Robot', 'LinkDictModDHKK2Robot', 'GetBasicProfile', 'GetFinalCoordinates', 'Add', 'GetTimes', 'Initialize', 'Evaluate', 'EvaluateCoordinate', 'VelocityManipulability', 'ForceManipulability', 'StiffnessManipulability', 'JointJacobian', 'MassMatrix', 'DynamicManipulability', 'calculateAllMeasures', 'FilterSensorOutput', 'FilterSignal', 'ComputeFFT', 'GetInterpolatedSignalValue', 'SolverErrorMessage', 'SolveStatic', 'SolveDynamic', 'ComputeLinearizedSystem', 'ComputeODE2Eigenvalues', 'CheckSolverInfoStatistics', 'ShowOnlyObjects', 'HighlightItem', '__UFsensorDistance', 'AddDistanceSensor', 'UFsensorRecord', 'AddSensorRecorder', 'LoadSolutionFile', 'NumpyInt8ArrayToString', 'BinaryReadIndex', 'BinaryReadReal', 'BinaryReadString', 'BinaryReadArrayIndex', 'BinaryReadRealVector', 'LoadBinarySolutionFile', 'RecoverSolutionFile', 'InitializeFromRestartFile', 'SetSolutionState', 'AnimateSolution', 'DrawSystemGraph', 'CreateTCPIPconnection', 'TCPIPsendReceive', 'CloseTCPIPconnection', ]

listPyClassNames=['OpenAIGymInterfaceEnv(Env)', 'MaterialBaseClass', 'KirchhoffMaterial(MaterialBaseClass)', 'FiniteElement', 'Tet4(FiniteElement)', 'ObjectFFRFinterface', 'ObjectFFRFreducedOrderInterface', 'HCBstaticModeSelection(Enum)', 'FEMinterface', 'InteractiveDialog', 'KinematicTree33', 'KinematicTree66', 'RigidBodyInertia', 'InertiaCuboid(RigidBodyInertia)', 'InertiaRodX(RigidBodyInertia)', 'InertiaMassPoint(RigidBodyInertia)', 'InertiaSphere(RigidBodyInertia)', 'InertiaHollowSphere(RigidBodyInertia)', 'InertiaCylinder(RigidBodyInertia)', 'VRobotLink', 'RobotLink', 'VRobotTool', 'RobotTool', 'VRobotBase', 'RobotBase', 'Robot', 'InverseKinematicsNumerical()', 'ProfileConstantAcceleration', 'ProfileLinearAccelerationsList', 'ProfilePTP', 'Trajectory', 'TCPIPdata', ]

