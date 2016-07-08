#include "SFMLua/LuaScript.h"
#include <sstream>
#include <iostream>

#include "windows.h"

sf::Context context;

int main(int argc, char* argv[])
{
    int ret = LuaScript::sfml_run(argc, argv);
    return ret;
}

extern "C"
{
    LUA_API int luaopen_sfmlua(lua_State* L)
    {
        LuaScript::RegisterFunctions(L);
        return 0;
    }
}
