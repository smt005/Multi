cmake_minimum_required(VERSION 2.8)

message(" START from make")

# Проект ---------------------------------------------------------------------------------

set(PROJECT_COPYRIGHT "Copyright (c) smt005 2017")
set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")

add_definitions(-D"BUILD_${BUILD_PALTFORM}")

set(PLATFORM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Platform")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")

# SRC

set(SOURCE_DIRS
    .
	Draw
	Platform/${BUILD_PALTFORM}
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
	
# PLATFORM
	
set(SOURCE_PLATFORM_DIRS
	${BUILD_PALTFORM}
)

foreach(DIR ${SOURCE_PLATFORM_DIRS})
	file(GLOB
		HEADERS
		${PLATFORM_DIR}/${DIR}/*.h
	)

	file(GLOB
		SOURCES
		${PLATFORM_DIR}/${DIR}/*.cpp
	)

	# для имен групп в xcode нужен обратный слэш
	string(REGEX REPLACE "/" "\\\\" GROUP_NAME ${DIR})
	source_group("${GROUP_NAME}" FILES ${HEADERS} ${SOURCES})
	
	message("PLATFORM: " ${HEADERS} ${SOURCES})
	
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
	Textures
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

	file(GLOB
		RESOURCE_FRAG
		${RES_DIR}/${DIR}/*.frag
	)

		file(GLOB
		RESOURCE_VERT
		${RES_DIR}/${DIR}/*.vert
	)

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
	string(REGEX REPLACE "/" "\\\\" GROUP_NAME "res/${DIR}")
	source_group("${GROUP_NAME}" FILES ${RESOURCE_TTF} ${RESOURCE_TXT} ${RESOURCE_FRAG} ${RESOURCE_VERT} ${RESOURCE_JSON} ${RESOURCE_PNG} ${RESOURCE_JPG})
	
	list(APPEND ALL_RESOURCES ${RESOURCE_TTF})
	list(APPEND ALL_RESOURCES ${RESOURCE_TXT})
	list(APPEND ALL_RESOURCES ${RESOURCE_FRAG})
	list(APPEND ALL_RESOURCES ${RESOURCE_VERT})
	list(APPEND ALL_RESOURCES ${RESOURCE_JSON})
	list(APPEND ALL_RESOURCES ${RESOURCE_PNG})
	list(APPEND ALL_RESOURCES ${RESOURCE_JPG})

endforeach(DIR)
		
# DEFINES
if (${BUILD_PALTFORM} STREQUAL "WIN32")
	add_definitions(-DWIN32_RESOURCES_DIR="${RES_DIR}/")
elseif()
endif()		

# Подключение библиотек -----------------------------------------------------------------

# FreeImage
#set(FREEIMAGE_NAME_LIB FreeImage)
#set(FREEIMAGE_DIR "ThirdParty/${FREEIMAGE_NAME_LIB}")
#include_directories("${FREETYPE_DIR}/Source")
#add_subdirectory(${FREEIMAGE_DIR})

# freetype
set(FREETYPE_NAME_LIB freetype)
set(FREETYPE_DIR "ThirdParty/${FREETYPE_NAME_LIB}")
include_directories("${FREETYPE_DIR}/include")
add_subdirectory(${FREETYPE_DIR})

# Platform, glm
include_directories(${PLATFORM_DIR}/${BUILD_PALTFORM})
include_directories(${THIRD_PARTY_DIR})

if (${BUILD_PALTFORM} STREQUAL "WIN32")
	# OpenGL es
	include_directories("${THIRD_PARTY_DIR}/GLES/include")
	link_directories("${THIRD_PARTY_DIR}/GLES/Windows/x86_32")
	
	# FreeImage
	include_directories("${THIRD_PARTY_DIR}/FreeImage/Source")
	link_directories("${THIRD_PARTY_DIR}/FreeImage/Windows/x86_32")
	
	message("include link LIB WIN32")
	
elseif()
		message("include link LIB no WIN32")
endif()

#---------------------------

set(ENGINE_NAME_LIB Engine)
include_directories(${ENGINE_NAME_LIB}/Source)
add_subdirectory(${ENGINE_NAME_LIB})

set(EXAMPLE_NAME_LIB Example)
include_directories(${EXAMPLE_NAME_LIB}/Source)
add_subdirectory(${EXAMPLE_NAME_LIB})

set(GAME_NAME_LIB Game)
include_directories(${GAME_NAME_LIB}/Source)
add_subdirectory(${GAME_NAME_LIB})

set(PLATFORM_NAME_LIB Platform)
include_directories("${PLATFORM_NAME_LIB}/Source")
#include_directories("${PLATFORM_NAME_LIB}/${BUILD_PALTFORM}/Source")
add_subdirectory(${PLATFORM_NAME_LIB})

# -----------------

# Создание проекта -----------------------------------------------------------------
if (${BUILD_PALTFORM} STREQUAL "WIN32")
	add_executable(${PROJECT_NAME} WIN32  ${ALL_HEADERS} ${ALL_SOURCES} ${ALL_RESOURCES} )
	message("add_executable WIN32")
elseif()
	add_executable(${PROJECT_NAME} EXCLUDE_FROM_ALL  ${ALL_HEADERS} ${ALL_SOURCES} ${ALL_RESOURCES} )
	message("add_executable no WIN32")
endif()

# Подключение библиотеи -----------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}  ${ENGINE_NAME_LIB})
target_link_libraries(${PROJECT_NAME}  ${EXAMPLE_NAME_LIB})
target_link_libraries(${PROJECT_NAME}  ${GAME_NAME_LIB})
target_link_libraries(${PROJECT_NAME}  ${PLATFORM_NAME_LIB})

target_link_libraries(${PROJECT_NAME}  ${FREEIMAGE_NAME_LIB})
target_link_libraries(${PROJECT_NAME}  ${FREETYPE_NAME_LIB})

if (${BUILD_PALTFORM} STREQUAL "WIN32")
	# OpenGL ES
	target_link_libraries(${PROJECT_NAME}  "libEGL")
	target_link_libraries(${PROJECT_NAME}  "libGLESv2")
	
	file(COPY "${THIRD_PARTY_DIR}/GLES/Windows/x86_32/libEGL.dll" DESTINATION Debug)
	file(COPY "${THIRD_PARTY_DIR}/GLES/Windows/x86_32/libGLESv2.dll" DESTINATION Debug)
	
	# FreeImage
	target_link_libraries(${PROJECT_NAME}  "freeImage")
	file(COPY "${THIRD_PARTY_DIR}/FreeImage/Windows/x86_32/FreeImage.dll" DESTINATION Debug)
	
	message("target_link_libraries LIB WIN32")
elseif()
	message("target_link_libraries LIB no WIN32")
endif()

message(" END from make")