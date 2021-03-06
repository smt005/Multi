cmake_minimum_required(VERSION 2.8)

message(" START generatte ${EXAMPLE_NAME_LIB}")

set(EXAMPLE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")

# SOURCE

include_directories(${PROJECT_DIR})
include_directories(${EXAMPLE_DIR})
include_directories(${THIRD_PARTY_DIR})

set(EXAMPLE_SOURCE_DIR
    .
	Draw
	Classes
)

foreach(DIR ${EXAMPLE_SOURCE_DIR})
	file(GLOB
		HEADERS
		${EXAMPLE_DIR}/${DIR}/*.h
	)

	file(GLOB
		SOURCES
		${EXAMPLE_DIR}/${DIR}/*.cpp
	)

	# для имен групп в xcode нужен обратный слэш
	string(REGEX REPLACE "/" "\\\\" GROUP_NAME ${DIR})
	source_group("${GROUP_NAME}" FILES ${HEADERS} ${SOURCES})
	
	#message("SOURCE: " ${HEADERS} ${SOURCES})
	
	list(APPEND ALL_EXAMPLE_HEADERS ${HEADERS})
	list(APPEND ALL_EXAMPLE_SOURCES ${SOURCES})

endforeach(DIR)

IF (WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
ENDIF()

add_library(${EXAMPLE_NAME_LIB} STATIC ${ALL_EXAMPLE_HEADERS} ${ALL_EXAMPLE_SOURCES})
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

message(" END generatte ${EXAMPLE_NAME_LIB}")