#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

namespace Font
{
    void Register(lua_State* L);

    int LoadFromFile(lua_State* L);
    int Create(lua_State* L);
    int Call(lua_State* L);
    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",        Create},
        {"loadFromFile",  LoadFromFile},
    {0, 0}};

    static const luaL_Reg Meta[] = {
        {"__gc",          Destroy},
    {0, 0}};
}

#endif // FONT_H_INCLUDED
