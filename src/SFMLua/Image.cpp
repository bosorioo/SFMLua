#include "SFMLua/LuaAux.h"
#include "SFMLua/Classes.h"
#include <fstream>
#include <sys/stat.h>

void Image::Register(lua_State* L)
{
    LuaAux::newClass(L, Methods, Meta, IMAGE_TABLE, Call);
}

int Image::Call(lua_State* L)
{
    lua_remove(L, 1);
    return Create(L);
}

int Image::Create(lua_State* L)
{
    std::string path;
    unsigned int w = 0, h = 0;
    sf::Color color;
    sf::Texture *tex = NULL;

    if (lua_type(L, 1) == LUA_TSTRING)
    {
        path.assign(lua_tostring(L, 1), lua_rawlen(L, 1));
    }
    else if (lua_isnumber(L, 1) && lua_isnumber(L, 2))
    {
         w = lua_tonumber(L, 1);
         h = lua_tonumber(L, 2);

         if (lua_gettop(L) > 2)
             color = LuaAux::getColor(L, 3, lua_gettop(L) == 3);
    }
    else if (lua_gettop(L) && !(tex = LuaAux::getUserData<sf::Texture>(L, 1, TEXTURE_TABLE)))
    {
        lua_pushboolean(L, false);
        return 1;
    }

    sf::Image* image = LuaAux::newUserData<sf::Image>(L, true, IMAGE_TABLE);

    if (tex)
        *image = tex->copyToImage();
    else if (w && h)
        image->create(w, h, color);
    else if (!path.empty())
    {
        struct stat filestat;

        if (stat(path.c_str(), &filestat) == 0)
        {
            if (!image->loadFromFile(path))
                return 0;
        }
        else if (!image->loadFromMemory(path.data(), path.size()))
            return 0;
    }

    return 1;
}

int Image::Resize(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image || lua_tointeger(L, 2) <= 0 || lua_tointeger(L, 3) <= 0)
        return 0;

    std::vector<float> coefficients;
    uint16_t index;

    if (lua_istable(L, 4) && lua_rawlen(L, 4) > 0)
    {
        for (index = 0; index < lua_rawlen(L, 4); index++)
        {
            lua_rawgeti(L, 4, index + 1);
            lua_pop(L, 1);
            coefficients.push_back(lua_tonumber(L, 0));
        }
    }
    else
    {
        coefficients.push_back(1.f);
    }

    sf::Image* newImage = LuaAux::newUserData<sf::Image>(L, true, IMAGE_TABLE);
    newImage->create(lua_tointeger(L, 2), lua_tointeger(L, 3), sf::Color(255, 0, 255));

    sf::Vector2i size = sf::Vector2i(newImage->getSize().x, newImage->getSize().y);
    sf::Vector2i originalSize = sf::Vector2i(image->getSize().x, image->getSize().y);
    sf::Color color, temp;
    int16_t window_size = std::sqrt(coefficients.size());
    int16_t right = window_size / 2;
    int16_t left = window_size - 1 - right;
    float xfactor = (float)originalSize.x / (float)size.x;
    float yfactor = (float)originalSize.y / (float)size.y;
    float r, g, b, a;

    for (int32_t row = 0; row < size.y; row++)
    {
        for (int32_t col = 0; col < size.x; col++)
        {
            color = sf::Color::Black;
            int32_t x = col * xfactor;
            int32_t y = row * yfactor;
            float coefficient_sum = 0.f;
            index = 0;

            r = g = b = a = 0.f;


            for (int32_t _y = y - left; _y <= y + right; _y++)
            {
                for (int32_t _x = x - left; _x <= x + right; _x++, index++)
                {
                    float factor = coefficients[index];
                    coefficient_sum += factor;

                    if (_x < 0 || _x >= originalSize.x || _y < 0 || _y >= originalSize.y)
                        continue;

                    color = image->getPixel(_x, _y);

                    r += color.r * factor;
                    g += color.g * factor;
                    b += color.b * factor;
                    a += color.a * factor;
                }

            }

            r /= coefficient_sum;
            g /= coefficient_sum;
            b /= coefficient_sum;
            a /= coefficient_sum;

            color.r = static_cast<uint8_t>(std::min(255.f, std::max(0.f, r)));
            color.g = static_cast<uint8_t>(std::min(255.f, std::max(0.f, g)));
            color.b = static_cast<uint8_t>(std::min(255.f, std::max(0.f, b)));
            color.a = static_cast<uint8_t>(std::min(255.f, std::max(0.f, a)));

            newImage->setPixel(col, row, color);
        }
    }

    return 1;
}

