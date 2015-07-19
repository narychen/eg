# CMake generated Testfile for 
# Source directory: /home/nary/egserv/src/mysub/langtest/cppformat-1.1.0/test
# Build directory: /home/nary/egserv/src/mysub/langtest/cppformat-1.1.0/test
# 
# This file includes the relevent testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
ADD_TEST(gtest-extra-test "/home/nary/egserv/src/mysub/langtest/cppformat-1.1.0/bin/gtest-extra-test")
ADD_TEST(format-test "/home/nary/egserv/src/mysub/langtest/cppformat-1.1.0/bin/format-test")
ADD_TEST(format-impl-test "/home/nary/egserv/src/mysub/langtest/cppformat-1.1.0/bin/format-impl-test")
ADD_TEST(printf-test "/home/nary/egserv/src/mysub/langtest/cppformat-1.1.0/bin/printf-test")
ADD_TEST(util-test "/home/nary/egserv/src/mysub/langtest/cppformat-1.1.0/bin/util-test")
ADD_TEST(posix-test "/home/nary/egserv/src/mysub/langtest/cppformat-1.1.0/bin/posix-test")
ADD_TEST(compile-test "/usr/bin/ctest" "--build-and-test" "/home/nary/egserv/src/mysub/langtest/cppformat-1.1.0/test/compile-test" "/home/nary/egserv/src/mysub/langtest/cppformat-1.1.0/test/compile-test" "--build-generator" "Unix Makefiles" "--build-makeprogram" "/usr/bin/make")
