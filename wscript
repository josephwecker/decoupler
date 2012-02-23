#!/usr/bin/env python
# encoding: utf-8
APPNAME = 'decoupler'
VERSION = '0.0.1'

top     = '.'
out     = '.build'

# Optimization flags
OPTF = ['-O3', '-finline-limit=20000', '-fwhole-program', '-funroll-loops',
        '-fvariable-expansion-in-unroller', '-ftracer', '-fivopts', '-fsee',
        '-ftree-vectorize', '-ffast-math', '-frename-registers',
        '-maccumulate-outgoing-args']


from waflib import Configure
Configure.autoconfig = True

def options(opt):
	opt.load('compiler_c')
	opt.load('gnu_dirs')

def configure(conf):
    import waflib.extras.cpuinfo as cpuinfo
    import platform
    conf.load('compiler_c')
    conf.check_large_file(mandatory=False)
    conf.env.CFLAGS = ['-Wall', '-pipe'] + OPTF
    if platform.system() == 'Darwin':
        conf.env.CFLAGS += ['-arch', 'x86_64', '-fast']
    conf.write_config_header('config.h')

def build(bld):
    bld.program(source='src/dcpl.c', target='dcpl', includes=['include'],
            vnum=VERSION)

