set(SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

add_executable(Game
  ${SRC}
)

target_link_libraries(Game WTF)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "game")
set_target_properties(Game PROPERTIES
  LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS"
)
