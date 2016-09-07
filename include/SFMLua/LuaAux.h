#ifndef LUAHELPER_H_INCLUDED
#define LUAHELPER_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "SFMLuaClass.h"
#include <string>
#include <memory>
#include "Lua/lua.hpp"

namespace LuaAux
{
    int dumpFunction(lua_State* L, const void* p, size_t sz, void* d);
    int standardUserData__eq(lua_State* L);

    int getCurrentLine(lua_State* L);

    template <class C> C* getUserData(lua_State* L, int index, std::string typeCheck = "");
    template <class C> C* newUserData(lua_State* L, bool defaultConstructor = true, std::string metaType = "");

    bool referUserData(lua_State* L, LuaUserData* ref_ptr);
    bool aliasUserData(lua_State* L, LuaUserData* ref_ptr);

    template <class Number> Number getNumber(lua_State* L, int index, Number defaultNumber = 0);
    std::string getString(lua_State* L, int index);
    bool getBool(lua_State* L, int index, bool defaultBool = false);
    sf::Color getColor(lua_State* L, int index, bool isTable = true);
    sf::Color getColorFromTable(lua_State*, int index);

    void popBottom(lua_State* L, int elements = 1);

    std::string getEntryFile(int argc, char* args[]);
    std::vector<std::string> splitLuaErrorPath(std::string error);

    void newClass(lua_State* L, const luaL_Reg* methods, const luaL_Reg* metamethods,
                          std::string className, lua_CFunction callfunction, bool setGlobal = true);

    void newGlobal(lua_State* L, std::string globalName, lua_CFunction var);
    void newGlobal(lua_State* L, std::string globalName, unsigned var);
    void newGlobal(lua_State* L, std::string globalName, int var);
    void newGlobal(lua_State* L, std::string globalName, float var);
    void newGlobal(lua_State* L, std::string globalName, std::string var);
    void newGlobal(lua_State* L, std::string globalName, void* var);
    void newGlobal(lua_State* L, std::string tableName, std::string globalName, lua_CFunction var);
    void newGlobal(lua_State* L, std::string tableName, std::string globalName, unsigned var);
    void newGlobal(lua_State* L, std::string tableName, std::string globalName, int var);
    void newGlobal(lua_State* L, std::string tableName, std::string globalName, float var);
    void newGlobal(lua_State* L, std::string tableName, std::string globalName, std::string var);
    void newGlobal(lua_State* L, std::string tableName, std::string globalName, void* var);
    void setGlobal(lua_State* L, std::string tableName, std::string globalName);
};

template <class C> C* LuaAux::newUserData(lua_State* L, bool defaultConstructor, std::string metaType)
{
    LuaUserData* ptr = static_cast<LuaUserData*>(lua_newuserdata(L, sizeof(LuaUserData)));

    if (!ptr)
        return nullptr;

    ptr->alloc<C>(sizeof(C), defaultConstructor);

    if (!metaType.empty())
    {
        luaL_setmetatable(L, metaType.c_str());
        ptr->metaname = metaType;
    }

    return reinterpret_cast<C*>(ptr->block);
}

template <class C> C* LuaAux::getUserData(lua_State* L, int index, std::string typeCheck)
{
    if (!lua_isuserdata(L, index))
        return nullptr;

    LuaUserData* ptr = static_cast<LuaUserData*>(lua_touserdata(L, index));

    if (ptr)
    {
        C* userdata = reinterpret_cast<C*>(ptr->block);

        if (typeCheck.empty())
            return userdata;

        return ptr->metaname == typeCheck ? userdata : nullptr;
    }

    return nullptr;
}

template <class Number> Number LuaAux::getNumber(lua_State* L, int index, Number defaultNumber)
{
    if (!lua_isnumber(L, index))
        return defaultNumber;

    return (Number)lua_tonumber(L, index);
}

#endif // LUAHELPER_H_INCLUDED
