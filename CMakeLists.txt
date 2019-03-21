cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(SRC)
  get_filename_component(projectname ${SRC} NAME_WE)
else()
  set(projectname IMT2531)
endif(SRC)

project(${projectname})
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

message( "system name is: " ${CMAKE_SYSTEM} )
message( "system processor is: " ${CMAKE_SYSTEM_PROCESSOR} )

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  SET( LINUX 1)
  SET( APPLE 0)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET( LINUX 0)
  SET( APPLE 1 )
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  SET( WINDOWS 1)
endif()

find_package(OpenGL REQUIRED)

file( GLOB SOURCES src/src/*.cpp )
file( GLOB HEADERS src/headers/* )
file( GLOB SHADERS resources/shaders/*)

foreach(FILE ${SHADERS})
    source_group("Shader Files" FILES "${FILE}")
endforeach()

SET(MAIN_SRCS
    ${SOURCES}
    ${SHADERS}
)
SET(MAIN_HDRS
    ${HEADERS}
)

if(APPLE OR LINUX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
endif(APPLE OR LINUX)

set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "turn off build examples")
add_subdirectory(glfw)
link_directories(${PROJECT_BINARY_DIR})
link_directories(${GLFW_BINARY_DIR})

add_subdirectory(SOIL)
link_directories(${PROJECT_BINARY_DIR})
link_directories(${SOIL_BINARY_DIR})

set(SDL2_LIB_DIR "${PROJECT_SOURCE_DIR}/SDL2/lib/x86/SDL2.lib;${PROJECT_SOURCE_DIR}/SDL2/lib/x86/SDL2main.lib")
string(STRIP "${SDL2_LIB_DIR}" SDL2_LIB_DIR)
link_directories(${PROJECT_BINARY_DIR})
link_directories(${SDL2_LIB_DIR})

set(SDL2M_LIB_DIR "${PROJECT_SOURCE_DIR}/SDL2_mixer/lib/x86/SDL2_mixer.lib")
string(STRIP "${SDL2M_LIB_DIR}" SDL2M_LIB_DIR)
link_directories(${PROJECT_BINARY_DIR})
link_directories(${SDL2M_LIB_DIR})

set(SDL2ttf_LIB_DIR "${PROJECT_SOURCE_DIR}/SDL2_ttf/lib/x86/SDL2_ttf.lib")
string(STRIP "${SDL2ttf_LIB_DIR}" SDL2ttf_LIB_DIR)
link_directories(${PROJECT_BINARY_DIR})
link_directories(${SDL2ttf_LIB_DIR})


if(WINDOWS)
link_directories(${PROJECT_SOURCE_DIR}/glew/lib/)
endif(WINDOWS)

if(SRC)
  get_filename_component(target ${SRC} NAME_WE)
  add_executable(${target} ${SRC} )
	if(WINDOWS)
		target_link_libraries(${target} glfw glew32 SOIL ${GLFW_LIBRARIES}${SOIL_LIBRARIES}${SDL2_LIB_DIR} ${SDL2M_LIB_DIR} ${SDL2ttf_LIB_DIR})
		else()
		target_link_libraries(${target} glfw SOIL ${GLEW_LIBRARY} ${GLFW_LIBRARIES} ${SOIL_LIBRARIES} SDL2 ${SDL2TTF_LIBRARY})
	endif(WINDOWS)

else()
  file( GLOB EXAMPLES src/*.cpp )
  foreach( srcfile ${EXAMPLES} )
    get_filename_component(target ${srcfile} NAME_WE)
    if(target)
	    add_executable(${target} ${srcfile} ${MAIN_SRCS} ${MAIN_HDRS})
		if(WINDOWS)
			target_link_libraries(${target} glfw glew32 SOIL ${GLFW_LIBRARIES} ${SOIL_LIBRARIES}${SDL2_LIB_DIR} ${SDL2M_LIB_DIR} ${SDL2ttf_LIB_DIR})
			else()
			target_link_libraries(${target} glfw SOIL ${GLEW_LIBRARY} ${GLFW_LIBRARIES} ${SOIL_LIBRARIES} SDL2 ${SDL2TTF_LIBRARY})
		endif(WINDOWS)
	endif(target)
  endforeach( srcfile ${EXAMPLES} )
endif()
#file (GLOB         PROJECT_DLL dll/glew32.dll)

include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/resources/)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/glfw/include/)
include_directories(${PROJECT_SOURCE_DIR}/soil/src/)
include_directories(${PROJECT_SOURCE_DIR}/glm/)
include_directories(${PROJECT_SOURCE_DIR}/glm/gtx/)
include_directories(${PROJECT_SOURCE_DIR}/glm/gtc/)
include_directories(${PROJECT_SOURCE_DIR}/glew/include/)
include_directories(${PROJECT_SOURCE_DIR}/SDL2/include/)
include_directories(${PROJECT_SOURCE_DIR}/SDL2_mixer/include/)
include_directories(${PROJECT_SOURCE_DIR}/SDL2_ttf/include/)

#file(COPY  ${PROJECT_SOURCE_DIR}/include/vertex.vert DESTINATION ${PROJECT_BINARY_DIR})
#file(COPY  ${PROJECT_SOURCE_DIR}/include/fragment.frag DESTINATION ${PROJECT_BINARY_DIR})

if(WINDOWS)
include_directories(${PROJECT_SOURCE_DIR}/glew/include/)

file(COPY  ${PROJECT_SOURCE_DIR}/dll/ DESTINATION ${PROJECT_BINARY_DIR})
endif(WINDOWS)

if(APPLE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${GLEW_INCLUDE_DIRS}")
endif(APPLE)


get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()
