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
  bld.env()['HAVE_DOCS'] and bld_subdirs.append('doc')
  bld.env()['HAVE_DEMO'] and bld_subdirs.append('src/demo')
  bld.add_subdirs(bld_subdirs)

def shutdown():
  pass
