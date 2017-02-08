#ifndef STEXTUREMANAGER_H
#define STEXTUREMANAGER_H

#include <memory>
#include <map>

#include <SAFE/Texture.h>
#include <SAFE/Texture.h>

namespace safe {

class TextureManager
{
    public:
        // Loads a texture given the path. If already loaded, returns the shared_ptr
        std::shared_ptr<Texture> LoadFromFile(const std::string& path);

        std::shared_ptr<Texture> GetFromID ( const std::string& id);
        std::shared_ptr<Texture> CreateEmpty( int width, int height, const std::string& id);


        void Release ( std::string path, bool onlyIfUnused = true );
        void ReleaseAll(bool onlyIfUnused = true);

    private:
        std::map < std::string, std::shared_ptr< Texture > > mLoadedTextures;
};

} // namespace safe



#endif // STEXTUREMANAGER_H
