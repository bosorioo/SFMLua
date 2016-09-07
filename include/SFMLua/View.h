#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

namespace View
{
    void Register(lua_State* L);

    int Create(lua_State* L);
    int Call(lua_State* L);
    int SetSize(lua_State* L);
    int SetRotation(lua_State* L);
    int SetViewport(lua_State* L);
    int SetCenter(lua_State* L);
    int GetSize(lua_State* L);
    int GetCenter(lua_State* L);
    int GetViewport(lua_State* L);
    int GetRotation(lua_State* L);
    int Zoom(lua_State* L);
    int Move(lua_State* L);
    int Rotate(lua_State* L);
    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",          Create},
        {"setSize",         SetSize},
        {"getSize",         GetSize},
        {"setRotation",     SetRotation},
        {"getRotation",     GetRotation},
        {"setViewport",     SetViewport},
        {"getViewport",     GetViewport},
        {"setCenter",       SetCenter},
        {"getCenter",       GetCenter},
        {"zoom",            Zoom},
        {"move",            Move},
        {"rotate",          Rotate},
    {0, 0}};

    static const luaL_Reg Meta[] = {
        {"__gc",        Destroy},
        {"__eq", LuaAux::standardUserData__eq},
    {0, 0}};
}

#endif // VIEW_H_INCLUDED
