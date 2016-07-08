#include "SFMLua/LuaAux.h"
#include "SFMLua/Classes.h"
#include <set>

void Network::Register(lua_State* L)
{
    LuaAux::newClass(L, LuaUdpSocket::Methods, LuaUdpSocket::Meta, UDPSOCKET_TABLE, LuaUdpSocket::Call);
    LuaAux::newClass(L, LuaTcpSocket::Methods, LuaTcpSocket::Meta, TCPSOCKET_TABLE, LuaTcpSocket::Call);
    LuaAux::newClass(L, LuaSocketSelector::Methods, LuaSocketSelector::Meta, SOCKETSELECTOR_TABLE, LuaSocketSelector::Call);
    LuaAux::newClass(L, LuaTcpListener::Methods, LuaTcpListener::Meta, TCPLISTENER_TABLE, LuaTcpListener::Call);

    LuaAux::newGlobal(L, "Network", "Done", sf::TcpSocket::Done);
    LuaAux::newGlobal(L, "Network", "NotReady", sf::TcpSocket::NotReady);
    LuaAux::newGlobal(L, "Network", "Partial", sf::TcpSocket::Partial);
    LuaAux::newGlobal(L, "Network", "Disconnected", sf::TcpSocket::Disconnected);
    LuaAux::newGlobal(L, "Network", "Error", sf::TcpSocket::Error);

    LuaAux::newGlobal(L, "Network", "getLocalIp", Network::LuaFunction_getLocalIp);
    LuaAux::newGlobal(L, "Network", "getPublicIp", Network::LuaFunction_getPublicIp);
    LuaAux::newGlobal(L, "Network", "getIp", Network::LuaFunction_getIp);
}

std::set<sf::TcpSocket*> g_tcpSockets;
std::set<sf::UdpSocket*> g_udpSockets;

void Network::CloseSockets()
{
    auto it_tcp_end = g_tcpSockets.end();
    auto it_udp_end = g_udpSockets.end();

    for (auto it = g_tcpSockets.begin(); it != it_tcp_end; it++)
        (*it)->disconnect();

    for (auto it = g_udpSockets.begin(); it != it_udp_end; it++)
        (*it)->unbind();
}

int Network::LuaFunction_getLocalIp(lua_State* L)
{
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    bool stringFormat = LuaAux::getBool(L, 2, true);

    if (stringFormat)
        lua_pushstring(L, ip.toString().c_str());
    else
        lua_pushinteger(L, ip.toInteger());

    return 1;
}

int Network::LuaFunction_getPublicIp(lua_State* L)
{
    sf::IpAddress ip = sf::IpAddress::getPublicAddress();
    bool stringFormat = LuaAux::getBool(L, 2, true);

    if (stringFormat)
        lua_pushstring(L, ip.toString().c_str());
    else
        lua_pushinteger(L, ip.toInteger());

    return 1;
}

int Network::LuaFunction_getIp(lua_State* L)
{
    bool asString = true;

    if ((lua_gettop(L) == 2 || lua_gettop(L) == 5) && lua_isboolean(L, lua_gettop(L)) && lua_toboolean(L, lua_gettop(L)))
        asString = false;

    if (lua_gettop(L) >= 4)
    {
        if (asString)
            lua_pushstring(L, sf::IpAddress(lua_tonumber(L, 1), lua_tonumber(L, 2),
                                        lua_tonumber(L, 3), lua_tonumber(L, 4)).toString().c_str());
        else
            lua_pushinteger(L, sf::IpAddress(lua_tonumber(L, 1), lua_tonumber(L, 2),
                                        lua_tonumber(L, 3), lua_tonumber(L, 4)).toInteger());
        return 1;
    }

    switch (lua_type(L, 1))
    {
        case LUA_TSTRING:
            if (asString)
                lua_pushstring(L, sf::IpAddress(lua_tostring(L, 1)).toString().c_str());
            else
                lua_pushinteger(L, sf::IpAddress(lua_tostring(L, 1)).toInteger());
            return 1;

        case LUA_TNUMBER:
            if (asString)
                lua_pushstring(L, sf::IpAddress(lua_tonumber(L, 1)).toString().c_str());
            else
                lua_pushinteger(L, sf::IpAddress(lua_tonumber(L, 1)).toInteger());
            return 1;
        default: break;
    }

    return 0;
}

int LuaUdpSocket::Call(lua_State* L)
{
    lua_remove(L, 1);
    return LuaUdpSocket::Create(L);
}

