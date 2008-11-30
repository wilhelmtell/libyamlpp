#!/bin/bash
###############################################################################
# Build libyaml++.  Grabs and sets up Boost.Build if necessary.
#
# Requires:  wget, bash, gnu core-utils
#            Unless Boost.Build and GoogleTest are installed, an internet
#            connection is required as well.
###############################################################################

ORIGINAL_DIR=$(pwd)
LIBYAMLPP_DIR="$ORIGINAL_DIR/$(dirname $0)"
BUILD_SH_CONF=$LIBYAMLPP_DIR/.build
LOG=$(pwd)/build.log && cat /dev/null >$LOG

fail() {
  echo -e "\033[0;31mFAIL\033[0m"
  echo -e "FAIL" >>$LOG
  cleanup_mess
  exit 1;
}

succ() {
  if [ "$#" -gt 0 ]; then
    echo -e "\033[0;32m$1\033[0m"
    echo -e "$1" >>$LOG
  else
    echo -e "\033[0;32mOK\033[0m"
    echo -e "OK" >>$LOG
  fi
}

# installation directory may be wiped out later, so it must be a dedicated
# directory.
echo -n "Determining install directory of helper tools ...  "
if [ -r $BUILD_SH_CONF ]; then
  INSTALL_DIR="$(grep -i '^\s*install_dir\s*=' "$BUILD_SH_CONF" |cut -d= -f2)"
  INSTALL_DIR="$(echo "$INSTALL_DIR" |sed 's/^\s*\|\s*$//g')"
else
  INSTALL_DIR=$(mktemp -t -d libyamlpp.XXXXXXXXXX)
  cat <<EOF >>"$BUILD_SH_CONF"
# WARNING:  The installation directory MUST be a dedicated directory!  It may
#           be WIPED OUT by the build.sh script when it's done!
EOF
  echo "install_dir=$INSTALL_DIR" >>"$BUILD_SH_CONF"
fi
succ $INSTALL_DIR

trap cleanup_mess 1 2 3 4 5 6 14 15

cleanup_mess() {
  echo -e -n "Cleaning up ...  " | tee -a $LOG
  cd $ORIGINAL_DIR
  rm -rf $INSTALL_DIR

  # in case tarballs were not moved successfully
  rm -rf $ORIGINAL_DIR/gtest-1.1.0
  rm -rf $ORIGINAL_DIR/boost-build
  succ
}

# running "./build.sh wipeclean", with no other arguments, will wipe clean the
# distribution and mess this script might have done in the past.
if [ "$#" -eq 1 -a "$1" = "wipeclean" ]; then
  cleanup_mess
  echo -e -n "Wiping clean ...  "
  rm -rf /tmp/boost-build-2.0-m12.tar.bz2
  rm -rf /tmp/gtest-1.1.0.tar.bz2
  rm -rf "$BUILD_SH_CONF"
  cd $LIBYAMLPP_DIR
  find . -name bin -type d |xargs rm -rf
  cd $ORIGINAL_DIR
  succ
  rm -f $LOG
  echo -e "\n\033[0;32mDone.\033[0m"
  exit 0
fi

mkdir -p "$INSTALL_DIR"

