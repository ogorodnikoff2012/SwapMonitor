cmake_minimum_required(VERSION 3.10)

# Project name and version
project(SwapMonitor VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Set optimization flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -pthread")

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(APPINDICATOR REQUIRED appindicator3-0.1)

# Include directories
include_directories(${GTK3_INCLUDE_DIRS} ${APPINDICATOR_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS} ${APPINDICATOR_LIBRARY_DIRS})

# Define the executable
add_executable(swap_monitor swap_monitor.c)

# Link libraries
target_link_libraries(swap_monitor ${GTK3_LIBRARIES} ${APPINDICATOR_LIBRARIES} m)

# Installation paths
set(BIN_INSTALL_DIR "/usr/local/bin")
set(DESKTOP_INSTALL_DIR "/usr/share/applications")
set(ICON_INSTALL_DIR "/usr/share/icons/hicolor/128x128/apps")

# Install the binary
install(TARGETS swap_monitor DESTINATION ${BIN_INSTALL_DIR})

# Install the .desktop file
install(FILES swap_monitor.desktop DESTINATION ${DESKTOP_INSTALL_DIR})

# Install the icon
install(FILES swap_monitor.png DESTINATION ${ICON_INSTALL_DIR})