int LuaUdpSocket::Create(lua_State* L)
{
    sf::UdpSocket* udp = LuaAux::newUserData<sf::UdpSocket>(L, true, UDPSOCKET_TABLE);

    if (!udp)
        return 0;

    g_udpSockets.insert(udp);

    if (lua_isnumber(L, 1))
    {
        lua_pushboolean(L, udp->bind(lua_tointeger(L, 1)) == sf::UdpSocket::Done);
        return 2;
    }

    return 1;
}

int LuaUdpSocket::Bind(lua_State* L)
{
    sf::UdpSocket* udp = LuaAux::getUserData<sf::UdpSocket>(L, 1, UDPSOCKET_TABLE);

    if (!udp)
        return 0;

    if (lua_isnumber(L, 2))
    {
        if (udp->getLocalPort())
            udp->unbind();

        if (lua_tointeger(L, 2) == 0)
            lua_pushboolean(L, udp->bind(sf::UdpSocket::AnyPort) == sf::UdpSocket::Done);
        else
            lua_pushboolean(L, udp->bind(lua_tointeger(L, 2)) == sf::UdpSocket::Done);

        return 1;
    }

    udp->unbind();
    return 0;
}

int LuaUdpSocket::GetPort(lua_State* L)
{
    sf::UdpSocket* udp = LuaAux::getUserData<sf::UdpSocket>(L, 1, UDPSOCKET_TABLE);

    if (!udp)
        return 0;

    lua_pushinteger(L, udp->getLocalPort());
    return 1;
}

int LuaUdpSocket::Send(lua_State* L)
{
    sf::UdpSocket* udp = LuaAux::getUserData<sf::UdpSocket>(L, 1, UDPSOCKET_TABLE);
    sf::IpAddress ip;
    size_t data_len;
    const char* data = lua_tolstring(L, 2, &data_len);

    if (!udp || data_len == 0)
        return 0;

    if (lua_type(L, 3) == LUA_TNUMBER)
        ip = sf::IpAddress(lua_tointeger(L, 3));
    else if (lua_type(L, 3) == LUA_TSTRING)
        ip = sf::IpAddress(lua_tostring(L, 3));
    else
        return 0;

    sf::UdpSocket::Status result = udp->send(data, data_len, ip, luaL_optinteger(L, 4, udp->getLocalPort()));
    lua_pushboolean(L, result == sf::UdpSocket::Done);
    return 1;
}

int LuaUdpSocket::Receive(lua_State* L)
{
    sf::UdpSocket* udp = LuaAux::getUserData<sf::UdpSocket>(L, 1, UDPSOCKET_TABLE);

    if (!udp)
        return 0;

    std::size_t dataMaxSize = luaL_optinteger(L, 2, 2048);
    std::size_t receivedSize;
    sf::IpAddress senderIp;
    sf::UdpSocket::Status result;
    unsigned short senderPort;
    char* dataPtr;

    try {

        dataPtr = new char[dataMaxSize];
        result = udp->receive((void*)dataPtr, dataMaxSize, receivedSize, senderIp, senderPort);

        if (result != sf::UdpSocket::Done)
        {
            delete [] dataPtr;
            lua_pushboolean(L, false);
            lua_pushinteger(L, result);
            return 2;
        }

        lua_pushlstring(L, dataPtr, receivedSize);

        if (LuaAux::getBool(L, 3, false))
            lua_pushstring(L, senderIp.toString().c_str());
        else
            lua_pushinteger(L, senderIp.toInteger());

        lua_pushinteger(L, senderPort);
        delete [] dataPtr;
        return 3;

    } catch (...) {

        lua_pushboolean(L, false);
        return 1;

    }

    return 0;
}

int LuaUdpSocket::SetBlocking(lua_State* L)
{
    sf::UdpSocket* udp = LuaAux::getUserData<sf::UdpSocket>(L, 1, UDPSOCKET_TABLE);

    if (!udp)
        return 0;

    udp->setBlocking(LuaAux::getBool(L, 2, true));
    return 0;
}

int LuaUdpSocket::IsBlocking(lua_State* L)
{
    sf::UdpSocket* udp = LuaAux::getUserData<sf::UdpSocket>(L, 1, UDPSOCKET_TABLE);

    if (!udp)
        return 0;

    lua_pushboolean(L, udp->isBlocking());
    return 1;
}

int LuaUdpSocket::Destroy(lua_State* L)
{
    SFMLuaClassDestroy(sf::UdpSocket);

    sf::UdpSocket* udp = LuaAux::getUserData<sf::UdpSocket>(L, 1, UDPSOCKET_TABLE);
    g_udpSockets.erase(udp);

    return 0;
}

int LuaTcpSocket::Call(lua_State* L)
{
    lua_remove(L, 1);
    return LuaTcpSocket::Create(L);
}

