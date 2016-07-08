#ifndef REGULARSHAPE_H_INCLUDED
#define REGULARSHAPE_H_INCLUDED

namespace RegularShape
{
    void Register(lua_State* L);

    int Create(lua_State* L);
    int SetSize(lua_State* L);
    int GetSize(lua_State* L);
    int SetSideCount(lua_State* L);
    int GetSideCount(lua_State* L);
    int Call(lua_State* L);
    int SetFillColor(lua_State* L);
    int SetPosition(lua_State* L);
    int SetRotation(lua_State* L);
    int SetScale(lua_State* L);
    int Rotate(lua_State* L);
    int Scale(lua_State* L);
    int Move(lua_State* L);
    int GetFillColor(lua_State* L);
    int GetPosition(lua_State* L);
    int GetRotation(lua_State* L);
    int GetScale(lua_State* L);
    int GetTexture(lua_State* L);
    int SetTexture(lua_State* L);
    int SetOrigin(lua_State* L);
    int Draw(lua_State* L);
    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",      Create},
        {"setSize",     SetSize},
        {"getSize",     GetSize},
        {"setSideCount",SetSideCount},
        {"getSideCount",GetSideCount},
        {"setTexture",  SetTexture},
        {"getTexture",  GetTexture},
        {"setFillColor",SetFillColor},
        {"getFillColor",GetFillColor},
        {"setOrigin",   SetOrigin},
        {"setPosition", SetPosition},
        {"getPosition", GetPosition},
        {"setRotation", SetRotation},
        {"getRotation", GetRotation},
        {"setScale",    SetScale},
        {"getScale",    GetScale},
        {"scale",       Scale},
        {"move",        Move},
        {"rotate",      Rotate},
        {"draw",        Draw},
    {0, 0}};

     static const luaL_Reg Meta[] = {
        {"__gc",        Destroy},
    {0, 0}};
}

#endif // REGULARSHAPE_H_INCLUDED
