# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/workspaces/2026-Game-Project/debug-build/_deps/sdl3_image-src"
  "/workspaces/2026-Game-Project/debug-build/_deps/sdl3_image-build"
  "/workspaces/2026-Game-Project/debug-build/_deps/sdl3_image-subbuild/sdl3_image-populate-prefix"
  "/workspaces/2026-Game-Project/debug-build/_deps/sdl3_image-subbuild/sdl3_image-populate-prefix/tmp"
  "/workspaces/2026-Game-Project/debug-build/_deps/sdl3_image-subbuild/sdl3_image-populate-prefix/src/sdl3_image-populate-stamp"
  "/workspaces/2026-Game-Project/debug-build/_deps/sdl3_image-subbuild/sdl3_image-populate-prefix/src"
  "/workspaces/2026-Game-Project/debug-build/_deps/sdl3_image-subbuild/sdl3_image-populate-prefix/src/sdl3_image-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/workspaces/2026-Game-Project/debug-build/_deps/sdl3_image-subbuild/sdl3_image-populate-prefix/src/sdl3_image-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/workspaces/2026-Game-Project/debug-build/_deps/sdl3_image-subbuild/sdl3_image-populate-prefix/src/sdl3_image-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
