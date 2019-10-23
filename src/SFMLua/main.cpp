#include "SFMLua/LuaScript.h"
#include <sstream>
#include <iostream>

#ifdef _WIN32
#include "windows.h"
#endif

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
