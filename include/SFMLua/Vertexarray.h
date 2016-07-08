#ifndef VERTEXARRAY_H_INCLUDED
#define VERTEXARRAY_H_INCLUDED

namespace VertexArray
{
    void Register(lua_State* L);

    int Create(lua_State* L);
    int Call(lua_State* L);
    int Draw(lua_State* L);
    int Translate(lua_State* L);
    int SetVertex(lua_State* L);
    int SetPrimitive(lua_State* L);
    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",      Create},
        {"setVertex",   SetVertex},
        {"setPrimitive",SetPrimitive},
        {"translate",   Translate},
        {"draw",        Draw},
    {0, 0}};

    static const luaL_Reg Meta[] = {
        {"__gc",        Destroy},
    {0, 0}};
}

#endif // VERTEXARRAY_H_INCLUDED
