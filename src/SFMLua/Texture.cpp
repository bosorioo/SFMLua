#include "SFMLua/LuaScript.h"
#include "SFMLua/LuaAux.h"
#include "SFMLua/Classes.h"

int GetMaxTexSize(lua_State* L)
{
    lua_pushnumber(L, sf::Texture::getMaximumSize());
    return 1;
}

void Texture::Register(lua_State* L)
{
    LuaAux::newClass(L, Methods, Meta, TEXTURE_TABLE, Call);
    LuaAux::newGlobal(L, TEXTURE_TABLE, "getMaximumSize", GetMaxTexSize);
}

int Texture::Call(lua_State* L)
{
    lua_remove(L, 1);
    return Create(L);
}

int Texture::Create(lua_State* L)
{
    sf::IntRect loadArea;

    if (lua_isnumber(L, 2)) loadArea.left = lua_tointeger(L, 2);
    if (lua_isnumber(L, 3)) loadArea.top = lua_tointeger(L, 3);
    if (lua_isnumber(L, 4)) loadArea.width = lua_tointeger(L, 4);
    if (lua_isnumber(L, 5)) loadArea.height = lua_tointeger(L, 5);

    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);
    sf::Texture* tex = LuaAux::newUserData<sf::Texture>(L, true, TEXTURE_TABLE);

    if (lua_type(L, 1) == LUA_TSTRING)
    {
        std::string filePath(lua_tostring(L, 1));

        if(!tex->loadFromFile(filePath, loadArea))
            LuaScript::cout("Failed to load texture from path " + filePath);
    }
    else if (image)
    {
        if (!tex->loadFromImage(*image, loadArea))
            lua_pushboolean(L, false);
    }
    else if (lua_isnumber(L, 1) && lua_isnumber(L, 2))
        tex->create(lua_tointeger(L, 1), lua_tointeger(L, 2));

    return 1;
}

int Texture::GetSize(lua_State* L)
{
    sf::Texture* tex = LuaAux::getUserData<sf::Texture>(L, 1, TEXTURE_TABLE);

    if (!tex)
        return 0;

    sf::Vector2u size = tex->getSize();
    lua_pushinteger(L, size.x);
    lua_pushinteger(L, size.y);
    return 2;
}

int Texture::GetImage(lua_State* L)
{
    sf::Texture* tex = LuaAux::getUserData<sf::Texture>(L, 1, TEXTURE_TABLE);

    if (!tex)
        return 0;

    sf::Image* image = LuaAux::newUserData<sf::Image>(L, true, IMAGE_TABLE);

    if (!image)
        return 0;

    *image = tex->copyToImage();
    return 1;
}

int Texture::SetSmooth(lua_State* L)
{
    sf::Texture* tex = LuaAux::getUserData<sf::Texture>(L, 1, TEXTURE_TABLE);

    if (!tex)
        return 0;

    tex->setSmooth(LuaAux::getBool(L, 2, !tex->isSmooth()));
    return 0;
}

int Texture::SetRepeated(lua_State* L)
{
    sf::Texture* tex = LuaAux::getUserData<sf::Texture>(L, 1, TEXTURE_TABLE);

    if (!tex)
        return 0;

    tex->setRepeated(LuaAux::getBool(L, 2, !tex->isRepeated()));
    return 0;
}

int Texture::LoadFile(lua_State* L)
{
    sf::Texture* tex = LuaAux::getUserData<sf::Texture>(L, 1, TEXTURE_TABLE);

    if (!tex)
        return 0;

    lua_pushboolean(L, tex->loadFromFile(lua_tostring(L, 2)));
    return 1;
}

int Texture::LoadImage(lua_State* L)
{
    sf::Texture* tex = LuaAux::getUserData<sf::Texture>(L, 1, TEXTURE_TABLE);
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 2, IMAGE_TABLE);

    if (!tex || !image)
        return 0;

    lua_pushboolean(L, tex->loadFromImage(*image));
    return 1;
}

int Texture::Update(lua_State* L)
{
    sf::Texture* tex = LuaAux::getUserData<sf::Texture>(L, 1, TEXTURE_TABLE);
    sf::Image* image;

    if (!tex)
        return 0;

    if ((image = LuaAux::getUserData<sf::Image>(L, 2, IMAGE_TABLE)))
    {
        switch (lua_gettop(L))
        {
            case 2: tex->update(*image); break;
            case 4: tex->update(image->getPixelsPtr(), lua_tointeger(L, 3), lua_tointeger(L, 4), 0, 0); break;
            case 6: tex->update(image->getPixelsPtr(), lua_tointeger(L, 3), lua_tointeger(L, 4),
                                lua_tointeger(L, 5), lua_tointeger(L, 6)); break;
            default: break;
        }
    }
    else
        return 0;

    lua_pushboolean(L, true);
    return 1;
}

int Texture::LoadWindow(lua_State* L)
{
    sf::Texture* tex = LuaAux::getUserData<sf::Texture>(L, 1, TEXTURE_TABLE);
    sf::RenderWindow* window = LuaAux::getUserData<sf::RenderWindow>(L, 2, WINDOW_TABLE);

    if (!tex || !window)
        return 0;

    tex->create(window->getSize().x, window->getSize().y);
    tex->update(*window);
    lua_pushboolean(L, true);
    return 1;
}

int Texture::Bind(lua_State* L)
{
    sf::Texture* tex = LuaAux::getUserData<sf::Texture>(L, 1, TEXTURE_TABLE);
    sf::Texture::bind(tex);
    return 0;
}

int Texture::Destroy(lua_State* L)
{
    SFMLuaClassDestroy(sf::Texture);

    return 0;
}
