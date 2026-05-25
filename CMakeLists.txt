cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

project(DungeonRitual)
project(DungeonRitual_server)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(/home/jakub/sdk/asio-1.24.0/include)
include_directories(/home/jakub/sdk/ServerInfrastructure-1.0.1/ServerInfrastructure)
include_directories(/home/jakub/sdk/rapidxml-1.13)

set(common  common/Data.h
            common/GameMechanics/Actor/ActorController.h    common/GameMechanics/Actor/ActorController.cpp
            common/GameMechanics/Actor/Actor.h              common/GameMechanics/Actor/Actor.cpp
            common/GameMechanics/Actor/OutsideController.h
            common/GameMechanics/Actor/Abilities/Dash.h
            common/GameMechanics/Actor/Abilities/Hit.h
            common/GameMechanics/Actor/Abilities/Ability.h
            common/GameMechanics/Objects/GameObject.h       common/GameMechanics/Objects/GameObject.cpp
            common/GameMechanics/Objects/geometry/Point.h   common/GameMechanics/Objects/geometry/Point.cpp
            common/GameMechanics/Objects/geometry/Segment.h
            common/GameMechanics/Objects/geometry/Geometry.h
            common/GameMechanics/Objects/geometry/Circle.h
            common/GameMechanics/Objects/geometry/Figures.h
            common/GameMechanics/Objects/geometry/Shape.h
            common/GameMechanics/Objects/geometry/Rectangle.h
            common/GameMechanics/Objects/geometry/CircularSector.h
            common/GameMechanics/Objects/geometry/Collisions.h
            common/GameMechanics/Objects/ObjectCollisions.h
            common/GameMechanics/Objects/Slope.h            common/GameMechanics/Objects/Slope.cpp
            common/GameMechanics/PhysicsParameters.h
            common/GameMechanics/Match.h                    common/GameMechanics/Match.cpp
            common/GameMechanics/GameModes/Tag.h
            common/GameMechanics/GameModes/GameMode.h
            common/GameMechanics/Communication/Events/Event.h
            common/GameMechanics/Communication/Events/HitEvent.h
            common/GameMechanics/Communication/Observer.h
            common/GameMechanics/AssetLoader/AssetLoader.h
            common/GameMechanics/MatchEnvironment.h
            common/Network/NetContext.h)

set (dungeon_ritual DungeonRitual/main.cpp
                    DungeonRitual/Display/Animation.h
                    DungeonRitual/Display/SurfacePainter.h
                    DungeonRitual/Display/ActorAnimations.h
                    DungeonRitual/Display/Window.h
                    DungeonRitual/Display/DisplayParameters.h
                    DungeonRitual/Game/LocalMatch.h             DungeonRitual/Game/LocalMatch.cpp
                    DungeonRitual/Game/Game.h                   DungeonRitual/Game/Game.cpp
                    DungeonRitual/Game/Client.h)

set (dungeon_ritual_server  DungeonRitual_server/main.cpp
                            DungeonRitual_server/Server.h
                            DungeonRitual_server/GameClient.h
                            DungeonRitual_server/ServerMatch.h  DungeonRitual_server/ServerMatch.cpp)

# Dungeon Ritual

add_executable(DungeonRitual ${common} ${dungeon_ritual})
target_link_libraries(DungeonRitual PUBLIC ${SDL2_LIBRARIES} m)

# Dungeon Ritual server

# add_executable(DungeonRitual_server ${common} ${dungeon_ritual_server})
# target_link_libraries(DungeonRitual_server PUBLIC ${SDL2_LIBRARIES} m)