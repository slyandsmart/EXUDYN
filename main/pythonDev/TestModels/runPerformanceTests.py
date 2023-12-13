#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is the test suite which shall serve as a general driver to run:
#   - model unit tests
#   - test examples (e.g. from example folder)
#   - internal EXUDYN unit tests
#
# Author:   Johannes Gerstmayr
# Date:     2019-11-01
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
import sys, platform
import multiprocessing #for determining if on laptop or workstation

if sys.version_info.major != 3 or sys.version_info.minor < 6:# or sys.version_info.minor > 9:
    raise ImportError("EXUDYN only supports python versions >= 3.6")
isMacOS = (sys.platform == 'darwin')
isWindows = (sys.platform == 'win32')

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include right exudyn module now:
import numpy as np
if (sys.version_info.major == 3 and 
    #(sys.version_info.minor == 7 or sys.version_info.minor == 10)): #for these versions, we use exudynFast; since 2022-12-19/exudynV1.4.53: 3.7 and 3.10 in performance tests
    (sys.version_info.minor == 10)): #for these versions, we use exudynFast; since 2022-12-19/exudynV1.4.53: 3.7 and 3.10 in performance tests
    sys.exudynFast = True
    print('trying to use exudynFast')
else:
    sys.exudynFast = False

import exudyn as exu

if sys.exudynFast:
    import exudyn.exudynCPPfast as exuCPP #this is the cpp file, 
else:
    import exudyn.exudynCPP as exuCPP #this is the cpp file, 

from modelUnitTests import RunAllModelUnitTests, TestInterface, ExudynTestStructure, exudynTestGlobals
import time

psutilExists = False
try:
    import psutil #for cpu_percent
    psutilExists = True
except:
    print('*** WARNING: no psutils installed! Will NOT show CPU load during performance tests! ***')

SC = exu.SystemContainer()
mbs = SC.AddSystem()

#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#parse command line arguments:
# -quiet
writeToConsole = True  #do not output to console / shell
#copyLog = False         #copy log to final TestSuiteLogs
# if sys.version_info.major == 3 and sys.version_info.minor == 7:
#     copyLog = True #for P3.7 tests always copy log to WorkingRelease
if len(sys.argv) > 1:
    for i in range(len(sys.argv)-1):
        #print("arg", i+1, "=", sys.argv[i+1])
        if sys.argv[i+1] == '-quiet':
            writeToConsole = False
        # elif sys.argv[i+1] == '-copylog': #not needed any more
        #     copyLog = True
        else:
            print("ERROR in runPerformanceTests: unknown command line argument '"+sys.argv[i+1]+"'")

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#current date and time
def NumTo2digits(n):
    if n < 10:
        return '0'+str(n)
    return str(n)
    
import datetime # for current date
now=datetime.datetime.now()
dateStr = str(now.year) + '-' + NumTo2digits(now.month) + '-' + NumTo2digits(now.day) + ' ' + NumTo2digits(now.hour) + ':' + NumTo2digits(now.minute) + ':' + NumTo2digits(now.second)
#date and time of exudyn library:
import os #for retrieving file information
from datetime import datetime #datetime contains .fromtimestamp(...)

fileInfo=os.stat(exuCPP.__file__)
exuDate = datetime.fromtimestamp(fileInfo.st_mtime) 
exuDateStr = str(exuDate.year) + '-' + NumTo2digits(exuDate.month) + '-' + NumTo2digits(exuDate.day) + ' ' + NumTo2digits(exuDate.hour) + ':' + NumTo2digits(exuDate.minute) + ':' + NumTo2digits(exuDate.second)


#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
testTolerance = 1e-10

platformString = platform.architecture()[0]#'32bit'
platformString += 'P'+str(sys.version_info.major) +'.'+ str(sys.version_info.minor)
if isMacOS:
    platformString += 'MacOSX'
elif not isWindows: #add linux, to distinguish linux tests from windows tests!
    platformString += sys.platform

subFolder = ''
#only works on Windows: if platform.processor() == 'Intel64 Family 6 Model 142 Stepping 10, GenuineIntel':
if multiprocessing.cpu_count() == 20:
    subFolder = 'i7-1370P/' #this is for internal use only!
    
logFileName = '../PerformanceLogs/'+subFolder+'performanceLog_V'+exu.GetVersionString()+'_'+platformString+'.txt'
exu.SetWriteToFile(filename=logFileName, flagWriteToFile=True, flagAppend=False) #write all testSuite logs to files

exu.SetWriteToConsole(writeToConsole) #stop output from now on

exu.Print('\n+++++++++++++++++++++++++++++++++++++++++++')
exu.Print('+++++    EXUDYN PERFORMANCE TESTS     +++++')
exu.Print('+++++++++++++++++++++++++++++++++++++++++++')
exu.Print('EXUDYN version      = '+exu.GetVersionString())
exu.Print('EXUDYN build date   = '+exuDateStr)
exu.Print('platform            = '+platform.architecture()[0])
exu.Print('system              = '+sys.platform)

#Surface book 2 = 'Intel64 Family 6 Model 142 Stepping 10, GenuineIntel'
exu.Print('processor           = '+platform.processor()) 

