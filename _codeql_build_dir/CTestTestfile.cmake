# CMake generated Testfile for 
# Source directory: /home/runner/work/device-ui/device-ui
# Build directory: /home/runner/work/device-ui/device-ui/_codeql_build_dir
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(tests "/home/runner/work/device-ui/device-ui/_codeql_build_dir/bin/tests")
set_tests_properties(tests PROPERTIES  _BACKTRACE_TRIPLES "/home/runner/work/device-ui/device-ui/CMakeLists.txt;83;add_test;/home/runner/work/device-ui/device-ui/CMakeLists.txt;0;")
subdirs("_deps/portduino-build")
subdirs("_deps/lovyangfx-build")
subdirs("_deps/lvgl-build")
subdirs("_deps/nanopb-build")
subdirs("_deps/doctest-build")
