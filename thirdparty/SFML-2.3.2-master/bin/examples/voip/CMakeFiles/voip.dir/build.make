# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.4

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files (x86)\CodeBlocks\Cmake 3.4\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files (x86)\CodeBlocks\Cmake 3.4\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\bin"

# Include any dependencies generated for this target.
include examples/voip/CMakeFiles/voip.dir/depend.make

# Include the progress variables for this target.
include examples/voip/CMakeFiles/voip.dir/progress.make

# Include the compile flags for this target's objects.
include examples/voip/CMakeFiles/voip.dir/flags.make

examples/voip/CMakeFiles/voip.dir/VoIP.cpp.obj: examples/voip/CMakeFiles/voip.dir/flags.make
examples/voip/CMakeFiles/voip.dir/VoIP.cpp.obj: examples/voip/CMakeFiles/voip.dir/includes_CXX.rsp
examples/voip/CMakeFiles/voip.dir/VoIP.cpp.obj: ../examples/voip/VoIP.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\bin\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/voip/CMakeFiles/voip.dir/VoIP.cpp.obj"
	cd /d C:\Users\Bruno\Dropbox\SFMLPR~1\THIRDP~1\SFML-2~1.2-M\bin\examples\voip && C:\PROGRA~2\CODEBL~1\MinGW\bin\G__~1.EXE   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\voip.dir\VoIP.cpp.obj -c "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\examples\voip\VoIP.cpp"

examples/voip/CMakeFiles/voip.dir/VoIP.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/voip.dir/VoIP.cpp.i"
	cd /d C:\Users\Bruno\Dropbox\SFMLPR~1\THIRDP~1\SFML-2~1.2-M\bin\examples\voip && C:\PROGRA~2\CODEBL~1\MinGW\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\examples\voip\VoIP.cpp" > CMakeFiles\voip.dir\VoIP.cpp.i

examples/voip/CMakeFiles/voip.dir/VoIP.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/voip.dir/VoIP.cpp.s"
	cd /d C:\Users\Bruno\Dropbox\SFMLPR~1\THIRDP~1\SFML-2~1.2-M\bin\examples\voip && C:\PROGRA~2\CODEBL~1\MinGW\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\examples\voip\VoIP.cpp" -o CMakeFiles\voip.dir\VoIP.cpp.s

examples/voip/CMakeFiles/voip.dir/VoIP.cpp.obj.requires:

.PHONY : examples/voip/CMakeFiles/voip.dir/VoIP.cpp.obj.requires

examples/voip/CMakeFiles/voip.dir/VoIP.cpp.obj.provides: examples/voip/CMakeFiles/voip.dir/VoIP.cpp.obj.requires
	$(MAKE) -f examples\voip\CMakeFiles\voip.dir\build.make examples/voip/CMakeFiles/voip.dir/VoIP.cpp.obj.provides.build
.PHONY : examples/voip/CMakeFiles/voip.dir/VoIP.cpp.obj.provides

examples/voip/CMakeFiles/voip.dir/VoIP.cpp.obj.provides.build: examples/voip/CMakeFiles/voip.dir/VoIP.cpp.obj


examples/voip/CMakeFiles/voip.dir/Client.cpp.obj: examples/voip/CMakeFiles/voip.dir/flags.make
examples/voip/CMakeFiles/voip.dir/Client.cpp.obj: examples/voip/CMakeFiles/voip.dir/includes_CXX.rsp
examples/voip/CMakeFiles/voip.dir/Client.cpp.obj: ../examples/voip/Client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\bin\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object examples/voip/CMakeFiles/voip.dir/Client.cpp.obj"
	cd /d C:\Users\Bruno\Dropbox\SFMLPR~1\THIRDP~1\SFML-2~1.2-M\bin\examples\voip && C:\PROGRA~2\CODEBL~1\MinGW\bin\G__~1.EXE   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\voip.dir\Client.cpp.obj -c "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\examples\voip\Client.cpp"

examples/voip/CMakeFiles/voip.dir/Client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/voip.dir/Client.cpp.i"
	cd /d C:\Users\Bruno\Dropbox\SFMLPR~1\THIRDP~1\SFML-2~1.2-M\bin\examples\voip && C:\PROGRA~2\CODEBL~1\MinGW\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\examples\voip\Client.cpp" > CMakeFiles\voip.dir\Client.cpp.i

examples/voip/CMakeFiles/voip.dir/Client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/voip.dir/Client.cpp.s"
	cd /d C:\Users\Bruno\Dropbox\SFMLPR~1\THIRDP~1\SFML-2~1.2-M\bin\examples\voip && C:\PROGRA~2\CODEBL~1\MinGW\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\examples\voip\Client.cpp" -o CMakeFiles\voip.dir\Client.cpp.s

examples/voip/CMakeFiles/voip.dir/Client.cpp.obj.requires:

.PHONY : examples/voip/CMakeFiles/voip.dir/Client.cpp.obj.requires

examples/voip/CMakeFiles/voip.dir/Client.cpp.obj.provides: examples/voip/CMakeFiles/voip.dir/Client.cpp.obj.requires
	$(MAKE) -f examples\voip\CMakeFiles\voip.dir\build.make examples/voip/CMakeFiles/voip.dir/Client.cpp.obj.provides.build
.PHONY : examples/voip/CMakeFiles/voip.dir/Client.cpp.obj.provides

examples/voip/CMakeFiles/voip.dir/Client.cpp.obj.provides.build: examples/voip/CMakeFiles/voip.dir/Client.cpp.obj