exu.Print('python version      = '+str(sys.version_info.major)+'.'+str(sys.version_info.minor)+'.'+str(sys.version_info.micro))
exu.Print('test tolerance      = ',testTolerance)
exu.Print('test date (now)     = '+dateStr)
if psutilExists:
    exu.Print('CPU usage (%/thread)= '+str(psutil.cpu_percent(interval=1, percpu=True)))
    if psutil.sensors_battery()!=None:
        exu.Print('Power plugged       = '+str(psutil.sensors_battery().power_plugged))
exu.Print('+++++++++++++++++++++++++++++++++++++++++++')


#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
testFileList = [
                'generalContactSpheresTest.py',
                'perf3DRigidBodies.py',
                'perfObjectFFRFreducedOrder.py',
                'perfRigidPendulum.py',
                'perfSpringDamperExplicit.py',
                'perfSpringDamperUserFunction.py',
                ]


totalTests = len(testFileList)
testsFailed = [] #list of numbers containing the test numbers of failed tests
exudynTestGlobals.useGraphics = False
exudynTestGlobals.performTests = True
exudynTestGlobals.isPerformanceTest = True


#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#run general test examples
examplesTestSolList={}
examplesTestErrorList={}
invalidResult = 1234567890123456 #should not happen occasionally
totalTime = 0

from runTestSuiteRefSol import PerformanceTestsReferenceSolution
performanceTestRefSol = PerformanceTestsReferenceSolution()

testExamplesCnt = 0
for file in testFileList:
    name = file #.split('.')[0] #without '.py'
    exu.Print('\n\n****************************************************')
    exu.Print('  START PERFORMANCE TEST ' + str(testExamplesCnt) + ' ("' + file + '"):')
    exu.Print('****************************************************')
    SC.Reset()
    exudynTestGlobals.testError = -1 #default value !=-1, if there is an error in the calculation
    exudynTestGlobals.testResult = invalidResult #strange default value to see if there is a missing testResult
    timeStart= -time.time()
    exudynTestGlobals.testTolFact = 1 #special factor for some examples which make problems, e.g., due to sparse eigenvalue solver
    try:
        exec(open(file).read(), globals())
    except Exception as e:
        exu.Print('PERFORMANCE TEST ' + str(testExamplesCnt) + ' ("' + file + '") raised exception:\n'+str(e))
        print('PERFORMANCE TEST ' + str(testExamplesCnt) + ' ("' + file + '") raised exception:\n'+str(e))

    timeStart += time.time()
    totalTime += timeStart

    examplesTestErrorList[name] = exudynTestGlobals.testError
    examplesTestSolList[name] = exudynTestGlobals.testResult
    
    #compute error from reference solution
    exudynTestGlobals.testError = exudynTestGlobals.testResult - performanceTestRefSol[name]
    if abs(exudynTestGlobals.testError) < testTolerance*exudynTestGlobals.testTolFact:
        exu.Print('****************************************************')
        exu.Print('  PERFORMANCE TEST ' + str(testExamplesCnt) + ' ("' + file + '") FINISHED SUCCESSFUL')
    else:
        exu.Print('****************************************************')
        exu.Print('  PERFORMANCE TEST ' + str(testExamplesCnt) + ' ("' + file + '") *FAILED*')
        testsFailed = testsFailed + [testExamplesCnt]

    exu.Print('  RESULT   = ' + str(exudynTestGlobals.testResult))
    exu.Print('  ERROR    = ' + str(exudynTestGlobals.testError))
    exu.Print('  CPU TIME = ' + str(timeStart))
    exu.Print('****************************************************')
    
    testExamplesCnt += 1

exu.Print('\n')
exu.SetWriteToConsole(True) #final output always written
exu.SetWriteToFile(filename=logFileName, flagWriteToFile=True, flagAppend=True) #write also to file (needed?)

if psutilExists:
    exu.Print('CPU usage (%/thread)= '+str(psutil.cpu_percent(interval=1, percpu=True)))

exu.Print('****************************************************')
if len(testsFailed) == 0:
    exu.Print('ALL ' + str(totalTests) + ' PERFORMANCE TESTS SUCCESSFUL')
else:
    exu.Print(str(len(testsFailed)) + ' PERFORMANCE TEST(S) OUT OF '+ str(totalTests) + ' FAILED: ')
    for i in testsFailed:
        exu.Print('  PERFORMANCE TEST ' + str(i) + ' (' + testFileList[i] + ') FAILED')

exu.Print('TOTAL PERFORMANCE TEST TIME = ' + str(totalTime) + ' seconds')
#exu.Print('Reference value (i9)        = 88.12 seconds (32bit) / 74.11 seconds (regular) / 57.30 seconds (exudynFast)')
exu.Print('Reference value (i9, 2023-12, Windows)= 48 - 51 seconds (regular) / 39.5 seconds  (exudynFast)')
exu.Print('Reference value (i9, 2023-12, Linux  )= 42 - 44 seconds (regular) / 34 - 36 seconds (exudynFast)')
exu.Print('****************************************************')

    
exu.SetWriteToFile(filename='', flagWriteToFile=False, flagAppend=False) #stop writing to file, close file


