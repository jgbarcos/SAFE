#ifndef XTEXTUREWRAPPER_H
#define XTEXTUREWRAPPER_H

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>

#include "SAFE/Camera.h"
#include "SAFE/Color.h"
#include "SAFE/Rect.h"

namespace safe {

/** 
 * Wrapper that act as a boundary between SDL and SAFE textures.
 * Reduces the coupling with SDL.
 */
class Texture
{
    public:
        /**
         * Constructor that loads the texture from disk.
         * @param path filepath.
         */
        Texture( const std::string& path){
            LoadFromFile( path );
        };

        /**
         * Constructor that creates an empty texture.
         * @param width in pixels.
         * @param height in pixels.
         * @param id unique identifier.
         */
        Texture( int width, int height, const std::string& id)
                : mID(id), mWidth(width), mHeight(height) {
            CreateEmpty( width, height);
        }

        // TODO: Some sort of Copy Constructor
        
        /**
         * Destructor, releases the texture.
         */
        virtual ~Texture(){
            Free();
        };
        
        /**
         * Checks whether if the texture is loaded as it could be released without destructor.
         */
        bool IsLoaded(){
            return mpTexture != nullptr;
        };

        /**
         * Renders the image into the target Camera (which contain the SDL_Renderer)
         * @param camera target Camera.
         * @param screenPos position of the image in screen coordinates (pixels).
         * @param scale scales the image up or down.
         * @param angle rotate the texture.
         * @param center defines the center of the texture, offsets texture position.
         * @param clip rectangle of the texture to be rendered.
         */
        void Render(const Camera& camera, const Vector2& screenPos, const Vector2& scale, 
                float angle, const Vector2& center, const Rect& clip);

        int GetWidth() const { return mWidth; }
        int GetHeight() const { return mHeight; }
        std::string getID() const { return mID; }

        /**
         * Update the default renderer target, used if none is provided.
         * @param defaultRenderer target SDL_Renderer.
         */
        static void SetDefaultRenderer(SDL_Renderer* defaultRenderer){
            spDefaultRenderer = defaultRenderer;
        }

        /**
         * Submit the texture to the GPU.
         */
        void SubmitTexture(); // TODO: This copies all the pixels. If required, allow direct pixel manipulation.
        
        /**
         * Submits the texture to the GPU if there are any changes.
         */
        void SubmitIfRequired(){
            if(mModified){
                SubmitTexture();
                mModified = false;
            }
        }

        /**
         * Releases the texture (memory and GPU).
         */
        void Free();
        
        /**
         * Reads a pixel from the texture.
         * @param x
         * @param y
         * @return color as Uint32 (Color helps with formatting into RGBA)
         */
        Uint32 GetPixel32(int x, int y)
        {
            return GetPixel32(mpSurface, x, y);
        }

        /**
         * Writes a pixel into the texture.
         * @param x
         * @param y
         * @param pixel as Uint32 (Color helps with formatting into RGBA)
         */
        void PutPixel32(int x, int y, Uint32 color )
        {
            PutPixel32(mpSurface, x, y, color);
            mModified = true;
        }

        /**
         * Reads a pixel from the provided SDL_Surface.
         * @param surface provided texture
         * @param x
         * @param y
         * @return color as Uint32 (Color helps with formatting into RGBA)
         */
        static Uint32 GetPixel32(SDL_Surface* surface, int x, int y){
            //Convert the pixels to 32 bit
            Uint32 *pixels = (Uint32 *)surface->pixels;

            //Get the requested pixel
            return pixels[ ( y * surface->w ) + x ];
        }

        /**
         * Writes a pixel into the provided SDL_Surface.
         * @param surface provided texture
         * @param x
         * @param y
         * @param color as Uint32 (Color helps with formatting into RGBA)
         */
        static void PutPixel32(SDL_Surface* surface, int x, int y, Uint32 color){
            //Convert the pixels to 32 bit
            Uint32 *pixels = (Uint32 *)surface->pixels;

            //Set the pixel
            pixels[ ( y * surface->w ) + x ] = color;
        }
        
        /**
         * Plots text into the texture.
         * @param font pointer to TTF_Font (non ownership).
         * @param text string contents.
         * @param origin start point of the text.
         * @param color Color of the text.
         */
        void PlotText(TTF_Font* font, const std::string& text, const Vector2& origin = Vector2(0), Color color = Color(0,0,0,255));   
        
        /**
         * Fills a rectangle with a color into the texture.
         * @param rect target rectangle.
         * @param color Color of the rectangle.
         */
        void FillRect(Rect rect, Color color);


    
    private:
        /**
         * Renders the texture with SDL parameters.
         * @param renderer
         * @param xpos
         * @param ypos
         * @param xscale
         * @param yscale
         * @param angle
         * @param clip
         * @param center
         * @param flip
         */
        void RenderSDL( SDL_Renderer* renderer, int xpos, int ypos, float xscale, float yscale,
                               float angle, SDL_Rect* clip, SDL_Point* center, SDL_RendererFlip flip);

        /**
         * Load texture from file path.
         * @param path loaded file.
         * @return true if success.
         */
        bool LoadFromFile( const std::string& path );
        
        /**
         * Creates an empty texture.
         * @param width in pixels.
         * @param height in pixels.
         * @return true if success.
         */
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
