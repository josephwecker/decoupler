#!/usr/bin/env python
# encoding: utf-8
APPNAME = 'decoupler'
VERSION = '0.0.1'

top     = '.'
out     = '.build'

# Optimization flags
OPTF = ['-O3', '-finline-limit=20000', '-funroll-loops',
        '-fvariable-expansion-in-unroller', '-ftracer', '-fivopts', '-fsee',
        '-ftree-vectorize', '-ffast-math', '-frename-registers',
        '-maccumulate-outgoing-args', '-fPIC']


from waflib import Configure
Configure.autoconfig = True

def build(bld):
    bld.shlib(source='lib/dio.c', includes=['.build','include'],
            target='dio', vnum=VERSION)
    bld.program(source='src/dcpl.c', target='dcpl',
            includes=['.build','include'], use=["dio"], vnum=VERSION)

def options(opt):
    opt.load('compiler_c')
    opt.load('gnu_dirs')

def configure(conf):
    import waflib.extras.cpuinfo as cpuinfo
    import platform
    conf.load('compiler_c')
    # Just for checks below- gets overwritten before config is done
    conf.env.CFLAGS = ['-fmessage-length=0','-Wfatal-errors','-fdiagnostics-show-location=once']
    conf.check_cc(fragment="int main() { return 0; }\n", execute=True, msg="Checking main")
    conf.multicheck(
            {'header_name':'stdio.h'},
            {'header_name':'unistd.h'},
            {'header_name':'stdlib.h'},
            {'header_name':'errno.h'},
            {'header_name':'fcntl.h'},
            {'header_name':'string.h'},
            msg="Checking some standard headers")
    conf.multicheck(
            {'header_name':'ctype.h'},
            {'header_name':'sys/socket.h'},
            {'header_name':'sys/un.h'},
            {'header_name':'sys/types.h'},
            msg="Checking for sockets / unix sockets")
    conf.check_cc(header_name='sys/stat.h')
    conf.check_large_file(mandatory=False)
    conf.check_cc(function_name='sendfile',
            header_name='sys/sendfile.h',
            define_name='HAVE_LINUX_SENDFILE',
            mandatory=False,
            errmsg="not found",
            msg="Checking for Linux version of sendfile")
    conf.check_cc(function_name='sendfile',
            header_name=['sys/types.h','sys/socket.h','sys/uio.h'],
            define_name='HAVE_BSD_SENDFILE',
            mandatory=False, msg="Checking for BSD version of sendfile")
    conf.check_cc(function_name='splice', header_name='fcntl.h',
            defines="_GNU_SOURCE", mandatory=False)
    conf.check_cc(function_name='tee', header_name='fcntl.h',
            defines="_GNU_SOURCE", mandatory=False)
    conf.check_cc(function_name='vmsplice', header_name='fcntl.h',
            defines="_GNU_SOURCE", mandatory=False)

    conf.env.CFLAGS = ['-Wall', '-pipe'] + OPTF
    if platform.system() == 'Darwin':
        conf.env.CFLAGS += ['-arch', 'x86_64', '-fast']
        conf.env.LINKFLAGS += ['-arch', 'x86_64']
    conf.write_config_header('config.h')


