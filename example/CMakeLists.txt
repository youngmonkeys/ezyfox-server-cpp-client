cmake_minimum_required(VERSION 3.6)

set( CMAKE_CXX_FLAGS "-std=gnu++11 -pthread")

add_definitions(-DEZY_DEBUG)

set(APP_NAME example)

project(${APP_NAME})

add_subdirectory(../ ../)

# add cross-platforms source files and header files 
list(APPEND APP_SOURCE
     main.cpp
     )
list(APPEND APP_HEADER
    )

# mark app complie info and libs info
set(all_code_files
    ${APP_HEADER}
    ${APP_SOURCE}
    )

add_executable(${APP_NAME} main.cpp)

target_link_libraries(${APP_NAME} ezyfox-client)
