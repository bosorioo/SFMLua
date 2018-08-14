#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

namespace Text
{
    void Register(lua_State* L);

    int Create(lua_State* L);
    int Call(lua_State* L);
    int SetStyle(lua_State* L);
    int SetColor(lua_State* L);
    int GetColor(lua_State* L);
    int SetString(lua_State* L);
    int GetString(lua_State* L);
    int SetRotation(lua_State* L);
    int GetRotation(lua_State* L);
    int SetPosition(lua_State* L);
    int GetPosition(lua_State* L);
    int SetOrigin(lua_State* L);
    int GetOrigin(lua_State* L);
    int SetCharacterSize(lua_State* L);
    int GetCharacterSize(lua_State* L);
    int GetFont(lua_State* L);
    int SetFont(lua_State* L);
    int SetScale(lua_State* L);
    int Scale(lua_State* L);
    int Move(lua_State* L);
    int Draw(lua_State* L);
    int GetCharacterPosition(lua_State *);
    int GetSize(lua_State* L);
    int GetBounds(lua_State *);
    int GetWidth(lua_State *);
    int GetHeight(lua_State *);
    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",          Create},
        {"setStyle",        SetStyle},
        {"setColor",        SetColor},
        {"getColor",        GetColor},
        {"setString",       SetString},
        {"getString",       GetString},
        {"setRotation",     SetRotation},
        {"getRotation",     GetRotation},
        {"setOrigin",       SetOrigin},
        {"getOrigin",       GetOrigin},
        {"setPosition",     SetPosition},
        {"getPosition",     GetPosition},
        {"getCharPosition", GetCharacterPosition},
        {"getBounds",       GetBounds},
        {"getWidth",        GetWidth},
        {"getHeight",       GetHeight},
        {"getSize",         GetSize},
        {"getCharacterSize",GetCharacterSize},
        {"setCharacterSize",SetCharacterSize},
        {"setFont",         SetFont},
        {"getFont",         GetFont},
        {"setScale",        SetScale},
        {"move",            Move},
        {"scale",           Scale},
        {"draw",            Draw},
    {0, 0}};

     static const luaL_Reg Meta[] = {
        {"__gc",          Destroy},
        //{"__tostring",    GetString},
        {"__eq", LuaAux::standardUserData__eq},
    {0, 0}};
}

#endif // TEXT_H_INCLUDED