int LuaTcpSocket::Create(lua_State* L)
{
    if (lua_isnumber(L, 1))
        return LuaTcpListener::Create(L);

    sf::TcpSocket* tcp = LuaAux::newUserData<sf::TcpSocket>(L, true, TCPSOCKET_TABLE);

    if (!tcp)
        return 0;

    g_tcpSockets.insert(tcp);

    return 1;
}

int LuaTcpSocket::Accept(lua_State* L)
{
    sf::TcpSocket* tcp = LuaAux::getUserData<sf::TcpSocket>(L, 1, TCPSOCKET_TABLE);

    if (!tcp)
        return 0;

    sf::TcpSocket::Status result;
    sf::TcpListener listener;
    sf::SocketSelector selector;
    sf::Time timeout = sf::milliseconds(luaL_optinteger(L, 3, 0));

    result = listener.listen(lua_tointeger(L, 2));

    if (result != sf::TcpSocket::Done)
    {
        lua_pushboolean(L, false);
        lua_pushinteger(L, result);
        return 2;
    }

    selector.add(listener);

    if (selector.wait(timeout))
    {
        result = listener.accept(*tcp);

        if (result != sf::TcpSocket::Done)
        {
            lua_pushboolean(L, false);
            lua_pushinteger(L, result);
            return 2;
        }

        lua_pushboolean(L, true);
        return 1;
    }

    lua_pushboolean(L, false);
    return 1;
}

int LuaTcpSocket::Connect(lua_State* L)
{
    sf::TcpSocket* tcp = LuaAux::getUserData<sf::TcpSocket>(L, 1, TCPSOCKET_TABLE);

    if (!tcp)
        return 0;

    if (lua_gettop(L) == 1)
    {
        tcp->disconnect();
        return 0;
    }

    sf::IpAddress remoteAddress;
    unsigned short remotePort;
    sf::Time connectionTimeout = sf::Time::Zero;
    sf::TcpSocket::Status result;

    if (lua_type(L, 2) == LUA_TSTRING)
        remoteAddress = sf::IpAddress(lua_tostring(L, 2));
    else if (lua_type(L, 2) == LUA_TNUMBER)
        remoteAddress = sf::IpAddress(lua_tointeger(L, 2));
    else
        return 0;

    if (lua_isnumber(L, 3))
        remotePort = lua_tointeger(L, 3);
    else
        return 0;

    if (lua_isnumber(L, 4))
        connectionTimeout = sf::milliseconds(lua_tointeger(L, 4));

    result = tcp->connect(remoteAddress, remotePort, connectionTimeout);

    if (result == sf::TcpSocket::Done)
    {
        lua_pushboolean(L, true);
        return 1;
    }

    lua_pushboolean(L, false);
    lua_pushinteger(L, result);
    return 2;
}

int LuaTcpSocket::IsConnected(lua_State* L)
{
     sf::TcpSocket* tcp = LuaAux::getUserData<sf::TcpSocket>(L, 1, TCPSOCKET_TABLE);

    if (!tcp)
        return 0;

    lua_pushboolean(L, tcp->getRemoteAddress().toInteger() != 0);
    return 1;
}

int LuaTcpSocket::GetPort(lua_State* L)
{
    sf::TcpSocket* tcp = LuaAux::getUserData<sf::TcpSocket>(L, 1, TCPSOCKET_TABLE);

    if (!tcp)
        return 0;

    lua_pushinteger(L, tcp->getLocalPort());
    return 1;
}

int LuaTcpSocket::GetRemotePort(lua_State* L)
{
    sf::TcpSocket* tcp = LuaAux::getUserData<sf::TcpSocket>(L, 1, TCPSOCKET_TABLE);

    if (!tcp)
        return 0;

    lua_pushinteger(L, tcp->getRemotePort());
    return 1;
}

int LuaTcpSocket::GetRemoteIp(lua_State* L)
{
    sf::TcpSocket* tcp = LuaAux::getUserData<sf::TcpSocket>(L, 1, TCPSOCKET_TABLE);

    if (!tcp)
        return 0;

    sf::IpAddress ip = tcp->getRemoteAddress();

    if (LuaAux::getBool(L, 2, true))
        lua_pushstring(L, ip.toString().c_str());
    else
        lua_pushinteger(L, ip.toInteger());

    return 1;
}

