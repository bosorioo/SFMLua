#include "SFMLua/LuaAux.h"
#include "SFMLua/Classes.h"

extern sf::Font g_Font;

void Text::Register(lua_State* L)
{
    LuaAux::newClass(L, Methods, Meta, TEXT_TABLE, Call);
}

int Text::Call(lua_State* L)
{
    lua_remove(L, 1);
    return Create(L);
}

int Text::Create(lua_State* L)
{
    sf::Text* newText = LuaAux::newUserData<sf::Text>(L, false, TEXT_TABLE);
    sf::Font* font = LuaAux::getUserData<sf::Font>(L, 3, FONT_TABLE);

    if (!newText)
        return 0;

    new (newText) sf::Text("", font ? * font : g_Font, 18);

    if (lua_isstring(L, 1))
        newText->setString(sf::String(lua_tostring(L, 1)));

    if (lua_isnumber(L, 2))
        newText->setCharacterSize(lua_tonumber(L, 2));

    return 1;
}

int Text::SetStyle(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    txt->setStyle(lua_tonumber(L, 2));
    return 0;
}

int Text::SetColor(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    txt->setFillColor(LuaAux::getColor(L, 2, lua_gettop(L) == 2));
    return 0;
}

int Text::GetColor(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    sf::Color color = txt->getFillColor();
    lua_pushinteger(L, color.r);
    lua_pushinteger(L, color.g);
    lua_pushinteger(L, color.b);
    lua_pushinteger(L, color.a);
    return 4;
}

int Text::GetString(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    lua_pushstring(L, reinterpret_cast<const char*>(txt->getString().toUtf8().c_str()));
    return 1;
}

int Text::SetString(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    sf::Vector2f old_pos = txt->getPosition();
    txt->setString(luaL_optstring(L, 2, ""));
    txt->setPosition(old_pos);
    return 0;
}

int Text::Draw(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    sf::RenderWindow* window = LuaAux::getUserData<sf::RenderWindow>(L, 2, WINDOW_TABLE);
    sf::RenderTexture* canvas = LuaAux::getUserData<sf::RenderTexture>(L, 2, RENDERTEXTURE_TABLE);

    if (window)
        window->draw(*txt);
    else if (canvas)
        canvas->draw(*txt);

    return 0;
}

int Text::SetPosition(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        return 0;

    txt->setPosition(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int Text::GetPosition(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    sf::Vector2f pos = txt->getPosition();
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int Text::SetOrigin(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    txt->setOrigin(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int Text::GetOrigin(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    sf::Vector2f pos = txt->getOrigin();
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int Text::GetBounds(lua_State* L)
{
    if (lua_type(L, 1) == LUA_TSTRING)
    {
        sf::Text text(lua_tostring(L, 1), g_Font, luaL_optinteger(L, 2, 20));
        sf::FloatRect b = text.getLocalBounds();
        lua_pushnumber(L, b.left);
        lua_pushnumber(L, b.top);
        lua_pushnumber(L, b.width);
        lua_pushnumber(L, b.height);
        return 4;
    }

    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    sf::FloatRect b = lua_toboolean(L, 2) ? txt->getGlobalBounds() : txt->getLocalBounds();
    lua_pushnumber(L, b.left);
    lua_pushnumber(L, b.top);
    lua_pushnumber(L, b.width);
    lua_pushnumber(L, b.height);
    return 4;
}

int Text::GetSize(lua_State* L)
{
    if (lua_isstring(L, 1))
    {
        sf::Font* fnt = LuaAux::getUserData<sf::Font>(L, 3, FONT_TABLE);
        sf::Text text(lua_tostring(L, 1), fnt ? *fnt : g_Font, luaL_optinteger(L, 2, 20));
        sf::FloatRect b = text.getGlobalBounds();
        lua_pushnumber(L, b.width);
        lua_pushnumber(L, b.height);
        return 2;
    }

    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    sf::FloatRect b = txt->getGlobalBounds();
    lua_pushnumber(L, b.width);
    lua_pushnumber(L, b.height);
    return 2;
}

int Text::GetWidth(lua_State* L)
{
    if (lua_isstring(L, 1))
    {
        sf::Font* fnt = LuaAux::getUserData<sf::Font>(L, 3, FONT_TABLE);
        sf::Text text(lua_tostring(L, 1), fnt ? *fnt : g_Font, luaL_optinteger(L, 2, 20));
        sf::FloatRect b = text.getGlobalBounds();
        lua_pushnumber(L, b.width);
        return 1;
    }

    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    sf::FloatRect b = txt->getGlobalBounds();
    lua_pushnumber(L, b.width);
    return 1;
}

int Text::GetHeight(lua_State* L)
{
    if (lua_isstring(L, 1))
    {
        std::string str(lua_tostring(L, 1), lua_rawlen(L, 1));
        sf::Font* fnt = LuaAux::getUserData<sf::Font>(L, 3, FONT_TABLE);
        sf::Text text (str, fnt ? *fnt : g_Font, luaL_optinteger(L, 2, 20));
        sf::FloatRect b = text.getGlobalBounds();
        lua_pushnumber(L, b.height);
        return 1;
    }

    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    sf::FloatRect b = txt->getGlobalBounds();
    lua_pushnumber(L, b.height);
    return 1;
}

int Text::GetCharacterPosition(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    sf::Vector2f pos = txt->findCharacterPos(lua_tointeger(L, 2));
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int Text::GetRotation(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    lua_pushnumber(L, txt->getRotation());
    return 1;
}

int Text::SetRotation(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    txt->setRotation(lua_tonumber(L, 2));
    return 0;
}

int Text::SetScale(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    txt->setScale(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int Text::Scale(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    txt->scale(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int Text::Move(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    txt->move(lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

int Text::GetCharacterSize(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    lua_pushnumber(L, txt->getCharacterSize());
    return 1;
}

int Text::SetCharacterSize(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    txt->setCharacterSize(lua_tointeger(L, 2));
    return 0;
}

int Text::SetFont(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);
    sf::Font* f = LuaAux::getUserData<sf::Font>(L, 2, FONT_TABLE);

    if (!txt || !f)
        return 0;

    txt->setFont(*f);
    return 0;
}

int Text::GetFont(lua_State* L)
{
    sf::Text* txt = LuaAux::getUserData<sf::Text>(L, 1, TEXT_TABLE);

    if (!txt)
        return 0;

    lua_pushlightuserdata(L, (void*)txt->getFont());
    luaL_setmetatable(L, FONT_TABLE);
    return 1;
}

int Text::Destroy (lua_State* L)
{
    SFMLuaClassDestroy(sf::Text);

    return 0;
}
