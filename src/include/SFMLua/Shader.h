#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

namespace Shader
{
    void Register(lua_State* L);

    int Create(lua_State* L);
    int Call(lua_State* L);
    int IsAvailable(lua_State* L);
    int Bind(lua_State* L);
    int SetParameter(lua_State* L);
    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",      Create},
        {"isAvailable", IsAvailable},
        {"setParameter", SetParameter},
        {"bind", Bind},
    {0, 0}};

    static const luaL_Reg Meta[] = {
        {"__gc",        Destroy},
        {"__eq", LuaAux::standardUserData__eq},
    {0, 0}};

}

#endif // SHADER_H_INCLUDED
