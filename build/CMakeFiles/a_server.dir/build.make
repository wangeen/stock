# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /home/jinwang/bin/cmake

# The command to remove a file.
RM = /home/jinwang/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jinwang/Dropbox/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jinwang/Dropbox/test/build

# Include any dependencies generated for this target.
include CMakeFiles/a_server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/a_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/a_server.dir/flags.make

CMakeFiles/a_server.dir/server/./a_server_generated_coefficient.cu.o: CMakeFiles/a_server.dir/server/a_server_generated_coefficient.cu.o.depend
CMakeFiles/a_server.dir/server/./a_server_generated_coefficient.cu.o: CMakeFiles/a_server.dir/server/a_server_generated_coefficient.cu.o.cmake
CMakeFiles/a_server.dir/server/./a_server_generated_coefficient.cu.o: ../server/coefficient.cu
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jinwang/Dropbox/test/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Building NVCC (Device) object CMakeFiles/a_server.dir/server/./a_server_generated_coefficient.cu.o"
	cd /home/jinwang/Dropbox/test/build/CMakeFiles/a_server.dir/server && /home/jinwang/bin/cmake -E make_directory /home/jinwang/Dropbox/test/build/CMakeFiles/a_server.dir/server/.
	cd /home/jinwang/Dropbox/test/build/CMakeFiles/a_server.dir/server && /home/jinwang/bin/cmake -D verbose:BOOL=$(VERBOSE) -D build_configuration:STRING= -D generated_file:STRING=/home/jinwang/Dropbox/test/build/CMakeFiles/a_server.dir/server/./a_server_generated_coefficient.cu.o -D generated_cubin_file:STRING=/home/jinwang/Dropbox/test/build/CMakeFiles/a_server.dir/server/./a_server_generated_coefficient.cu.o.cubin.txt -P /home/jinwang/Dropbox/test/build/CMakeFiles/a_server.dir/server/a_server_generated_coefficient.cu.o.cmake

CMakeFiles/a_server.dir/server/main.cpp.o: CMakeFiles/a_server.dir/flags.make
CMakeFiles/a_server.dir/server/main.cpp.o: ../server/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jinwang/Dropbox/test/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a_server.dir/server/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a_server.dir/server/main.cpp.o -c /home/jinwang/Dropbox/test/server/main.cpp

CMakeFiles/a_server.dir/server/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a_server.dir/server/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jinwang/Dropbox/test/server/main.cpp > CMakeFiles/a_server.dir/server/main.cpp.i

CMakeFiles/a_server.dir/server/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a_server.dir/server/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jinwang/Dropbox/test/server/main.cpp -o CMakeFiles/a_server.dir/server/main.cpp.s

CMakeFiles/a_server.dir/server/main.cpp.o.requires:
.PHONY : CMakeFiles/a_server.dir/server/main.cpp.o.requires

CMakeFiles/a_server.dir/server/main.cpp.o.provides: CMakeFiles/a_server.dir/server/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/a_server.dir/build.make CMakeFiles/a_server.dir/server/main.cpp.o.provides.build
.PHONY : CMakeFiles/a_server.dir/server/main.cpp.o.provides

CMakeFiles/a_server.dir/server/main.cpp.o.provides.build: CMakeFiles/a_server.dir/server/main.cpp.o

# Object files for target a_server
a_server_OBJECTS = \
"CMakeFiles/a_server.dir/server/main.cpp.o"

# External object files for target a_server
a_server_EXTERNAL_OBJECTS = \
"/home/jinwang/Dropbox/test/build/CMakeFiles/a_server.dir/server/./a_server_generated_coefficient.cu.o"

a_server: CMakeFiles/a_server.dir/server/main.cpp.o
a_server: CMakeFiles/a_server.dir/server/./a_server_generated_coefficient.cu.o
a_server: CMakeFiles/a_server.dir/build.make
a_server: /usr/local/cuda/lib/libcudart.so
a_server: /usr/lib/libcuda.so
a_server: /usr/local/lib/libpcap.so
a_server: CMakeFiles/a_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable a_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/a_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/a_server.dir/build: a_server
.PHONY : CMakeFiles/a_server.dir/build

CMakeFiles/a_server.dir/requires: CMakeFiles/a_server.dir/server/main.cpp.o.requires
.PHONY : CMakeFiles/a_server.dir/requires

CMakeFiles/a_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/a_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/a_server.dir/clean

CMakeFiles/a_server.dir/depend: CMakeFiles/a_server.dir/server/./a_server_generated_coefficient.cu.o
	cd /home/jinwang/Dropbox/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jinwang/Dropbox/test /home/jinwang/Dropbox/test /home/jinwang/Dropbox/test/build /home/jinwang/Dropbox/test/build /home/jinwang/Dropbox/test/build/CMakeFiles/a_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/a_server.dir/depend
