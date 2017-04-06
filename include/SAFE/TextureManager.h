#ifndef STEXTUREMANAGER_H
#define STEXTUREMANAGER_H

#include <memory>
#include <map>

#include <SAFE/Texture.h>

namespace safe {
/**
 * Manages the lifetime of textures, allowing the same texture to be reused.
 */
class TextureManager
{
    public:
        /**
         * Loads a texture for the given path. If already loaded, returns the same pointer.
         * @param path path to the texture file.
         * @return pointer to texture (shared ownership)
         */
        std::shared_ptr<Texture> LoadFromFile(const std::string& path);
        
        /**
         * Get texture by ID. The ID of a loaded texture is the original path.
         * @param id unique id of the texture.
         * @return pointer to Texture (shared ownership).
         */
        std::shared_ptr<Texture> GetFromID ( const std::string& id);
        
        
        /**
         * Creates a new texture as an empty texture.
         * @param width in pixels.
         * @param height in pixels.
         * @param id unique id of the new texture.
         * @return pointer to Texture (shared ownership)
         */
        std::shared_ptr<Texture> CreateEmpty( int width, int height, const std::string& id);

        /**
         * Releases the texture. Takes into account the shared ownership.
         * @param id
         * @param onlyIfUnused if false, it forces the removal of the texture.
         */
        void Release ( std::string id, bool onlyIfUnused = true );
        /**
         * Releases all textures. Takes into account the shared ownership of each 
         * texture.
         * @param onlyIfUnused if false, it forces the removal of the textures.
         */
        void ReleaseAll(bool onlyIfUnused = true);
        
        // Set whether to print or not debug data into the console
        bool mDebugLog = false;

    private:
        std::map < std::string, std::shared_ptr< Texture > > mLoadedTextures;
};

} // namespace safe



#endif // STEXTUREMANAGER_H
