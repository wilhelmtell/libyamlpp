#!/usr/bin/env python
###############################################################################
# Build script for the waf build tool.
#
# See http://code.google.com/p/waf
###############################################################################

import Params

srcdir = '.'
blddir = 'build'

def init():
  pass

def set_options(opt):
  opt.add_option("--debug",
                 action = "store_true",
                 default = False,
                 help = "Build for debugging or development"
                )
  opt.add_option("--documentation",
                 action = "store_true",
                 default = False,
                 help = "Generate documentation"
                )
  opt.tool_options('compiler_cxx')

def configure(conf):
  # check for and set up a c++ compiler
  conf.check_tool('compiler_cxx')
  conf.env['CXXFLAGS'] += ' -Wall -ansi -pedantic'

  # debug?
  if Params.g_options.debug:
    conf.define('DEBUG', 1)
    conf.env['CXXFLAGS'] += ' -g'
  else:
    conf.define('NDEBUG', 1)

  # documentation?
  if Params.g_options.documentation:
    conf.define('HAVE_DOCUMENTATION', 1)
    conf.check_tool('tex')

  conf.write_config_header('config.h')
  conf.sub_config('src')

def build(bld):
  bld_subdirs = ['src']
  bld.env()['HAVE_DOCUMENTATION'] and bld_subdirs.append('doc')
  bld.add_subdirs(bld_subdirs)

def shutdown():
  pass
