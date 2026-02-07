# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/runner/work/device-ui/device-ui/_codeql_build_dir/_deps/lovyangfx-src")
  file(MAKE_DIRECTORY "/home/runner/work/device-ui/device-ui/_codeql_build_dir/_deps/lovyangfx-src")
endif()
file(MAKE_DIRECTORY
  "/home/runner/work/device-ui/device-ui/_codeql_build_dir/_deps/lovyangfx-build"
  "/home/runner/work/device-ui/device-ui/_codeql_build_dir/_deps/lovyangfx-subbuild/lovyangfx-populate-prefix"
  "/home/runner/work/device-ui/device-ui/_codeql_build_dir/_deps/lovyangfx-subbuild/lovyangfx-populate-prefix/tmp"
  "/home/runner/work/device-ui/device-ui/_codeql_build_dir/_deps/lovyangfx-subbuild/lovyangfx-populate-prefix/src/lovyangfx-populate-stamp"
  "/home/runner/work/device-ui/device-ui/_codeql_build_dir/_deps/lovyangfx-subbuild/lovyangfx-populate-prefix/src"
  "/home/runner/work/device-ui/device-ui/_codeql_build_dir/_deps/lovyangfx-subbuild/lovyangfx-populate-prefix/src/lovyangfx-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/runner/work/device-ui/device-ui/_codeql_build_dir/_deps/lovyangfx-subbuild/lovyangfx-populate-prefix/src/lovyangfx-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/runner/work/device-ui/device-ui/_codeql_build_dir/_deps/lovyangfx-subbuild/lovyangfx-populate-prefix/src/lovyangfx-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
