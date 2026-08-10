# CMake generated Testfile for 
# Source directory: /workspaces/Game-Engine-Core/Cobalt2D
# Build directory: /workspaces/Game-Engine-Core/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[cobalt_test_math]=] "/workspaces/Game-Engine-Core/build/cobalt_test_math")
set_tests_properties([=[cobalt_test_math]=] PROPERTIES  _BACKTRACE_TRIPLES "/workspaces/Game-Engine-Core/Cobalt2D/CMakeLists.txt;91;add_test;/workspaces/Game-Engine-Core/Cobalt2D/CMakeLists.txt;0;")
add_test([=[cobalt_test_scene]=] "/workspaces/Game-Engine-Core/build/cobalt_test_scene")
set_tests_properties([=[cobalt_test_scene]=] PROPERTIES  _BACKTRACE_TRIPLES "/workspaces/Game-Engine-Core/Cobalt2D/CMakeLists.txt;91;add_test;/workspaces/Game-Engine-Core/Cobalt2D/CMakeLists.txt;0;")
add_test([=[cobalt_test_runtime]=] "/workspaces/Game-Engine-Core/build/cobalt_test_runtime")
set_tests_properties([=[cobalt_test_runtime]=] PROPERTIES  _BACKTRACE_TRIPLES "/workspaces/Game-Engine-Core/Cobalt2D/CMakeLists.txt;91;add_test;/workspaces/Game-Engine-Core/Cobalt2D/CMakeLists.txt;0;")
add_test([=[cobalt_test_renderer]=] "/workspaces/Game-Engine-Core/build/cobalt_test_renderer")
set_tests_properties([=[cobalt_test_renderer]=] PROPERTIES  _BACKTRACE_TRIPLES "/workspaces/Game-Engine-Core/Cobalt2D/CMakeLists.txt;91;add_test;/workspaces/Game-Engine-Core/Cobalt2D/CMakeLists.txt;0;")
add_test([=[cobalt_test_upgrade]=] "/workspaces/Game-Engine-Core/build/cobalt_test_upgrade")
set_tests_properties([=[cobalt_test_upgrade]=] PROPERTIES  _BACKTRACE_TRIPLES "/workspaces/Game-Engine-Core/Cobalt2D/CMakeLists.txt;91;add_test;/workspaces/Game-Engine-Core/Cobalt2D/CMakeLists.txt;0;")
subdirs("_deps/box2d-build")
