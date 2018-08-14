#include "SFMLua/LuaAux.h"
#include "SFMLua/Classes.h"

extern sf::Shader* g_Shader;

void Gif::Register(lua_State* L)
{
    LuaAux::newClass(L, Gif::Methods, Gif::Meta, GIF_TABLE, Gif::Call);
}

void Gif::setGifError(std::string& out, int error)
{
    switch (error)
    {
        case D_GIF_ERR_OPEN_FAILED:
            out = "Failed to open given file"; break;
        case D_GIF_ERR_READ_FAILED:
            out = "Failed to read from given file"; break;
        case D_GIF_ERR_NOT_GIF_FILE:
            out = "Data is not a GIF file"; break;
        case D_GIF_ERR_NO_SCRN_DSCR:
            out = "No screen descriptor detected"; break;
        case D_GIF_ERR_NO_IMAG_DSCR:
            out = "No image descriptor detected"; break;
        case D_GIF_ERR_NO_COLOR_MAP:
            out = "Neither global nor local color map"; break;
        case D_GIF_ERR_WRONG_RECORD:
            out = "Wrong record type detected"; break;
        case D_GIF_ERR_DATA_TOO_BIG:
            out = "Number of pixels bigger than width * height"; break;
        case D_GIF_ERR_NOT_ENOUGH_MEM:
            out = "Failed to allocate required memory"; break;
        case D_GIF_ERR_CLOSE_FAILED:
            out = "Failed to close given file"; break;
        case D_GIF_ERR_NOT_READABLE:
            out = "Given file was not opened for read"; break;
        case D_GIF_ERR_IMAGE_DEFECT:
            out = "Image is defective, decoding aborted"; break;
        case D_GIF_ERR_EOF_TOO_SOON:
            out = "Image EOF detected, before image complete"; break;
        default:
            break;
    }
}

bool Gif::loadGif(GifImage& gif, const char* path, std::string& error_out)
{
    int error;

    GifFileType* gifFile = DGifOpenFileName(path, &error);

    if (gifFile == NULL)
    {
        setGifError(error_out, error);
        return false;
    }

    if (DGifSlurp(gifFile) == GIF_ERROR)
    {
        setGifError(error_out, gifFile->Error);
        DGifCloseFile(gifFile, NULL);
        return false;
    }

    if (gifFile->SColorMap)
    {
        GifColorType color = gifFile->SColorMap->Colors[gifFile->SBackGroundColor];
        gif.backgroundColor = sf::Color(color.Red, color.Green, color.Blue);
    }

    if (!gif.canvas.create(gifFile->SWidth, gifFile->SHeight, sf::ContextSettings()))
    {
        DGifCloseFile(gifFile, NULL);
        return false;
    }

    gif.sprite.setTexture(gif.canvas.getTexture());
    gif.frameCount = gifFile->ImageCount;
    GraphicsControlBlock gcb;
    GifFrame* last = NULL;

    for (int imageIndex = 0; imageIndex < gifFile->ImageCount; imageIndex++)
    {
        GifFrame* frame = new GifFrame;

        frame->nextFrame = NULL;
        frame->index = imageIndex;

        if (DGifSavedExtensionToGCB(gifFile, imageIndex, &gcb) != GIF_ERROR)
        {
            frame->disposal = gcb.DisposalMode;
            frame->delay = gcb.DelayTime;
        }
        else
            gcb.TransparentColor = -1;

        SavedImage* gifFrameImage = &gifFile->SavedImages[imageIndex];
        sf::Image frameImage;
        frameImage.create(gifFrameImage->ImageDesc.Width, gifFrameImage->ImageDesc.Height);

        for (int x = 0; x < gifFrameImage->ImageDesc.Width; x++)
        {
            for (int y = 0; y < gifFrameImage->ImageDesc.Height; y++)
            {
                GifByteType colorIndex = gifFrameImage->RasterBits[x + y * gifFrameImage->ImageDesc.Width];

                if (colorIndex == gcb.TransparentColor)
                {
                    frameImage.setPixel(x, y, sf::Color(0, 0, 0, 0));
                    continue;
                }

                ColorMapObject* colorMap = gifFrameImage->ImageDesc.ColorMap != NULL ?
                    gifFrameImage->ImageDesc.ColorMap : gifFile->SColorMap;
                GifColorType* color = &colorMap->Colors[colorIndex];
                frameImage.setPixel(x, y, sf::Color(color->Red, color->Green, color->Blue));
            }
        }

        frame->texture.loadFromImage(frameImage);
        frame->sprite.setTexture(frame->texture);
        frame->sprite.setPosition(gifFrameImage->ImageDesc.Left, gifFrameImage->ImageDesc.Top);

        if (last != NULL)
            last->nextFrame = frame;
        else
            gif.frames = frame;

        last = frame;
    }

    last->nextFrame = gif.frames;
    if (gif.frames->disposal == 2) // Dispose to background color
        gif.canvas.clear(gif.backgroundColor);
    gif.canvas.draw(gif.frames->sprite);
    gif.canvas.display();
    DGifCloseFile(gifFile, NULL);
    return true;
}

