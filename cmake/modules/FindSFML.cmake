# === Outputs: ===
# SFML_FOUND
#  
# SFML_INCLUDE_DIR
# 
# SFML_GRAPHICS_LIBRARY
# SFML_WINDOW_LIBRARY
# SFML_SYSTEM_LIBRARY 
# SFML_AUDIO_LIBRARY 
# SFML_NETWORK_LIBRARY 


# Variables
IF(WIN32)
SET(SFML_PATH_SEARCH_INCLUDE
    X:/SFML-1.5/include
    C:/cygwin/usr/local/include
    C:/msys/local/include
    C:/MinGW/include/SFML
    C:/MinGW/include
    /usr/i586-mingw32msvc/include
    /usr/i586-mingw32msvc/include/SFML
    NO_DEFAULT_PATH
)
ELSE(WIN32)
SET(SFML_PATH_SEARCH_INCLUDE
    /usr/include/SFML
    /usr/local/include/SFML
    NO_DEFAULT_PATH
)
ENDIF(WIN32)
SET(SFML_PATH_SEARCH_LIB
    X:/SFML-1.5/lib/mingw
    /usr/lib
    /usr/local/lib
    C:/cygwin/usr/local/lib
    C:/msys/local/lib
    C:/MinGW/lib
    /usr/i586-mingw32msvc/lib
    NO_DEFAULT_PATH
)

# ------- Include ----------------

FIND_PATH(SFML_INCLUDE_DIR Graphics.hpp 
    ${SFML_PATH_SEARCH_INCLUDE}
)

FIND_PATH(SFML_INCLUDE_DIR Graphics.hpp)

# ------- Graphics ---------------

FIND_LIBRARY(SFML_GRAPHICS_LIBRARY NAMES libsfml-graphics sfml-graphics PATHS 
    ${SFML_PATH_SEARCH_LIB}
)

FIND_LIBRARY(SFML_GRAPHICS_LIBRARY NAMES libsfml-graphics sfml-graphics)

# ------- Window ---------------

FIND_LIBRARY(SFML_WINDOW_LIBRARY NAMES libsfml-window sfml-window PATHS 
  ${SFML_PATH_SEARCH_LIB}
)

FIND_LIBRARY(SFML_WINDOW_LIBRARY NAMES libsfml-window sfml-window)

# ------- System ---------------

FIND_LIBRARY(SFML_SYSTEM_LIBRARY NAMES libsfml-system sfml-system PATHS 
  ${SFML_PATH_SEARCH_LIB}
)

FIND_LIBRARY(SFML_SYSTEM_LIBRARY NAMES libsfml-system sfml-system)

# ------- Audio ----------------

FIND_LIBRARY(SFML_AUDIO_LIBRARY NAMES libsfml-audio sfml-audio PATHS 
    ${SFML_PATH_SEARCH_LIB}
)

FIND_LIBRARY(SFML_AUDIO_LIBRARY NAMES libsfml-audio sfml-audio)

# ------- Network ----------------

FIND_LIBRARY(SFML_NETWORK_LIBRARY NAMES libsfml-network sfml-network PATHS 
    ${SFML_PATH_SEARCH_LIB}
)

FIND_LIBRARY(SFML_NETWORK_LIBRARY NAMES libsfml-network sfml-network)
# ------ End ---------------------

IF (SFML_AUDIO_LIBRARY AND SFML_NETWORK_LIBRARY AND SFML_WINDOW_LIBRARY AND SFML_GRAPHICS_LIBRARY AND SFML_SYSTEM_LIBRARY AND SFML_INCLUDE_DIR)
    SET(SFML_FOUND TRUE)
ELSE (SFML_AUDIO_LIBRARY AND SFML_NETWORK_LIBRARY AND SFML_WINDOW_LIBRARY AND SFML_GRAPHICS_LIBRARY AND SFML_SYSTEM_LIBRARY AND SFML_INCLUDE_DIR)
    # nope
ENDIF (SFML_AUDIO_LIBRARY AND SFML_NETWORK_LIBRARY AND SFML_WINDOW_LIBRARY AND SFML_GRAPHICS_LIBRARY AND SFML_SYSTEM_LIBRARY AND SFML_INCLUDE_DIR)

SET(SFML_LIBRARIES ${SFML_WINDOW_LIBRARY} ${SFML_GRAPHICS_LIBRARY} ${SFML_SYSTEM_LIBRARY} ${SFML_NETWORK_LIBRARY} ${SFML_AUDIO_LIBRARY})

IF (SFML_INCLUDE_DIR)
    IF (NOT SFML_FIND_QUIETLY)
        MESSAGE(STATUS "Found SFML")
    ENDIF (NOT SFML_FIND_QUIETLY)
ELSE (SFML_INCLUDE_DIR)
    IF (SFML_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find SFML")
    ELSE (SFML_FIND_REQUIRED)
        IF (NOT SFML_FIND_QUIETLY)
            MESSAGE(STATUS "Could not find SFML")
        ENDIF (NOT SFML_FIND_QUIETLY)
    ENDIF (SFML_FIND_REQUIRED)
ENDIF (SFML_INCLUDE_DIR)
