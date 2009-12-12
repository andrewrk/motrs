CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(PROGRAM_NAME "motrs")
PROJECT(${PROGRAM_NAME})

CMAKE_POLICY(SET CMP0004 OLD)

SET(MINGW_PATH "/usr/i586-mingw32msvc")
SET(MINGW_DLL_PATH "${MINGW_PATH}/dll")
SET(RESOURCES_FILE "${CMAKE_BINARY_DIR}/resources.dat")

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# prepare for special include files
SET(OUT_INCLUDE_PATH "${CMAKE_BINARY_DIR}/cmakeincludes")

# create version number
SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "0")
EXECUTE_PROCESS(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND cat ${CMAKE_SOURCE_DIR}/cmake/deploy_revision
    OUTPUT_VARIABLE REV)
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${REV}")
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/cmake/include/version.h.in
    ${OUT_INCLUDE_PATH}/version.h)
MESSAGE("Compiling version ${VERSION}")

####################### Dependencies #######################
SET(DEP_INCLUDES "")
SET(DEP_LIBS "")
SET(DLL_FILES "")

# check for SDL
FIND_PACKAGE(SDL)
IF(SDL_FOUND)
    MESSAGE("Found SDL")

    SET(HAVE_SDL TRUE)
    SET(STATUS_SDL "OK")
    SET(DEP_INCLUDES "${DEP_INCLUDES} ${SDL_INCLUDE_DIR}")
    SET(DEP_LIBS "${DEP_LIBS} ${SDL_LIBRARY}")
    SET(SDL_DLL "${MINGW_DLL_PATH}/SDL.dll")

    IF(EXISTS ${SDL_DLL})
        SET(DLL_FILES "${DLL_FILES} ${SDL_DLL}")
    ELSE(EXISTS ${SDL_DLL})
        MESSAGE("...but missing ${SDL_DLL} for Windows build.")
    ENDIF(EXISTS ${SDL_DLL})
        
ELSE(SDL_FOUND)
    SET(STATUS_SDL "not found")
    MESSAGE("Could not find SDL")
ENDIF(SDL_FOUND)
################################################################



# compile 

SET(CMAKE_CXX_FLAGS "-ggdb -Wall")
FILE(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
FILE(GLOB_RECURSE INCLUDES ${CMAKE_SOURCE_DIR}/src/*.h)

INCLUDE_DIRECTORIES(${OUT_INCLUDE_PATH} ${DEP_INCLUDES})
LINK_LIBRARIES(${DEP_LIBS})

ADD_EXECUTABLE(${PROGRAM_NAME} ${SOURCES})

ADD_DEPENDENCIES(${PROGRAM_NAME} ${RESOURCES_FILE})

# set folders to install to
IF(WIN32)
    SET(DATA_DIR .)
    INSTALL(TARGETS ${PROGRAM_NAME} RUNTIME DESTINATION .)
    INSTALL(FILES ${DLL_FILES} DESTINATION .)
ELSE(WIN32)
    SET(DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/${PROGRAM_NAME}")
    INSTALL(TARGETS ${PROGRAM_NAME} RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
ENDIF(WIN32)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/cmake/include/resources.h.in
    ${OUT_INCLUDE_PATH}/resources.h)

# make sub directories
ADD_SUBDIRECTORY("resources")

MESSAGE("\n"
"Installation Summary\n"
"--------------------\n"
"* Install Directory            : ${CMAKE_INSTALL_PREFIX}\n"
)

MESSAGE(
"Required Libraries\n"
"------------------\n"
"* SDL                       : ${STATUS_SDL}\n"
)

MESSAGE(
"If everything is OK, proceed with\n"
"make\n"
)

