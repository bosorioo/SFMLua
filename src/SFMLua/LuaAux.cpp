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

int LuaAux::standardUserData__eq(lua_State* L)
{
    if (!lua_isuserdata(L, 1) || !lua_isuserdata(L, 2))
        lua_pushboolean(L, false);
    else
        lua_pushboolean(L, static_cast<LuaUserData*>(lua_touserdata(L, 1))->block == static_cast<LuaUserData*>(lua_touserdata(L, 2))->block);

    return 1;
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
            color.a = (sf::Uint8)((arg & 0xFF000000) >> 24);
            color.r = (sf::Uint8)((arg & 0x00FF0000) >> 16);
            color.g = (sf::Uint8)((arg & 0x0000FF00) >> 8);
            color.b = (sf::Uint8)((arg & 0x000000FF));
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

std::vector<std::string> LuaAux::splitLuaErrorPath(const std::string& error)
{
    std::vector<std::string> result;
    unsigned index;

    index = error.find(':');

    if (index == std::string::npos)
        return result;

    std::string prettyError = Utils::getPrettyPath(std::string(error).erase(index));
    std::string temp;

    for (index = 0; index < prettyError.size(); index++)
    {
        if (prettyError[index] == SLASH)
        {
            if (!temp.empty())
                result.push_back(temp);

            temp.clear();
        }
        else
            temp += prettyError[index];
    }

    return result;
}

void LuaAux::newClass(lua_State* L, const luaL_Reg* methods, const luaL_Reg* metamethods,
                      const std::string& className, lua_CFunction callfunction, bool setGlobal)
{
    // This will create a table containing all functions provided with parameter 'methods'.
    // A metatable associated with 'className' will be created with all functions provided with 'metamethods'
    // Whenever an instance of the class 'className' is created, that instance will have this metatable set for it.
    // The call function, if provided, is inserted in a new table that will be the metatable for the first table created.
    // The call function allows the syntax 'local instance = className(...)', instead of 'local instance = className.create(...)'.


    lua_newtable(L); // table = {}

    if (methods)
    {
        for (int i = 0; methods[i].name; i++)
        {
            lua_pushcfunction(L, methods[i].func);
            lua_setfield(L, -2, methods[i].name); // table[methodName] = method
        }
    }

    luaL_newmetatable(L, className.c_str()); // metatable = {}, associated with className

    if (metamethods)
        for (int i = 0; metamethods[i].name; i++)
        {
            lua_pushcfunction(L, metamethods[i].func);
            lua_setfield(L, -2, metamethods[i].name); // metatable[methodName] = method
        }

    lua_pushvalue(L, -2);
    lua_setfield(L, -2, "__index"); // metatable.__index = table

    //lua_pushvalue(L, -2);
    //lua_setfield(L, -2, "__metatable");

    lua_pop(L, 1); // remove metatable from stack

    if (callfunction)
    {
        lua_newtable(L); // call_table = {}
        lua_pushcfunction(L, callfunction);
        lua_setfield(L, -2, "__call"); // call_table.__call = callfunction
        lua_setmetatable(L, -2); // setmetatable(table, call_table)
    }

    if (setGlobal)
        lua_setglobal(L, className.c_str());
}

void LuaAux::newGlobal(lua_State* L, const std::string& globalName, lua_CFunction var)
{
    lua_pushcfunction(L, var);
    lua_setglobal(L, globalName.c_str());
}

void LuaAux::newGlobal(lua_State* L, const std::string& globalName, unsigned var)
{
    lua_pushnumber(L, var);
    lua_setglobal(L, globalName.c_str());
}

void LuaAux::newGlobal(lua_State* L, const std::string& globalName, int var)
{
    lua_pushinteger(L, var);
    lua_setglobal(L, globalName.c_str());
}

void LuaAux::newGlobal(lua_State* L, const std::string& globalName, float var)
{
    lua_pushnumber(L, var);
    lua_setglobal(L, globalName.c_str());
}

void LuaAux::newGlobal(lua_State* L, const std::string& globalName, const std::string& var)
{
    lua_pushstring(L, var.c_str());
    lua_setglobal(L, globalName.c_str());
}

void LuaAux::newGlobal(lua_State* L, const std::string& globalName, void* var)
{
    lua_pushlightuserdata(L, var);
    lua_setglobal(L, globalName.c_str());
}

void LuaAux::newGlobal(lua_State* L, const std::string& tableName, const std::string& globalName, lua_CFunction var)
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

void LuaAux::newGlobal(lua_State* L, const std::string& tableName, const std::string& globalName, unsigned var)
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

void LuaAux::newGlobal(lua_State* L, const std::string& tableName, const std::string& globalName, int var)
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

void LuaAux::newGlobal(lua_State* L, const std::string& tableName, const std::string& globalName, float var)
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

void LuaAux::newGlobal(lua_State* L, const std::string& tableName, const std::string& globalName, const std::string& var)
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

void LuaAux::newGlobal(lua_State* L, const std::string& tableName, const std::string& globalName, void* var)
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

void LuaAux::setGlobal(lua_State* L, const std::string& tableName, const std::string& globalName)
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

LuaData LuaAux::getTableField(lua_State* L, int tableIndex, const std::string& fieldName)
{
    lua_getfield(L, tableIndex, fieldName.c_str());
    LuaData data(L, -1);
    lua_pop(L, 1);
    return data;
}
