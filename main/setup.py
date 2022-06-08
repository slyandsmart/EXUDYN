# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++# setup.py file for EXUDYN; generated by Johannes Gerstmayr and based on# template provided provided in pybind11# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import os
import sys
import platform
import setuptools
import time

#from src.pythonGenerator.exudynVersion import exudynVersionString #does not run under MacOS
file='src/pythonGenerator/exudynVersion.py'
exec(open(file).read(), globals())


#os.environ["CC"] = "gcc-8" #use gcc-8.4 on linux; does not work on windows
#os.environ["CXX"] = "gcc-8"

startTime = time.time()
#print('setup.py: START time:', startTime)

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#check GLFW
USEGLFW = True
if '--noglfw' in sys.argv:
    USEGLFW = False
    print("setup.py: *** compiling without graphics (OpenGL and GLFW) ***")
    sys.argv.remove('--noglfw')

compileParallel = False
if '--parallel' in sys.argv:
    compileParallel = True
    print("setup.py: *** trying to compile in parallel ***")
    print("          in case that parallel compile fails, remove the --parallel option")
    sys.argv.remove('--parallel')

compileExudynFast = True #but only for selected Python versions
if '--nofast' in sys.argv:
    compileExudynFast = False
    print("setup.py: *** no exudynCPPfast created for this Python version ***")
    sys.argv.remove('--nofast')

myIncludeDirs=[]
msvcGLFWlibs =[] #add only if flag set
unixGLFWlibs = [] #add only if flag set
msvcCppGLFWflag = ['/D', '__NOGLFW'] #indicates, that no GLFW shall be used
unixCppGLFWflag = ['-D__NOGLFW'] #indicates, that no GLFW shall be used

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#check system and platform
isWindows = False
isLinux = False
isMacOS = False

#detect the platform used during running the setup tool:
if platform.system() == 'Windows':
    isWindows = True
    print("platform == Windows")
if platform.system() == 'Linux':
    isLinux = True
    print("platform == Linux")

if sys.platform == 'darwin':
    isMacOS = True
    print("platform == MacOS")
    #platform.architecture() returns '64bit'
    #platform.processor() returns 'arm' in M1 mode and 'i386' under rosetta 2 (Intel mode)

is64bits = False
is32bits = False
if platform.architecture()[0] == '64bit':
    is64bits = True
else:
    is32bits = True

addLibrary_dirs = []
#find whether 32 or 64 bits are used
if isWindows:
    if is32bits:
        addLibrary_dirs=['libs/libs32' ]
        print("architecture==32bits")
    else:
        addLibrary_dirs=['libs/libs64' ]
        print("architecture==64bits")

if isMacOS:
    addLibrary_dirs=['libs/libsmacos' ] #should contain glfw-libraries

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
print("python version =",platform.python_version())

#detect python version:
pyVersionString = str(sys.version_info.major) + '.' + str(sys.version_info.minor)

#create macro variable for C++ compilation, knowing Python version
exudynPythonMacro = '__EXUDYN__PYTHON'+str(sys.version_info.major)+str(sys.version_info.minor)

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#set some platform-specific linker and include options
if USEGLFW:
    msvcCppGLFWflag = [] #GLFW will be used
    unixCppGLFWflag = [] #GLFW will be used

    myIncludeDirs=["include/glfw/deps", "include/glfw",]
    msvcGLFWlibs = ['opengl32.lib', 'glfw3.lib'] #opengl32.lib: not needed since VS2015?
                    
    if isMacOS:
        unixGLFWlibs = ['-lglfw3', #GLFW static library
                        '-framework Cocoa',
                        '-framework OpenGL', #do not include openGL lib, but the framework; see https://www.glfw.org/docs/3.3.1/build_guide.html
                        '-framework IOKit'
                        ] #OpenGL
    else:
        #unix: for graphics; libs (*.so) need to be installed on your linux system -> see setupToolsHowTo.txt:
        unixGLFWlibs = ['-lglfw', #GLFW
                        '-lGL'] #OpenGL



#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#check EXUDYN version
__version__ = exudynVersionString
print('build Exudyn version',exudynVersionString)


