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
CMAKE_SOURCE_DIR = /home/guifei/server_Mpath_proto/server_MPath_proto/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/guifei/server_Mpath_proto/server_MPath_proto/src/build

# Include any dependencies generated for this target.
include CMakeFiles/test_reader.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_reader.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_reader.dir/flags.make

CMakeFiles/test_reader.dir/test_videoReader.cpp.o: CMakeFiles/test_reader.dir/flags.make
CMakeFiles/test_reader.dir/test_videoReader.cpp.o: ../test_videoReader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guifei/server_Mpath_proto/server_MPath_proto/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_reader.dir/test_videoReader.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_reader.dir/test_videoReader.cpp.o -c /home/guifei/server_Mpath_proto/server_MPath_proto/src/test_videoReader.cpp

CMakeFiles/test_reader.dir/test_videoReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_reader.dir/test_videoReader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guifei/server_Mpath_proto/server_MPath_proto/src/test_videoReader.cpp > CMakeFiles/test_reader.dir/test_videoReader.cpp.i

CMakeFiles/test_reader.dir/test_videoReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_reader.dir/test_videoReader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guifei/server_Mpath_proto/server_MPath_proto/src/test_videoReader.cpp -o CMakeFiles/test_reader.dir/test_videoReader.cpp.s

CMakeFiles/test_reader.dir/test_videoReader.cpp.o.requires:

.PHONY : CMakeFiles/test_reader.dir/test_videoReader.cpp.o.requires

CMakeFiles/test_reader.dir/test_videoReader.cpp.o.provides: CMakeFiles/test_reader.dir/test_videoReader.cpp.o.requires
	$(MAKE) -f CMakeFiles/test_reader.dir/build.make CMakeFiles/test_reader.dir/test_videoReader.cpp.o.provides.build
.PHONY : CMakeFiles/test_reader.dir/test_videoReader.cpp.o.provides

CMakeFiles/test_reader.dir/test_videoReader.cpp.o.provides.build: CMakeFiles/test_reader.dir/test_videoReader.cpp.o


CMakeFiles/test_reader.dir/video_reader.cpp.o: CMakeFiles/test_reader.dir/flags.make
CMakeFiles/test_reader.dir/video_reader.cpp.o: ../video_reader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guifei/server_Mpath_proto/server_MPath_proto/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test_reader.dir/video_reader.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_reader.dir/video_reader.cpp.o -c /home/guifei/server_Mpath_proto/server_MPath_proto/src/video_reader.cpp

CMakeFiles/test_reader.dir/video_reader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_reader.dir/video_reader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guifei/server_Mpath_proto/server_MPath_proto/src/video_reader.cpp > CMakeFiles/test_reader.dir/video_reader.cpp.i

CMakeFiles/test_reader.dir/video_reader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_reader.dir/video_reader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guifei/server_Mpath_proto/server_MPath_proto/src/video_reader.cpp -o CMakeFiles/test_reader.dir/video_reader.cpp.s

CMakeFiles/test_reader.dir/video_reader.cpp.o.requires:

.PHONY : CMakeFiles/test_reader.dir/video_reader.cpp.o.requires

CMakeFiles/test_reader.dir/video_reader.cpp.o.provides: CMakeFiles/test_reader.dir/video_reader.cpp.o.requires
	$(MAKE) -f CMakeFiles/test_reader.dir/build.make CMakeFiles/test_reader.dir/video_reader.cpp.o.provides.build
.PHONY : CMakeFiles/test_reader.dir/video_reader.cpp.o.provides

CMakeFiles/test_reader.dir/video_reader.cpp.o.provides.build: CMakeFiles/test_reader.dir/video_reader.cpp.o


CMakeFiles/test_reader.dir/data_manager.cpp.o: CMakeFiles/test_reader.dir/flags.make
CMakeFiles/test_reader.dir/data_manager.cpp.o: ../data_manager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guifei/server_Mpath_proto/server_MPath_proto/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/test_reader.dir/data_manager.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_reader.dir/data_manager.cpp.o -c /home/guifei/server_Mpath_proto/server_MPath_proto/src/data_manager.cpp

CMakeFiles/test_reader.dir/data_manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_reader.dir/data_manager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guifei/server_Mpath_proto/server_MPath_proto/src/data_manager.cpp > CMakeFiles/test_reader.dir/data_manager.cpp.i

CMakeFiles/test_reader.dir/data_manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_reader.dir/data_manager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guifei/server_Mpath_proto/server_MPath_proto/src/data_manager.cpp -o CMakeFiles/test_reader.dir/data_manager.cpp.s

CMakeFiles/test_reader.dir/data_manager.cpp.o.requires:

.PHONY : CMakeFiles/test_reader.dir/data_manager.cpp.o.requires

CMakeFiles/test_reader.dir/data_manager.cpp.o.provides: CMakeFiles/test_reader.dir/data_manager.cpp.o.requires
	$(MAKE) -f CMakeFiles/test_reader.dir/build.make CMakeFiles/test_reader.dir/data_manager.cpp.o.provides.build
