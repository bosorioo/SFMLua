#include "SFMLua/LuaAux.h"
#include "SFMLua/Classes.h"

extern sf::Shader* g_Shader;

void RegularShape::Register(lua_State* L)
{
    LuaAux::newClass(L, Methods, Meta, REGULARSHAPE_TABLE, Call);
}

int RegularShape::Call(lua_State* L)
{
    lua_remove(L, 1);
    return Create(L);
}

int RegularShape::Create(lua_State* L)
{
    unsigned int point_count = 4, size = 1;

    if (lua_isnumber(L, 1))
        size = lua_tointeger(L, 1);

    if (lua_isnumber(L, 2))
        point_count = lua_tointeger(L, 2);

    sf::CircleShape* shp = LuaAux::newUserData<sf::CircleShape>(L, false, REGULARSHAPE_TABLE);
    new (shp) sf::CircleShape(size, point_count);

    return 1;
}

int RegularShape::SetSize(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp)
        return 0;

    shp->setRadius(lua_tointeger(L, 2));
    return 0;
}

int RegularShape::GetSize(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp)
        return 0;

    lua_pushnumber(L, shp->getRadius());
    return 1;
}

int RegularShape::SetSideCount(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp)
        return 0;

    shp->setPointCount(lua_tointeger(L, 2));
    return 0;
}

int RegularShape::GetSideCount(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp)
        return 0;

    lua_pushnumber(L, shp->getPointCount());
    return 1;
}

int RegularShape::SetTexture(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);
    sf::Texture* tex = LuaAux::getUserData<sf::Texture>(L, 2, TEXTURE_TABLE);

    if (!shp || !tex)
        return 0;

    shp->setTexture(tex);
    return 0;
}

int RegularShape::SetFillColor(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp)
        return 0;

    shp->setFillColor(LuaAux::getColor(L, 2, lua_gettop(L) == 2));
    return 0;
}

int RegularShape::GetFillColor(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp)
        return 0;

    sf::Color color = shp->getFillColor();
    lua_pushinteger(L, color.r);
    lua_pushinteger(L, color.g);
    lua_pushinteger(L, color.b);
    lua_pushinteger(L, color.a);
    return 4;
}

int RegularShape::SetOrigin(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        return 0;

    shp->setOrigin(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int RegularShape::SetPosition(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        return 0;

    shp->setPosition(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int RegularShape::Move(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        return 0;

    shp->move(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int RegularShape::GetPosition(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp)
        return 0;

    sf::Vector2f pos = shp->getPosition();
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int RegularShape::SetRotation(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp || !lua_isnumber(L, 2))
        return 0;

    shp->setRotation(lua_tonumber(L, 2));
    return 0;
}

int RegularShape::Rotate(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp || !lua_isnumber(L, 2))
        return 0;

    shp->rotate(lua_tonumber(L, 2));
    return 0;
}

int RegularShape::GetRotation(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp)
        return 0;

    lua_pushnumber(L, shp->getRotation());
    return 1    ;
}

int RegularShape::SetScale(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

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

int RegularShape::Scale(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

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

int RegularShape::GetScale(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp)
        return 0;

    sf::Vector2f factors = shp->getScale();
    lua_pushnumber(L, factors.x);
    lua_pushnumber(L, factors.y);
    return 2;
}

int RegularShape::GetTexture(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp)
        return 0;

    const sf::Texture* tex = shp->getTexture();
    lua_pushlightuserdata(L, (void*)tex);
    luaL_setmetatable(L, TEXTURE_TABLE);
    return 1;
}

int RegularShape::Draw(lua_State* L)
{
    sf::CircleShape* shp = LuaAux::getUserData<sf::CircleShape>(L, 1, REGULARSHAPE_TABLE);

    if (!shp)
        return 0;

    sf::RenderWindow* window = LuaAux::getUserData<sf::RenderWindow>(L, 2, WINDOW_TABLE);
    sf::RenderTexture* canvas = LuaAux::getUserData<sf::RenderTexture>(L, 2, RENDERTEXTURE_TABLE);

    if (window)
        window->draw(*shp, g_Shader);
    else if (canvas)
        canvas->draw(*shp, g_Shader);

    return 0;
}

int RegularShape::Destroy(lua_State* L)
{
    SFMLuaClassDestroy(sf::CircleShape);

    return 0;
}
