set(TEST_SRC "TestPlugin.cpp")

set(TEST_DEPENDENCIES 
	Boost::unit_test_framework
	Boost::boost
	Boost::filesystem
	Showtime::Showtime
)

add_executable(TestMidiPlugin ${TEST_SRC})
target_link_libraries(TestMidiPlugin PUBLIC ${TEST_DEPENDENCIES})

add_dependencies(TestMidiPlugin ${RTMIDI_PLUGIN_TARGET})

if(WIN32)
  # Copy runtime dependencies
  add_custom_command(TARGET TestMidiPlugin POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:Showtime::Showtime>" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
      VERBATIM
  )
endif()
