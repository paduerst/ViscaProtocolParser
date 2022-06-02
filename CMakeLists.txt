cmake_minimum_required(VERSION 3.13)



##################################################################################
## PROJECT
##################################################################################
project(ViscaProtocolParser LANGUAGES CXX)



##################################################################################
## SETTINGS
## basic project settings before use
##################################################################################
# If this project is used as a submodule, the variable should be overridden
# to "OFF" in the top-level application (to disable forced cache rewriting)
option(${PARENT}_SUBMODULE_CACHE_OVERWRITE "Enable forced cache rewriting" ON)
if (${PARENT}_SUBMODULE_CACHE_OVERWRITE)
    SET(REWRITE_FORCE "FORCE")
else()
    SET(REWRITE_FORCE "")
endif()



##################################################################################
## CONFIGURATION
## project configuration
##################################################################################
SET(${PARENT}_VISCA_PROTOCOL_PARSER_EXAMPLES   ON CACHE BOOL "" ${REWRITE_FORCE})



##################################################################################
## ADD PROJECTS
##################################################################################

# API class
add_subdirectory(src)

# Examples
if (${PARENT}_VISCA_PROTOCOL_PARSER_EXAMPLES)
    add_subdirectory(examples)
endif()

