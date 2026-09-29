# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/workspaces/2026-Game-Project/debug-build/_deps/sdl3-src"
  "/workspaces/2026-Game-Project/debug-build/_deps/sdl3-build"
  "/workspaces/2026-Game-Project/debug-build/_deps/sdl3-subbuild/sdl3-populate-prefix"
  "/workspaces/2026-Game-Project/debug-build/_deps/sdl3-subbuild/sdl3-populate-prefix/tmp"
  "/workspaces/2026-Game-Project/debug-build/_deps/sdl3-subbuild/sdl3-populate-prefix/src/sdl3-populate-stamp"
  "/workspaces/2026-Game-Project/debug-build/_deps/sdl3-subbuild/sdl3-populate-prefix/src"
  "/workspaces/2026-Game-Project/debug-build/_deps/sdl3-subbuild/sdl3-populate-prefix/src/sdl3-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/workspaces/2026-Game-Project/debug-build/_deps/sdl3-subbuild/sdl3-populate-prefix/src/sdl3-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/workspaces/2026-Game-Project/debug-build/_deps/sdl3-subbuild/sdl3-populate-prefix/src/sdl3-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
