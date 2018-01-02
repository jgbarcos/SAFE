#include "SAFE/Texture.h"

namespace safe {

SDL_Renderer* Texture::spDefaultRenderer = nullptr;
TTF_Font* Texture::spDefaultFont = nullptr;

void Texture::Render(const Camera& camera, const Vector2& screenPos, 
                     const Vector2& scale, float angle, const Vector2& center, 
                     const Rect& clip) {

    Vector2 global_scale = scale;
    global_scale.x = global_scale.x * camera.mTransform.mScale.x;
    global_scale.y = global_scale.y * camera.mTransform.mScale.y;

    SDL_Rect sdl_clip;
    sdl_clip.x = clip.getX();
    sdl_clip.y = clip.getY();
    sdl_clip.w = clip.getWidth();
    sdl_clip.h = clip.getHeight();

    if (sdl_clip.w <= 0 && sdl_clip.h <= 0) {
        sdl_clip.x = 0;
        sdl_clip.y = 0;
        sdl_clip.w = mWidth;
        sdl_clip.h = mHeight;
    }

    SDL_Point sdl_center;
    sdl_center.x = center.x * sdl_clip.w * global_scale.x;
    sdl_center.y = center.y * sdl_clip.h * global_scale.y;

    int x = screenPos.getX() - sdl_center.x;
    int y = screenPos.getY() - sdl_center.y;

    RenderSDL(camera.getSDLRenderer(),
              x, y, global_scale.x, global_scale.y, angle,
              &sdl_clip, &sdl_center, SDL_FLIP_NONE);
}

void Texture::SubmitTexture() {
    // Transfer all changes of texture into surface
    SDL_LockTexture(mpTexture, nullptr, &mSDLPixels, &mSDLPitch);
    memcpy(mSDLPixels, mpSurface->pixels, mpSurface->pitch * mpSurface->h);
    SDL_UnlockTexture(mpTexture);
    mSDLPixels = nullptr; // after unlocking this is invalid, so better put a null on it
}

void Texture::Free() {
    //Free texture if it exists
    if (mpTexture != NULL) {
        SDL_DestroyTexture(mpTexture);
        SDL_FreeSurface(mpSurface);
        mpTexture = nullptr;
        mpSurface = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

std::vector<std::string> GetLines(const std::string& text) {
    std::vector<std::string> lines;
    std::string currentString = "";
    for (size_t i = 0; i < text.size(); i++) {
        if (text.at(i) == '\n') {
            lines.push_back(currentString);
            currentString = "";
        }
        else {
            currentString += text.at(i);
        }
    }
    if (currentString.size() > 0) {
        lines.push_back(currentString);
    }
    return lines;
}

Vector2 TextSize(TTF_Font* font, const std::string& text) {
    std::vector<std::string> lines = GetLines(text);
    int maxWidth = 0;
    int sumHeight = 0;
    for (auto& l : lines) {
        int w, h;
        TTF_SizeText(font, l.c_str(), &w, &h);
        sumHeight += h;
        if (w > maxWidth) {
            maxWidth = w;
        }
    }
    return Vector2(maxWidth, sumHeight);
}

void Texture::PlotText(TTF_Font* font, const std::string& text, 
                       const Vector2& origin, Color color, bool resizeIfRequired) {
    if (font == nullptr) {
        font = spDefaultFont;
    }
    if (resizeIfRequired) {
        auto size = TextSize(font, text);
        if (mWidth < size.x || mHeight < size.y) {
            CreateEmpty(size.x, size.y);
        }
    }
    if (IsLoaded()) {
        SDL_Color sdlColor = {color.r, color.g, color.b, color.a};

        int sumHeight = 0;
        for (auto& l : GetLines(text)) {

            SDL_Surface* genSurface = TTF_RenderText_Solid(font, l.c_str(), sdlColor);
            if (genSurface == nullptr) {
                printf("Unable to render text into surface! SDL Error: %s\n", SDL_GetError());
                return;
            }

            SDL_Surface* textSurface = SDL_ConvertSurfaceFormat(genSurface, SDL_PIXELFORMAT_RGBA8888, 0);
            if (textSurface == nullptr) {
                printf("Unable to convert generated surface to display format! SDL Error: %s\n", SDL_GetError());
                return;
            }

            SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_BLEND);
            SDL_Rect target;
            target.x = 0;
            target.y = sumHeight;
            target.w = mpSurface->w;
            target.h = mpSurface->h;
            SDL_BlitSurface(textSurface, NULL, mpSurface, &target);

            int w, h;
            TTF_SizeText(font, l.c_str(), &w, &h);
            sumHeight += h;

            // Free everything used
            SDL_FreeSurface(textSurface);
            SDL_FreeSurface(genSurface);

            mModified = true;
        }

    }
}

void Texture::FillRect(Rect rect, Color color) {
    int x = rect.getX();
    int y = rect.getY();
    int size_x = rect.getWidth();
    int size_y = rect.getHeight();

    if (x < 0) {
        size_x -= x;
        x = 0;
    }

    if (y < 0) {
        size_y -= y;
        y = 0;
    }

    Uint32 c = color.to32BE();

    for (int i = x; i < size_x + x && i < GetWidth(); i++) {
        for (int j = y; j < size_y - y && j < GetHeight(); j++) {
            PutPixel32(i, j, c);
        }
    }
}

void Texture::RenderSDL(SDL_Renderer* renderer, int xpos, int ypos, float xscale, float yscale,
                        float angle, SDL_Rect* clip, SDL_Point* center, SDL_RendererFlip flip) {
    //Set rendering space and render to screen
    SDL_Rect renderQuad = {xpos, ypos, clip->w, clip->h};

    renderQuad.w *= xscale;
    renderQuad.h *= yscale;
    
    if(renderQuad.w < 0){
        renderQuad.w = -renderQuad.w;
        renderQuad.x -= renderQuad.w;
        flip = (SDL_RendererFlip)(flip ^ SDL_FLIP_HORIZONTAL);
    }
    if(renderQuad.h < 0){
        renderQuad.h = -renderQuad.h;
        renderQuad.y -= renderQuad.h;
        flip = (SDL_RendererFlip)(flip ^ SDL_FLIP_VERTICAL);
    }

    SubmitIfRequired();

    //Render to screen
    SDL_RenderCopyEx(renderer, mpTexture, clip, &renderQuad, angle, center, flip);
}

bool Texture::LoadFromFile(const std::string& path) {
    //Get rid of preexisting texture
    Free();

    //Load image at specified path
    mID = path;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }

    mpSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(loadedSurface);
    if (mpSurface == NULL) {
        printf("Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    mWidth = mpSurface->w;
    mHeight = mpSurface->h;

    mpTexture = SDL_CreateTexture(spDefaultRenderer, SDL_PIXELFORMAT_RGBA8888, 
                                  SDL_TEXTUREACCESS_STREAMING,
                                  mWidth, mHeight);

    if (mpTexture == nullptr) {
        printf("Unable to create texture %s! SDL Error: %s\n", mID.c_str(), SDL_GetError());
        return false;
    }

    SDL_SetTextureBlendMode(mpTexture, SDL_BLENDMODE_BLEND);

    mModified = true;

    return true;
}

bool Texture::CreateEmpty(int width, int height) {
    Free();
    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_Surface* createdSurface = SDL_CreateRGBSurface(0, width, height, 32,
                                                       rmask, gmask, bmask, amask);

    mpSurface = SDL_ConvertSurfaceFormat(createdSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(createdSurface);

    if (mpSurface == nullptr) {
        SDL_Log("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
        return false;
    }

    //Get image dimensions
    mWidth = mpSurface->w;
    mHeight = mpSurface->h;

    mpTexture = SDL_CreateTexture(spDefaultRenderer, SDL_PIXELFORMAT_RGBA8888, 
                                  SDL_TEXTUREACCESS_STREAMING,
                                  mWidth, mHeight);

    if (mpTexture == nullptr) {
        printf("Unable to create texture %s! SDL Error: %s\n", mID.c_str(), SDL_GetError());
        return false;
    }

    SDL_SetTextureBlendMode(mpTexture, SDL_BLENDMODE_BLEND);

    mModified = true;

    return true;
}

} // namespace safe