int Image::PixelTransform(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image || !lua_isfunction(L, 2))
        return 0;

    sf::Vector2u v = image->getSize();
    unsigned int pixels_count = v.x * v.y * 4;
    sf::Uint8* ptr = const_cast<sf::Uint8*>(image->getPixelsPtr());
    sf::Uint8* end = ptr + pixels_count;
    unsigned int i, x = 0, y = 0;
    while (ptr < end)
    {
        lua_pushvalue(L, 2);
        lua_pushinteger(L, x);
        lua_pushinteger(L, y);
        for (i = 0; i < 4; i++)
            lua_pushinteger(L, *ptr++);
        lua_pcall(L, 6, 4, 0);
        for (i = 0; i < 4; i++)
        {
            lua_pop(L, 1);
            *--ptr = static_cast<uint8_t>(lua_tointeger(L, 0));
        }
        ptr += 4;
        if (++x >= v.x)
        {
            x = 0;
            y++;
        }
    }
    return 0;
}

int Image::Save(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image ||!lua_isstring(L, 2))
        return 0;

    image->saveToFile(std::string(lua_tostring(L, 2)));
    return 0;
}

int Image::GetSize(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image)
        return 0;

    sf::Vector2u dimensions = image->getSize();
    lua_pushinteger(L, dimensions.x);
    lua_pushinteger(L, dimensions.y);
    return 2;
}

int Image::GetPixel(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        return 0;

    sf::Color pixelColor = image->getPixel(lua_tointeger(L, 2), lua_tointeger(L, 3));
    lua_pushinteger(L, pixelColor.r);
    lua_pushinteger(L, pixelColor.g);
    lua_pushinteger(L, pixelColor.b);
    lua_pushinteger(L, pixelColor.a);
    return 4;
}

int Image::SetPixel(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
        return 0;

    sf::Color pixelColor;
    pixelColor = LuaAux::getColor(L, 4, lua_gettop(L) == 4);
    image->setPixel(lua_tonumber(L, 2), lua_tonumber(L, 3), pixelColor);
    return 0;
}

int Image::PixelIndexing(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image)
        return 0;

    sf::Color pixelColor;
    if (lua_gettop(L) == 3)
    {
        pixelColor = image->getPixel(lua_tointeger(L, 2), lua_tointeger(L, 3));
        lua_pushinteger(L, pixelColor.r);
        lua_pushinteger(L, pixelColor.g);
        lua_pushinteger(L, pixelColor.b);
        lua_pushinteger(L, pixelColor.a);
        return 4;
    }

    pixelColor = LuaAux::getColor(L, 4, lua_gettop(L) == 4);
    image->setPixel(lua_tointeger(L, 2), lua_tointeger(L, 3), pixelColor);
    return 0;
}

int Image::Grayscale(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image)
        return 0;

    sf::Vector2u v = image->getSize();
    unsigned int pixels_count = v.x * v.y * 4;
    sf::Uint8* ptr = const_cast<sf::Uint8*>(image->getPixelsPtr());
    sf::Uint8* end = ptr + pixels_count;
    sf::Uint8 g_scale;
    while (ptr < end)
    {
        g_scale = *ptr * 0.19 + *(ptr + 1) * 0.64 + *(ptr + 2) * 0.17;
        *ptr++ = g_scale;
        *ptr++ = g_scale;
        *ptr++ = g_scale;
        ptr++;
    }
    return 0;
}

