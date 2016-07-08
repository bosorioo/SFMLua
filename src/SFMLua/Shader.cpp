#include "SFMLua/LuaAux.h"
#include "SFMLua/Classes.h"

sf::Shader* g_Shader = NULL;

void Shader::Register(lua_State* L)
{
    LuaAux::newClass(L, Methods, Meta, SHADER_TABLE, Call);
}

int Shader::Call(lua_State* L)
{
    lua_remove(L, 1);
    return Create(L);
}

int Shader::Create(lua_State* L)
{
    bool loaded = false;
    sf::Shader::Type type = sf::Shader::Fragment;
    std::string text = luaL_optstring(L, 1, "");

    if (lua_toboolean(L, 2))
        type = sf::Shader::Vertex;

    sf::Shader* s = LuaAux::newUserData<sf::Shader>(L, true, SHADER_TABLE);

    if (text.find('\n') == std::string::npos)
        loaded = s->loadFromFile(text, type);
    else
        loaded = s->loadFromMemory(text, type);

    if (!loaded)
    {
        lua_pushboolean(L, false);
        return 1;
    }

    return 1;
}

int Shader::IsAvailable(lua_State* L)
{
    lua_pushboolean(L, sf::Shader::isAvailable());
    return 1;
}

int Shader::Bind(lua_State* L)
{
    sf::Shader* s = LuaAux::getUserData<sf::Shader>(L, 1, SHADER_TABLE);
    sf::Shader::bind(s);
    g_Shader = s;
    return 0;
}

int Shader::SetParameter(lua_State* L)
{
    sf::Shader* s = LuaAux::getUserData<sf::Shader>(L, 1, SHADER_TABLE);

    if (!s || lua_type(L, 2) != LUA_TSTRING)
        return 0;

    if (lua_gettop(L) == 3)
        s->setParameter(lua_tostring(L, 2), lua_tonumber(L, 3));
    else if (lua_gettop(L) == 4)
        s->setParameter(lua_tostring(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
    else if (lua_gettop(L) == 5)
        s->setParameter(lua_tostring(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
    else if (lua_gettop(L) == 6)
        s->setParameter(lua_tostring(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
    return 0;
}

int Shader::Destroy(lua_State* L)
{
    SFMLuaClassDestroy(sf::Shader);
    return 0;
}
