cmake_minimum_required(VERSION 3.12)

project(ProtocolMasterDevelopeTest)

#set(ROOT_PATH D:/Osim/C_n_C++projs/C_MultiThread_Templates)
set(ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../")
#on Lib located inc and src 
set(LIB_DIR ${ROOT_PATH}/Lib)
#inc and src located together on root path
set(ROOT_SRC_DIR ${ROOT_PATH})
set(HARDWARE_PERIPHERAL ${ROOT_PATH}/ProtocolDevelopingTest/HardwarePeripheral)
set(PROTOCOL_MASTER_MAIN ${ROOT_PATH}/ProtocolDevelopingTest)

set(SOURCES
	${PROTOCOL_MASTER_MAIN}/ProtocolMasterImmitationDevelopeTest_main.c 
	${ROOT_SRC_DIR}/ConsoleMenuOptions.c
	${ROOT_SRC_DIR}/MultiThreadSupport.c
	${LIB_DIR}/SimpleTimerWP.c
	${HARDWARE_PERIPHERAL}/HardwareInterfaceUnit.c
)
	#${ROOT_SRC_DIR}/*.c
	#${LIB_DIR}/*.c
	#${HARDWARE_PERIPHERAL}/*.c

set(HEADERS
	${ROOT_SRC_DIR}/ConsoleMenuOptions.h
	${ROOT_SRC_DIR}/MultiThreadSupport.h
	${LIB_DIR}/SimpleTimerWP.h
	${HARDWARE_PERIPHERAL}/HardwareInterfaceUnit.h
)
	#${ROOT_SRC_DIR}/*.h
	#${LIB_DIR}/*.h
	#${HARDWARE_PERIPHERAL}/*.h

add_executable(ProtocolMasterDevelopeTest ${SOURCES} ${HEADERS})

if(${CMAKE_VERSION} VERSION_LESS "2.8.12")
    include_directories(ProtocolMasterDevelopeTest PUBLIC ${INCLUDE_DIR})
else()
    target_include_directories(ProtocolMasterDevelopeTest PUBLIC ${INCLUDE_DIR})
endif()