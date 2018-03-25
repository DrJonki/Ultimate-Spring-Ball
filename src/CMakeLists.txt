set(INCDIR ${CMAKE_SOURCE_DIR}/include/WTF)
set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/WTF)

#########################
#     SOURCES BELOW     #
#########################

set(SRC_ROOT
  ${INCDIR}/Macros.hpp
)
source_group("WTF" FILES ${SRC_ROOT})

# Scene
set(SRC_SCENE
  ${INCDIR}/Scene/Component.hpp
  ${INCDIR}/Scene/Drawable.hpp
  ${INCDIR}/Scene/Drawable.inl
  ${INCDIR}/Scene/Entity.hpp
  ${INCDIR}/Scene/Entity.inl
  ${INCDIR}/Scene/Layer.hpp
  ${INCDIR}/Scene/Layer.inl
  ${INCDIR}/Scene/Scene.hpp

  ${SRCDIR}/Scene/Component.cpp
  ${SRCDIR}/Scene/Entity.cpp
  ${SRCDIR}/Scene/Layer.cpp
  ${SRCDIR}/Scene/Scene.cpp
)
source_group("WTF\\Scene" FILES ${SRC_SCENE})

# System
set(SRC_SYSTEM
  ${INCDIR}/System/Engine.hpp
  ${INCDIR}/System/Engine.inl
  ${INCDIR}/System/Config.hpp
  ${INCDIR}/System/Network.hpp
  ${INCDIR}/System/Resource.hpp

  ${SRCDIR}/System/Engine.cpp
  ${SRCDIR}/System/Config.cpp
  ${SRCDIR}/System/Network.cpp
  ${SRCDIR}/System/Resource.cpp
)
source_group("WTF\\System" FILES ${SRC_SYSTEM})

# Util
set(SRC_UTIL
  ${INCDIR}/Util/Randomizer.hpp
  ${INCDIR}/Util/Randomizer.inl

  ${SRCDIR}/Util/Randomizer.cpp
)
source_group("WTF\\Util" FILES ${SRC_UTIL})

# Server
set(INCDIR ${CMAKE_SOURCE_DIR}/include/Server)
set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/Server)
set(SRC_SERVER
  ${INCDIR}/Bot.hpp
  ${INCDIR}/Player.hpp
  ${INCDIR}/Room.hpp
  ${INCDIR}/Server.hpp
  ${INCDIR}/Socket.hpp
  ${INCDIR}/Level/Level.hpp

  ${SRCDIR}/Bot.cpp
  ${SRCDIR}/Player.cpp
  ${SRCDIR}/Room.cpp
  ${SRCDIR}/Server.cpp
  ${SRCDIR}/Socket.cpp
  ${SRCDIR}/Level/Level.cpp
)
source_group("Server" FILES ${SRC_SERVER})

# Game
set(INCDIR ${CMAKE_SOURCE_DIR}/include/Game)
set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/Game)
set(SRC_GAME
  ${SRCDIR}/main.cpp

  ${INCDIR}/Entities/Obstacle.hpp
  ${INCDIR}/Entities/PhysicsBody.hpp
  ${INCDIR}/Entities/PhysicsBody.inl
  ${INCDIR}/Entities/Wall.hpp
  ${INCDIR}/Scenes/Base.hpp
  ${INCDIR}/Scenes/Game.hpp
  ${INCDIR}/Scenes/Title.hpp

  ${SRCDIR}/Entities/Obstacle.cpp
  ${SRCDIR}/Entities/Wall.cpp
  ${SRCDIR}/Scenes/Base.cpp
  ${SRCDIR}/Scenes/Game.cpp
  ${SRCDIR}/Scenes/Title.cpp
)
source_group("Game" FILES ${SRC_GAME})

set(SRC
  ${SRC_ROOT}
  ${SRC_SCENE}
  ${SRC_SYSTEM}
  ${SRC_UTIL}
  ${SRC_SERVER}
  ${SRC_GAME}
)

#########################
#      SOURCES END      #
#########################

add_executable(WTF ${SRC})
add_dependencies(WTF SFML GLM JSON Chipmunk)
add_definitions(-DSFML_STATIC)

