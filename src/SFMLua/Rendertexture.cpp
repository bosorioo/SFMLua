#include "SFMLua/LuaAux.h"
#include "SFMLua/Classes.h"

extern sf::Shader* g_Shader;

void RenderTexture::Register(lua_State* L)
{
    LuaAux::newClass(L, Methods, Meta, RENDERTEXTURE_TABLE, Call);
}

int RenderTexture::Call(lua_State* L)
{
    lua_remove(L, 1);
    return Create(L);
}

int RenderTexture::Create(lua_State* L)
{
    unsigned int height = 0, width = 0;
    bool depthBuffer = false;

    if (lua_isboolean(L, 3))
        depthBuffer = lua_toboolean(L, 3);

    if (lua_isnumber(L, 1) && lua_isnumber(L, 2))
    {
        height = lua_tonumber(L, 2);
        width = lua_tonumber(L, 1);
    }

    sf::RenderTexture* canvas = LuaAux::newUserData<sf::RenderTexture>(L, true, RENDERTEXTURE_TABLE);

    if (canvas)
    {
        canvas->create(width, height, depthBuffer);
    }

    return 1;
}

int RenderTexture::Clear(lua_State* L)
{
    sf::RenderTexture* canvas = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);

    if (!canvas)
        return 0;

    if (lua_gettop(L) > 1)
        canvas->clear(LuaAux::getColor(L, 2, lua_gettop(L) == 2));
    else
        canvas->clear();

    return 0;
}

int RenderTexture::GetImage(lua_State* L)
{
    sf::RenderTexture* canvas = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);

    if (!canvas)
        return 0;

    sf::Image* image = LuaAux::newUserData<sf::Image>(L, true, IMAGE_TABLE);

    if (!image)
        return 0;

    *image = canvas->getTexture().copyToImage();
    return 1;
}

int RenderTexture::SetSmooth(lua_State* L)
{
    sf::RenderTexture* canvas = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);

    if (!canvas)
        return 0;

    canvas->setSmooth(lua_toboolean(L, 2));
    return 0;
}

int RenderTexture::SetActive(lua_State* L)
{
    sf::RenderTexture* canvas = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);

    if (!canvas)
        return 0;

    canvas->setActive(lua_toboolean(L, 2));
    return 0;
}

int RenderTexture::GetSize(lua_State* L)
{
    sf::RenderTexture* canvas = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);

    if (!canvas)
        return 0;

    sf::Vector2u size = canvas->getSize();
    lua_pushnumber(L, size.x);
    lua_pushnumber(L, size.y);
    return 2;
}

int RenderTexture::Draw(lua_State* L)
{
    sf::RenderTexture* canvas = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);

    if (!canvas)
        return 0;

    sf::RenderWindow* targetwindow = LuaAux::getUserData<sf::RenderWindow>(L, 2, WINDOW_TABLE);
    sf::RenderTexture* targetCanvas = LuaAux::getUserData<sf::RenderTexture>(L, 2, RENDERTEXTURE_TABLE);
    sf::Sprite sprite(canvas->getTexture());

    if (lua_tonumber(L, 3) && lua_tonumber(L, 4))
        sprite.move((float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4));

    if (targetwindow)
        targetwindow->draw(sprite, g_Shader);
    else if (targetCanvas)
        targetCanvas->draw(sprite, g_Shader);

    return 0;
}

int RenderTexture::Display(lua_State* L)
{
    sf::RenderTexture* canvas = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);

    if (!canvas)
        return 0;

    canvas->display();
    return 0;
}

int RenderTexture::Destroy (lua_State* L)
{
    SFMLuaClassDestroy(sf::RenderTexture);
    return 0;
}

int RenderTexture::MapPixelPosition(lua_State* L)
{
    sf::RenderTexture* rtexture = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);
    sf::View* view = LuaAux::getUserData<sf::View>(L, 2, VIEW_TABLE);

    if (!rtexture)
        return 0;

    sf::Vector2f pos;

    if (view)
        pos = rtexture->mapPixelToCoords(sf::Vector2i(lua_tonumber(L, 2), lua_tonumber(L, 3)), *view);
    else
        pos = rtexture->mapPixelToCoords(sf::Vector2i(lua_tonumber(L, 2), lua_tonumber(L, 3)));

    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int RenderTexture::MapCoordPosition(lua_State* L)
{
    sf::RenderTexture* rtexture = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);
    sf::View* view = LuaAux::getUserData<sf::View>(L, 2, VIEW_TABLE);

    if (!rtexture)
        return 0;

    sf::Vector2i pos;

    if (view)
        pos = rtexture->mapCoordsToPixel(sf::Vector2f(lua_tonumber(L, 2), lua_tonumber(L, 3)), *view);
    else
        pos = rtexture->mapCoordsToPixel(sf::Vector2f(lua_tonumber(L, 2), lua_tonumber(L, 3)));

    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int RenderTexture::SaveGLStates(lua_State* L)
{
    sf::RenderTexture* rtexture = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);

    if (!rtexture)
        return 0;

    rtexture->pushGLStates();
    return 0;
}

int RenderTexture::RestoreGLStates(lua_State* L)
{
    sf::RenderTexture* rtexture = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);

    if (!rtexture)
        return 0;

    rtexture->popGLStates();
    return 0;
}

int RenderTexture::ResetGLStates(lua_State* L)
{
    sf::RenderTexture* rtexture = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);

    if (!rtexture)
        return 0;

    rtexture->resetGLStates();
    return 0;
}

int RenderTexture::SetView(lua_State* L)
{
    sf::RenderTexture* rtexture = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);
    sf::View* view = LuaAux::getUserData<sf::View>(L, 2, VIEW_TABLE);

    if (!rtexture || !view)
        return 0;

    rtexture->setView(*view);
    return 0;
}

int RenderTexture::GetView(lua_State* L)
{
    sf::RenderTexture* rtexture = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);

    if (!rtexture)
        return 0;

    sf::View* view = LuaAux::newUserData<sf::View>(L, true, VIEW_TABLE);

    if (view)
        *view = rtexture->getView();

    return 1;
}

int RenderTexture::GetTexture(lua_State* L)
{
    sf::RenderTexture* rtexture = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);

    if (!rtexture)
        return 0;

    sf::Texture* texture = LuaAux::newUserData<sf::Texture>(L, true, TEXTURE_TABLE);

    if (!texture)
        return 0;

    *texture = rtexture->getTexture();
    return 1;
}

int RenderTexture::GetSprite(lua_State* L)
{
    sf::RenderTexture* rtexture = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);

    if (!rtexture)
        return 0;

    sf::Sprite* spr = LuaAux::newUserData<sf::Sprite>(L, true, SPRITE_TABLE);

    if (!spr)
        return 0;

    spr->setTexture(rtexture->getTexture());
    return 1;
}

int RenderTexture::ResetView(lua_State* L)
{
    sf::RenderTexture* rtexture = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);

    if (!rtexture)
        return 0;

    rtexture->setView(rtexture->getDefaultView());
    return 0;
}


int RenderTexture::TranslateView(lua_State* L)
{
    sf::RenderTexture* rtexture = LuaAux::getUserData<sf::RenderTexture>(L, 1, RENDERTEXTURE_TABLE);

    if (!rtexture)
        return 0;

    sf::View view = rtexture->getView();
    view.move(lua_tonumber(L, 2), lua_tonumber(L, 3));
    rtexture->setView(view);
    return 0;
}
