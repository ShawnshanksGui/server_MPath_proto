# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/guifei/server_Mpath_proto/server_MPath_proto/src/test_parser

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/guifei/server_Mpath_proto/server_MPath_proto/src/test_parser/build

# Include any dependencies generated for this target.
include CMakeFiles/test_parser.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_parser.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_parser.dir/flags.make

CMakeFiles/test_parser.dir/codeStreaming_parser.o: CMakeFiles/test_parser.dir/flags.make
CMakeFiles/test_parser.dir/codeStreaming_parser.o: ../codeStreaming_parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guifei/server_Mpath_proto/server_MPath_proto/src/test_parser/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_parser.dir/codeStreaming_parser.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_parser.dir/codeStreaming_parser.o -c /home/guifei/server_Mpath_proto/server_MPath_proto/src/test_parser/codeStreaming_parser.cpp

CMakeFiles/test_parser.dir/codeStreaming_parser.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_parser.dir/codeStreaming_parser.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guifei/server_Mpath_proto/server_MPath_proto/src/test_parser/codeStreaming_parser.cpp > CMakeFiles/test_parser.dir/codeStreaming_parser.i

CMakeFiles/test_parser.dir/codeStreaming_parser.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_parser.dir/codeStreaming_parser.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guifei/server_Mpath_proto/server_MPath_proto/src/test_parser/codeStreaming_parser.cpp -o CMakeFiles/test_parser.dir/codeStreaming_parser.s

CMakeFiles/test_parser.dir/codeStreaming_parser.o.requires:

.PHONY : CMakeFiles/test_parser.dir/codeStreaming_parser.o.requires

CMakeFiles/test_parser.dir/codeStreaming_parser.o.provides: CMakeFiles/test_parser.dir/codeStreaming_parser.o.requires
	$(MAKE) -f CMakeFiles/test_parser.dir/build.make CMakeFiles/test_parser.dir/codeStreaming_parser.o.provides.build
.PHONY : CMakeFiles/test_parser.dir/codeStreaming_parser.o.provides

CMakeFiles/test_parser.dir/codeStreaming_parser.o.provides.build: CMakeFiles/test_parser.dir/codeStreaming_parser.o


# Object files for target test_parser
test_parser_OBJECTS = \
"CMakeFiles/test_parser.dir/codeStreaming_parser.o"

# External object files for target test_parser
test_parser_EXTERNAL_OBJECTS =

test_parser: CMakeFiles/test_parser.dir/codeStreaming_parser.o
test_parser: CMakeFiles/test_parser.dir/build.make
test_parser: CMakeFiles/test_parser.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/guifei/server_Mpath_proto/server_MPath_proto/src/test_parser/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_parser"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_parser.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_parser.dir/build: test_parser

.PHONY : CMakeFiles/test_parser.dir/build

CMakeFiles/test_parser.dir/requires: CMakeFiles/test_parser.dir/codeStreaming_parser.o.requires

.PHONY : CMakeFiles/test_parser.dir/requires

CMakeFiles/test_parser.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_parser.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_parser.dir/clean

CMakeFiles/test_parser.dir/depend:
	cd /home/guifei/server_Mpath_proto/server_MPath_proto/src/test_parser/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/guifei/server_Mpath_proto/server_MPath_proto/src/test_parser /home/guifei/server_Mpath_proto/server_MPath_proto/src/test_parser /home/guifei/server_Mpath_proto/server_MPath_proto/src/test_parser/build /home/guifei/server_Mpath_proto/server_MPath_proto/src/test_parser/build /home/guifei/server_Mpath_proto/server_MPath_proto/src/test_parser/build/CMakeFiles/test_parser.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_parser.dir/depend

