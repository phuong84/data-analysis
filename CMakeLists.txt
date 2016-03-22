############################################
#                                          #
#     CMakeLists.txt for DataAnalysis      #
#                                          #
############################################

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(DATA_ANALYSIS)

set(PROJECT_BINARY_DIR     ${CMAKE_BINARY_DIR}/..)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_FILES_DIRECTORY  ${PROJECT_BINARY_DIR}/build)
set(CMAKE_MODULE_PATH      ${PROJECT_BINARY_DIR}/cmake-modules ${CMAKE_MODULE_PATH})

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})
set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR})

include_directories(
	${PROJECT_BINARY_DIR}/source/Common/include 
	${PROJECT_BINARY_DIR}/source/MCNPAnalysis/include 
	${PROJECT_BINARY_DIR}/source/SpectrumAnalysis/include)

file(GLOB common_headers ${PROJECT_BINARY_DIR}/source/Common/include/*.h)
file(GLOB common_sources ${PROJECT_BINARY_DIR}/source/Common/src/*.cxx)
file(GLOB mcnp_headers   ${PROJECT_BINARY_DIR}/source/MCNPAnalysis/include/*.h)
file(GLOB mcnp_sources   ${PROJECT_BINARY_DIR}/source/MCNPAnalysis/src/*.cxx)
file(GLOB spec_headers   ${PROJECT_BINARY_DIR}/source/SpectrumAnalysis/include/*.h)
file(GLOB spec_sources   ${PROJECT_BINARY_DIR}/source/SpectrumAnalysis/src/*.cxx)

add_library   (Common     STATIC       ${common_sources} ${common_headers})
add_executable(MCNPAnalysis            ${mcnp_sources} ${mcnp_headers})
add_executable(SpectrumAnalysis        ${spec_sources} ${spec_headers})
target_link_libraries(MCNPAnalysis     Common ${ROOT_LIBRARIES} -lTreePlayer -lMinuit -lSpectrum)
target_link_libraries(SpectrumAnalysis Common ${ROOT_LIBRARIES} -lTreePlayer -lMinuit -lSpectrum)

add_custom_target(mcnp DEPENDS MCNPAnalysis)
add_custom_target(spec DEPENDS SpectrumAnalysis)

