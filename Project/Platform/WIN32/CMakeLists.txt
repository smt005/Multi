cmake_minimum_required(VERSION 2.8)

message(" START generatte ${PLATFORM_NAME_LIB}")

set(PLATFORM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)
set(PLATFORM_LIB_DIR WIN32)

# DEFINES
add_definitions(-DWIN32_RESOURCES_DIR="${RES_DIR}/")

# SOURCE

include_directories(${PROJECT_DIR})
include_directories(${PLATFORM_DIR}/Source)
include_directories(${PLATFORM_DIR}/${PLATFORM_LIB_DIR}/Source)
include_directories(${THIRD_PARTY_DIR})

set(PLATFORM_SOURCE_DIR
	Source
	${PLATFORM_LIB_DIR}/Source
)

foreach(DIR ${PLATFORM_SOURCE_DIR})
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
	
	#message("PLATFORM WIN32: " ${HEADERS} ${SOURCES})
	
	list(APPEND ALL_PLATFORM_HEADERS ${HEADERS})
	list(APPEND ALL_PLATFORM_SOURCES ${SOURCES})

endforeach(DIR)

add_library(${PLATFORM_NAME_LIB} STATIC ${ALL_PLATFORM_HEADERS} ${ALL_PLATFORM_SOURCES})

message(" END generatte ${PLATFORM_NAME_LIB}")