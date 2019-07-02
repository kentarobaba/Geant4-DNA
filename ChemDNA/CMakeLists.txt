cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(ChemDNA)

option(WITH_GEANT4_UIVIS "Build g4ChemDNA with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(g4ChemDNA g4ChemDNA.cc ${sources} ${headers})
target_link_libraries(g4ChemDNA ${Geant4_LIBRARIES})

set(G4CHEMDNA_SCRIPTS
  g4ChemDNA.in
  g4ChemDNA.out
  ini.mac
  vis.mac
  )

foreach(_script ${G4CHEMDNA_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

add_custom_target(ChemDNA DEPENDS g4ChemDNA)

install(TARGETS g4ChemDNA DESTINATION bin)


