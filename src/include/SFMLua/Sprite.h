#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

namespace Sprite
{
    void Register(lua_State* L);

    int Create(lua_State* L);
    int Call(lua_State* L);
    int SetColor(lua_State* L);
    int SetPosition(lua_State* L);
    int SetRotation(lua_State* L);
    int SetScale(lua_State* L);
    int Rotate(lua_State* L);
    int Scale(lua_State* L);
    int Move(lua_State* L);
    int GetColor(lua_State* L);
    int GetPosition(lua_State* L);
    int GetSize(lua_State* L);
    int SetSize(lua_State* L);
    int GetRotation(lua_State* L);
    int GetScale(lua_State* L);
    int GetTexture(lua_State* L);
    int SetTexture(lua_State* L);
    int SetRect(lua_State* L);
    int GetRect(lua_State* L);
    int SetOrigin(lua_State* L);
    int GetOrigin(lua_State* L);
    int Draw(lua_State* L);
    int GetBounds(lua_State* L);
    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",      Create},
        {"setTexture",  SetTexture},
        {"setRect",     SetRect},
        {"getRect",     GetRect},
        {"getTexture",  GetTexture},
        {"setColor",    SetColor},
        {"getColor",    GetColor},
        {"setOrigin",   SetOrigin},
        {"getOrigin",   GetOrigin},
        {"setPosition", SetPosition},
        {"getPosition", GetPosition},
        {"setRotation", SetRotation},
        {"getRotation", GetRotation},
        {"getBounds",   GetBounds},
        {"getSize",     GetSize},
        {"setSize",     SetSize},
        {"setScale",    SetScale},
        {"getScale",    GetScale},
        {"scale",       Scale},
        {"move",        Move},
        {"rotate",      Rotate},
        {"draw",        Draw},
    {0, 0}};

     static const luaL_Reg Meta[] = {
        {"__gc",        Destroy},
        {"__eq", LuaAux::standardUserData__eq},
    {0, 0}};
}

#endif // SPRITE_H_INCLUDED
