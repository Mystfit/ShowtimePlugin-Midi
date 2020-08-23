cmake_minimum_required(VERSION 3.13)

project(ShowtimePluginMidi VERSION 0.0.1)


# Use C14
set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED)

set(RTMIDI_PLUGIN_TARGET ${PROJECT_NAME})
message(STATUS "Configuring ${RTMIDI_PLUGIN_TARGET} target")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake;${CMAKE_MODULE_PATH}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_SYSTEM_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/${CMAKE_SYSTEM_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/${CMAKE_SYSTEM_NAME})
if(NOT DEFINED PLUGIN_OUTPUT_DIR)
    set(PLUGIN_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins)
endif()

# Detect 64bit
if(${CMAKE_SIZEOF_VOID_P} STREQUAL "8")
  set(64bit TRUE)
elseif(${CMAKE_SIZEOF_VOID_P} STREQUAL "8")
  set(64bit FALSE)
endif()

# Build options
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
  set(BOOST_COMPONENTS 
    unit_test_framework
    filesystem
  )
endif()

if(WIN32)
  option(BUILD_WITH_TEVIRTUALMIDI "Build with teVirtualMidi loopback support")
endif()

# Plugin dependant libs
# Inbclude packages
find_package(Showtime CONFIG REQUIRED)
find_package(rtmidi CONFIG REQUIRED)
find_package(Boost COMPONENTS ${BOOST_COMPONENTS} CONFIG)
if(NOT Boost_FOUND)
    message(STATUS "Searching for Boost using cmake's FindBoost")
    find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
endif()
if(BUILD_WITH_TEVIRTUALMIDI)
  find_package(teVirtualMidiSDK)
endif()

# Set linked libs
set(PLUGIN_LIBS 
    Boost::boost
    RtMidi::rtmidi
    Showtime::Showtime
)
if(BUILD_WITH_TEVIRTUALMIDI)
  list(APPEND PLUGIN_LIBS teVirtualMIDISDK::teVirtualMIDISDK)
endif()

# Pull content
include(FetchContent)
FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.9.1)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Plugin source files
set(SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(ZST_RTMIDI_PLUGIN_HEADERS
  "${SOURCE_DIR}/plugin.h"
  "${SOURCE_DIR}/MidiConstants.h"
  "${SOURCE_DIR}/MidiFactory.h"
  "${SOURCE_DIR}/MidiBidirectionalPort.h"
  "${SOURCE_DIR}/MidiInPort.h"
  "${SOURCE_DIR}/MidiOutPort.h"
  "${SOURCE_DIR}/MidiCCGroup.h"
  "${SOURCE_DIR}/RtMidiInPort.h"
  "${SOURCE_DIR}/RtMidiOutPort.h"
  "${SOURCE_DIR}/RtMidiBidirectionalPort.h"
)
set(ZST_RTMIDI_PLUGIN_SRC
  "${SOURCE_DIR}/plugin.cpp"
  "${SOURCE_DIR}/MidiFactory.cpp"
  "${SOURCE_DIR}/MidiBidirectionalPort.cpp"
  "${SOURCE_DIR}/MidiInPort.cpp"
  "${SOURCE_DIR}/MidiOutPort.cpp"
  "${SOURCE_DIR}/MidiCCGroup.cpp"
  "${SOURCE_DIR}/RtMidiInPort.cpp"
  "${SOURCE_DIR}/RtMidiOutPort.cpp"
  "${SOURCE_DIR}/RtMidiBidirectionalPort.cpp"
)
if(BUILD_WITH_TEVIRTUALMIDI)
  list(APPEND ZST_RTMIDI_PLUGIN_HEADERS "${SOURCE_DIR}/VirtualMidi.h")
  list(APPEND ZST_RTMIDI_PLUGIN_SRC "${SOURCE_DIR}/VirtualMidi.cpp")
endif()

# Plugin compile defs
set(PLUGIN_DEFINITIONS_PRIVATE "-DZST_EXPORT_PLUGIN_API")
set(PLUGIN_DEFINITIONS_INTERFACE "-DZST_IMPORT_PLUGIN_API")
if(BUILD_WITH_TEVIRTUALMIDI)
  list(APPEND PLUGIN_DEFINITIONS_PRIVATE "-DHAS_TEVIRTUALMIDI")
endif()

# Create Library
add_library(${RTMIDI_PLUGIN_TARGET} MODULE)

# Include files in target
target_sources(${RTMIDI_PLUGIN_TARGET} PRIVATE 
    ${ZST_RTMIDI_PLUGIN_HEADERS}
    ${ZST_RTMIDI_PLUGIN_SRC}
)

# Set compiler definitions
if(WIN32)
    target_compile_definitions(${RTMIDI_PLUGIN_TARGET} PRIVATE ${PLUGIN_DEFINITIONS_PRIVATE})
    target_compile_definitions(${RTMIDI_PLUGIN_TARGET} INTERFACE ${PLUGIN_DEFINITIONS_INTERFACE})
endif()

# Set plugin output dir
set_target_properties(${RTMIDI_PLUGIN_TARGET} PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PLUGIN_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PLUGIN_OUTPUT_DIR}
)

# Link libraries
target_link_libraries(${RTMIDI_PLUGIN_TARGET} PRIVATE ${PLUGIN_LIBS} nlohmann_json::nlohmann_json)

# Copy plugin data
add_custom_command(TARGET ${RTMIDI_PLUGIN_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory  "${CMAKE_CURRENT_LIST_DIR}/midi_maps" "${PLUGIN_OUTPUT_DIR}/plugin_data/${CMAKE_PROJECT_NAME}/midi_maps"
    VERBATIM
)

# Tests
if(BUILD_TESTS)
  include( CTest ) 
  enable_testing()
  add_subdirectory(tests)
endif()