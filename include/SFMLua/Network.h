#ifndef SOCKET_H
#define SOCKET_H

#include <SFML/Network.hpp>

namespace Network
{
    void Register(lua_State* L);
    void CloseSockets();

    int LuaFunction_getLocalIp(lua_State* L);
    int LuaFunction_getPublicIp(lua_State* L);
    int LuaFunction_getIp(lua_State* L);
}

namespace LuaUdpSocket
{
    int Create(lua_State* L);
    int Call(lua_State* L);
    int Bind(lua_State* L);
    int GetPort(lua_State* L);
    int Send(lua_State* L);
    int Receive(lua_State* L);
    int SetBlocking(lua_State* L);
    int IsBlocking(lua_State* L);
    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",  Create},
        {"bind",    Bind},
        {"getPort", GetPort},
        {"send",    Send},
        {"receive", Receive},
        {"setBlocking", SetBlocking},
        {"isBlocking", IsBlocking},
    {0, 0}};

    static const luaL_Reg Meta[] = {
        {"__gc",          Destroy},
        {"__eq", LuaAux::standardUserData__eq},
    {0, 0}};
}

namespace LuaTcpSocket
{
    int Create(lua_State* L);
    int Call(lua_State* L);
    int Accept(lua_State* L);
    int Connect(lua_State* L);
    int GetPort(lua_State* L);
    int GetRemotePort(lua_State* L);
    int GetRemoteIp(lua_State* L);
    int Send(lua_State* L);
    int Receive(lua_State* L);
    int SetBlocking(lua_State* L);
    int IsBlocking(lua_State* L);
    int IsConnected(lua_State* L);
    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",  Create},
        {"accept",  Accept},
        {"connect", Connect},
        {"isConnected", IsConnected},
        {"getPort", GetPort},
        {"getRemotePort", GetRemotePort},
        {"getRemoteIp", GetRemoteIp},
        {"send",    Send},
        {"receive", Receive},
        {"setBlocking", SetBlocking},
        {"isBlocking", IsBlocking},
    {0, 0}};

    static const luaL_Reg Meta[] = {
        {"__gc",          Destroy},
        {"__eq", LuaAux::standardUserData__eq},
    {0, 0}};
}

namespace LuaTcpListener
{
    int Create(lua_State* L);
    int Call(lua_State* L);
    int Accept(lua_State* L);
    int Listen(lua_State* L);
    int GetPort(lua_State* L);
    int SetBlocking(lua_State* L);
    int IsBlocking(lua_State* L);
    int Close(lua_State* L);
    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",  Create},
        {"getPort",  GetPort},
        {"accept",  Accept},
        {"listen", Listen},
        {"close", Close},
        {"setBlocking", SetBlocking},
        {"isBlocking", IsBlocking},
    {0, 0}};

    static const luaL_Reg Meta[] = {
        {"__gc",          Destroy},
        {"__eq", LuaAux::standardUserData__eq},
    {0, 0}};
}

namespace LuaSocketSelector
{
    int Create(lua_State* L);
    int Call(lua_State* L);
    int Add(lua_State* L);
    int Remove(lua_State* L);
    int Clear(lua_State* L);
    int Wait(lua_State* L);
    int IsReady(lua_State* L);
    int Destroy(lua_State* L);

    static const luaL_Reg Methods [] = {
        {"create",  Create},
        {"add",  Add},
        {"remove", Remove},
        {"clear", Clear},
        {"wait", Wait},
        {"isReady", IsReady},
    {0, 0}};

    static const luaL_Reg Meta[] = {
        {"__gc",          Destroy},
        {"__eq", LuaAux::standardUserData__eq},
    {0, 0}};
}
#endif // SOCKET_H
