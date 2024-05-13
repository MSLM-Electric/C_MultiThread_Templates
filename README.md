C templates for simulate, tests, experiments and implementing multi threading.

You can launch the console app to see what happens on this example application. To do it: go to the folder "C_MultiThread_tests_and_templates/Debug/" and open "/C_MultiThread_tests_and_templates.exe"
Download this demo "*.exe" by clicking "Download raw file" and run it.


Projects structure (folders & files)

Currently working at: ProtocolDevelopTemplate Project
```
- ROOT_PATH (Root folder)
   |- Lib
   |   |- SimpleTimerWP.c
   |   |- SimpleTimerWP.h
   |   |- fileHandle.c
   |   |- fileHandle.h
   |   |- type_def.h
   |
   |- ProtocolDevelopingTest
   |   |- ProtocolMasterImmitationDevelopeTest_main.c
   |   |- globDefines.cmake
   |   |- HardwarePeripheral
   |   |   |- HardwareInterfaceUnit.c
   |   |   |- HardwareInterfaceUnit.h
   |   |
   |   |- IO_immitationBetweenMasterSlave
   |   |   |- iofile.txt
   |   |   |- globMutexHold.txt
   |   |   |- MasterImmitationCfg.h
   |   |   |- PortsBusMessages.h
   |   |
   |   |- ProtocolMasterDevelopeTest
   |       |- CMakeLists.txt   <-- (here our project)
   |       |- README.md        <-- (instructions to build and run)
   |
   |- ConsoleMenuOptions.c
   |- ConsoleMenuOptions.h
   |- MultiThreadSupport.c
   |- MultiThreadSupport.h
```