void Gif::unloadGif(GifImage& gif)
{
    GifFrame *deleteFrame, *currentFrame;
    currentFrame = gif.frames->nextFrame;
    while (currentFrame != gif.frames)
    {
        deleteFrame = currentFrame;
        currentFrame = currentFrame->nextFrame;
        delete deleteFrame;
    }
    delete gif.frames;
    gif.frames = NULL;
}

int Gif::Call(lua_State* L)
{
    lua_remove(L, 1);
    return Gif::Create(L);
}

int Gif::Create(lua_State* L)
{
    GifImage* gif = LuaAux::newUserData<GifImage>(L, true, GIF_TABLE);

    if (lua_type(L, 1) == LUA_TSTRING)
    {
        std::string error;

        if (!loadGif(*gif, lua_tostring(L, 1), error))
        {
            lua_pushboolean(L, false);
            lua_pushstring(L, error.c_str());
            return 2;
        }
    }

    return 1;
}

int Gif::LoadFromFile(lua_State* L)
{
    GifImage* gif = LuaAux::getUserData<GifImage>(L, 1, GIF_TABLE);

    if (!gif)
        return 0;

    std::string error;

    if (lua_type(L, 1) != LUA_TSTRING)
        return 0;

    if(!loadGif(*gif, lua_tostring(L, 1), error))
    {
        lua_pushboolean(L, false);
        lua_pushstring(L, error.c_str());
        return 2;
    }

    lua_pushboolean(L, true);
    return 1;
}

int Gif::GetSprite(lua_State* L)
{
    GifImage* gif = LuaAux::getUserData<GifImage>(L, 1, GIF_TABLE);

    if (!gif)
        return 0;

    lua_pushlightuserdata(L, &gif->sprite);
    luaL_setmetatable(L, SPRITE_TABLE);
    return 1;
}

int Gif::GetCanvas(lua_State* L)
{
    GifImage* gif = LuaAux::getUserData<GifImage>(L, 1, GIF_TABLE);

    if (!gif)
        return 0;

    lua_pushlightuserdata(L, &gif->canvas);
    luaL_setmetatable(L, RENDERTEXTURE_TABLE);
    return 1;
}

int Gif::GetFrameCount(lua_State* L)
{
    GifImage* gif = LuaAux::getUserData<GifImage>(L, 1, GIF_TABLE);

    if (!gif)
        return 0;

    lua_pushnumber(L, gif->frameCount);
    return 1;
}

int Gif::SetFrameIndex(lua_State* L)
{
    GifImage* gif = LuaAux::getUserData<GifImage>(L, 1, GIF_TABLE);
    int newIndex = lua_tonumber(L, 2);

    if (!gif || newIndex < 0 || newIndex >= gif->frameCount || newIndex == gif->frames->index)
        return 0;

    if (newIndex > gif->frames->index)
    {
        for (int index = gif->frames->index; index < newIndex; index++)
        {
            gif->frames = gif->frames->nextFrame;
            if (gif->frames->disposal == 2) // Dispose to background color
                gif->canvas.clear(gif->backgroundColor);
            gif->canvas.draw(gif->frames->sprite, g_Shader);
        }
    }
    else
    {
        while (gif->frames->nextFrame->index != 0)
            gif->frames = gif->frames->nextFrame;

        for (int index = 0; index <= newIndex; index++)
        {
            gif->frames = gif->frames->nextFrame;
            if (gif->frames->disposal == 2) // Dispose to background color
                gif->canvas.clear(gif->backgroundColor);
            gif->canvas.draw(gif->frames->sprite, g_Shader);
        }
    }
    gif->canvas.display();
    return 0;
}

int Gif::StepFrame(lua_State* L)
{
    GifImage* gif = LuaAux::getUserData<GifImage>(L, 1, GIF_TABLE);

    if (!gif)
        return 0;

    gif->frames = gif->frames->nextFrame;

    if (gif->frames->disposal == 2) // Dispose to background color
        gif->canvas.clear(gif->backgroundColor);

    gif->canvas.draw(gif->frames->sprite, g_Shader);
    gif->canvas.display();

    return 0;
}

int Gif::Draw(lua_State* L)
{
    GifImage* gif = LuaAux::getUserData<GifImage>(L, 1, GIF_TABLE);

    if (!gif)
        return 0;

    sf::RenderWindow* targetWindow = LuaAux::getUserData<sf::RenderWindow>(L, 2, WINDOW_TABLE);
    sf::RenderTexture* targetCanvas = LuaAux::getUserData<sf::RenderTexture>(L, 2, RENDERTEXTURE_TABLE);

    if (targetWindow)
        targetWindow->draw(gif->sprite, g_Shader);
    else if (targetCanvas)
        targetCanvas->draw(gif->sprite, g_Shader);

    return 0;
}

int Gif::Destroy (lua_State* L)
{
    SFMLuaClassDestroy(GifImage);
    return 0;
}

Gif::GifImage::~GifImage()
{
    unloadGif(*this);
}
