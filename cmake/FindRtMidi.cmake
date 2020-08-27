include (FindPackageHandleStandardArgs)

find_path(RTMIDI_INCLUDE_DIR NAMES RtMidi.h
	HINTS "/usr/local/include"
)

message(STATUS ${RTMIDI_INCLUDE_DIR})

set(LIBRARY_NAMES "RtMidi")

find_library(RTMIDI_LIBRARIES
	NAMES ${LIBRARY_NAMES}
	HINTS "/usr/local/lib"
)

message(STATUS ${RTMIDI_LIBRARIES})
message(STATUS ${RTMIDI_INCLUDE_DIR})

if(RTMIDI_INCLUDE_DIR AND RTMIDI_LIBRARIES)
	set(RTMIDI_FOUND ON)
endif()

add_library(RtMidi::rtmidi SHARED IMPORTED)
set_property(TARGET RtMidi::rtmidi PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${RTMIDI_INCLUDE_DIR})
set_property(TARGET RtMidi::rtmidi PROPERTY IMPORTED_LOCATION ${RTMIDI_LIBRARIES})
