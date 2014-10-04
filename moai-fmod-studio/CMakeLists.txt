cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-fmod-studio )


  set ( PLUGIN_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

  set ( FMOD_LIB_ROOT ${PLUGIN_ROOT}/../fmod/lib)

  if (BUILD_WINDOWS)
    if (MSVC)
      find_library(FMOD_LIB fmod_vc PATHS ${FMOD_LIB_ROOT}/windows)
    else (MSVC)
      set (FMOD_LIB ${FMOD_LIB_ROOT}/windows/fmod.dll)
    endif (MSVC)
  elseif (BUILD_OSX)
    find_library(FMOD_LIB fmod PATHS ${FMOD_LIB_ROOT}/osx)
  elseif (BUILD_ANDROID)
    #message ("looking for fmod in ${FMOD_LIB_ROOT}/android/${ANDROID_ABI}/libfmod.so")
    #find_library(FMOD_LIB libfmod PATHS "${FMOD_LIB_ROOT}/android/${ANDROID_ABI}")   
    set (FMOD_LIB "${FMOD_LIB_ROOT}/android/${ANDROID_ABI}/libfmod.so")
  endif()

  
  if (BUILD_IOS)
    set (FMOD_LIB "${FMOD_LIB_ROOT}/ios/libfmod_iphoneos.a")
  endif (BUILD_IOS)
  

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
  target_link_libraries (moai-fmod-studio moai-core moai-util moai-sim zlcore ${FMOD_LIB})

