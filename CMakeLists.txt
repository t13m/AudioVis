#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)


# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

project(AudioVis)
set(CMAKE_CXX_STANDARD 14)

# Set version information in a config.h file
set(AudioVis_VERSION_MAJOR 1)
set(AudioVis_VERSION_MINOR 0)
configure_file(
        "${PROJECT_SOURCE_DIR}/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
)
include_directories("${PROJECT_BINARY_DIR}")

# Define sources and executable
set(EXECUTABLE_NAME "AudioVis")
add_executable(${EXECUTABLE_NAME} main.cpp Recorder.cc Recorder.hh FFT.hh F0Estimator.cc F0Estimator.hh)


# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()

# Detect and add PortAudio
find_package(PORTAUDIO)
if(PORTAUDIO_FOUND)
    include_directories(${PORTAUDIO_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${PORTAUDIO_LIBRARIES})
    MESSAGE( STATUS "PORTAUDIO_INCLUDE_DIR:       " ${PORTAUDIO_INCLUDE_DIR} )
    MESSAGE( STATUS "PORTAUDIO_LIBRARIES:         " ${PORTAUDIO_LIBRARIES} )
endif()


# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)


# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${AudioVis_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${AudioVis_VERSION_MINOR}")
include(CPack)