int LuaTcpSocket::Send(lua_State* L)
{
    sf::TcpSocket* tcp = LuaAux::getUserData<sf::TcpSocket>(L, 1, TCPSOCKET_TABLE);
    size_t data_len;
    const char* data = lua_tolstring(L, 2, &data_len);

    if (!tcp || !data_len)
        return 0;

    sf::TcpSocket::Status result = tcp->send((void*)data, data_len);

    if (result == sf::TcpSocket::Done)
    {
        lua_pushboolean(L, true);
        return 1;
    }

    lua_pushboolean(L, false);
    lua_pushinteger(L, result);
    return 2;
}

int LuaTcpSocket::Receive(lua_State* L)
{
    sf::TcpSocket* tcp = LuaAux::getUserData<sf::TcpSocket>(L, 1, TCPSOCKET_TABLE);

    if (!tcp)
        return 0;

    std::size_t dataMaxSize = luaL_optinteger(L, 2, 2048);
    std::size_t receivedSize;
    sf::TcpSocket::Status result;
    char* dataPtr = NULL;

    try {

        dataPtr = new char[dataMaxSize];
        result = tcp->receive((void*)dataPtr, dataMaxSize, receivedSize);

        if (result != sf::TcpSocket::Done)
        {
            delete [] dataPtr;
            lua_pushboolean(L, false);
            lua_pushinteger(L, result);
            return 2;
        }

        lua_pushlstring(L, dataPtr, receivedSize);
        lua_pushinteger(L, result);
        delete [] dataPtr;
        return 2;

    } catch (...) {

        if (dataPtr)
            delete [] dataPtr;

        lua_pushboolean(L, false);
        return 1;

    }

    return 0;
}

int LuaTcpSocket::SetBlocking(lua_State* L)
{
    sf::TcpSocket* tcp = LuaAux::getUserData<sf::TcpSocket>(L, 1, TCPSOCKET_TABLE);

    if (!tcp)
        return 0;

    tcp->setBlocking(LuaAux::getBool(L, 2, true));
    return 0;
}

int LuaTcpSocket::IsBlocking(lua_State* L)
{
    sf::TcpSocket* tcp = LuaAux::getUserData<sf::TcpSocket>(L, 1, TCPSOCKET_TABLE);

    if (!tcp)
        return 0;

    lua_pushboolean(L, tcp->isBlocking());
    return 1;
}

int LuaTcpSocket::Destroy(lua_State* L)
{
    SFMLuaClassDestroy(sf::TcpSocket);

    sf::TcpSocket* tcp = LuaAux::getUserData<sf::TcpSocket>(L, 1, TCPSOCKET_TABLE);
    g_tcpSockets.erase(tcp);

    return 0;
}

int LuaTcpListener::Call(lua_State* L)
{
    lua_remove(L, 1);
    return LuaTcpListener::Create(L);
}

int LuaTcpListener::Create(lua_State* L)
{
    sf::TcpListener* lst = LuaAux::newUserData<sf::TcpListener>(L, true, TCPLISTENER_TABLE);

    if (!lst)
        return 0;

    if (lua_isnumber(L, 1))
    {
        lua_pushinteger(L, lst->listen(luaL_optinteger(L, 1, 0)));
        return 2;
    }

    return 1;
}

int LuaTcpListener::Accept(lua_State* L)
{
    sf::TcpListener* lst = LuaAux::getUserData<sf::TcpListener>(L, 1, TCPLISTENER_TABLE);
    sf::TcpSocket* tcp = LuaAux::getUserData<sf::TcpSocket>(L, 2, TCPSOCKET_TABLE);

    if (!tcp || !lst)
        return 0;

    sf::Socket::Status result;
    result = lst->accept(*tcp);
    lua_pushboolean(L, result == sf::Socket::Done);
    lua_pushinteger(L, result);
    return 2;
}

int LuaTcpListener::Listen(lua_State* L)
{
    sf::TcpListener* lst = LuaAux::getUserData<sf::TcpListener>(L, 1, TCPLISTENER_TABLE);

    if (!lst)
        return 0;

    sf::Socket::Status result;
    result = lst->listen(luaL_optinteger(L, 2, 0));
    lua_pushboolean(L, result == sf::Socket::Done);
    lua_pushinteger(L, result);
    return 2;
}

int LuaTcpListener::GetPort(lua_State* L)
{
    sf::TcpListener* lst = LuaAux::getUserData<sf::TcpListener>(L, 1, TCPLISTENER_TABLE);

    if (!lst)
        return 0;

    lua_pushinteger(L, lst->getLocalPort());
    return 1;
}

int LuaTcpListener::Close(lua_State* L)
{
    sf::TcpListener* lst = LuaAux::getUserData<sf::TcpListener>(L, 1, TCPLISTENER_TABLE);

    if (!lst)
        return 0;

    lst->close();
    return 0;
}

