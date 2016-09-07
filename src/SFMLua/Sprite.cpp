#include "SFMLua/LuaAux.h"
#include "SFMLua/Classes.h"
#include "SFMLua/Sprite.h"

extern sf::Shader* g_Shader;

void Sprite::Register(lua_State* L)
{
    LuaAux::newClass(L, Methods, Meta, SPRITE_TABLE, Call);
}

int Sprite::Call(lua_State* L)
{
    lua_remove(L, 1);
    return Create(L);
}

int Sprite::Create(lua_State* L)
{
    sf::IntRect rect;
    bool cut = false;

    if (lua_gettop(L) == 5)
    {
        rect.left = lua_tointeger(L, 2);
        rect.top = lua_tointeger(L, 3);
        rect.width = lua_tointeger(L, 4);
        rect.height = lua_tointeger(L, 5);
        cut = true;
    }

    sf::Texture* tex = LuaAux::getUserData<sf::Texture>(L, 1, TEXTURE_TABLE);
    sf::Sprite* spr = LuaAux::newUserData<sf::Sprite>(L, false, SPRITE_TABLE);

    if (tex)
        new (spr) sf::Sprite(*tex);
    else
        new (spr) sf::Sprite();

    sf::Sprite* copySpr;

    if ((copySpr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE)))
        *spr = *copySpr;

    if (cut)
        spr->setTextureRect(rect);

    return 1;
}

int Sprite::SetTexture(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);
    sf::Texture* tex = LuaAux::getUserData<sf::Texture>(L, 2, TEXTURE_TABLE);

    if (!spr || !tex)
        return 0;

    spr->setTexture(*tex);
    return 0;
}

int Sprite::SetRect(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr)
        return 0;

    sf::IntRect rect;

    if (lua_gettop(L) > 1)
        rect.left = lua_tonumber(L, 2);

    if (lua_gettop(L) > 2)
        rect.top = lua_tonumber(L, 3);

    if (lua_gettop(L) > 3)
        rect.width = lua_tonumber(L, 4);

    if (lua_gettop(L) > 4)
        rect.height = lua_tonumber(L, 5);

    spr->setTextureRect(rect);
    return 0;
}

int Sprite::GetRect(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr)
        return 0;

    sf::IntRect rect = spr->getTextureRect();
    lua_pushnumber(L, rect.left);
    lua_pushnumber(L, rect.top);
    lua_pushnumber(L, rect.width);
    lua_pushnumber(L, rect.height);
    return 4;
}

int Sprite::SetColor(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr)
        return 0;

    spr->setColor(LuaAux::getColor(L, 2, lua_gettop(L) == 2));
    return 0;
}

int Sprite::GetColor(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr)
        return 0;

    sf::Color color = spr->getColor();
    lua_pushinteger(L, color.r);
    lua_pushinteger(L, color.g);
    lua_pushinteger(L, color.b);
    lua_pushinteger(L, color.a);
    return 4;
}

int Sprite::SetOrigin(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        return 0;

    spr->setOrigin(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int Sprite::GetOrigin(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr)
        return 0;

    sf::Vector2f origin = spr->getOrigin();
    lua_pushnumber(L, origin.x);
    lua_pushnumber(L, origin.y);
    return 2;
}

int Sprite::SetPosition(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        return 0;

    spr->setPosition(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int Sprite::Move(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        return 0;

    spr->move(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int Sprite::GetPosition(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr)
        return 0;

    sf::Vector2f pos = spr->getPosition();
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int Sprite::GetBounds(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr)
        return 0;

    sf::FloatRect b = lua_toboolean(L, 2) ? spr->getGlobalBounds() : spr->getLocalBounds();
    lua_pushnumber(L, b.left);
    lua_pushnumber(L, b.top);
    lua_pushnumber(L, b.width);
    lua_pushnumber(L, b.height);
    return 4;
}

int Sprite::GetSize(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr)
        return 0;

    sf::FloatRect b = lua_toboolean(L, 2) ? spr->getGlobalBounds() : spr->getLocalBounds();
    lua_pushnumber(L, b.width);
    lua_pushnumber(L, b.height);
    return 2;
}

int Sprite::SetSize(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr)
        return 0;

    sf::FloatRect b = lua_toboolean(L, 4) ? spr->getGlobalBounds() : spr->getLocalBounds();
    spr->setScale(lua_tonumber(L, 2) / b.width, lua_tonumber(L, 3) / b.height);
    return 0;
}

int Sprite::SetRotation(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr || !lua_isnumber(L, 2))
        return 0;

    spr->setRotation(lua_tonumber(L, 2));
    return 0;
}

int Sprite::Rotate(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr || !lua_isnumber(L, 2))
        return 0;

    spr->rotate(lua_tonumber(L, 2));
    return 0;
}

int Sprite::GetRotation(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr)
        return 0;

    lua_pushnumber(L, spr->getRotation());
    return 1;
}

int Sprite::SetScale(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr)
        return 0;

    sf::Vector2f factors(1., 1.);

    if (lua_isnumber(L, 2))
        factors.x = lua_tonumber(L, 2);

    if (lua_isnumber(L, 3))
        factors.y = lua_tonumber(L, 3);
    else
        factors.y = factors.x;

    spr->setScale(factors);
    return 0;
}

int Sprite::Scale(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr)
        return 0;

    sf::Vector2f factors(0., 0.);

    if (lua_isnumber(L, 2))
        factors.x = lua_tonumber(L, 2);

    if (lua_isnumber(L, 3))
        factors.y = lua_tonumber(L, 3);
    else
        factors.y = factors.x;

    spr->scale(factors);
    return 0;
}

int Sprite::GetScale(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr)
        return 0;

    sf::Vector2f factors = spr->getScale();
    lua_pushnumber(L, factors.x);
    lua_pushnumber(L, factors.y);
    return 2;
}

int Sprite::GetTexture(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr)
        return 0;

    const sf::Texture* tex = spr->getTexture();

    if (tex == nullptr)
        return 0;

    LuaUserData lud;
    lud.aliases = static_cast<int16_t*>(LuaUserData::from_userdata_block(const_cast<sf::Texture*>(tex)));
    lud.metaname = TEXTURE_TABLE;

    if (!LuaAux::referUserData(L, &lud))
        lua_pushnil(L);

    return 1;
}

int Sprite::Draw(lua_State* L)
{
    sf::Sprite* spr = LuaAux::getUserData<sf::Sprite>(L, 1, SPRITE_TABLE);

    if (!spr)
        return 0;

    sf::RenderWindow* wnd = LuaAux::getUserData<sf::RenderWindow>(L, 2, WINDOW_TABLE);
    sf::RenderTexture* canvas = LuaAux::getUserData<sf::RenderTexture>(L, 2, RENDERTEXTURE_TABLE);

    if (wnd)
        wnd->draw(*spr, g_Shader);
    else if (canvas)
        canvas->draw(*spr, g_Shader);

    return 0;
}

int Sprite::Destroy(lua_State* L)
{
    SFMLuaClassDestroy(sf::Sprite);
    return 0;
}
