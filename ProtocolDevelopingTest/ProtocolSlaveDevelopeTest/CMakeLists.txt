cmake_minimum_required(VERSION 3.12)

project(ProtocolSlaveDevelopeTest)
set(PROJ_NAME ProtocolSlaveDevelopeTest)

add_definitions(-DSLAVE_PORT_PROJECT)
add_definitions(-D_WINSOCKAPI_)

set(ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../")
#on Lib located inc and src
set(LIB_DIR ${ROOT_PATH}/Lib)
#inc and src located together on root path
set(ROOT_SRC_DIR ${ROOT_PATH})
set(HARDWARE_PERIPHERAL ${ROOT_PATH}/ProtocolDevelopingTest/HardwarePeripheral)
set(PROTOCOL_MASTER_MAIN ${ROOT_PATH}/ProtocolDevelopingTest)
include(${PROTOCOL_MASTER_MAIN}/globDefines.cmake)

set(SOURCES
	${PROTOCOL_MASTER_MAIN}/ProtocolSlaveImmitationDevelopeTest_main.c
	${ROOT_SRC_DIR}/ConsoleMenuOptions.c
	${ROOT_SRC_DIR}/MultiThreadSupport.c
	${LIB_DIR}/SimpleTimerWP.c
	${LIB_DIR}/fileHandle.c
	${HARDWARE_PERIPHERAL}/HardwareInterfaceUnit.c
	${IOFILE_PATH}/CmdsScenarioScripts.c
	${IOFILE_PATH}/ioserversocket.c
)
	#${ROOT_SRC_DIR}/*.c
	#${LIB_DIR}/*.c
	#${HARDWARE_PERIPHERAL}/*.c

set(HEADERS
	${ROOT_SRC_DIR}/ConsoleMenuOptions.h
	${ROOT_SRC_DIR}/MultiThreadSupport.h
	${LIB_DIR}/SimpleTimerWP.h
	${LIB_DIR}/fileHandle.h
	${LIB_DIR}/type_def.h
	${HARDWARE_PERIPHERAL}/HardwareInterfaceUnit.h
	#${IOFILE_PATH}/MasterImmitationCfg.h  //not needed! Its for MasterProtocolDeveloping
	${IOFILE_PATH}/SlaveImmitationCfg.h
	${IOFILE_PATH}/PortsBusMessages.h
	${IOFILE_PATH}/CmdsScenarioScripts.h
	${IOFILE_PATH}/iosocket.h
)

#add_library(ProtocolDevelopeLibrary STATIC ${SOURCES} ${HEADERS})

add_executable(ProtocolSlaveDevelopeTest ${SOURCES} ${HEADERS})

if(${CMAKE_VERSION} VERSION_LESS "2.8.12")
    include_directories(ProtocolSlaveDevelopeTest PUBLIC ${INCLUDE_DIR})
else()
    target_include_directories(ProtocolSlaveDevelopeTest PUBLIC ${INCLUDE_DIR})
endif()

target_link_options(${PROJ_NAME} PRIVATE "/MAP:${PROJECT_SOURCE_DIR}/outputSlaveProject.map")