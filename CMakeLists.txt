cmake_minimum_required(VERSION 3.10)
project(ofcm C CXX)
project(base C CXX)

find_package(OpenCV 3.2.0 QUIET)
if(NOT OpenCV_FOUND)
  find_package(OpenCV 3.0.0 QUIET)
endif()
if(NOT OpenCV_FOUND)
  find_package(OpenCV 4.0.0 QUIET)
endif()
if(NOT OpenCV_FOUND)
  find_package(OpenCV 2.4.9.1 REQUIRED)
endif()
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

message(STATUS "\nOpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")


add_library(base
  src/utils.cpp    
)


add_executable(out src/main.cpp)

set_target_properties(out PROPERTIES 
                      LINKER_LANGUAGE CXX)
target_link_libraries(out PRIVATE base ${OpenCV_LIBS})
install(TARGETS out DESTINATION /usr/lib)

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-O2")
endif()
