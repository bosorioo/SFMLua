#include <iostream>
#include "SFMLua/LuaAux.h"
#include "SFMLua/Classes.h"

extern sf::Shader* g_Shader;

void ConvexShape::Register(lua_State* L)
{
    LuaAux::newClass(L, Methods, Meta, CONVEXSHAPE_TABLE, Call);
}

int ConvexShape::Call(lua_State* L)
{
    lua_remove(L, 1);
    return Create(L);
}

int ConvexShape::Create(lua_State* L)
{
    unsigned pointCount = luaL_optnumber(L, 1, 0);
    sf::ConvexShape* shp = LuaAux::newUserData<sf::ConvexShape>(L, false, CONVEXSHAPE_TABLE);
    new (shp) sf::ConvexShape(pointCount);
    return 1;
}

int ConvexShape::SetPoints(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);
    sf::Vector2f position;

    if (!shp)
        return 0;

    if (!lua_istable(L, 2))
        return 0;

    unsigned length = lua_rawlen(L, 2);

    if (length < 6 || length % 2)
        return 0;

    shp->setPointCount(length/2);

    for (unsigned i = 0; i < length;)
    {
        lua_rawgeti(L, 2, ++i); lua_pop(L, 1);
        position.x = lua_tonumber(L, 0);

        lua_rawgeti(L, 2, ++i); lua_pop(L, 1);
        position.y = lua_tonumber(L, 0);

        shp->setPoint(i/2-1, position);
    }

    return 0;
}

int ConvexShape::SetPointCount(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp || !lua_isnumber(L, 2))
        return 0;

    shp->setPointCount(lua_tonumber(L, 2));
    return 0;
}

int ConvexShape::GetPointCount(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp)
        return 0;

    lua_pushnumber(L, shp->getPointCount());
    return 1;
}

int ConvexShape::SetPoint(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp || !lua_isnumber(L, 4) || !lua_isnumber(L, 3) || !lua_isnumber(L, 2))
        return 0;

    sf::Vector2f position(lua_tonumber(L, 3), lua_tonumber(L, 4));
    unsigned i = lua_tonumber(L, 2) - 1;

    if (i > shp->getPointCount())
        return 0;

    shp->setPoint(i, position);
    return 0;
}

int ConvexShape::GetPoint(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp || !lua_isnumber(L, 2))
        return 0;

    unsigned i = lua_tonumber(L, 2) - 1;

    if (i > shp->getPointCount())
        return 0;

    sf::Vector2f position = shp->getPoint(i);
    lua_pushnumber(L, position.x);
    lua_pushnumber(L, position.y);
    return 2;
}

int ConvexShape::SetTexture(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);
    sf::Texture* tex = LuaAux::getUserData<sf::Texture>(L, 2, TEXTURE_TABLE);

    if (!shp || !tex)
        return 0;

    shp->setTexture(tex);
    return 0;
}

int ConvexShape::SetFillColor(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp)
        return 0;

    shp->setFillColor(LuaAux::getColor(L, 2, lua_gettop(L) == 2));
    return 0;
}

int ConvexShape::SetOutlineColor(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp)
        return 0;

    shp->setOutlineColor(LuaAux::getColor(L, 2, lua_gettop(L) == 2));
    return 0;
}

int ConvexShape::SetOutlineThick(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp)
        return 0;

    shp->setOutlineThickness(lua_tonumber(L, 2));
    return 0;
}

int ConvexShape::GetOutlineThick(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp)
        return 0;

    lua_pushnumber(L, shp->getOutlineThickness());
    return 1;
}

int ConvexShape::GetFillColor(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp)
        return 0;

    sf::Color color = shp->getFillColor();
    lua_pushinteger(L, color.r);
    lua_pushinteger(L, color.g);
    lua_pushinteger(L, color.b);
    lua_pushinteger(L, color.a);
    return 4;
}

int ConvexShape::GetOutlineColor(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp)
        return 0;

    sf::Color color = shp->getOutlineColor();
    lua_pushinteger(L, color.r);
    lua_pushinteger(L, color.g);
    lua_pushinteger(L, color.b);
    lua_pushinteger(L, color.a);
    return 4;
}

int ConvexShape::GetBounds(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp)
        return 0;

    sf::FloatRect b = lua_toboolean(L, 2) ? shp->getGlobalBounds() : shp->getLocalBounds();
    lua_pushnumber(L, b.left);
    lua_pushnumber(L, b.top);
    lua_pushnumber(L, b.width);
    lua_pushnumber(L, b.height);
    return 4;
}

int ConvexShape::SetOrigin(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        return 0;

    shp->setOrigin(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int ConvexShape::SetPosition(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        return 0;

    shp->setPosition(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int ConvexShape::Move(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        return 0;

    shp->move(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int ConvexShape::GetPosition(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp)
        return 0;

    sf::Vector2f pos = shp->getPosition();
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int ConvexShape::SetRotation(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp || !lua_isnumber(L, 2))
        return 0;

    shp->setRotation(lua_tonumber(L, 2));
    return 0;
}

int ConvexShape::Rotate(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp || !lua_isnumber(L, 2))
        return 0;

    shp->rotate(lua_tonumber(L, 2));
    return 0;
}

int ConvexShape::GetRotation(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp)
        return 0;

    lua_pushnumber(L, shp->getRotation());
    return 1    ;
}

int ConvexShape::SetScale(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp)
        return 0;

    sf::Vector2f factors(1., 1.);

    if (lua_isnumber(L, 2))
        factors.x = lua_tonumber(L, 2);

    if (lua_isnumber(L, 3))
        factors.y = lua_tonumber(L, 3);
    else
        factors.y = factors.x;

    shp->setScale(factors);
    return 0;
}

int ConvexShape::Scale(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp)
        return 0;

    sf::Vector2f factors(0., 0.);

    if (lua_isnumber(L, 2))
        factors.x = lua_tonumber(L, 2);
    if (lua_isnumber(L, 3))
        factors.y = lua_tonumber(L, 3);
    else
        factors.y = factors.x;

    shp->scale(factors);
    return 0;
}

int ConvexShape::GetScale(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp)
        return 0;

    sf::Vector2f factors = shp->getScale();
    lua_pushnumber(L, factors.x);
    lua_pushnumber(L, factors.y);
    return 2;
}

int ConvexShape::GetTexture(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp)
        return 0;

    const sf::Texture* tex = shp->getTexture();
    lua_pushlightuserdata(L, (void*)tex);
    luaL_setmetatable(L, TEXTURE_TABLE);
    return 1;
}

int ConvexShape::Draw(lua_State* L)
{
    sf::ConvexShape* shp = LuaAux::getUserData<sf::ConvexShape>(L, 1, CONVEXSHAPE_TABLE);

    if (!shp)
        return 0;

    sf::RenderWindow* window = LuaAux::getUserData<sf::RenderWindow>(L, 2, WINDOW_TABLE);;
    sf::RenderTexture* canvas = LuaAux::getUserData<sf::RenderTexture>(L, 2, RENDERTEXTURE_TABLE);;

    if (window)
        window->draw(*shp, g_Shader);
    else if (canvas)
        canvas->draw(*shp, g_Shader);

    return 0;
}

int ConvexShape::Destroy(lua_State* L)
{
    SFMLuaClassDestroy(sf::ConvexShape);
    return 0;
}