#++++++++++++++++++++++++++++++++++++++++++



class get_pybind_include(object):
    def __str__(self):
        import pybind11
        return pybind11.get_include()


cppFiles = [
                 'src/Autogenerated/versionCpp.cpp',
                 'src/Graphics/GlfwClient.cpp',
                 'src/Graphics/GlfwClientExtended.cpp',
                 'src/Graphics/PostProcessData.cpp',
                 'src/Graphics/VisualizationPrimitives.cpp',
                 'src/Graphics/VisualizationSystem.cpp',
                 'src/Graphics/VisualizationSystemContainer.cpp',
                 'src/Graphics/VisualizationSystemData.cpp',
                 'src/Graphics/VisualizationUserFunctions.cpp',
                 'src/Linalg/LinearSolver.cpp',
                 'src/Linalg/Matrix.cpp',
                 'src/Linalg/Vector.cpp',
                 'src/Main/CSystem.cpp',
                 'src/Main/MainObjectFactory.cpp',
                 'src/Main/MainSystem.cpp',
                 'src/Main/MainSystemContainer.cpp',
                 'src/Main/rendererPythonInterface.cpp',
                 'src/Main/Stdoutput.cpp',
                 'src/Objects/checkPreAssembleConsistencies.cpp',
                 'src/Objects/CMarkerBodyCable2DCoordinates.cpp',
                 'src/Objects/CMarkerBodyCable2DShape.cpp',
                 'src/Objects/CMarkerBodyMass.cpp',
                 'src/Objects/CMarkerBodyPosition.cpp',
                 'src/Objects/CMarkerBodyRigid.cpp',
                 'src/Objects/CMarkerKinematicTreeRigid.cpp',
                 'src/Objects/CMarkerNodeCoordinate.cpp',
                 'src/Objects/CMarkerNodeCoordinates.cpp',
                 'src/Objects/CMarkerNodeODE1Coordinate.cpp',
                 'src/Objects/CMarkerNodePosition.cpp',
                 'src/Objects/CMarkerNodeRigid.cpp',
                 'src/Objects/CMarkerNodeRotationCoordinate.cpp',
                 'src/Objects/CMarkerObjectODE2Coordinates.cpp',
                 'src/Objects/CMarkerSuperElementPosition.cpp',
                 'src/Objects/CMarkerSuperElementRigid.cpp',
                 'src/Objects/CNode1D.cpp',
                 'src/Objects/CNodeGenericAE.cpp',
                 'src/Objects/CNodeGenericData.cpp',
                 'src/Objects/CNodeGenericODE1.cpp',
                 'src/Objects/CNodeGenericODE2.cpp',
                 'src/Objects/CNodePoint.cpp',
                 'src/Objects/CNodePoint2D.cpp',
                 'src/Objects/CNodePoint2DSlope1.cpp',
                 'src/Objects/CNodePoint3DSlope1.cpp',
                 'src/Objects/CNodePoint3DSlope23.cpp',
                 'src/Objects/CNodePointGround.cpp',
                 'src/Objects/CNodeRigidBody2D.cpp',
                 'src/Objects/CNodeRigidBodyEP.cpp',
                 'src/Objects/CNodeRigidBodyRotVecLG.cpp',
                 'src/Objects/CNodeRigidBodyRxyz.cpp',
                 'src/Objects/CObjectALEANCFCable2D.cpp',
                 'src/Objects/CObjectANCFBeam3D.cpp',
                 'src/Objects/CObjectANCFCable2D.cpp',
                 'src/Objects/CObjectBeamGeometricallyExact2D.cpp',
                 'src/Objects/CObjectBeamGeometricallyExact3D.cpp',
                 'src/Objects/CObjectConnectorCartesianSpringDamper.cpp',
                 'src/Objects/CObjectConnectorCoordinate.cpp',
                 'src/Objects/CObjectConnectorCoordinateSpringDamper.cpp',
                 'src/Objects/CObjectConnectorCoordinateVector.cpp',
                 'src/Objects/CObjectConnectorDistance.cpp',
                 'src/Objects/CObjectConnectorGravity.cpp',
                 'src/Objects/CObjectConnectorLinearSpringDamper.cpp',
                 'src/Objects/CObjectConnectorRigidBodySpringDamper.cpp',
                 'src/Objects/CObjectConnectorRollingDiscPenalty.cpp',
                 'src/Objects/CObjectConnectorSpringDamper.cpp',
                 'src/Objects/CObjectConnectorTorsionalSpringDamper.cpp',
                 'src/Objects/CObjectContactCircleCable2D.cpp',
                 'src/Objects/CObjectContactConvexRoll.cpp',
                 'src/Objects/CObjectContactCoordinate.cpp',
                 'src/Objects/CObjectContactFrictionCircleCable2D.cpp',
                 'src/Objects/CObjectContactFrictionCircleCable2DOld.cpp',
                 'src/Objects/CObjectFFRF.cpp',
                 'src/Objects/CObjectFFRFreducedOrder.cpp',
                 'src/Objects/CObjectGenericODE1.cpp',
                 'src/Objects/CObjectGenericODE2.cpp',
                 'src/Objects/CObjectGround.cpp',
                 'src/Objects/CObjectJointALEMoving2D.cpp',
                 'src/Objects/CObjectJointGeneric.cpp',
                 'src/Objects/CObjectJointPrismatic2D.cpp',
                 'src/Objects/CObjectJointPrismaticX.cpp',
                 'src/Objects/CObjectJointRevolute2D.cpp',
                 'src/Objects/CObjectJointRevoluteZ.cpp',
                 'src/Objects/CObjectJointRollingDisc.cpp',
                 'src/Objects/CObjectJointSliding2D.cpp',
                 'src/Objects/CObjectJointSpherical.cpp',
                 'src/Objects/CObjectKinematicTree.cpp',
                 'src/Objects/CObjectMass1D.cpp',
                 'src/Objects/CObjectMassPoint.cpp',
                 'src/Objects/CObjectMassPoint2D.cpp',
                 'src/Objects/CObjectRigidBody.cpp',
                 'src/Objects/CObjectRigidBody2D.cpp',
                 'src/Objects/CObjectRotationalMass1D.cpp',
                 'src/Objects/evaluateUserFunctions.cpp',
                 'src/Objects/VisuNodePoint.cpp',
                 'src/Pymodules/PybindModule.cpp',
                 'src/Pymodules/PyMatrixContainer.cpp',
                 'src/Pymodules/pythonTests.cpp',
                 'src/Solver/CSolver.cpp',
                 'src/Solver/CSolverBase.cpp',
                 'src/Solver/CSolverExplicit.cpp',
                 'src/Solver/CSolverImplicitSecondOrder.cpp',
                 'src/Solver/CSolverStatic.cpp',
                 'src/Solver/MainSolver.cpp',
                 'src/Solver/MainSolverBase.cpp',
                 'src/System/CContact.cpp',
                 'src/System/CLoad.cpp',
                 'src/System/CNode.cpp',
                 'src/System/CObjectBody.cpp',
                 'src/System/CObjectConnector.cpp',
                 'src/System/CSensor.cpp',
                 'src/System/MainNode.cpp',
                 'src/System/MainObject.cpp',
                 'src/Tests/UnitTestBase.cpp',
                 'src/Utilities/BasicFunctions.cpp',
                 'include/ngs-core-master/bitarray.cpp',
                 'include/ngs-core-master/exception.cpp',
                 'include/ngs-core-master/localheap.cpp',
                 'include/ngs-core-master/paje_interface.cpp',
                 'include/ngs-core-master/profiler.cpp',
                 'include/ngs-core-master/table.cpp',
                 'include/ngs-core-master/taskmanager.cpp',

    ]

