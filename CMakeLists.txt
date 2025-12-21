cmake_minimum_required(VERSION 3.10)
project(ARP-Assignment2 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

# Find required libraries
find_package(Curses REQUIRED)
find_package(Threads REQUIRED)  # If needed for threading

# Include directories
include_directories(.)

# Executables
add_executable(main main.c)
target_link_libraries(main ${CMAKE_THREAD_LIBS_INIT})

add_executable(process_Drone process_Drone.c)
target_link_libraries(process_Drone m)  # Math library

add_executable(BlackBoard BlackBoard.c)
target_link_libraries(BlackBoard ${CURSES_LIBRARIES} m)

add_executable(process_In process_In.c)

add_executable(process_Ob process_Ob.c)

add_executable(process_Ta process_Ta.c)

add_executable(watchdog watchdog.c)

add_executable(system_logger system_logger.c)

# Custom target for all
add_custom_target(all DEPENDS main process_Drone BlackBoard process_In process_Ob process_Ta watchdog system_logger)

# Clean target
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove main process_Drone BlackBoard process_In process_Ob process_Ta watchdog system_logger
)