int Image::GetPixelString(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image)
        return 0;

    sf::Vector2u v = image->getSize();
    unsigned int pixels_count = v.x * v.y * 4;
    sf::Uint8* ptr = const_cast<sf::Uint8*>(image->getPixelsPtr());
    sf::Uint8* end = ptr + pixels_count;
    char* result = new char[pixels_count];
    while (ptr < end)
    {
        if (!(*ptr))
            *result++ = (char)1;
        else
            *result++ = (char)(*ptr);
        ptr++;
    }
    lua_pushstring(L, result - pixels_count);
    delete [] result;
    return 1;
}

int Image::GetPixelData(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image)
        return 0;

    sf::Uint8* ptr = const_cast<sf::Uint8*>(image->getPixelsPtr());
    lua_pushlightuserdata(L, (void*)ptr);
    lua_pushnumber(L, image->getSize().x * image->getSize().y * 4);
    return 2;
}

int Image::SetPixelString(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image)
        return 0;

    char* sptr = (char*)lua_tostring(L, 2);
    unsigned int w = 0;
    std::size_t  s = lua_rawlen(L, 2);
    sf::Uint8* ptr = const_cast<sf::Uint8*>(image->getPixelsPtr());

    while (w < s)
        *ptr++ = (sf::Uint8)(sptr[w++]);

    return 0;
}

int Image::AlphaMask(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image || !lua_istable(L, 2))
        return 0;

    sf::Uint8 alpha = 0;
    sf::Color color = LuaAux::getColor(L, 2, true);

    if (lua_isnumber(L, 3))
        alpha = lua_tointeger(L, 3);

    image->createMaskFromColor(color, alpha);
    return 0;

}

int Image::HorizontalFlip(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image)
        return 0;

    image->flipHorizontally();
    return 0;

}

int Image::VerticalFlip(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image)
        return 0;

    image->flipVertically();
    return 0;

}

int Image::CopyPixels(lua_State* L)
{
    sf::Image* dst = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);
    sf::Image* src = LuaAux::getUserData<sf::Image>(L, 2, IMAGE_TABLE);

    if (!dst || !src)
        return 0;

    //sf::Vector2u dstSize = dst->getSize();
    sf::Vector2u srcSize = src->getSize();

    unsigned destLeft = luaL_optnumber(L, 3, 0);
    unsigned destTop = luaL_optnumber(L, 4, 0);
    unsigned srcLeft = luaL_optnumber(L, 5, 0);
    unsigned srcTop = luaL_optnumber(L, 6, 0);
    unsigned srcWidth = luaL_optnumber(L, 7, srcSize.x);
    unsigned srcHeight = luaL_optnumber(L, 8, srcSize.y);

    /*if (destLeft >= dstSize.x) destLeft = dstSize.x - 1;
    if (destTop >= dstSize.y) destTop = dstSize.y - 1;
    if (srcLeft >= srcSize.x) srcLeft = srcSize.x - 1;
    if (srcTop >= srcSize.y) srcTop = srcSize.y - 1;
    if (srcWidth + srcLeft > srcSize.x) srcWidth = srcSize.x - srcLeft;
    if (srcHeight + srcTop > srcSize.y) srcHeight = srcSize.y - srcTop;
    if (srcWidth + destLeft > dstSize.x) srcWidth = dstSize.x - destLeft;
    if (srcHeight + destTop > dstSize.y) srcHeight = dstSize.y - destTop;*/

    if (!srcWidth || !srcHeight)
        return 0;

    sf::IntRect srcRect(srcLeft, srcTop, srcWidth, srcHeight);
    dst->copy(*src, destLeft, destTop, srcRect, lua_toboolean(L, 9));
    return 0;

}

int Image::LoadFromFile(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image)
        return 0;

    lua_pushboolean(L, image->loadFromFile(sf::String(lua_tostring(L, 2))));
    return 1;
}

int Image::LoadFromString(lua_State* L)
{
    sf::Image* image = LuaAux::getUserData<sf::Image>(L, 1, IMAGE_TABLE);

    if (!image)
        return 0;

    lua_pushboolean(L, image->loadFromMemory((void*)lua_tostring(L, 2), lua_rawlen(L, 2)));
    return 1;
}

int Image::Destroy(lua_State* L)
{
    SFMLuaClassDestroy(sf::Image);

    return 0;
}
