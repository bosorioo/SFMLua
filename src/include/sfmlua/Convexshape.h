#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

namespace ConvexShape
{
    void Register(lua_State* L);

    int Create(lua_State* L);
    int SetPoints(lua_State* L);
    int SetPointCount(lua_State* L);
    int GetPointCount(lua_State* L);
    int SetPoint(lua_State* L);
    int GetPoint(lua_State* L);
    int Call(lua_State* L);
    int SetFillColor(lua_State* L);
    int SetOutlineColor(lua_State* L);
    int SetPosition(lua_State* L);
    int SetRotation(lua_State* L);
    int SetScale(lua_State* L);
    int Rotate(lua_State* L);
    int Scale(lua_State* L);
    int Move(lua_State* L);
    int GetFillColor(lua_State* L);
    int GetOutlineColor(lua_State* L);
    int GetPosition(lua_State* L);
    int GetRotation(lua_State* L);
    int SetOutlineThick(lua_State* L);
    int GetOutlineThick(lua_State* L);
    int GetBounds(lua_State* L);
    int GetScale(lua_State* L);
    int GetTexture(lua_State* L);
    int SetTexture(lua_State* L);
    int SetOrigin(lua_State* L);
    int Draw(lua_State* L);
    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",        Create},
        {"setPoints",     SetPoints},
        {"setPointCount", SetPointCount},
        {"getPointCount", GetPointCount},
        {"setPoint",      SetPoint},
        {"getPoint",      GetPoint},
        {"setTexture",    SetTexture},
        {"getTexture",    GetTexture},
        {"setFillColor",  SetFillColor},
        {"setOutlineColor",SetOutlineColor},
        {"getFillColor",  GetFillColor},
        {"getOutlineColor",GetOutlineColor},
        {"getOutlineThick",GetOutlineThick},
        {"setOutlineThick",SetOutlineThick},
        {"setOrigin",     SetOrigin},
        {"setPosition",   SetPosition},
        {"getPosition",   GetPosition},
        {"setRotation",   SetRotation},
        {"getRotation",   GetRotation},
        {"setScale",      SetScale},
        {"getScale",      GetScale},
        {"getBounds",     GetBounds},
        {"scale",         Scale},
        {"move",          Move},
        {"rotate",        Rotate},
        {"draw",          Draw},
    {0, 0}};

    static const luaL_Reg Meta[] = {
         {"__gc",          Destroy},
         {"__eq", LuaAux::standardUserData__eq},
    {0, 0}};
}

#endif // SHAPE_H_INCLUDED
