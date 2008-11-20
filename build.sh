#!/bin/bash
###############################################################################
# Build libyaml++.  Grabs and sets up Boost.Build if necessary.
#
# Requires:  wget, bash, gnu core-utils
#            Unless Boost.Build and GoogleTest are installed, an internet
#            connection is required as well.
###############################################################################

# WARNING:  The installation directory MUST be a dedicated directory!  It will
#           be WIPED OUT at the end of an unseccessful build!
INSTALL_DIR=$(mktemp -t -d libyamlpp.XXXXXXXXXX)
LOG=$(pwd)/build.log
ORIGINAL_DIR=$(pwd)

trap cleanup_mess 1 2 3 4 5 6 14 15

cleanup_mess() {
  echo -e -n "Cleaning Up ...  " | tee -a $LOG
  cd $ORIGINAL_DIR
  rm -rf $INSTALL_DIR
  rm -rf $ORIGINAL_DIR/gtest-1.1.0
  rm -rf $ORIGINAL_DIR/boost-build
  succ
}

fail() {
  echo -e "\033[0;31mFAIL\033[0m"
  echo -e "FAIL" >>$LOG
  cleanup_mess
  exit 1;
}

succ() {
  echo -e "\033[0;32mOK\033[0m"
  echo -e "OK" >>$LOG
}

cat <<EOF
###############################################################################
#                                INITIAL BUILD                                #
#                                                                             #
# Please be patient.  This will take a while.  If you wish to build again     #
# later you should run "./bjam" from the project's root directory instead, as #
# it will be much faster, easier on system resources and not abusive on the   #
# Boost.Build and GoogleTest websites.  This initial build procedure will set #
# up bjam for this purpose.                                                   #
###############################################################################


EOF

mkdir -p "$INSTALL_DIR"
cat /dev/null >$LOG

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
  export BOOST_BUILD_PATH="$INSTALL_DIR/boost-build"

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
[ -z "$BJAM" ] && export BJAM=$(find $INSTALL_DIR/boost-build -name bjam)
[ -n "$BJAM" ] && succ || fail
rm -f $ORIGINAL_DIR/bjam
ln -s $BJAM $ORIGINAL_DIR/bjam

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
echo -n "Building libyaml++ ...  " |tee -a $LOG
"$BJAM" libyaml++ check demo >>$LOG 2>&1 && succ || fail
echo -e "\n\033[0;32mDone.\033[0m" |tee -a $LOG
echo -e "\nDone." >>$LOG
cat <<EOF

NOTE:  Run "./bjam" if you wish to build libyaml++ again.  But before you do
that, make sure you export BOOST_BUILD_PATH so that bjam can find Boost.Build:

 export BOOST_BUILD_PATH=$INSTALL_DIR/boost-build
EOF