#some specific include directories for exudyn:
myIncludeDirs += [
        			"src",
        			"include",
        			"include/pybind11", #already includes everything that is needed
        			"include/lest",
                 ]

ext_modules = [
    Extension(
        'exudyn.exudynCPP', #this is the regular C++ library with range checks inside the exudyn package
        # Alternative, but not needed, as .cpp file list is generated automatically: sort input source files to ensure bit-for-bit reproducible builds: sorted([...])
        cppFiles,
        include_dirs=[get_pybind_include(), #does some magic for Pybind11
                     ]+myIncludeDirs,
        library_dirs = addLibrary_dirs,
		define_macros=[],
        language='c++'
    ),
]
if compileExudynFast and (pyVersionString == '3.7' or pyVersionString == '3.8'):
    print('***  preparing C++ module also for __FAST_EXUDYN_LINALG  ***')
    ext_modules += [
        Extension(
            'exudyn.exudynCPPfast', #this is the fast C++ library without range checks and try/catch inside the exudyn package
            cppFiles,
            include_dirs=[get_pybind_include(), #does some magic for Pybind11
                         ]+myIncludeDirs,
            library_dirs = addLibrary_dirs,
		    define_macros=[('__FAST_EXUDYN_LINALG', '')],
            language='c++'
        ),
        ]

