#!/bin/bash

for FILENAME in "$@"; do
  SUITE="$(echo -n "$FILENAME" |xargs basename |sed -r 's/(^\s*|\.[^\.]*$)//g;
                                                 s/[-. ~\/]/_/g;
                                                 s/^(.)(.*)/\u\1\2/;
                                                 s/_([a-zA-Z])/\u\1/g')"
  sed -r -e 's/(#include\s*<)(unittest\+\+\/)?UnitTest\+\+\.h/\1gtest\/gtest.h/;
             s/\bTEST\(([^\)]*)\)/TEST('"$SUITE"', \1)/g;
             s/\bCHECK_EQUAL\(([^\)]*)\)/EXPECT_EQ(\1)/;
             s/^(\s*)return\s+UnitTest::RunAllTests\(\)/\1testing::InitGoogleTest(\&argc, argv);\n\1return RUN_ALL_TESTS()/;
             s/\bCHECK\(/EXPECT_TRUE(/g
             s/(<include>.*)unittest\+\+/\1gtest/g
             s/(<linkflags>)(.*)-lUnitTest\+\+(.*)/\1\2-lgtest\3/' $FILENAME
done