if (BUILD_SHARED_LIBS)
  add_definitions(-DWTF_EXPORTS)
else()
  add_definitions(-DWTF_STATIC)
endif()

set(CMK_SLP ${CMAKE_STATIC_LIBRARY_PREFIX})
set(CMK_SLS ${CMAKE_STATIC_LIBRARY_SUFFIX})
ExternalProject_Get_Property(SFML BINARY_DIR)
ExternalProject_Get_Property(SFML SOURCE_DIR)

set(SFML_BIN_DIR ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR})
target_link_libraries(WTF
  # SFML libs
  debug ${SFML_BIN_DIR}/${CMK_SLP}sfml-audio-s-d${CMK_SLS}
  debug ${SFML_BIN_DIR}/${CMK_SLP}sfml-graphics-s-d${CMK_SLS}
  debug ${SFML_BIN_DIR}/${CMK_SLP}sfml-network-s-d${CMK_SLS}
  debug ${SFML_BIN_DIR}/${CMK_SLP}sfml-window-s-d${CMK_SLS}
  debug ${SFML_BIN_DIR}/${CMK_SLP}sfml-system-s-d${CMK_SLS}
)
target_link_libraries(WTF
  # SFML libs
  optimized ${SFML_BIN_DIR}/${CMK_SLP}sfml-audio-s${CMK_SLS}
  optimized ${SFML_BIN_DIR}/${CMK_SLP}sfml-graphics-s${CMK_SLS}
  optimized ${SFML_BIN_DIR}/${CMK_SLP}sfml-network-s${CMK_SLS}
  optimized ${SFML_BIN_DIR}/${CMK_SLP}sfml-window-s${CMK_SLS}
  optimized ${SFML_BIN_DIR}/${CMK_SLP}sfml-system-s${CMK_SLS}
)

target_link_libraries(WTF
  debug ${SFML_BIN_DIR}/${CMK_SLP}sfml-main-d${CMK_SLS}
)
target_link_libraries(WTF
  optimized ${SFML_BIN_DIR}/${CMK_SLP}sfml-main${CMK_SLS}
)

set(SFML_EXT_DIR ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86)

target_link_libraries(WTF
  # SFML dependencies
  ${SFML_EXT_DIR}/flac.lib
  ${SFML_EXT_DIR}/freetype.lib
  ${SFML_EXT_DIR}/ogg.lib
  ${SFML_EXT_DIR}/openal32.lib
  ${SFML_EXT_DIR}/vorbis.lib
  ${SFML_EXT_DIR}/vorbisenc.lib
  ${SFML_EXT_DIR}/vorbisfile.lib

  opengl32.lib
  winmm.lib
  gdi32.lib
  ws2_32.lib
  Wldap32.lib
)

# Chipmunk
ExternalProject_Get_Property(Chipmunk BINARY_DIR)
set(CHIP_BIN_DIR ${BINARY_DIR}/src/${CMAKE_CFG_INTDIR})

target_link_libraries(WTF ${CHIP_BIN_DIR}/${CMK_SLP}chipmunk${CMK_SLS})


# Copy assets
add_custom_target(wtf_assets ALL DEPENDS asset_copy)
add_custom_command(OUTPUT asset_copy PRE_BUILD
  DEPENDS always_rebuild
  
  COMMAND ${CMAKE_COMMAND} -E copy
          ${SOURCE_DIR}/extlibs/bin/x86/openal32.dll
          ${CMAKE_BINARY_DIR}/src/openal32.dll

  COMMAND ${CMAKE_COMMAND} -E copy
          ${SOURCE_DIR}/extlibs/bin/x86/openal32.dll
          ${CMAKE_BINARY_DIR}/src/${CMAKE_CFG_INTDIR}/openal32.dll

  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets
          ${CMAKE_BINARY_DIR}/src/${CMAKE_CFG_INTDIR}/assets

  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets
          ${CMAKE_BINARY_DIR}/src/assets
)

add_custom_command(OUTPUT always_rebuild COMMAND cmake -E echo)
add_dependencies(wtf_assets WTF)
