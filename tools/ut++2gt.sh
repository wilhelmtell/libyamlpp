#!/bin/bash
###############################################################################
# Convert UnitTest++ code to GoogleTest code
#
# Convert API code as well as Jamfiles.
###############################################################################

BIN=$(basename $0)
BIN_NAME=$(echo $BIN |sed 's/\.[^.]*$//')
if [ $# -eq 0 ]; then
  cat <<EOF
$BIN_NAME translates UnitTest++ code into GoogleTest code.
$BIN_NAME also handles some UnitTest++ constructs in Boost.Jam files.

Just try and see.  No (significant) harm will be done.

Usage:  $BIN <filename>*

Output is directed to stdout.
EOF
  exit -1
fi

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
