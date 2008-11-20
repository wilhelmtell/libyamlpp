#!/bin/bash
###############################################################################
# Build libyaml++.  Grabs and sets up Boost.Build if necessary.
#
# Requires:  bash, wget, tar, rm, grep
###############################################################################

if [ \! -d boost-build ]; then
  echo "Downloading Boost.Build ..."
  wget -q http://prdownloads.sourceforge.net/boost/boost-build-2.0-m12.tar.bz2
  echo "Extracting Boost.Build ..."
  tar jxf boost-build-2.0-m12.tar.bz2
  rm -f boost-build-2.0-m12.tar.bz2
  cd boost-build/jam_src
  echo "Building Boost.Build ..."
  ./build.sh >/dev/null 2>&1
  cd - >/dev/null 2>&1
fi
echo "Configuring Boost.Build ..."
grep -q '^using gcc ;' boost-build/user-config.jam

for TOOL in gcc doxygen ; do
  grep -q "^using $TOOL ;" boost-build/user-config.jam
  if [ $? -ne 0 ]; then
    echo "using $TOOL ;" >> boost-build/user-config.jam
  fi
done

if [ -z "$BOOST_BUILD_PATH" ]; then
  export BOOST_BUILD_PATH="$(pwd)/boost-build"
fi

echo "Building libyaml++ ..."
(boost-build/jam_src/bin.linuxx86/bjam libyaml++ check demo) >/dev/null 2>&1
echo
echo "###############################################################################"
echo "# DONE KTHXBAI                                                                #"
echo "###############################################################################"
