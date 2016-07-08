#include "SFMLua/LuaAux.h"
#include "SFMLua/Utils.h"
#include <string>
#include <iostream>
#include <iterator>

int LuaAux::dumpFunction(lua_State *L, const void* p, size_t sz, void* d)
{
    ((std::string*)d)->append((const char*)p, sz);
    return 0;
}

int LuaAux::getCurrentLine(lua_State* L)
{
    lua_Debug ar;
    lua_getstack(L, 1, &ar);
    lua_getinfo(L, "l", &ar);
    return ar.currentline;
}

bool LuaAux::referUserData(lua_State* L, LuaUserData* lud)
{
    LuaUserData* ptr = static_cast<LuaUserData*>(lua_newuserdata(L, sizeof(LuaUserData)));

    if (!ptr)
        return false;

    ptr->refer(lud->aliases);
    ptr->metaname = lud->metaname;

    if (!ptr->metaname.empty())
        luaL_setmetatable(L, ptr->metaname.c_str());

    return true;
}

bool LuaAux::aliasUserData(lua_State* L, LuaUserData* lud)
{
    LuaUserData* ptr = static_cast<LuaUserData*>(lua_newuserdata(L, sizeof(LuaUserData)));

    if (!ptr)
        return false;

    ptr->alias(lud->aliases);
    ptr->metaname = lud->metaname;

    if (!ptr->metaname.empty())
        luaL_setmetatable(L, ptr->metaname.c_str());

    return true;
}

std::string LuaAux::getString(lua_State* L, int index)
{
    if (!lua_isstring(L, index))
        return std::string();

    return std::string(lua_tostring(L, index), lua_rawlen(L, index));
}

bool LuaAux::getBool(lua_State* L, int index, bool defaultBool)
{
    if (lua_isboolean(L, index))
        return lua_toboolean(L, index);

    if (lua_isnumber(L, index))
        return lua_tonumber(L, index) != 0;

    return defaultBool;
}
sf::Color LuaAux::getColor(lua_State* L, int index, bool isOneArg)
{
    sf::Color color(255, 255, 255, 255);
    unsigned arg;

    if (!isOneArg && lua_gettop(L) >= index + 3)
        color.a = (sf::Uint8)lua_tonumber(L, index + 3);

    if (!isOneArg && lua_gettop(L) >= index + 2)
    {
        color.r = (sf::Uint8)lua_tonumber(L, index);
        color.g = (sf::Uint8)lua_tonumber(L, index + 1);
        color.b = (sf::Uint8)lua_tonumber(L, index + 2);
    }
    else if (isOneArg)
    {
        if (lua_istable(L, index))
            color = getColorFromTable(L, index);
        else
        {
            arg = lua_tonumber(L, index);
            color.r = (sf::Uint8)((arg & 0xFF0000) >> 16);
            color.g = (sf::Uint8)((arg & 0xFF00) >> 8);
            color.b = (sf::Uint8)((arg & 0xFF));
            color.a = (sf::Uint8)((arg & 0xFF000000) >> 24);
        }
    }

    return color;
}

sf::Color LuaAux::getColorFromTable(lua_State* L, int index)
{
    sf::Color color(255, 255, 255, 255);
    int tableIndex = index;

    if (index == 0)
    {
        lua_pushvalue(L, 0);
        tableIndex = -1;
    }

    if (lua_istable(L, tableIndex))
    {
        lua_rawgeti(L, tableIndex, 1); lua_pop(L, 1); color.r = (sf::Uint8)lua_tonumber(L, 0);
        lua_rawgeti(L, tableIndex, 2); lua_pop(L, 1); color.g = (sf::Uint8)lua_tonumber(L, 0);
        lua_rawgeti(L, tableIndex, 3); lua_pop(L, 1); color.b = (sf::Uint8)lua_tonumber(L, 0);
        lua_rawgeti(L, tableIndex, 4); lua_pop(L, 1);

        if (lua_isnumber(L, 0))
            color.a = (sf::Uint8)lua_tonumber(L, 0);
    }

    if (index == 0)
        lua_pop(L, 1);

    return color;
}

