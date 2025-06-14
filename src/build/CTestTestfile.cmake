# CMake generated Testfile for 
# Source directory: C:/PROGA/PROJECT/cpp_project/src
# Build directory: C:/PROGA/PROJECT/cpp_project/src/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[project_tests]=] "C:/PROGA/PROJECT/cpp_project/src/build/Debug/project_tests.exe")
  set_tests_properties([=[project_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/PROGA/PROJECT/cpp_project/src/CMakeLists.txt;58;add_test;C:/PROGA/PROJECT/cpp_project/src/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[project_tests]=] "C:/PROGA/PROJECT/cpp_project/src/build/Release/project_tests.exe")
  set_tests_properties([=[project_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/PROGA/PROJECT/cpp_project/src/CMakeLists.txt;58;add_test;C:/PROGA/PROJECT/cpp_project/src/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test([=[project_tests]=] "C:/PROGA/PROJECT/cpp_project/src/build/MinSizeRel/project_tests.exe")
  set_tests_properties([=[project_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/PROGA/PROJECT/cpp_project/src/CMakeLists.txt;58;add_test;C:/PROGA/PROJECT/cpp_project/src/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[project_tests]=] "C:/PROGA/PROJECT/cpp_project/src/build/RelWithDebInfo/project_tests.exe")
  set_tests_properties([=[project_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/PROGA/PROJECT/cpp_project/src/CMakeLists.txt;58;add_test;C:/PROGA/PROJECT/cpp_project/src/CMakeLists.txt;0;")
else()
  add_test([=[project_tests]=] NOT_AVAILABLE)
endif()
subdirs("_deps/googletest-build")
subdirs("sfml-build")
