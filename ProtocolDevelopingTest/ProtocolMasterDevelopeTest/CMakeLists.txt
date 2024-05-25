cmake_minimum_required(VERSION 3.12)

project(ProtocolMasterDevelopeTest)
set(PROJ_NAME ProtocolMasterDevelopeTest)

add_definitions(-DMASTER_PORT_PROJECT)

set(ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../")
#on Lib located inc and src
set(LIB_DIR ${ROOT_PATH}/Lib)
#inc and src located together on root path
set(ROOT_SRC_DIR ${ROOT_PATH})
set(HARDWARE_PERIPHERAL ${ROOT_PATH}/ProtocolDevelopingTest/HardwarePeripheral)
set(PROTOCOL_MASTER_MAIN ${ROOT_PATH}/ProtocolDevelopingTest)
include(${PROTOCOL_MASTER_MAIN}/globDefines.cmake)

set(SOURCES
	${PROTOCOL_MASTER_MAIN}/ProtocolMasterImmitationDevelopeTest_main.c
	${ROOT_SRC_DIR}/ConsoleMenuOptions.c
	${ROOT_SRC_DIR}/MultiThreadSupport.c
	${LIB_DIR}/SimpleTimerWP.c
	${LIB_DIR}/fileHandle.c
	${HARDWARE_PERIPHERAL}/HardwareInterfaceUnit.c
	${IOFILE_PATH}/CmdsScenarioScripts.c
)
	#${ROOT_SRC_DIR}/*.c
	#${LIB_DIR}/*.c
	#${HARDWARE_PERIPHERAL}/*.c
message(${IOFILE_PATH} )
set(HEADERS
	${ROOT_SRC_DIR}/ConsoleMenuOptions.h
	${ROOT_SRC_DIR}/MultiThreadSupport.h
	${LIB_DIR}/SimpleTimerWP.h
	${LIB_DIR}/fileHandle.h
	${LIB_DIR}/type_def.h
	${HARDWARE_PERIPHERAL}/HardwareInterfaceUnit.h
	${IOFILE_PATH}/MasterImmitationCfg.h
	${IOFILE_PATH}/PortsBusMessages.h
	${IOFILE_PATH}/CmdsScenarioScripts.h
	#${IOFILE_PATH}/SlaveImmitationCfg.h  //not needed! Its for SlaveProtocolDeveloping
)

#next line exports lib file on build/Debug/*.dll (shared); *.lib (static)
#add_library(ProtocolDevelopeLibrary SHARED ${SOURCES} ${HEADERS})
#add_library(ProtocolDevelopeLibrary STATIC ${SOURCES} ${HEADERS})
#add_library(ProtocolDevelopeLibrary MODULE ${SOURCES} ${HEADERS})

add_executable(ProtocolMasterDevelopeTest ${SOURCES} ${HEADERS})

#set(INCLUDE_DIR ${PROTOCOL_MASTER_MAIN}/include)

if(${CMAKE_VERSION} VERSION_LESS "2.8.12")
    include_directories(ProtocolMasterDevelopeTest PUBLIC ${INCLUDE_DIR})
else()
    target_include_directories(ProtocolMasterDevelopeTest PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)
endif()

#target_link_options(${PROJ_NAME} PUBLIC LINKER:-Map=${PROJ_NAME}.map)
target_link_options(${PROJ_NAME} PRIVATE "/MAP:${PROJECT_SOURCE_DIR}/outputMasterProject.map")
#the next is only gcc option and helps us to see how many memory used
#before using it configure the linker script file!
#target_link_options(${PROJ_NAME} PRIVATE -Wl,--print-memory-usage)