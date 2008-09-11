#!/usr/bin/env python
###############################################################################
# Build script for the waf build tool.
#
# See http://code.google.com/p/waf
###############################################################################

srcdir = '.'
blddir = 'output'

def init():
  pass

def set_options(opt):
  opt.add_option("--debug",
                 action = "store_true",
                 default = False,
                 help = "Build for debugging or development"
                )
  opt.tool_options('compiler_cxx')

def configure(conf):
  import Params
  if Params.g_options.debug:
      conf.define('DEBUG', 1)
      conf.env.append_value('CXXFLAGS', '-g')
  else:
      conf.define('NDEBUG', 1)
  conf.env.append_value('CXXFLAGS', '-Wall')
  conf.write_config_header('config.h')
  conf.sub_config('src')
  conf.check_tool('compiler_cxx')

def build(bld):
  bld.add_subdirs('src')

def shutdown():
  pass
