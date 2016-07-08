Compiler = g++
Flags = -Wall -mwindows
LibPath = lib
IncludePath = include
LinkLibs = lua52d sfml-network-s sfml-graphics-s sfml-window-s sfml-system-s glu32 glaux freetype glew jpeg Ws2_32 Winmm opengl32
OutputFile = SFMLua.exe
SourceFiles = $(wildcard src/*/*.cpp) $(wildcard src/*/*.c)

ls:
	@echo $(wildcard src/*/*.cpp) $(wildcard src/*/*.c) 
	@echo fuck

compile: main.o
	@echo $(Compiler) -o $(OutputFile) -L$(LibPath) 

.PHONY: ls