# cf http://bugs.python.org/issue26689
def has_flag(compiler, flagname):
#    Return a boolean indicating whether a flag name is supported on
#    the specified compiler.
    import tempfile
    import os
    with tempfile.NamedTemporaryFile('w', suffix='.cpp', delete=False) as f:
        f.write('int main (int argc, char **argv) { return 0; }')
        fname = f.name
    try:
        compiler.compile([fname], extra_postargs=[flagname])
    except setuptools.distutils.errors.CompileError:
        return False
    finally:
        try:
            os.remove(fname)
        except OSError:
            pass
    return True


def cpp_flag(compiler):
#    Return the -std=c++[11/14/17] compiler flag.
#    The newer version is prefered over c++11 (when it is available).
    flags = ['-std=c++17', '-std=c++14', '-std=c++11']
    # only needed for older MacOS: (ElCapitan):
    # if isMacOS: #C++ code will work without C++17
    #     flags = ['-std=c++14', '-std=c++11']

    for flag in flags:
        if has_flag(compiler, flag):
            return flag

    raise RuntimeError('Unsupported compiler -- at least C++11 support '
                       'is needed!')


class BuildExt(build_ext):
#    A custom build extension for adding compiler-specific options.
    #options used for all builds:
    allMacros = ['EXUDYN_RELEASE'] #exclude experimental parts

    #changed with #1116
    # if isWindows and pyVersionString == '3.8':
    #     allMacros += ['__FAST_EXUDYN_LINALG'] #exclude range checks for Python 3.8 version
    #     print('**********************************************************')
    #     print('*  compiling __FAST_EXUDYN_LINALG in Python 3.8 version  *')
    #     print('**********************************************************')
    
    
    commonCopts = []
    for macroString in allMacros:
        if isWindows:
            commonCopts += ['/D', macroString]
        else: #works same for MacOS and linux
            commonCopts += ['-D'+macroString]
    
    #platform-specific s_opts:
    c_opts = {
        #'msvc': ['/EHsc'],
        'msvc': [
                '/EHsc',
                '/cgthreads8', #specify number of threads for cl.exe, affects mainly linker time
				'/permissive-',
				'/MP', '/GS', '/Qpar',
				'/GL', '/W3', '/Gy', 
				'/Zc:wchar_t',
				'/Gm-', 
				'/O2', 
				'/sdl',
				'/Zc:inline',
				'/fp:precise',
			    '/D', '_MBCS', 
				'/D', '_WINDLL',
				'/D','_CRT_SECURE_NO_WARNINGS', #/D and _CRT_SECURE_NO_WARNINGS must be consecutive==>WORKS!
				'/errorReport:prompt', 
				'/WX-', 
				'/Zc:forScope',
				#'/arch:AVX2', #add only if not 32bits
				'/Gd', '/Oy', '/Oi', 
				#'/MD', #/MT=multithreaded, /MD=multithreaded DLL, overwirtes /MT
				'/openmp',
				'/std:c++17',
				'/FC',
                '/Ot', #favor faster code
				'/Zc:twoPhase-',
                '/D', exudynPythonMacro,
            ]+msvcCppGLFWflag+commonCopts,
        'unix': [
         '-Wno-comment', #deactivate multiline comment warning /* ... * * ...*/
         '-Wno-unknown-pragmas', #warning from ngs_core.hpp/taskmanager.hpp (NGsolve)
         '-Wno-sign-compare', #warning from taskmanager.hpp (NGsolve)
 		 '-Wall',
         '-D'+exudynPythonMacro,
         #'-std=c++17', #==>chosen automatic
         #'-fpermissive', #because of exceptions ==> allows compilation
         #'-fopenmp',
 		 #'-O3', #takes long ...
 		 #'-shared',
 		 #'-fPIC',

#		#working:
#		'-Wno-non-template-friend', #deactivate warning for several vector/array templates
#		'-Wno-comment', #deactivate multiline comment warning /* ... * * ...*/
#		'-Wall',
        ]+unixCppGLFWflag+commonCopts,
    }
    #if not is32bits: #for 32bits, we assume that processors may not support avx
    if not (sys.version_info.major == 3 and sys.version_info.minor == 6):  #since V1.2.29
        c_opts['msvc'] += ['/arch:AVX2']

    #perform C++ unit tests: for 64bits, Python 3.6
    #if not is32bits and sys.version_info.major == 3 and sys.version_info.minor == 6: 
    if sys.version_info.major == 3 and sys.version_info.minor == 7: #since V1.2.29
        print('***************************\nadd flag PERFORM_UNIT_TESTS\n***************************\n')
        c_opts['msvc'] += ['/D', 'PERFORM_UNIT_TESTS']
	
    l_opts = {
        'msvc': [
        'kernel32.lib',
        'user32.lib',
        'gdi32.lib',
        'shell32.lib',
        #additional libs that may be needed in future:
        #"winspool.lib" "comdlg32.lib" "advapi32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib"        
        ]+msvcGLFWlibs,
        'unix': unixGLFWlibs,
    }

    if sys.platform == 'darwin':
        # darwin_opts = ['-stdlib=libc++', '-mmacosx-version-min=10.7']  #without c++17 support
        # darwin_opts += ['-Wno-unused-variable','-Wno-missing-braces','-Wno-return-stack-address']
        darwin_opts = ['-stdlib=libc++', '-mmacosx-version-min=11.0'] #for c++17 support (not working on older MacOSX)
        c_opts['unix'] += darwin_opts + ['-Wno-inconsistent-missing-override', 
        '-Wno-overloaded-virtual', #avoid too many warnings
        '-Wno-deprecated-declarations', #avoid too many OpenGL warnings on Big Sur
        '-Wno-unreachable-code', #avoid too many warnings on Big Sur
        '-Wno-unused-variable', #avoid too many warnings on Big Sur
        ]+commonCopts
        l_opts['unix'] += darwin_opts
    else:
        l_opts['unix'] += [
            '-lgomp', #for openmp ==> needed for omp_get_num_threads()
            '-lstdc++fs', #for autocreate directories, using std::filesystem from c++17 std
            ]        
        #warnings not available in clang:
        c_opts['unix'] += [
        '-Wno-non-template-friend', #deactivate warning for several vector/array templates
        '-Wno-class-memaccess', #warning in SearchTree Box3D; BUT: does not work on older gcc; avoid warnings on gcc-8 regarding memory access in class
        ]

    def build_extensions(self):
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        link_opts = self.l_opts.get(ct, [])
        if ct == 'unix':
            opts.append(cpp_flag(self.compiler))
            if has_flag(self.compiler, '-fvisibility=hidden'):
                opts.append('-fvisibility=hidden')

        for ext in self.extensions:
            ext.define_macros = ext.define_macros + [('VERSION_INFO', '"{}"'.format(self.distribution.get_version()))]
            ext.extra_compile_args = opts
            ext.extra_link_args = link_opts
        build_ext.build_extensions(self)