int LuaTcpListener::SetBlocking(lua_State* L)
{
    sf::TcpListener* lst = LuaAux::getUserData<sf::TcpListener>(L, 1, TCPLISTENER_TABLE);

    if (!lst)
        return 0;

    lst->setBlocking(LuaAux::getBool(L, 2, true));
    return 0;
}

int LuaTcpListener::IsBlocking(lua_State* L)
{
    sf::TcpListener* lst = LuaAux::getUserData<sf::TcpListener>(L, 1, TCPLISTENER_TABLE);

    if (!lst)
        return 0;

    lua_pushboolean(L, lst->isBlocking());
    return 1;
}

int LuaTcpListener::Destroy(lua_State* L)
{
    SFMLuaClassDestroy(sf::TcpListener);
    return 0;
}

int LuaSocketSelector::Call(lua_State* L)
{
    lua_remove(L, 1);
    return LuaSocketSelector::Create(L);
}

int LuaSocketSelector::Create(lua_State* L)
{
    sf::SocketSelector* ss = LuaAux::newUserData<sf::SocketSelector>(L, true, SOCKETSELECTOR_TABLE);

    if (!ss)
        return 0;

    return 1;
}

int LuaSocketSelector::Add(lua_State* L)
{
    sf::SocketSelector* ss = LuaAux::getUserData<sf::SocketSelector>(L, 1, SOCKETSELECTOR_TABLE);

    if (!ss)
        return 0;

    sf::UdpSocket* udp;
    sf::TcpSocket* tcp;
    sf::TcpListener* lst;

    if ((udp = LuaAux::getUserData<sf::UdpSocket>(L, 2, UDPSOCKET_TABLE)))
        ss->add(*udp);
    else if ((tcp = LuaAux::getUserData<sf::TcpSocket>(L, 2, TCPSOCKET_TABLE)))
        ss->add(*tcp);
    else if ((lst = LuaAux::getUserData<sf::TcpListener>(L, 2, TCPLISTENER_TABLE)))
        ss->add(*lst);

    return 0;
}

int LuaSocketSelector::Remove(lua_State* L)
{
    sf::SocketSelector* ss = LuaAux::getUserData<sf::SocketSelector>(L, 1, SOCKETSELECTOR_TABLE);

    if (!ss)
        return 0;

    sf::UdpSocket* udp;
    sf::TcpSocket* tcp;
    sf::TcpListener* lst;

    if ((udp = LuaAux::getUserData<sf::UdpSocket>(L, 2, UDPSOCKET_TABLE)))
        ss->remove(*udp);
    else if ((tcp = LuaAux::getUserData<sf::TcpSocket>(L, 2, TCPSOCKET_TABLE)))
        ss->remove(*tcp);
    else if ((lst = LuaAux::getUserData<sf::TcpListener>(L, 2, TCPLISTENER_TABLE)))
        ss->remove(*lst);

    return 0;
}

int LuaSocketSelector::Clear(lua_State* L)
{
    sf::SocketSelector* ss = LuaAux::getUserData<sf::SocketSelector>(L, 1, SOCKETSELECTOR_TABLE);

    if (!ss)
        return 0;

    ss->clear();

    return 0;
}

int LuaSocketSelector::Wait(lua_State* L)
{
    sf::SocketSelector* ss = LuaAux::getUserData<sf::SocketSelector>(L, 1, SOCKETSELECTOR_TABLE);

    if (!ss)
        return 0;

    lua_pushboolean(L, ss->wait(sf::milliseconds(LuaAux::getNumber<int>(L, 2, 0))));

    return 1;
}

int LuaSocketSelector::IsReady(lua_State* L)
{
    sf::SocketSelector* ss = LuaAux::getUserData<sf::SocketSelector>(L, 1, SOCKETSELECTOR_TABLE);

    if (!ss)
        return 0;

    sf::UdpSocket* udp;
    sf::TcpSocket* tcp;
    sf::TcpListener* lst;

    if ((udp = LuaAux::getUserData<sf::UdpSocket>(L, 2, UDPSOCKET_TABLE)))
        lua_pushboolean(L, ss->isReady(*udp));
    else if ((tcp = LuaAux::getUserData<sf::TcpSocket>(L, 2, TCPSOCKET_TABLE)))
        lua_pushboolean(L, ss->isReady(*tcp));
    else if ((lst = LuaAux::getUserData<sf::TcpListener>(L, 2, TCPLISTENER_TABLE)))
        lua_pushboolean(L, ss->isReady(*lst));

    return 1;
}

int LuaSocketSelector::Destroy(lua_State* L)
{
    SFMLuaClassDestroy(sf::SocketSelector);

    return 0;
}
