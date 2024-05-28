C templates for simulate, tests, experiments and implementing multi threading.

You can launch the console app to see what happens on this example application. To do it: go to the folder "C_MultiThread_tests_and_templates/Debug/" and open "/C_MultiThread_tests_and_templates.exe"
Download this demo "*.exe" by clicking "Download raw file" and run it.


Projects structure (folders & files)

Currently working at: ProtocolDevelopTemplate Project
```
- ROOT_PATH (Root folder)  for Master project
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
   |   |   |- CmdsSceanarioScripts.c
   |   |   |- CmdsSceanarioScripts.c
   |   |
   |   |- ProtocolMasterDevelopeTest
   |       |- CMakeLists.txt   <-- (here our Master project)
   |       |- README.md        <-- (instructions to build and run)
   |
   |- ConsoleMenuOptions.c  (independent lib)
   |- ConsoleMenuOptions.h  (independent lib)
   |- MultiThreadSupport.c  (ind)
   |- MultiThreadSupport.h
   
   
   The Slave Project looks almost the same:
- ROOT_PATH (Root folder)  for Slave project
   |- Lib
   |   |- ...
   |   |- ...
   |
   |
   |- ProtocolDevelopingTest
   |   |- ProtocolSlaveImmitationDevelopeTest_main.c
   |   |- globDefines.cmake
   |   |- HardwarePeripheral ...
   |   |
   |   |- IO_immitationBetweenMasterSlave
   |   |   |- iofile.txt ...
   |   |   |- SlaveImmitationCfg.h
   |   |   |- ...
   |   |
   |   |- ProtocolSlaveDevelopeTest
   |       |- CMakeLists.txt   <-- (here our Slave project)
   |       |- README.md        <-- (instructions to build and run)
   |
   |- ...
   |- ...
```