set(CMAKE_CXX_FLAGS "-Wall -pedantic -std=gnu++0x")
set(CMAKE_CXX_FLAGS_DEBUG "-g -pg -Wall -Wextra -pedantic -std=gnu++0x")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Werror -pedantic -std=gnu++0x -s")

set(CMAKE_INCLUDE_CURRENT_DIR ON)


file(GLOB QCLIB_SRC_FILES "QCLib/*.cpp" "QCLib/gates/*.cpp")
file(GLOB QCVIEWER_SRC_FILES "*.cpp")

LIST(APPEND QCLIB_SRC_FILES
	diracLexer.cpp
	diracParser.cpp
	QCParser.cpp
	QCLexer.cpp
	GateParser.cpp
	GateLexer.cpp
)

add_custom_command (
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/QCParser.cpp ${CMAKE_CURRENT_BINARY_DIR}/QCParser.h
	COMMAND bison ${CMAKE_CURRENT_SOURCE_DIR}/QCLib/QCParser.y
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/QCLib/QCParser.y
)
add_custom_command (
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/QCLexer.cpp
	COMMAND flex ${CMAKE_CURRENT_SOURCE_DIR}/QCLib/QCLexer.l
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/QCLib/QCLexer.l
)
add_custom_command (
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/diracLexer.cpp
	COMMAND flex ${CMAKE_CURRENT_SOURCE_DIR}/QCLib/diracLexer.l
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/QCLib/diracLexer.l
)
add_custom_command (
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/diracParser.cpp ${CMAKE_CURRENT_BINARY_DIR}/diracParser.h
	COMMAND bison -d -v -o diracParser.cpp ${CMAKE_CURRENT_SOURCE_DIR}/QCLib/diracParser.y
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/QCLib/diracParser.y
)
add_custom_command (
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/GateLexer.cpp
	COMMAND flex ${CMAKE_CURRENT_SOURCE_DIR}/QCLib/gates/GateLexer.l
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/QCLib/gates/GateLexer.l
)
add_custom_command (
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/GateParser.cpp ${CMAKE_CURRENT_BINARY_DIR}/GateParser.h
	COMMAND bison ${CMAKE_CURRENT_SOURCE_DIR}/QCLib/gates/GateParser.y
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/QCLib/gates/GateParser.y
)

set(MPFR_LIBS "-lmpfr")

if(WIN32)
  set(LCMS_LIBS "-llcms2")
endif(WIN32)


set(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost 1.52.0 REQUIRED COMPONENTS system program_options filesystem)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
set(Boost_USE_STATIC_LIBS ON)

FIND_PACKAGE(Poppler REQUIRED)
LINK_DIRECTORIES(${Poppler_LIBRARY_DIR})
include_directories(${Poppler_INCLUDE_DIR})

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

message(STATUS "gtkmm: ${GTKMM_LIBRARY_DIRS}")
include_directories(${GTKMM_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTKMM_LIBRARY_DIRS})

include_directories(${Freetype_INCLUDE_DIRS})
LINK_DIRECTORIES(${Freetype_LIBRARY_DIRS})

include_directories( ${QCViewer_SOURCE_DIR}/QCLib)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

add_library(qc ${QCLIB_SRC_FILES})
add_executable (qcviewer WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/qcviewer/qcviewer.cpp ${QCVIEWER_SRC_FILES} )
add_executable (qcimage ${CMAKE_CURRENT_SOURCE_DIR}/qcimage/qcimage.cpp)

target_link_libraries(qcviewer qc)
target_link_libraries(qcviewer ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${GTKMM_LIBRARIES} ${MPFR_LIBS} ${LCMS_LIBS})
target_link_libraries(qcimage qc)
target_link_libraries(qcimage ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${GTKMM_LIBRARIES} ${MPFR_LIBS} ${LCMS_LIBS})

install(TARGETS qcviewer DESTINATION ${QCViewer_SOURCE_DIR}/../bin)
install(TARGETS qcimage DESTINATION ${QCViewer_SOURCE_DIR}/../bin)

