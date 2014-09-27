cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-fmod-studio )


  set ( PLUGIN_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

  set ( MOAI_FMOD_INCLUDES 
        ${PLUGIN_ROOT}/../
        ${PLUGIN_ROOT}/../fmod/headers/
  )

  # Use all cpp files in directory as source code
  file ( GLOB MOAI_FMOD_SRC 
    "${PLUGIN_ROOT}/*.cpp" 
    "${PLUGIN_ROOT}/*.h" 
  )


  add_library ( moai-fmod-studio STATIC ${MOAI_FMOD_SRC} )
  target_include_directories( moai-fmod-studio PUBLIC ${MOAI_FMOD_INCLUDES})
  target_link_libraries (moai-fmod-studio moai-core moai-util moai-sim zlcore)
