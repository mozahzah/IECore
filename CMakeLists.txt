# Copyright © 2024 mozahzah (Incus Entertainment). All rights reserved.

cmake_minimum_required(VERSION 3.29)

# Check for required definitions
foreach(var IN ITEMS IMGUI_DIR STB_DIR)
    if(NOT DEFINED ${var})
        message(FATAL_ERROR "${var} is not set. Provide that required variable to cmake in the parent CMakeLists file.")
    endif()
endforeach()

add_subdirectory(${IMGUI_DIR} "${CMAKE_BINARY_DIR}/IECore/imgui")

if(APPLE)
  set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk)
  find_library(COREAUDIO_LIBRARY CoreAudio)
  file(GLOB_RECURSE MM_FILES "./*.mm")
endif()

file(GLOB_RECURSE CPP_FILES "./*.cpp")
list(APPEND SOURCE_FILES ${CPP_FILES} ${MM_FILES})
add_library(IECore STATIC ${SOURCE_FILES})
target_include_directories(IECore PUBLIC "./" ${STB_DIR})
target_link_libraries(IECore PUBLIC imgui)

if(WIN32)
    target_link_libraries(IECore PUBLIC dwmapi)
    target_link_libraries(IECore PUBLIC uxtheme)
elseif(APPLE)
    target_link_libraries(IECore PUBLIC ${COREAUDIO_LIBRARY})
    target_link_libraries(IECore PUBLIC "-framework Cocoa")
    target_link_libraries(IECore PUBLIC "-framework UserNotifications")
endif()

set(IECORE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "")
message("IECORE_DIR is cached as ${IECORE_DIR}")