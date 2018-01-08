cmake_minimum_required(VERSION 2.8)

message(" START generatte ${GAME_NAME_LIB}")

set(GAME_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")

# SOURCE

include_directories(${PROJECT_DIR})
include_directories(${GAME_DIR})
include_directories("${PROJECT_DIR}/Platform/Source")
include_directories(${THIRD_PARTY_DIR})

set(GAME_SOURCE_DIR
    .
	GameMini
)

foreach(DIR ${GAME_SOURCE_DIR})
	file(GLOB
		HEADERS
		${GAME_DIR}/${DIR}/*.h
	)

	file(GLOB
		SOURCES
		${GAME_DIR}/${DIR}/*.cpp
	)

	# для имен групп в xcode нужен обратный слэш
	string(REGEX REPLACE "/" "\\\\" GROUP_NAME ${DIR})
	source_group("${GROUP_NAME}" FILES ${HEADERS} ${SOURCES})
	
	#message("SOURCE: " ${HEADERS} ${SOURCES})
	
	list(APPEND ALL_GAME_HEADERS ${HEADERS})
	list(APPEND ALL_GAME_SOURCES ${SOURCES})

endforeach(DIR)

add_library(${GAME_NAME_LIB} STATIC ${ALL_GAME_HEADERS} ${ALL_GAME_SOURCES})

message(" END generatte ${GAME_NAME_LIB}")