.PHONY : CMakeFiles/test_reader.dir/data_manager.cpp.o.provides

CMakeFiles/test_reader.dir/data_manager.cpp.o.provides.build: CMakeFiles/test_reader.dir/data_manager.cpp.o


CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.o: CMakeFiles/test_reader.dir/flags.make
CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.o: ../codeStreaming_parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guifei/server_Mpath_proto/server_MPath_proto/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.o -c /home/guifei/server_Mpath_proto/server_MPath_proto/src/codeStreaming_parser.cpp

CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guifei/server_Mpath_proto/server_MPath_proto/src/codeStreaming_parser.cpp > CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.i

CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guifei/server_Mpath_proto/server_MPath_proto/src/codeStreaming_parser.cpp -o CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.s

CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.o.requires:

.PHONY : CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.o.requires

CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.o.provides: CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.o.requires
	$(MAKE) -f CMakeFiles/test_reader.dir/build.make CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.o.provides.build
.PHONY : CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.o.provides

CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.o.provides.build: CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.o


CMakeFiles/test_reader.dir/bitrate_select.cpp.o: CMakeFiles/test_reader.dir/flags.make
CMakeFiles/test_reader.dir/bitrate_select.cpp.o: ../bitrate_select.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guifei/server_Mpath_proto/server_MPath_proto/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/test_reader.dir/bitrate_select.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_reader.dir/bitrate_select.cpp.o -c /home/guifei/server_Mpath_proto/server_MPath_proto/src/bitrate_select.cpp

CMakeFiles/test_reader.dir/bitrate_select.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_reader.dir/bitrate_select.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guifei/server_Mpath_proto/server_MPath_proto/src/bitrate_select.cpp > CMakeFiles/test_reader.dir/bitrate_select.cpp.i

CMakeFiles/test_reader.dir/bitrate_select.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_reader.dir/bitrate_select.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guifei/server_Mpath_proto/server_MPath_proto/src/bitrate_select.cpp -o CMakeFiles/test_reader.dir/bitrate_select.cpp.s

CMakeFiles/test_reader.dir/bitrate_select.cpp.o.requires:

.PHONY : CMakeFiles/test_reader.dir/bitrate_select.cpp.o.requires

CMakeFiles/test_reader.dir/bitrate_select.cpp.o.provides: CMakeFiles/test_reader.dir/bitrate_select.cpp.o.requires
	$(MAKE) -f CMakeFiles/test_reader.dir/build.make CMakeFiles/test_reader.dir/bitrate_select.cpp.o.provides.build
.PHONY : CMakeFiles/test_reader.dir/bitrate_select.cpp.o.provides

CMakeFiles/test_reader.dir/bitrate_select.cpp.o.provides.build: CMakeFiles/test_reader.dir/bitrate_select.cpp.o


CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.o: CMakeFiles/test_reader.dir/flags.make
CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.o: ../fec_param_adjustor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guifei/server_Mpath_proto/server_MPath_proto/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.o -c /home/guifei/server_Mpath_proto/server_MPath_proto/src/fec_param_adjustor.cpp

CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guifei/server_Mpath_proto/server_MPath_proto/src/fec_param_adjustor.cpp > CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.i

CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guifei/server_Mpath_proto/server_MPath_proto/src/fec_param_adjustor.cpp -o CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.s

CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.o.requires:

.PHONY : CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.o.requires

CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.o.provides: CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.o.requires
	$(MAKE) -f CMakeFiles/test_reader.dir/build.make CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.o.provides.build
.PHONY : CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.o.provides

CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.o.provides.build: CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.o


CMakeFiles/test_reader.dir/path_selector.cpp.o: CMakeFiles/test_reader.dir/flags.make
CMakeFiles/test_reader.dir/path_selector.cpp.o: ../path_selector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guifei/server_Mpath_proto/server_MPath_proto/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/test_reader.dir/path_selector.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_reader.dir/path_selector.cpp.o -c /home/guifei/server_Mpath_proto/server_MPath_proto/src/path_selector.cpp

CMakeFiles/test_reader.dir/path_selector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_reader.dir/path_selector.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guifei/server_Mpath_proto/server_MPath_proto/src/path_selector.cpp > CMakeFiles/test_reader.dir/path_selector.cpp.i

CMakeFiles/test_reader.dir/path_selector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_reader.dir/path_selector.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guifei/server_Mpath_proto/server_MPath_proto/src/path_selector.cpp -o CMakeFiles/test_reader.dir/path_selector.cpp.s

CMakeFiles/test_reader.dir/path_selector.cpp.o.requires:

.PHONY : CMakeFiles/test_reader.dir/path_selector.cpp.o.requires

CMakeFiles/test_reader.dir/path_selector.cpp.o.provides: CMakeFiles/test_reader.dir/path_selector.cpp.o.requires
	$(MAKE) -f CMakeFiles/test_reader.dir/build.make CMakeFiles/test_reader.dir/path_selector.cpp.o.provides.build
.PHONY : CMakeFiles/test_reader.dir/path_selector.cpp.o.provides

