cmake_minimum_required(VERSION 2.8)

message(" START from make")

# Проект ---------------------------------------------------------------------------------

set(PROJECT_COPYRIGHT "Copyright (c) smt005 2017")
set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")

add_definitions(-D"BUILD_${BUILD_PALTFORM}")

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON) 

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set(RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Resources")


# SRC

set(MAIN_SOURCE "${SRC_DIR}/${BUILD_PALTFORM}_main.cpp")
list(APPEND ALL_SOURCES ${MAIN_SOURCE})

set(SOURCE_DIRS

)

foreach(DIR ${SOURCE_DIRS})
	file(GLOB
		HEADERS
		${SRC_DIR}/${DIR}/*.h
	)

	file(GLOB
		SOURCES
		${SRC_DIR}/${DIR}/*.cpp
	)

	# для имен групп в xcode нужен обратный слэш
	string(REGEX REPLACE "/" "\\\\" GROUP_NAME ${DIR})
	source_group("${GROUP_NAME}" FILES ${HEADERS} ${SOURCES})
	
	#message("SOURCE: " ${HEADERS} ${SOURCES})
	
	list(APPEND ALL_HEADERS ${HEADERS})
	list(APPEND ALL_SOURCES ${SOURCES})

endforeach(DIR)

# RES

set(RESOURCE_DIRS
    .
	Font
	Map
	Models
	Shaders
    Shaders/Text
	Textures
	Data
)

foreach(DIR ${RESOURCE_DIRS})
	file(GLOB
		RESOURCE_TTF
		${RES_DIR}/${DIR}/*.ttf
	)

	file(GLOB
		RESOURCE_TXT
		${RES_DIR}/${DIR}/*.txt
	)

if (${BUILD_PALTFORM} STREQUAL "OSX")
	file(GLOB
		RESOURCE_FRAG
		${RES_DIR}/${DIR}/${BUILD_PALTFORM}*.frag
	)

	file(GLOB
		RESOURCE_VERT
		${RES_DIR}/${DIR}/${BUILD_PALTFORM}*.vert
	)

	file(GLOB
		RESOURCE_OBJ
		${RES_DIR}/${DIR}/*.obj
	)	
endif()
#elseif()	
	file(GLOB
		RESOURCE_FRAG
		${RES_DIR}/${DIR}/*.frag
	)

		file(GLOB
		RESOURCE_VERT
		${RES_DIR}/${DIR}/*.vert
	)
#endif()

	#file(GLOB
	#	RESOURCE_OBJ
	#	${RES_DIR}/${DIR}/*.obj
	#)

	file(GLOB
		RESOURCE_JSON
		${RES_DIR}/${DIR}/*.json
	)
	
		file(GLOB
		RESOURCE_PNG
		${RES_DIR}/${DIR}/*.png
	)

		file(GLOB
		RESOURCE_JPG
		${RES_DIR}/${DIR}/*.jpg
	)

	# для имен групп в xcode нужен обратный слэш
	string(REGEX REPLACE "/" "\\\\" GROUP_NAME "Resources/${DIR}")
	source_group("${GROUP_NAME}" FILES ${RESOURCE_TTF} ${RESOURCE_TXT} ${RESOURCE_FRAG} ${RESOURCE_VERT} ${RESOURCE_JSON} ${RESOURCE_PNG} ${RESOURCE_JPG})
	
	list(APPEND ALL_RESOURCES ${RESOURCE_TTF})
	list(APPEND ALL_RESOURCES ${RESOURCE_TXT})
	list(APPEND ALL_RESOURCES ${RESOURCE_FRAG})
	list(APPEND ALL_RESOURCES ${RESOURCE_VERT})
	list(APPEND ALL_RESOURCES ${RESOURCE_JSON})
	list(APPEND ALL_RESOURCES ${RESOURCE_PNG})
	list(APPEND ALL_RESOURCES ${RESOURCE_JPG})

endforeach(DIR)

# Подключение библиотек -----------------------------------------------------------------

# freetype
set(FREETYPE_NAME_LIB freetype)
set(FREETYPE_DIR "ThirdParty/${FREETYPE_NAME_LIB}")
include_directories("${FREETYPE_DIR}")
include_directories("${FREETYPE_DIR}/include")
add_subdirectory(${FREETYPE_DIR})

#	Физика -------------------

set(BULLET_NAME_LIB "bullet3")
set(BULLET_PHYSICS_SOURCE_DIR ${THIRD_PARTY_DIR}/${BULLET_NAME_LIB})

message("BULLET_PHYSICS_SOURCE_DIR = ${BULLET_PHYSICS_SOURCE_DIR}")

OPTION(BUILD_SHARED_LIBS "Use shared libraries" ON)
OPTION(BUILD_UNIT_TESTS "Build Unit Tests"	OFF)
OPTION(BUILD_BULLET2_DEMOS "Build Unit Tests"	OFF)
OPTION(USE_GRAPHICAL_BENCHMARK "Use Graphical Benchmark" OFF)
OPTION(BUILD_CPU_DEMOS "Build original Bullet CPU examples" OFF)
OPTION(USE_GLUT "Use Glut"	OFF)
OPTION(BUILD_ENET "Set when you want to build apps with enet UDP networking support" OFF)
OPTION(BUILD_CLSOCKET "Set when you want to build apps with enet TCP networking support" OFF)
OPTION(BUILD_BULLET2_DEMOS "Set when you want to build the Bullet 2 demos" OFF)
OPTION(BUILD_EXTRAS "Set when you want to build the extras" OFF)
OPTION(INSTALL_CMAKE_FILES "Install generated CMake files" OFF)
OPTION(BUILD_OPENGL3_DEMOS "Set when you want to build the OpenGL3+ demos" OFF)
OPTION(BUILD_BULLET2_DEMOS "Set when you want to build the Bullet 2 demos" OFF)
OPTION(BUILD_EXTRAS "Set when you want to build the extras" OFF)
OPTION(BUILD_PYBULLET_MAC_USE_PYTHON_FRAMEWORK "Set when you want to use the Python Framework on Mac" OFF)
OPTION(BUILD_PYBULLET_ENET "Set when you want to build pybullet with enet UDP networking support" OFF)
OPTION(BUILD_PYBULLET_CLSOCKET "Set when you want to build pybullet with enet TCP networking support" OFF)
OPTION(BUILD_PYBULLET "Set when you want to build pybullet (Python bindings for Bullet)" OFF)

add_subdirectory(${BULLET_PHYSICS_SOURCE_DIR})

INCLUDE_DIRECTORIES(
	${BULLET_PHYSICS_SOURCE_DIR}/src
)

LINK_LIBRARIES(
	BulletDynamics BulletCollision LinearMath
)

# ---------------------

if (${BUILD_PALTFORM} STREQUAL "WIN_GLES")
	# OpenGL es
	include_directories("${THIRD_PARTY_DIR}/GLES/include")
	link_directories("${THIRD_PARTY_DIR}/GLES/WIN/x86_32")
	
	# FreeImage
	include_directories("${THIRD_PARTY_DIR}/FreeImage/Source")
	link_directories("${THIRD_PARTY_DIR}/FreeImage/Windows/x86_32")
	
	message("include link LIB WIN_GLES")
elseif(${BUILD_PALTFORM} STREQUAL "WIN_GLFW")
	# 
	include_directories("${THIRD_PARTY_DIR}/GLFW/include")
	link_directories("${THIRD_PARTY_DIR}/GLFW/WIN/x86_32")

	include_directories("${THIRD_PARTY_DIR}/glew/include")
	link_directories("${THIRD_PARTY_DIR}/glew/lib/Release/Win32")

	# FreeImage
	include_directories("${THIRD_PARTY_DIR}/FreeImage/Source")
	link_directories("${THIRD_PARTY_DIR}/FreeImage/Windows/x86_32")
	
	message("include link LIB WIN_GLFW")
elseif(${BUILD_PALTFORM} STREQUAL "OSX")
	include_directories("${THIRD_PARTY_DIR}/GLFW/include")
	link_directories("${THIRD_PARTY_DIR}/GLFW/OSX")

	# FreeImage
	set(FREEIMAGE_NAME_LIB FreeImage)
	set(FREEIMAGE_DIR "${THIRD_PARTY_DIR}/${FREEIMAGE_NAME_LIB}")
	include_directories("${THIRD_PARTY_DIR}/${FREEIMAGE_NAME_LIB}/Source")
	add_subdirectory("${FREEIMAGE_DIR}")

	set (USED_FRAMEWORKS
		"-framework OpenGL"
	)
	message("include link LIB OSX")
elseif()
	message("include link LIB ...")
endif()

#------------------------

set(PLATFORM_NAME_LIB Platform)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Platform/${BUILD_PALTFORM}/Source)
add_subdirectory(${PLATFORM_NAME_LIB})

set(ENGINE_NAME_LIB Engine)
include_directories(${ENGINE_NAME_LIB}/Source)
add_subdirectory(${ENGINE_NAME_LIB})

set(EXAMPLE_NAME_LIB Example)
include_directories(${EXAMPLE_NAME_LIB}/Source)
add_subdirectory(${EXAMPLE_NAME_LIB})

set(GAME_NAME_LIB Game)
include_directories(${GAME_NAME_LIB}/Source)
add_subdirectory(${GAME_NAME_LIB})


# Создание проекта -----------------------------------------------------------------

IF (WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
ENDIF()

if (${BUILD_PALTFORM} STREQUAL "WIN_GLES")
	add_executable(${PROJECT_NAME} WIN32  ${ALL_HEADERS} ${ALL_SOURCES} ${ALL_RESOURCES} )
	
	message("add_executable WIN_GLES")
elseif(${BUILD_PALTFORM} STREQUAL "WIN_GLFW")
	add_executable(${PROJECT_NAME} EXCLUDE_FROM_ALL ${ALL_HEADERS} ${ALL_SOURCES} ${ALL_RESOURCES} )

	message("add_executable WIN_GLFW")
elseif (${BUILD_PALTFORM} STREQUAL "OSX")
	add_executable(${PROJECT_NAME} MACOSX_BUNDLE  ${ALL_HEADERS} ${ALL_SOURCES} ${ALL_RESOURCES} )
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

	message("add_executable no OSX")
endif()

# Подключение библиотеи -----------------------------------------------------------------

	#	Физика -------------------
	SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES  DEBUG_POSTFIX "_Debug")
	SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES  MINSIZEREL_POSTFIX "_MinsizeRel")
	SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES  RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo")

	set_property(TARGET BulletDynamics PROPERTY FOLDER ${BULLET_NAME_LIB})
	set_property(TARGET BulletCollision PROPERTY FOLDER ${BULLET_NAME_LIB})
	set_property(TARGET LinearMath PROPERTY FOLDER ${BULLET_NAME_LIB})
	
	set_property(TARGET Bullet2FileLoader PROPERTY FOLDER ${BULLET_NAME_LIB})
	set_property(TARGET Bullet3Common PROPERTY FOLDER ${BULLET_NAME_LIB})
	set_property(TARGET Bullet3Collision PROPERTY FOLDER ${BULLET_NAME_LIB})
	set_property(TARGET Bullet3Dynamics PROPERTY FOLDER ${BULLET_NAME_LIB})
	set_property(TARGET Bullet3Geometry PROPERTY FOLDER ${BULLET_NAME_LIB})
	set_property(TARGET Bullet3OpenCL_clew PROPERTY FOLDER ${BULLET_NAME_LIB})
	set_property(TARGET BulletInverseDynamics PROPERTY FOLDER ${BULLET_NAME_LIB})
	set_property(TARGET BulletSoftBody PROPERTY FOLDER ${BULLET_NAME_LIB})
	set_property(TARGET Bullet3Dynamics PROPERTY FOLDER ${BULLET_NAME_LIB})
	
	target_link_libraries(${PROJECT_NAME} BulletDynamics)
	target_link_libraries(${PROJECT_NAME} BulletCollision)
	target_link_libraries(${PROJECT_NAME} LinearMath)

	#	-------------------
	

target_link_libraries(${PROJECT_NAME}  ${ENGINE_NAME_LIB})
target_link_libraries(${PROJECT_NAME}  ${EXAMPLE_NAME_LIB})
target_link_libraries(${PROJECT_NAME}  ${GAME_NAME_LIB})
target_link_libraries(${PROJECT_NAME}  ${PLATFORM_NAME_LIB})

target_link_libraries(${PROJECT_NAME}  ${FREEIMAGE_NAME_LIB})
target_link_libraries(${PROJECT_NAME}  ${FREETYPE_NAME_LIB})

if (${BUILD_PALTFORM} STREQUAL "WIN_GLES")
	# OpenGL ES
	target_link_libraries(${PROJECT_NAME}  "libEGL")
	target_link_libraries(${PROJECT_NAME}  "libGLESv2")
	
	file(COPY "${THIRD_PARTY_DIR}/GLES/WIN/x86_32/libEGL.dll" DESTINATION Debug)
	file(COPY "${THIRD_PARTY_DIR}/GLES/WIN/x86_32/libGLESv2.dll" DESTINATION Debug)
	
	# FreeImage
	target_link_libraries(${PROJECT_NAME}  "freeImage")
	file(COPY "${THIRD_PARTY_DIR}/FreeImage/Windows/x86_32/FreeImage.dll" DESTINATION Debug)
	
	message("target_link_libraries LIB WIN_GLES")
elseif(${BUILD_PALTFORM} STREQUAL "WIN_GLFW")
	# GLFW OpenGL
	target_link_libraries(${PROJECT_NAME}  "glfw3")
	target_link_libraries(${PROJECT_NAME}  "opengl32")

	target_link_libraries(${PROJECT_NAME}  "glew32")
	target_link_libraries(${PROJECT_NAME}  "glew32s")
	file(COPY "${THIRD_PARTY_DIR}/glew/bin/Release/Win32/glew32.dll" DESTINATION Debug)

	# FreeImage
	target_link_libraries(${PROJECT_NAME}  "freeImage")
	file(COPY "${THIRD_PARTY_DIR}/FreeImage/Windows/x86_32/FreeImage.dll" DESTINATION Debug)
	
	message("target_link_libraries LIB WIN_GLES")
elseif (${BUILD_PALTFORM} STREQUAL "OSX")
	target_link_libraries (${PROJECT_NAME} ${USED_FRAMEWORKS})
	
	find_library(GLFW_LIBRARY NAMES libglfw.3.dylib HINTS "${THIRD_PARTY_DIR}/GLFW/OSX")
	target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARY})
	file(COPY ${GLFW_LIBRARY} DESTINATION Debug)

	message("target_link_libraries LIB OSX")
elseif()
	message("target_link_libraries LIB ...")
endif()

message(" END from make")
