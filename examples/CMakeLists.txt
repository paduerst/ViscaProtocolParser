cmake_minimum_required(VERSION 3.13)



################################################################################
## PROJECT
## name and version
################################################################################
project(examples LANGUAGES CXX)



################################################################################
## CONFIGURATION
## project configuration
################################################################################
SET(${PARENT}_VISCA_PROTOCOL_PARSER_DEMO_APPLICATION    ON  CACHE BOOL "" FORCE)
SET(${PARENT}_VISCA_PROTOCOL_PARSER_TEST                ON  CACHE BOOL "" FORCE)



################################################################################
## INCLUDING SUBDIRECTORIES
## Adding subdirectories according to the configuration
################################################################################
if (${PARENT}_VISCA_PROTOCOL_PARSER_DEMO_APPLICATION)
    add_subdirectory(ViscaProtocolParserDemoApplication)
endif()
if (${PARENT}_VISCA_PROTOCOL_PARSER_TEST)
    add_subdirectory(ViscaProtocolParserTest)
endif()

