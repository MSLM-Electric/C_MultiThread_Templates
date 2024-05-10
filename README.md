C templates for simulate, tests, experiments and implementing multi threading.

You can launch the console app to see what happens on this example application. To do it: go to the folder "C_MultiThread_tests_and_templates/Debug/" and open "/C_MultiThread_tests_and_templates.exe"
Download this demo "*.exe" by clicking "Download raw file" and run it.


Projects structure (folders & files)

Currently working at: ProtocolDevelopTemplate Project
<br />- ROOT_PATH (Root folder)
<br />   |- Lib
<br />   |   |- SimpleTimerWP.c
<br />   |   |- SimpleTimerWP.h
<br />   |   |- fileHandle.c
<br />   |   |- fileHandle.h
<br />   |   |- type_def.h
<br />   |
<br />   |- ProtocolDevelopingTest
<br />   |   |- ProtocolMasterImmitationDevelopeTest_main.c
<br />   |   |- globDefines.cmake
<br />   |   |- HardwarePeripheral
<br />   |   |   |- HardwareInterfaceUnit.c
<br />   |   |   |- HardwareInterfaceUnit.h
<br />   |   |
<br />   |   |- IO_immitationBetweenMasterSlave
<br />   |   |   |- iofile.txt
<br />   |   |   |- globMutexHold.txt
<br />   |   |   |- MasterImmitationCfg.h
<br />   |   |   |- PortsBusMessages.h
<br />   |   |
<br />   |   |- ProtocolMasterDevelopeTest
<br />   |       |- CMakeLists.txt   <-- (here our project)
<br />   |       |- README.md        <-- (instructions to build and run)
<br />   |
<br />   |- ConsoleMenuOptions.c
<br />   |- ConsoleMenuOptions.h
<br />   |- MultiThreadSupport.c
<br />   |- MultiThreadSupport.h