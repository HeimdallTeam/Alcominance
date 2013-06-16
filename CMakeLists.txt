
cmake_minimum_required (VERSION 2.8)

project (Alcominance)

add_definitions(-D_LINUX)

find_package(Lua51 REQUIRED)

include_directories(${LUA_INCLUDE_DIR})
include_directories(/home/gejza/github/hoe/HoeCore/include)
include_directories(/home/gejza/github/hoe/Hoe3D/include)
include_directories(/home/gejza/github/hoe/HoeResource/include)
include_directories(/home/gejza/github/hoe/HoeGame/include)

add_subdirectory (src)

