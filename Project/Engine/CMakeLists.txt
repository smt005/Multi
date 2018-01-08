cmake_minimum_required(VERSION 2.8)

message(" START generatte ${ENGINE_NAME_LIB}")

set(ENGINE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")

# SOURCE

include_directories(${PROJECT_DIR})
include_directories(${ENGINE_DIR})
include_directories("${PROJECT_DIR}/Platform/Source")
include_directories(${THIRD_PARTY_DIR})

set(ENGINE_SOURCE_DIR
    .
	Callback
	Common
	Draw
	Object
)

foreach(DIR ${ENGINE_SOURCE_DIR})
	file(GLOB
		HEADERS
		${ENGINE_DIR}/${DIR}/*.h
	)

	file(GLOB
		SOURCES
		${ENGINE_DIR}/${DIR}/*.cpp
	)

	# для имен групп в xcode нужен обратный слэш
	string(REGEX REPLACE "/" "\\\\" GROUP_NAME ${DIR})
	source_group("${GROUP_NAME}" FILES ${HEADERS} ${SOURCES})
	
	#message("SOURCE: " ${HEADERS} ${SOURCES})
	
	list(APPEND ALL_ENGINE_HEADERS ${HEADERS})
	list(APPEND ALL_ENGINE_SOURCES ${SOURCES})

endforeach(DIR)

add_library(${ENGINE_NAME_LIB} STATIC ${ALL_ENGINE_HEADERS} ${ALL_ENGINE_SOURCES})

message(" END generatte ${ENGINE_NAME_LIB}")