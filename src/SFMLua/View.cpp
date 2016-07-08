#include <SFML/Graphics.hpp>
#include "SFMLua/LuaAux.h"
#include "SFMLua/Classes.h"

void View::Register(lua_State* L)
{
    LuaAux::newClass(L, Methods, Meta, VIEW_TABLE, Call);
}

int View::Call(lua_State* L)
{
    lua_remove(L, 1);
    return Create(L);
}

int View::Create(lua_State* L)
{
    sf::View* view = LuaAux::newUserData<sf::View>(L, true, VIEW_TABLE);

    if (!view)
        return 0;

    return 1;
}

int View::SetCenter(lua_State* L)
{
    sf::View* view = LuaAux::getUserData<sf::View>(L, 1, VIEW_TABLE);

    if (!view)
        return 0;

    view->setCenter(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int View::SetSize(lua_State* L)
{
    sf::View* view = LuaAux::getUserData<sf::View>(L, 1, VIEW_TABLE);

    if (!view)
        return 0;

    view->setSize(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int View::SetRotation(lua_State* L)
{
    sf::View* view = LuaAux::getUserData<sf::View>(L, 1, VIEW_TABLE);

    if (!view)
        return 0;

    view->setRotation(lua_tonumber(L, 2));
    return 0;
}

int View::SetViewport(lua_State* L)
{
    sf::View* view = LuaAux::getUserData<sf::View>(L, 1, VIEW_TABLE);

    if (!view)
        return 0;

    view->setViewport(sf::FloatRect(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5)));
    return 0;
}

int View::GetViewport(lua_State* L)
{
    sf::View* view = LuaAux::getUserData<sf::View>(L, 1, VIEW_TABLE);

    if (!view)
        return 0;

    sf::FloatRect r = view->getViewport();
    lua_pushnumber(L, r.top);
    lua_pushnumber(L, r.left);
    lua_pushnumber(L, r.width);
    lua_pushnumber(L, r.height);
    return 4;
}

int View::GetCenter(lua_State* L)
{
    sf::View* view = LuaAux::getUserData<sf::View>(L, 1, VIEW_TABLE);

    if (!view)
        return 0;

    sf::Vector2f center = view->getCenter();
    lua_pushnumber(L, center.x);
    lua_pushnumber(L, center.y);
    return 2;
}

int View::GetRotation(lua_State* L)
{
    sf::View* view = LuaAux::getUserData<sf::View>(L, 1, VIEW_TABLE);

    if (!view)
        return 0;

    float rot = view->getRotation();
    lua_pushnumber(L, rot);
    return 1;
}

int View::GetSize(lua_State* L)
{
    sf::View* view = LuaAux::getUserData<sf::View>(L, 1, VIEW_TABLE);

    if (!view)
        return 0;

    sf::Vector2f size = view->getSize();
    lua_pushnumber(L, size.x);
    lua_pushnumber(L, size.y);
    return 2;
}

int View::Zoom(lua_State* L)
{
    sf::View* view = LuaAux::getUserData<sf::View>(L, 1, VIEW_TABLE);

    if (!view)
        return 0;

    view->zoom(lua_tonumber(L, 2));
    return 0;
}

int View::Rotate(lua_State* L)
{
    sf::View* view = LuaAux::getUserData<sf::View>(L, 1, VIEW_TABLE);

    if (!view)
        return 0;

    view->rotate(lua_tonumber(L, 2));
    return 0;
}

int View::Move(lua_State* L)
{
    sf::View* view = LuaAux::getUserData<sf::View>(L, 1, VIEW_TABLE);

    if (!view)
        return 0;

    view->move(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int View::Destroy(lua_State* L)
{
    SFMLuaClassDestroy(sf::View);

    return 0;
}
