#!/usr/bin/env python
###############################################################################
# Build script for the waf build tool.
#
# See http://code.google.com/p/waf
###############################################################################

import re, Params, Scripting

APPNAME = "libyaml++"
VERSION = "0.1"
Scripting.g_gz = "gz"

srcdir = '.'
blddir = 'build'

def init():
  pass

def set_options(opt):
  opt.add_option("--with-tests",
                 action = "store_true",
                 default = False,
                 help = "Build tests"
                )
  opt.add_option("--verbose-testing",
                 action = "store_true",
                 default = False,
                 help = "On './waf check', test verbosely"
                )
  opt.add_option("--docs",
                 action = "store_true",
                 default = False,
                 help = "Generate documentation"
                )
  opt.add_option("--demo",
                 action = "store_true",
                 default = False,
                 help = "Build the demo application"
                )
  opt.tool_options('compiler_cxx')

def configure(conf):
  # check for and set up a c++ compiler
  conf.check_tool('compiler_cxx')
  if type(conf.env['CXXFLAGS']) is str:
      conf.env['CXXFLAGS'] = re.split(r'\s+', conf.env['CXXFLAGS'])
  else:
      conf.env['CXXFLAGS'] = []
  conf.env['CXXFLAGS'] += ['-Wall', '-ansi', '-pedantic']

  # compiler is bad if it can't compile a simple 'hello world' program
  e = conf.create_test_configurator()
  e.mandatory = 1
  e.code = '#include<iostream>\nint main(){std::cout<<"hello world"<<std::endl;return 0;}\n'
  e.want_message = 1
  e.run()

  if Params.g_options.with_tests:
    conf.env['BUILD_TESTS'] = True
    c = conf.create_header_configurator()
    c.name = 'gtest/gtest.h'
    c.uselib = 'GTEST_H'
    c.path = ['/usr/include', '/usr/local/include']
    c.mandatory = 1
    c.message = 'The GoogleTest library headers are required if tests are to be performed.'
    c.run()

    c = conf.create_library_configurator()
    c.name = 'gtest'
    c.uselib = 'GTEST'
    c.path = ['/usr/lib', '/usr/local/lib']
    c.mandatory = 1
    c.message = 'The GoogleTest library is required if tests are to be performed.'
    c.run()
    Params.pprint('CYAN', "Run './waf check' to run unit tests.")

  # documentation?
  if Params.g_options.docs:
    conf.define('HAVE_DOCS', 1)
    conf.check_tool('tex')

  # demo?
  if Params.g_options.demo:
    conf.define('HAVE_DEMO', 1)

  conf.write_config_header('config.h')
  conf.sub_config('src')

def build(bld):
  bld_subdirs = ['src']
  if bld.env()['HAVE_DOCS']:
    bld_subdirs.append('doc')
  if bld.env()['HAVE_DEMO']:
    bld_subdirs.append('src/demo')
  if bld.env()['BUILD_TESTS']:
    bld_subdirs.extend(['src/test', 'src/demo/test'])
  bld.add_subdirs(bld_subdirs)

def shutdown():
  import UnitTest
  ut = UnitTest.unit_test()
  ut.change_to_testfile_dir = True
  if Params.g_options.verbose_testing:
    ut.want_to_see_test_output = True
  else:
    ut.want_to_see_test_output = False
  ut.run()
  ut.print_results()
