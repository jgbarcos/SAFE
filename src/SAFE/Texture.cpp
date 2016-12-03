//
//#include "SAFE/Texture.h"
//
//#include <SDL_image.h>
//#include <SDL_ttf.h>
//
//#include <string>
//#include <stdio.h>
//#include <iostream>
//
//SDL_Renderer* Texture::sDefaultRenderer = NULL;
//TTF_Font* Texture::sDefaultFont = NULL;
//
//Texture::Texture()
//{
//	//Initialize
//	mTexture = NULL;
//	mWidth = 0;
//	mHeight = 0;
//}
//
//Texture::~Texture()
//{
//	//Deallocate
//	Free();
//}
//
//bool Texture::LoadFromFile( std::string path )
//{
//	//Get rid of preexisting texture
//	Free();
//
//	//The final texture
//	SDL_Texture* newTexture = NULL;
//
//	//Load image at specified path
//	mFilename = path;
//	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
//	if( loadedSurface == NULL )
//	{
//		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
//	}
//	else
//	{
//		//Color key image
//		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
//
//		//Create texture from surface pixels
//        newTexture = SDL_CreateTextureFromSurface( sDefaultRenderer, loadedSurface );
//		if( newTexture == NULL )
//		{
//			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
//		}
//		else
//		{
//			//Get image dimensions
//			mWidth = loadedSurface->w;
//			mHeight = loadedSurface->h;
//		}
//
//		//Get rid of old loaded surface
//		SDL_FreeSurface( loadedSurface );
//	}
//
//	//Return success
//	mTexture = newTexture;
//	return mTexture != NULL;
//}
//
//bool Texture::LoadFromRenderedText( std::string textureText, SDL_Color textColor )
//{
//    //Get rid of preexisting texture
//    Free();
//
//    //Render text surface
//    SDL_Surface* textSurface = TTF_RenderText_Solid( sDefaultFont, textureText.c_str(), textColor );
//    if( textSurface == NULL )
//    {
//        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
//    }
//    else
//    {
//        //Create texture from surface pixels
//        mTexture = SDL_CreateTextureFromSurface( sDefaultRenderer, textSurface );
//        if( mTexture == NULL )
//        {
//            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
//        }
//        else
//        {
//            //Get image dimensions
//            mWidth = textSurface->w;
//            mHeight = textSurface->h;
//        }
//
//        //Get rid of old surface
//        SDL_FreeSurface( textSurface );
//    }
//
//    //Return success
//    return mTexture != NULL;
//}
//
//void Texture::Free()
//{
//	//Free texture if it exists
//	if( mTexture != NULL )
//	{
//		SDL_DestroyTexture( mTexture );
//		mTexture = NULL;
//		mWidth = 0;
//		mHeight = 0;
//	}
//}
//
//void Texture::Render( SDL_Renderer* renderer, int xpos, int ypos, float xscale, float yscale,
//                       float angle, SDL_Rect* clip, SDL_Vector3* center, SDL_RendererFlip flip)
//{
//    xpos = xpos - (mWidth * xscale / 2.0);
//    ypos = ypos - (mHeight * yscale / 2.0);
//
//    //Set rendering space and render to screen
//    SDL_Rect renderQuad = { xpos, ypos, mWidth, mHeight };
//
//    //Set clip rendering dimensions
//    /*if( clip != NULL )
//    {
//        renderQuad.w = clip->w;
//        renderQuad.h = clip->h;
//    }*/
//
//    renderQuad.w *= xscale;
//    renderQuad.h *= yscale;
//
//    //Render to screen
//    SDL_RenderCopyEx( sDefaultRenderer, mTexture, clip, &renderQuad, angle, center, flip );
//}
//
//void Texture::SetAlpha( Uint8 alpha )
//{
//    //Modulate texture alpha
//    SDL_SetTextureAlphaMod( mTexture, alpha );
//}
//void Texture::SetColor( Uint8 r, Uint8 g, Uint8 b )
//{
//    //Modulate texture
//    SDL_SetTextureColorMod( mTexture, r, g, b );
//}
//
//int Texture::GetWidth()
//{
//	return mWidth;
//}
//
//int Texture::GetHeight()
//{
//	return mHeight;
//}
//
//void Texture::SetDefaultRenderer(SDL_Renderer* defaultRenderer){
//    sDefaultRenderer = defaultRenderer;
//}
//
//void Texture::SetDefaultRenderer(TTF_Font* defaultFont){
//    sDefaultFont = defaultFont;
//}