CMakeFiles/test_reader.dir/path_selector.cpp.o.provides.build: CMakeFiles/test_reader.dir/path_selector.cpp.o


CMakeFiles/test_reader.dir/myUtility.cpp.o: CMakeFiles/test_reader.dir/flags.make
CMakeFiles/test_reader.dir/myUtility.cpp.o: ../myUtility.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guifei/server_Mpath_proto/server_MPath_proto/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/test_reader.dir/myUtility.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_reader.dir/myUtility.cpp.o -c /home/guifei/server_Mpath_proto/server_MPath_proto/src/myUtility.cpp

CMakeFiles/test_reader.dir/myUtility.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_reader.dir/myUtility.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guifei/server_Mpath_proto/server_MPath_proto/src/myUtility.cpp > CMakeFiles/test_reader.dir/myUtility.cpp.i

CMakeFiles/test_reader.dir/myUtility.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_reader.dir/myUtility.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guifei/server_Mpath_proto/server_MPath_proto/src/myUtility.cpp -o CMakeFiles/test_reader.dir/myUtility.cpp.s

CMakeFiles/test_reader.dir/myUtility.cpp.o.requires:

.PHONY : CMakeFiles/test_reader.dir/myUtility.cpp.o.requires

CMakeFiles/test_reader.dir/myUtility.cpp.o.provides: CMakeFiles/test_reader.dir/myUtility.cpp.o.requires
	$(MAKE) -f CMakeFiles/test_reader.dir/build.make CMakeFiles/test_reader.dir/myUtility.cpp.o.provides.build
.PHONY : CMakeFiles/test_reader.dir/myUtility.cpp.o.provides

CMakeFiles/test_reader.dir/myUtility.cpp.o.provides.build: CMakeFiles/test_reader.dir/myUtility.cpp.o


# Object files for target test_reader
test_reader_OBJECTS = \
"CMakeFiles/test_reader.dir/test_videoReader.cpp.o" \
"CMakeFiles/test_reader.dir/video_reader.cpp.o" \
"CMakeFiles/test_reader.dir/data_manager.cpp.o" \
"CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.o" \
"CMakeFiles/test_reader.dir/bitrate_select.cpp.o" \
"CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.o" \
"CMakeFiles/test_reader.dir/path_selector.cpp.o" \
"CMakeFiles/test_reader.dir/myUtility.cpp.o"

# External object files for target test_reader
test_reader_EXTERNAL_OBJECTS =

test_reader: CMakeFiles/test_reader.dir/test_videoReader.cpp.o
test_reader: CMakeFiles/test_reader.dir/video_reader.cpp.o
test_reader: CMakeFiles/test_reader.dir/data_manager.cpp.o
test_reader: CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.o
test_reader: CMakeFiles/test_reader.dir/bitrate_select.cpp.o
test_reader: CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.o
test_reader: CMakeFiles/test_reader.dir/path_selector.cpp.o
test_reader: CMakeFiles/test_reader.dir/myUtility.cpp.o
test_reader: CMakeFiles/test_reader.dir/build.make
test_reader: rs_fft/libRS_FFT.a
test_reader: CMakeFiles/test_reader.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/guifei/server_Mpath_proto/server_MPath_proto/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable test_reader"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_reader.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_reader.dir/build: test_reader

.PHONY : CMakeFiles/test_reader.dir/build

CMakeFiles/test_reader.dir/requires: CMakeFiles/test_reader.dir/test_videoReader.cpp.o.requires
CMakeFiles/test_reader.dir/requires: CMakeFiles/test_reader.dir/video_reader.cpp.o.requires
CMakeFiles/test_reader.dir/requires: CMakeFiles/test_reader.dir/data_manager.cpp.o.requires
CMakeFiles/test_reader.dir/requires: CMakeFiles/test_reader.dir/codeStreaming_parser.cpp.o.requires
CMakeFiles/test_reader.dir/requires: CMakeFiles/test_reader.dir/bitrate_select.cpp.o.requires
CMakeFiles/test_reader.dir/requires: CMakeFiles/test_reader.dir/fec_param_adjustor.cpp.o.requires
CMakeFiles/test_reader.dir/requires: CMakeFiles/test_reader.dir/path_selector.cpp.o.requires
CMakeFiles/test_reader.dir/requires: CMakeFiles/test_reader.dir/myUtility.cpp.o.requires

.PHONY : CMakeFiles/test_reader.dir/requires

CMakeFiles/test_reader.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_reader.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_reader.dir/clean

CMakeFiles/test_reader.dir/depend:
	cd /home/guifei/server_Mpath_proto/server_MPath_proto/src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/guifei/server_Mpath_proto/server_MPath_proto/src /home/guifei/server_Mpath_proto/server_MPath_proto/src /home/guifei/server_Mpath_proto/server_MPath_proto/src/build /home/guifei/server_Mpath_proto/server_MPath_proto/src/build /home/guifei/server_Mpath_proto/server_MPath_proto/src/build/CMakeFiles/test_reader.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_reader.dir/depend

