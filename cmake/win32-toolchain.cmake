# Add to this list where mingw32 is on your system

# this folder should have include/ lib/ and bin/ folders, which have
# .h files, .a files, and .dll files, respectively
# the libraries the program depends on should be installed to this prefix,
# and the DLLs required to run should be copied to dll/.
SET(MINGW_PREFIXES
    /usr/i586-mingw32msvc
)

######### You shouldn't have to configure beyond this point #############

SET(MINGW_PREFIX 0)
# select the correct value for MINGW_PREFIX
FOREACH(PREFIX ${MINGW_PREFIXES})
    IF(EXISTS ${PREFIX})
        SET(MINGW_PREFIX ${PREFIX})
        BREAK()
    ENDIF(EXISTS ${PREFIX})
ENDFOREACH(PREFIX ${MINGW_PREFIXES})

IF(${MINGW_PREFIX} EQUAL 0)
    MESSAGE(FATAL_ERROR
        "ERROR: You need to set up a mingw32-msvc environment and add it to "
        "MINGW_PREFIXES in cmake/win32-toolchain.cmake")
ENDIF(${MINGW_PREFIX} EQUAL 0)

SET(MINGW_DLL_PATH "${MINGW_PREFIX}/dll")

SET(CMAKE_SYSTEM_NAME Windows)

# specify the cross compiler
SET(CMAKE_C_COMPILER /usr/bin/i586-mingw32msvc-gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/i586-mingw32msvc-g++)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH ${MINGW_PREFIX})

# override qt variables so that we can use linux binaries
SET(QT_BINARY_DIR ${MINGW_PREFIX}/bin)
SET(QT_LIBRARY_DIR ${MINGW_PREFIX}/lib)
SET(QT_INCLUDE_DIR ${MINGW_PREFIX}/include)
SET(QT_HEADERS_DIR ${MINGW_PREFIX}/include)
SET(QT_MOC_EXECUTABLE moc)
SET(QT_RCC_EXECUTABLE rcc)
SET(QT_QMAKE_EXECUTABLE qmake)
SET(QT_UIC_EXECUTABLE uic)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

