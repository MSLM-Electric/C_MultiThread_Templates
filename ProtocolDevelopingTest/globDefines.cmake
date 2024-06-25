###globDefinesConfig.cmake

get_filename_component(GLOB_DEFINES_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)  #TODO: test it in another projs and cmake versions lower 3.0.0! (PATH = Legacy alias for DIRECTORY (use for CMake <= 2.8.11))
                                                                                 #look at https://cmake.org/cmake/help/v2.8.12/cmake.html#command:get_filename_component
set(IOFILE_PATH "${GLOB_DEFINES_DIR}/IO_immitationBetweenMasterSlave" CACHE PATH "Path to IO file") #FORCE)
message(STATUS "IO file path: ${IOFILE_PATH}") #check it always in other platforms. The path may be not proper on another cases. //?!TODO: test it in another projs!  

add_definitions(-DIOFILE_PATH="${IOFILE_PATH}/iofile.txt")
add_definitions(-DGLOB_MUTEX_FILE="${IOFILE_PATH}/globMutexHold.txt")  #this is glob mutex between another independent applications working with only one iofile.txt
add_definitions(-DGLOB_SCANF_MUTEX="${IOFILE_PATH}/globScanfMutex.txt")
add_definitions(-DFILE_MUTEX_REALEASED="MUTEX_REALEASED")
add_definitions(-DFILE_MUTEX_TAKEN="MUTEX_TAKEN")
add_definitions(-DPROTOCOL_DEVELOPING_PROJECT)
#add_definitions(-DDEBUG_ON_VS);
add_definitions(-DSEPARATE_SOCKETS_TESTING) #SEPARATE_TESTING_SOCKETS sounds better