examples/voip/CMakeFiles/voip.dir/Server.cpp.obj: examples/voip/CMakeFiles/voip.dir/flags.make
examples/voip/CMakeFiles/voip.dir/Server.cpp.obj: examples/voip/CMakeFiles/voip.dir/includes_CXX.rsp
examples/voip/CMakeFiles/voip.dir/Server.cpp.obj: ../examples/voip/Server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\bin\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object examples/voip/CMakeFiles/voip.dir/Server.cpp.obj"
	cd /d C:\Users\Bruno\Dropbox\SFMLPR~1\THIRDP~1\SFML-2~1.2-M\bin\examples\voip && C:\PROGRA~2\CODEBL~1\MinGW\bin\G__~1.EXE   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\voip.dir\Server.cpp.obj -c "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\examples\voip\Server.cpp"

examples/voip/CMakeFiles/voip.dir/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/voip.dir/Server.cpp.i"
	cd /d C:\Users\Bruno\Dropbox\SFMLPR~1\THIRDP~1\SFML-2~1.2-M\bin\examples\voip && C:\PROGRA~2\CODEBL~1\MinGW\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\examples\voip\Server.cpp" > CMakeFiles\voip.dir\Server.cpp.i

examples/voip/CMakeFiles/voip.dir/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/voip.dir/Server.cpp.s"
	cd /d C:\Users\Bruno\Dropbox\SFMLPR~1\THIRDP~1\SFML-2~1.2-M\bin\examples\voip && C:\PROGRA~2\CODEBL~1\MinGW\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\examples\voip\Server.cpp" -o CMakeFiles\voip.dir\Server.cpp.s

examples/voip/CMakeFiles/voip.dir/Server.cpp.obj.requires:

.PHONY : examples/voip/CMakeFiles/voip.dir/Server.cpp.obj.requires

examples/voip/CMakeFiles/voip.dir/Server.cpp.obj.provides: examples/voip/CMakeFiles/voip.dir/Server.cpp.obj.requires
	$(MAKE) -f examples\voip\CMakeFiles\voip.dir\build.make examples/voip/CMakeFiles/voip.dir/Server.cpp.obj.provides.build
.PHONY : examples/voip/CMakeFiles/voip.dir/Server.cpp.obj.provides

examples/voip/CMakeFiles/voip.dir/Server.cpp.obj.provides.build: examples/voip/CMakeFiles/voip.dir/Server.cpp.obj


# Object files for target voip
voip_OBJECTS = \
"CMakeFiles/voip.dir/VoIP.cpp.obj" \
"CMakeFiles/voip.dir/Client.cpp.obj" \
"CMakeFiles/voip.dir/Server.cpp.obj"

# External object files for target voip
voip_EXTERNAL_OBJECTS =

examples/voip/voip.exe: examples/voip/CMakeFiles/voip.dir/VoIP.cpp.obj
examples/voip/voip.exe: examples/voip/CMakeFiles/voip.dir/Client.cpp.obj
examples/voip/voip.exe: examples/voip/CMakeFiles/voip.dir/Server.cpp.obj
examples/voip/voip.exe: examples/voip/CMakeFiles/voip.dir/build.make
examples/voip/voip.exe: lib/libsfml-audio-s.a
examples/voip/voip.exe: lib/libsfml-network-s.a
examples/voip/voip.exe: lib/libsfml-system-s.a
examples/voip/voip.exe: ../extlibs/libs-mingw/x86/libopenal32.a
examples/voip/voip.exe: ../extlibs/libs-mingw/x86/libvorbisenc.a
examples/voip/voip.exe: ../extlibs/libs-mingw/x86/libvorbisfile.a
examples/voip/voip.exe: ../extlibs/libs-mingw/x86/libvorbis.a
examples/voip/voip.exe: ../extlibs/libs-mingw/x86/libogg.a
examples/voip/voip.exe: ../extlibs/libs-mingw/x86/libFLAC.a
examples/voip/voip.exe: examples/voip/CMakeFiles/voip.dir/linklibs.rsp
examples/voip/voip.exe: examples/voip/CMakeFiles/voip.dir/objects1.rsp
examples/voip/voip.exe: examples/voip/CMakeFiles/voip.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\bin\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable voip.exe"
	cd /d C:\Users\Bruno\Dropbox\SFMLPR~1\THIRDP~1\SFML-2~1.2-M\bin\examples\voip && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\voip.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/voip/CMakeFiles/voip.dir/build: examples/voip/voip.exe

.PHONY : examples/voip/CMakeFiles/voip.dir/build

examples/voip/CMakeFiles/voip.dir/requires: examples/voip/CMakeFiles/voip.dir/VoIP.cpp.obj.requires
examples/voip/CMakeFiles/voip.dir/requires: examples/voip/CMakeFiles/voip.dir/Client.cpp.obj.requires
examples/voip/CMakeFiles/voip.dir/requires: examples/voip/CMakeFiles/voip.dir/Server.cpp.obj.requires

.PHONY : examples/voip/CMakeFiles/voip.dir/requires

examples/voip/CMakeFiles/voip.dir/clean:
	cd /d C:\Users\Bruno\Dropbox\SFMLPR~1\THIRDP~1\SFML-2~1.2-M\bin\examples\voip && $(CMAKE_COMMAND) -P CMakeFiles\voip.dir\cmake_clean.cmake
.PHONY : examples/voip/CMakeFiles/voip.dir/clean

examples/voip/CMakeFiles/voip.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master" "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\examples\voip" "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\bin" "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\bin\examples\voip" "C:\Users\Bruno\Dropbox\SFML Project\thirdparty\SFML-2.3.2-master\bin\examples\voip\CMakeFiles\voip.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : examples/voip/CMakeFiles/voip.dir/depend