void LuaAux::popBottom(lua_State* L, int elements)
{
    while (elements-- && lua_gettop(L))
        lua_remove(L, 1);
}

std::string LuaAux::getEntryFile(int argc, char* args[])
{
    const char* entryFile = "\0ForcedEntryFileModifiableString\1";

    if (entryFile[0] != '\0')
    // Allows user to modify binary after compilation
    // and force the program to that file as entry point
        return std::string(entryFile);

    std::string path;

    for (int i = 1; i < argc; i++)
    {
        path = args[i];

        if (Utils::checkFileExtension(path, ".lua") ||
            Utils::checkFileExtension(path, ".sfmlua"))
        // If argument is a lua file, load it
            return path;
        else if (Utils::isFile(path + "/main.lua"))
        // Maybe it is a directory, so check if it contains
        // a main.lua to be loaded.
            return path + "/main.lua";
    }

    if (Utils::isFile("main.lua"))
        return "main.lua";

    return std::string();
}

std::vector<std::string> LuaAux::splitLuaErrorPath(std::string error)
{
    std::vector<std::string> result;
    unsigned index;

    index = error.find(':');

    if (index == std::string::npos)
        return result;

    error = Utils::getPrettyPath(error.erase(index));
    std::string temp;

    for (index = 0; index < error.size(); index++)
    {
        if (error[index] == SLASH)
        {
            if (!temp.empty())
                result.push_back(temp);

            temp.clear();
        }
        else
            temp += error[index];
    }

    return result;
}

void LuaAux::newClass(lua_State* L, const luaL_Reg* methods, const luaL_Reg* metamethods,
                      std::string className, lua_CFunction callfunction, bool setGlobal)
{
    lua_newtable(L);

    if (methods)
    {
        for (int i = 0; methods[i].name; i++)
        {
            lua_pushcfunction(L, methods[i].func);
            lua_setfield(L, -2, methods[i].name);
        }
    }

    luaL_newmetatable(L, className.c_str());

    if (metamethods)
        for (int i = 0; metamethods[i].name; i++)
        {
            lua_pushcfunction(L, metamethods[i].func);
            lua_setfield(L, -2, metamethods[i].name);
        }

    lua_pushvalue(L, -2);
    lua_setfield(L, -2, "__index");

    //lua_pushvalue(L, -2);
    //lua_setfield(L, -2, "__metatable");

    lua_pop(L, 1);

    if (callfunction)
    {
        lua_newtable(L);
        lua_pushcfunction(L, callfunction);
        lua_setfield(L, -2, "__call");
        lua_setmetatable(L, -2);
    }

    if (setGlobal)
        lua_setglobal(L, className.c_str());
}

void LuaAux::newGlobal(lua_State* L, std::string globalName, lua_CFunction var)
{
    lua_pushcfunction(L, var);
    lua_setglobal(L, globalName.c_str());
}

void LuaAux::newGlobal(lua_State* L, std::string globalName, unsigned var)
{
    lua_pushnumber(L, var);
    lua_setglobal(L, globalName.c_str());
}

void LuaAux::newGlobal(lua_State* L, std::string globalName, int var)
{
    lua_pushinteger(L, var);
    lua_setglobal(L, globalName.c_str());
}

void LuaAux::newGlobal(lua_State* L, std::string globalName, float var)
{
    lua_pushnumber(L, var);
    lua_setglobal(L, globalName.c_str());
}

void LuaAux::newGlobal(lua_State* L, std::string globalName, std::string var)
{
    lua_pushstring(L, var.c_str());
    lua_setglobal(L, globalName.c_str());
}

void LuaAux::newGlobal(lua_State* L, std::string globalName, void* var)
{
    lua_pushlightuserdata(L, var);
    lua_setglobal(L, globalName.c_str());
}

