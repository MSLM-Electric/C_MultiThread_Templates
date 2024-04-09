C templates for simulate, tests, experiments and implementing multi threading.

You can launch the console app to see what happens on this example application. To do it: go to the folder "C_MultiThread_tests_and_templates/Debug/" and open "/C_MultiThread_tests_and_templates.exe"
Download this demo "*.exe" by clicking "Download raw file" and run it.


Projects structure (folders & files)

Currently working at:
- ROOT_PATH (Root folder)
	|- Lib
	|   |- SimpleTimerWP.c
	|   |- SimpleTimerWP.h
	|   |- fileHandle.c
	|   |- fileHandle.h
	|
	|- ProtocolDevelopingTest
	|   |- ProtocolMasterImmitationDevelopeTest_main.c
	|   |- HardwarePeripheral
	|   |   |- HardwareInterfaceUnit.c
	|   |   |- HardwareInterfaceUnit.h
	|   |
	|   |- IO_immitationBetweenMasterSlave
	|   |   |- iofile.txt
	|   |	|- MasterImmitationCfg.h
	|   |
	|   |- ProtocolMasterDevelopeTest
	|       |- CMakeLists.txt   <-- (here our project)
	|
	|- ConsoleMenuOptions.c
	|- ConsoleMenuOptions.h
	|- MultiThreadSupport.c
	|- MultiThreadSupport.h