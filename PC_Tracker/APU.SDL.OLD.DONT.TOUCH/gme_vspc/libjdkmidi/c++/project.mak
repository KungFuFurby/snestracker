
PROJECT=jdkmidi
PROJECT_NAME=libjdkmidi
PROJECT_VERSION=2.1
PROJECT_EMAIL='<jeffk@jdkoftinoff.com>'
PROJECT_LICENSE='GPL'

TOP_LIB_DIRS+=.

DEFINES_MINGW32+=
LDLIBS_MINGW32+=-lwinmm
COMPILE_FLAGS_MINGW32+=-mthreads

DEFINES_CYGWIN+=
LDLIBS_CYGWIN+=
COMPILE_FLAGS_CYGWIN+=

DEFINES_LINUX+=
LDLIBS_LINUX+=
COMPILE_FLAGS_LINUX+=

DEFINES_MACOSX+=
LDLIBS_MACOSX+=
COMPILE_FLAGS_MACOSX+=

DEFINES_MACOSX_PPC+=
LDLIBS_MACOSX_PPC+=
COMPILE_FLAGS_MACOSX_PPC+=

DEFINES_MACOSX_I386+=
LDLIBS_MACOSX_I386+=
COMPILE_FLAGS_MACOSX_I386+=

DEFINES_MACOSX_UNIVERSAL+=
LDLIBS_MACOSX_UNIVERSAL+=
COMPILE_FLAGS_MACOSX_UNIVERSAL+=
