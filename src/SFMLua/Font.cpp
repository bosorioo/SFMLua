#include "SFMLua/LuaAux.h"
#include "SFMLua/Classes.h"

void Font::Register(lua_State* L)
{
    LuaAux::newClass(L, Methods, Meta, FONT_TABLE, Call);
}

int Font::Call(lua_State* L)
{
    lua_remove(L, 1);
    return Create(L);
}

int Font::Create(lua_State* L)
{
    sf::Font* font = LuaAux::newUserData<sf::Font>(L, true, FONT_TABLE);
    std::string fontPath = LuaAux::getString(L, 1);
    bool loaded = true;

    if (!fontPath.empty())
        loaded = font->loadFromFile(fontPath);

    lua_pushboolean(L, loaded);
    return 2;
}

int Font::LoadFromFile(lua_State* L)
{
    sf::Font* f = LuaAux::getUserData<sf::Font>(L, 1, FONT_TABLE);

    if (!f)
        return 0;

    lua_pushboolean(L, f->loadFromFile(LuaAux::getString(L, 2)));
    return 1;
}

int Font::Destroy(lua_State* L)
{
    SFMLuaClassDestroy(sf::Font);

    return 0;
}