#+++++++++++++++++++++++++++++++++++++++++++++++++++++
#parallel compile, works for linux:
# http://stackoverflow.com/a/13176803
# monkey-patch for parallel compilation
# reduces compile time on 4-core machine by factor >2
if compileParallel:
    try:
        import multiprocessing
        import multiprocessing.pool
        if isLinux:
            def parallelCCompile(self, sources, output_dir=None, macros=None,
                    include_dirs=None, debug=0, extra_preargs=None, extra_postargs=None,
                    depends=None):
                print('************ PARALLEL COMPILE ************\n')
                # those lines are copied from distutils.ccompiler.CCompiler directly
                macros, objects, extra_postargs, pp_opts, build =                         self._setup_compile(output_dir, macros, include_dirs, sources,
                                depends, extra_postargs)
                objects.reverse() #puts the heavier files on top (better performance for parallel compile)
                cc_args = self._get_cc_args(pp_opts, debug, extra_preargs)
                # parallel code
                def _single_compile(obj):
                    try: src, ext = build[obj]
                    except KeyError: return
                    self._compile(obj, src, ext, cc_args, extra_postargs, pp_opts)
                # convert to list, imap is evaluated on-demand
                N_cores = multiprocessing.cpu_count() #better to use all threads, not only cpus
                list(multiprocessing.pool.ThreadPool(N_cores).imap(_single_compile,objects))
                return objects
        
            import distutils.ccompiler
            distutils.ccompiler.CCompiler.compile=parallelCCompile
    
        elif isWindows:
            def parallelCCompile(self, sources, output_dir=None, macros=None,
                    include_dirs=None, debug=0, extra_preargs=None, extra_postargs=None,
                    depends=None):
                print('************ PARALLEL COMPILE ************\n')
                #*** copied from _msvccompiler, excluding special cases for .rc and .mc files
                if not self.initialized:
                    self.initialize()
                compile_info = self._setup_compile(output_dir, macros, include_dirs,
                                                   sources, depends, extra_postargs)
                macros, objects, extra_postargs, pp_opts, build = compile_info
            
                objects.reverse() #puts the heavier files on top (better performance for parallel compile)
                compile_opts = extra_preargs or []
                compile_opts.append('/c')
                if debug:
                    compile_opts.extend(self.compile_options_debug)
                else:
                    compile_opts.extend(self.compile_options)

                #this function is called in a for loop in the original version:                    
                def _single_compile(obj):
                    add_cpp_opts = False
                    try:
                        src, ext = build[obj]
                    except KeyError:
                        return
                    if debug:
                        # pass the full pathname to MSVC in debug mode,
                        # this allows the debugger to find the source file
                        # without asking the user to browse for it
                        src = os.path.abspath(src)
            
                    if ext in self._c_extensions:
                        input_opt = "/Tc" + src
                    elif ext in self._cpp_extensions:
                        input_opt = "/Tp" + src
                        add_cpp_opts = True
                    else:
                        # how to handle this file?
                        raise ValueError("Don't know how to compile {} to {}"
                                           .format(src, obj))
            
                    args = [self.cc] + compile_opts + pp_opts
                    if add_cpp_opts:
                        args.append('/EHsc')
                    args.append(input_opt)
                    args.append("/Fo" + obj)
                    args.extend(extra_postargs)
            
                    try:
                        self.spawn(args)
                    except:
                        raise ValueError('failed to exucute: '+str(args))
            
                N_cores = multiprocessing.cpu_count() #better to use all threads, not only cpus
                # convert to list, imap is evaluated on-demand
                list(multiprocessing.pool.ThreadPool(N_cores).imap(_single_compile,objects))
    
                #serial:
                # for obj in objects:
                #     _single_compile(obj)
            
                return objects
            #*** end copy from _msvccompiler
            
            import distutils._msvccompiler 
            distutils._msvccompiler.MSVCCompiler.compile = parallelCCompile #for newer MSVC, the same way as with linux does not work!
            #distutils.msvc9compiler.MSVCCompiler.compile = parallelCCompile
            #distutils.msvccompiler.MSVCCompiler.compile = parallelCCompile
    except:
        print('************************************************')
        print('parallel compile FAILED; remove --parallel flag!')
        print('... trying serial compilation')
        print('************************************************')

