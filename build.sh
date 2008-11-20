#!/bin/bash
###############################################################################
# Build libyaml++.  Grabs and sets up Boost.Build if necessary.
#
# Requires:  wget, bash, gnu core-utils
#            Unless Boost.Build and GoogleTest are installed, an internet
#            connection is required as well.
###############################################################################

fail() {
  echo -e "\033[0;31mFAIL\033[0m"
  exit 1;
}

succ() {
  echo -e "\033[0;32mOK\033[0m"
}

which bjam >/dev/null 2>&1
if [ $? -ne 0 -o \! -d boost-build ]; then
  echo -n "Downloading Boost.Build ...  "
  wget -q http://prdownloads.sourceforge.net/boost/boost-build-2.0-m12.tar.bz2 && succ || fail
  echo -n "Extracting Boost.Build ...  "
  tar jxf boost-build-2.0-m12.tar.bz2 && succ || fail
  rm -f boost-build-2.0-m12.tar.bz2
  cd boost-build/jam_src
  echo -n "Building Boost.Jam ...  "
  ./build.sh >/dev/null 2>&1 && succ || fail
  cd - >/dev/null 2>&1
fi

if [ -z "$BOOST_BUILD_PATH" ]; then
  export BOOST_BUILD_PATH="$(pwd)/boost-build"
fi

for TOOL in gcc doxygen ; do
  grep -q "^using $TOOL ;" boost-build/user-config.jam
  if [ $? -ne 0 ]; then
    echo -n "Configuring Boost.Build for $TOOL ...  "
    echo "using $TOOL ;" >> boost-build/user-config.jam && succ || fail
  fi
done

echo -n "Building libyaml++ ...  "
(boost-build/jam_src/bin.linuxx86/bjam libyaml++ check demo) >/dev/null 2>&1 && succ || fail
echo -e "\nDone."
