cmake_minimum_required(VERSION 2.8)

message(" START from make")

# Проект ---------------------------------------------------------------------------------

set(PROJECT_COPYRIGHT "Copyright (c) smt005 2017")
set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")

add_definitions(-D"BUILD_${BUILD_PALTFORM}")

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")

# SRC

set(SOURCE_DIRS
    .
	Classes
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
	
	list(APPEND ALL_HEADERS ${HEADERS})
	list(APPEND ALL_SOURCES ${SOURCES})

endforeach(DIR)
		
# RES

set(RESOURCE_DIRS
    .
	Font
)

foreach(DIR ${RESOURCE_DIRS})
	file(GLOB
		RESOURCE
		${RES_DIR}/${DIR}/*.ttf
	)

	# для имен групп в xcode нужен обратный слэш
	string(REGEX REPLACE "/" "\\\\" GROUP_NAME ${DIR})
	source_group("${GROUP_NAME}" FILES ${RESOURCE})
	
	message("RESOURCE: " ${RESOURCE})
	
	list(APPEND ALL_RESOURCES ${RESOURCE})

endforeach(DIR)
		
# Подключение библиотек -----------------------------------------------------------------

# freetype
set(FREETYPE_NAME_LIB freetype)
set(FREETYPE_DIR "ThirdParty/${FREETYPE_NAME_LIB}")
include_directories("${FREETYPE_DIR}/include")
add_subdirectory(${FREETYPE_DIR})

# OpenGL es
include_directories("${THIRD_PARTY_DIR}/GLES/include")
link_directories("${THIRD_PARTY_DIR}/GLES/Windows/x86_32")

# Создание проекта -----------------------------------------------------------------
if (${BUILD_PALTFORM} STREQUAL "WIN32")
	add_executable(${PROJECT_NAME} WIN32  ${ALL_HEADERS} ${ALL_SOURCES} ${ALL_RESOURCES} )
	message("add_executable WIN32")
elseif()
	add_executable(${PROJECT_NAME} EXCLUDE_FROM_ALL  ${ALL_HEADERS} ${ALL_SOURCES} ${ALL_RESOURCES} )
	message("add_executable no WIN32")
endif()

# Подключение библиотеи -----------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}  ${FREETYPE_NAME_LIB})

if (${BUILD_PALTFORM} STREQUAL "WIN32")
	target_link_libraries(${PROJECT_NAME}  "libEGL")
	target_link_libraries(${PROJECT_NAME}  "libGLESv2")
	
	file(COPY "${THIRD_PARTY_DIR}/GLES/Windows/x86_32/libEGL.dll" DESTINATION Debug)
	file(COPY "${THIRD_PARTY_DIR}/GLES/Windows/x86_32/libGLESv2.dll" DESTINATION Debug)
	
	message("target_link_libraries GLES WIN32")
elseif()
	message("target_link_libraries no GLES WIN32")
endif()

message(" END from make")