#+++++++++++++++++++++++++++++++++++++++++++++++++++++



if exudynVersionString.find('.dev1') == -1:
    developmentStatus = "Development Status :: 5 - Production/Stable"
else:
    developmentStatus = "Development Status :: 4 - Beta"

long_description=''
long_description += '==========\n'
long_description += '**Exudyn**\n'
long_description += '==========\n\n'
long_description += 'A flexible multibody dynamics systems simulation code with Python and C++\n\n'
long_description += 'Exudyn is hosted on `Github <https://github.com/jgerstmayr/EXUDYN>`_ which provides full documentation, tutorial, examples, etc.\n\n'
long_description += 'See `License on github <https://github.com/jgerstmayr/EXUDYN/blob/master/LICENSE.txt>`_ .\n'
long_description += 'Pre-compiled available for Windows / Python 3.6 - 3.10.\n\n'
long_description += 'For more information, installation and tutorials see: \n\n'
long_description += 'https://github.com/jgerstmayr/EXUDYN \n\n'
long_description += 'For CHANGES (section Issues and Bugs), detailed DOCUMENTATION on theory, usage, and REFERENCE MANUAL on **600+** pages: \n\n'
long_description += 'https://github.com/jgerstmayr/EXUDYN/tree/master/docs/theDoc/theDoc.pdf\n\n'