void LuaAux::newGlobal(lua_State* L, std::string tableName, std::string globalName, lua_CFunction var)
{
    if (tableName.empty())
    {
        LuaAux::newGlobal(L, globalName, var);
        return;
    }

    lua_getglobal(L, tableName.c_str());

    if (lua_isnoneornil(L, -1) || !lua_istable(L, -1))
    {
        lua_pop(L, 1);
        lua_newtable(L);
    }

    lua_pushcfunction(L, var);
    lua_setfield(L, -2, globalName.c_str());
    lua_setglobal(L, tableName.c_str());
}

void LuaAux::newGlobal(lua_State* L, std::string tableName, std::string globalName, unsigned var)
{
    if (tableName.empty())
    {
        LuaAux::newGlobal(L, globalName, var);
        return;
    }

    lua_getglobal(L, tableName.c_str());

    if (lua_isnoneornil(L, -1) || !lua_istable(L, -1))
    {
        lua_pop(L, 1);
        lua_newtable(L);
    }

    lua_pushnumber(L, var);
    lua_setfield(L, -2, globalName.c_str());
    lua_setglobal(L, tableName.c_str());
}

void LuaAux::newGlobal(lua_State* L, std::string tableName, std::string globalName, int var)
{
    if (tableName.empty())
    {
        LuaAux::newGlobal(L, globalName, var);
        return;
    }

    lua_getglobal(L, tableName.c_str());

    if (lua_isnoneornil(L, -1) || !lua_istable(L, -1))
    {
        lua_pop(L, 1);
        lua_newtable(L);
    }

    lua_pushinteger(L, var);
    lua_setfield(L, -2, globalName.c_str());
    lua_setglobal(L, tableName.c_str());
}

void LuaAux::newGlobal(lua_State* L, std::string tableName, std::string globalName, float var)
{
    if (tableName.empty())
    {
        LuaAux::newGlobal(L, globalName, var);
        return;
    }

    lua_getglobal(L, tableName.c_str());

    if (lua_isnoneornil(L, -1) || !lua_istable(L, -1))
    {
        lua_pop(L, 1);
        lua_newtable(L);
    }

    lua_pushnumber(L, var);
    lua_setfield(L, -2, globalName.c_str());
    lua_setglobal(L, tableName.c_str());
}

void LuaAux::newGlobal(lua_State* L, std::string tableName, std::string globalName, std::string var)
{
    if (tableName.empty())
    {
        LuaAux::newGlobal(L, globalName, var);
        return;
    }

    lua_getglobal(L, tableName.c_str());

    if (lua_isnoneornil(L, -1) || !lua_istable(L, -1))
    {
        lua_pop(L, 1);
        lua_newtable(L);
    }

    lua_pushstring(L, var.c_str());
    lua_setfield(L, -2, globalName.c_str());
    lua_setglobal(L, tableName.c_str());
}

void LuaAux::newGlobal(lua_State* L, std::string tableName, std::string globalName, void* var)
{
    if (tableName.empty())
    {
        LuaAux::newGlobal(L, globalName, var);
        return;
    }

    lua_getglobal(L, tableName.c_str());

    if (lua_isnoneornil(L, -1) || !lua_istable(L, -1))
    {
        lua_pop(L, 1);
        lua_newtable(L);
    }

    lua_pushlightuserdata(L, var);
    lua_setfield(L, -2, globalName.c_str());
    lua_setglobal(L, tableName.c_str());
}

void LuaAux::setGlobal(lua_State* L, std::string tableName, std::string globalName)
{
    if (tableName.empty())
    {
        lua_setglobal(L, globalName.c_str());
        return;
    }

    int popIndex = lua_gettop(L);
    lua_getglobal(L, tableName.c_str());

    if (lua_isnoneornil(L, -1) || !lua_istable(L, -1))
    {
        lua_pop(L, 1);
        lua_newtable(L);
    }

    lua_pushvalue(L, popIndex);
    lua_setfield(L, -2, globalName.c_str());
    lua_setglobal(L, tableName.c_str());
    lua_pop(L, 1);
}
