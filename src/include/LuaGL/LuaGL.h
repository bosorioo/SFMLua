#ifndef LUAGL_H_INCLUDED
#define LUAGL_H_INCLUDED

#include "Lua/lua.hpp"

void lua_opengl(lua_State* L);

#include <stdio.h>

#if defined(__APPLE__) && defined(__MACH__)
    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
#else
    #include <LuaGL/glew.h>
    #include <LuaGL/gl.h>
#endif

#endif // LUAGL_H_INCLUDED