##this would work, but pypi does not read .rst files as properly as github does ...:
# long_description = 'Exudyn is hosted on `Github <https://github.com/jgerstmayr/EXUDYN>`_ which provides full documentation, tutorial, examples, etc.\n'
# long_description+= 'See `License on github <https://github.com/jgerstmayr/EXUDYN/blob/master/LICENSE.txt>`_ .\n'
# long_description+= 'Pre-compiled available for Windows / Python 3.6 - 3.9.\n'
# long_description+= 'The following description is copied from github, figures are missing!\n\n'
# with open("../README.rst", "r") as fh:
#     long_description += fh.read()

    
setup(
    name='exudyn',
    version=__version__,
    author='Johannes Gerstmayr',
    author_email='reply.exudyn@gmail.com',
    url='https://github.com/jgerstmayr/EXUDYN',
    description='EXUDYN flexible multibody dynamics simulation in C++ and Python',
    long_description = long_description,
    long_description_content_type='text/x-rst',
    #long_description_content_type='text/markdown', #standard is already text/x-rst
    package_dir={'':'pythonDev'},   #only add packages from that dir; must include a __init__.py file
    packages=['exudyn','exudyn/robotics'],            #adds all python files (=modules) in directories with __init__.py file; this is a subdirectory to the directory provided in package_dir
    #package_data={'tests': ['pythonDev/TestModels/*.py'],}, #include additional data, but do we want that?
    include_package_data=True, #not sure, if this is necessary!
    ext_modules=ext_modules,
    setup_requires=['pybind11==2.6.0'], #replaced previous require>=2.5.0, because compilation with VS2017 fails with pybind11 2.7.0 version of 2021-10-04: setup_requires=['pybind11>=2.5.0'],
    cmdclass={'build_ext': BuildExt},
    zip_safe=False,
    license = 'BSD',
    platforms='any', #2022-03-20: used in numpy - may improve behavior in pypi.org?
    classifiers=[
        developmentStatus,
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: BSD License",
        #"Operating System :: Microsoft :: Windows :: Windows 10",
        "Operating System :: Microsoft :: Windows", #allow Windows 11
        "Operating System :: POSIX :: Linux",
        "Operating System :: MacOS",
        "Topic :: Scientific/Engineering",
    ],
    #OLD: '==' makes problems with pypi? python_requires='=='+pyVersionString+'.*', #'.*' required on UBUNTU/Windows in order to accept any Python minor Version (e.g. 3.6.x) during installation
    #python_requires='>='+pyVersionString, #'.*' required on UBUNTU/Windows in order to accept any Python minor Version (e.g. 3.6.x) during installation
    python_requires='>=3.6', #for pypi.org, do only specify the minimum Python version which is needed for this exudyn version
)

#print('*** setup.py: END time:', time.time())
print('*** setup.py: DURATION =', round(time.time()-startTime,2), 'seconds')

