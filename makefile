# MSVC Makefile for Windows
.PHONY: all clean rebuild run

# Version configuration
WINDOWS_SDK_VERSION = 10.0.26100.0
MSVC_VERSION = 14.44.35207

# Path configuration
MSVC_ROOT = C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/$(MSVC_VERSION)
WINDOWS_SDK_ROOT = C:/Program Files (x86)/Windows Kits/10

# Include directories
MSVC_INCLUDE = $(MSVC_ROOT)/include
MSVC_ATLMFC_INCLUDE = $(MSVC_ROOT)/atlmfc/include
WINDOWS_SDK_WINRT = $(WINDOWS_SDK_ROOT)/Include/$(WINDOWS_SDK_VERSION)/winrt
WINDOWS_SDK_UCRT = $(WINDOWS_SDK_ROOT)/Include/$(WINDOWS_SDK_VERSION)/ucrt
WINDOWS_SDK_UM = $(WINDOWS_SDK_ROOT)/Include/$(WINDOWS_SDK_VERSION)/um
WINDOWS_SDK_SHARED = $(WINDOWS_SDK_ROOT)/Include/$(WINDOWS_SDK_VERSION)/shared
WINDOWS_SDK_CPPWINRT = $(WINDOWS_SDK_ROOT)/Include/$(WINDOWS_SDK_VERSION)/cppwinrt

# Library directories
MSVC_LIB_PATH = $(MSVC_ROOT)/lib/x64
MSVC_ATLMFC_LIB_PATH = $(MSVC_ROOT)/atlmfc/lib/x64
WINDOWS_SDK_UM_LIB = $(WINDOWS_SDK_ROOT)/Lib/$(WINDOWS_SDK_VERSION)/um/x64
WINDOWS_SDK_UCRT_LIB = $(WINDOWS_SDK_ROOT)/Lib/$(WINDOWS_SDK_VERSION)/ucrt/x64

# Toolchain
SHELL := cmd
CC := cl
CXX := cl
LINK_CC := cl

# Include paths for cl.exe (semicolon-separated)
INCLUDE_PATHS := $(MSVC_INCLUDE);$(MSVC_ATLMFC_INCLUDE);$(WINDOWS_SDK_WINRT);$(WINDOWS_SDK_UCRT);$(WINDOWS_SDK_UM);$(WINDOWS_SDK_SHARED);$(WINDOWS_SDK_CPPWINRT);./include

# Library configuration
LIBPATH_FLAGS := /LIBPATH:"$(MSVC_LIB_PATH)" /LIBPATH:"$(MSVC_ATLMFC_LIB_PATH)" /LIBPATH:"$(WINDOWS_SDK_UM_LIB)" /LIBPATH:"$(WINDOWS_SDK_UCRT_LIB)"
LIBS := User32.lib gdi32.lib opengl32.lib 

# Compiler flags
CFLAGS := /nologo /Zi
CXXFLAGS := /nologo /Zi /EHsc

# Linker flags
SUBSYSTEM ?= WINDOWS
LDFLAGS := /SUBSYSTEM:$(SUBSYSTEM)

# Source files and build targets
# Recursive wildcard function: $(call rwildcard, <dir>, <pattern>)
# This will traverse all subdirectories under the given dir and collect files
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)) $(filter $(subst *,%,$2),$(wildcard $1$2))

# ---------------------------------------------------------------------------
# Recursive include directory discovery
# Collect every subdirectory under include/ so headers can be included without
# specifying their relative path (e.g. "#include \"Vertex.hpp\"") even when
# the file actually lives in include/Renderable/Vertex.hpp.
# Pattern '*/' with rwildcard returns all nested directories.
INCLUDE_SUBDIRS_RAW := $(call rwildcard,include/,*/)
# Remove trailing slashes for consistency
INCLUDE_SUBDIRS := $(patsubst %/,%,$(INCLUDE_SUBDIRS_RAW))
# Generate /I flags for cl (avoid duplicates via sort)
INCLUDE_FLAGS := $(foreach d,$(sort $(INCLUDE_SUBDIRS)),/I"$(d)") /I"include"
# ---------------------------------------------------------------------------

# Collect C / C++ sources in project root and recursively under src/
SRCS_C := $(wildcard *.c) $(call rwildcard,src/,*.c)
SRCS_CPP := $(wildcard *.cpp) $(call rwildcard,src/,*.cpp)
SRCS := $(SRCS_C) $(SRCS_CPP)

OBJS_C := $(addprefix bin/,$(SRCS_C:.c=.obj))
OBJS_CPP := $(addprefix bin/,$(SRCS_CPP:.cpp=.obj))
OBJS := $(OBJS_C) $(OBJS_CPP)

TARGET := program.exe

# Build rules
all: $(TARGET)

$(TARGET): $(OBJS)
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	set "INCLUDE=$(INCLUDE_PATHS)" && $(LINK_CC) $(OBJS) /link $(LIBPATH_FLAGS) $(LIBS) /OUT:$@ $(LDFLAGS)

bin/%.obj: %.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	set "INCLUDE=$(INCLUDE_PATHS)" && $(CC) $(CFLAGS) $(INCLUDE_FLAGS) /c $< /Fo:$@

bin/%.obj: %.cpp
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	set "INCLUDE=$(INCLUDE_PATHS)" && $(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) /c $< /Fo:$@

# Run the program
run: all
	@cls
	@$(TARGET)

# Cleanup
clean:
	-if exist bin\*.obj del /q bin\*.obj 2>nul
	-if exist bin\src\*.obj del /q bin\src\*.obj 2>nul

rebuild: clean all
