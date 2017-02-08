#ifndef XTEXTUREWRAPPER_H
#define XTEXTUREWRAPPER_H

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>

#include "SAFE/Camera.h"
#include "SAFE/Color.h"
#include "SAFE/Rect.h"

namespace safe {

/** Wraps SDL_Texture, acts as a boundary between SDL and SAFE textures */
class Texture // TODO: Create XTextureWrapper.cpp when stablished
                      // Make abstract of public data if required
{
    public:
        /** Load texture from resource */
        Texture( const std::string& path){
            LoadFromFile( path );
        };

        Texture( int width, int height, const std::string& id)
                : mID(id), mWidth(width), mHeight(height) {
            CreateEmpty( width, height);
        }

        // TODO: Copy Constructor

        virtual ~Texture(){
            Free();
        };
        

        bool IsLoaded(){
            return mpTexture != nullptr;
        };

        void Render(const Camera& camera, const Vector2& screenPos, const Vector2& scale, float angle, const Vector2& center, const Rect& clip);

        int GetWidth() const { return mWidth; }
        int GetHeight() const { return mHeight; }
        std::string getID() const { return mID; }

        static void SetDefaultRenderer(SDL_Renderer* defaultRenderer){
            spDefaultRenderer = defaultRenderer;
        }

        /** Submit texture to gpu */
        void SubmitTexture(); // TODO: This copies all the pixels. If required, allow direct pixel writting
        void SubmitIfRequired(){
            if(mModified){
                SubmitTexture();
                mModified = false;
            }
        }

        void Free();
        
        
        /*
         * Pixel manipulations
         */

        Uint32 GetPixel32(int x, int y)
        {
            return GetPixel32(mpSurface, x, y);
        }

        void PutPixel32(int x, int y, Uint32 pixel )
        {
            PutPixel32(mpSurface, x, y, pixel);
            mModified = true;
        }

        static Uint32 GetPixel32(SDL_Surface* surface, int x, int y){
            //Convert the pixels to 32 bit
            Uint32 *pixels = (Uint32 *)surface->pixels;

            //Get the requested pixel
            return pixels[ ( y * surface->w ) + x ];
        }

        static void PutPixel32(SDL_Surface* surface, int x, int y, Uint32 pixel){
            //Convert the pixels to 32 bit
            Uint32 *pixels = (Uint32 *)surface->pixels;

            //Set the pixel
            pixels[ ( y * surface->w ) + x ] = pixel;
        }
        
        void PlotText(TTF_Font* font, const std::string& text, const Vector2& origin = Vector2(0), Color color = Color(0,0,0,255));        
        void FillRect(Rect rect, Color color);


    
    private:
        void RenderSDL( SDL_Renderer* renderer, int xpos, int ypos, float xscale, float yscale,
                               float angle, SDL_Rect* clip, SDL_Point* center, SDL_RendererFlip flip);

        bool LoadFromFile( const std::string& path );
        bool CreateEmpty(int width, int height);
        

    private:
        static SDL_Renderer* spDefaultRenderer;

        SDL_Texture* mpTexture = nullptr;
        SDL_Surface* mpSurface = nullptr;

        // these are used by surface->texture in Submit()
        void* mSDLPixels;
        int mSDLPitch;

        std::string mID;
        int mWidth;
        int mHeight;

        bool mModified = true;
};

} // namespace safe

inline std::ostream& operator<<(std::ostream &strm, const safe::Texture& a) {
  return strm << "\"" << a.getID() << "\"[" << a.GetWidth() << "x" << a.GetHeight() <<"]";
}

#endif // XTEXTUREWRAPPER_H
