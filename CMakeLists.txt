cmake_minimum_required(VERSION 3.3)

option(ALLOW_MOVE "Option description" OFF)

Project(unit-test)

if( NOT CMAKE_BUILD_TYPE )
set( CMAKE_BUILD_TYPE Release CACHE STRING
     "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
     FORCE )
endif()

if(ALLOW_MOVE)
  add_definitions(-DALLOW_MOVE)
endif()

add_executable(unit-test cpp-copy-elision-NRVO-URVO.cpp)
target_link_libraries(unit-test)