# boost.build
which bjam >/dev/null 2>&1
if [ $? -ne 0 -a \! -d $INSTALL_DIR/boost-build ]; then
  if [ \! -r /tmp/boost-build-2.0-m12.tar.bz2 ]; then
    echo -n "Downloading Boost.Build into /tmp ...  " |tee -a $LOG
    cd /tmp
    wget -q http://prdownloads.sourceforge.net/boost/boost-build-2.0-m12.tar.bz2 >>$LOG 2>&1 && succ || fail
    cd - >/dev/null 2>&1
  fi
  echo -n "Verifying Boost.Build tarball integrity ...  " |tee -a $LOG
  [ "$(md5sum /tmp/boost-build-2.0-m12.tar.bz2 |cut -d' ' -f1)"=38a40f1c0c2d6eb4f14aa4cf52e9236a ] && succ || fail
  echo -n "Extracting Boost.Build ...  " |tee -a $LOG
  tar jxf /tmp/boost-build-2.0-m12.tar.bz2 && succ || fail
  echo -n "Installing Boost.Build in $INSTALL_DIR ...  " |tee -a $LOG
  mv boost-build $INSTALL_DIR/ && succ || fail
  cd $INSTALL_DIR/boost-build/jam_src
  echo -n "Building Boost.Jam ...  " |tee -a $LOG
  ./build.sh >>$LOG 2>&1 && succ || fail
  cd - >/dev/null 2>&1

  for TOOL in gcc doxygen ; do
    grep -q "^using $TOOL ;" $INSTALL_DIR/boost-build/user-config.jam
    if [ $? -ne 0 ]; then
      echo -n "Configuring Boost.Build for $TOOL ...  " |tee -a $LOG
      echo "using $TOOL ;" >>$INSTALL_DIR/boost-build/user-config.jam && succ || fail
    fi
  done
fi

echo -e -n "Determinig path to bjam executable ...  " |tee -a $LOG
export BJAM=$(which bjam 2>/dev/null)
if [ -z "$BJAM" ]; then
  export BJAM=$(find $INSTALL_DIR/boost-build -name bjam)
  export BOOST_BUILD_PATH="$INSTALL_DIR/boost-build"
fi
[ -n "$BJAM" ] && succ $BJAM || fail

# gtest
if [ \! -r $INSTALL_DIR/include/gtest/gtest.h ]; then
  if [ \! -r /tmp/gtest-1.1.0.tar.bz2 ]; then
    echo -n "Downloading GoogleTest into /tmp ...  " |tee -a $LOG
    cd /tmp
    wget -q http://googletest.googlecode.com/files/gtest-1.1.0.tar.bz2 && succ || fail
    cd - >/dev/null 2>&1
  fi
  echo -n "Verifying GoogleTest tarball integrity ...  " |tee -a $LOG
  [ "$(md5sum /tmp/gtest-1.1.0.tar.bz2 |cut -d' ' -f1)"=aaec092aa4ac969ee16a1baddd0fa9ae ] && succ || fail
  echo -n "Extracting GoogleTest ...  " |tee -a $LOG
  tar jxf /tmp/gtest-1.1.0.tar.bz2 && succ || fail
  cd gtest-1.1.0
  echo -n "Patching GoogleTest to fix PATH_MAX compiler error ...  " |tee -a $LOG
  sed -i '44 i #include <limits.h>' src/gtest-filepath.cc && succ || fail
  echo -n "Configuring GoogleTest build ...  " |tee -a $LOG
  ./configure --prefix $INSTALL_DIR >>$LOG 2>&1 && succ || fail
  echo -n "Building GoogleTest ...  " |tee -a $LOG
  make >>$LOG 2>&1 && succ || fail
  echo -n "Installing GoogleTest in $INSTALL_DIR ...  " |tee -a $LOG
  make install >>$LOG 2>&1 && succ || fail
  cd - >/dev/null 2>&1
  rm -rf gtest-1.1.0
  export LD_LIBRARY_PATH=$INSTALL_DIR/lib
  export LIBRARY_PATH=$INSTALL_DIR/lib
  export CPLUS_INCLUDE_DIR=$INSTALL_DIR/include
fi

# libyaml++
echo -n "Building libyaml++$(if [ "$#" -gt 0 ]; then echo " targets $@"; fi) ...  " |tee -a $LOG
if echo "$@" |grep -i -v 'release\|debug' >/dev/null 2>&1; then
  BJAM_ARGS="$(echo "release $@" |sed 's/^\s*\|\s*$//g')"
else
  BJAM_ARGS="$@"
fi
"$BJAM" $BJAM_ARGS >>$LOG 2>&1 && succ || fail
echo -e "\n\033[0;32mDone.\033[0m"
echo -e "\nDone." >>$LOG
