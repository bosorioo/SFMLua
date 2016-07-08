#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

namespace Texture
{
    void Register(lua_State* L);

    int Create(lua_State* L);
    int Call(lua_State* L);
    int SetSmooth(lua_State* L);
    int SetRepeated(lua_State* L);
    int GetImage(lua_State* L);
    int GetSize(lua_State* L);
    int LoadFile(lua_State* L);
    int LoadImage(lua_State* L);
    int LoadWindow(lua_State* L);
    int Update(lua_State* L);
    int Bind(lua_State* L);
    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",      Create},
        {"setSmooth",   SetSmooth},
        {"setRepeated", SetRepeated},
        {"getImage",    GetImage},
        {"getSize",     GetSize},
        {"loadImage",   LoadImage},
        {"loadFile",    LoadFile},
        {"loadWindow",  LoadWindow},
        {"update",  Update},
        {"bind",        Bind},
    {0, 0}};

     static const luaL_Reg Meta[] = {
        {"__gc",        Destroy},
    {0, 0}};
}

#endif // TEXTURE_H_INCLUDED
