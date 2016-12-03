#ifndef STEXTUREMANAGER_H
#define STEXTUREMANAGER_H

#include <memory>
#include <map>

#include <SAFE/Texture.h>
#include <SAFE/TextureWrapper.h>

namespace safe {

class TextureManager
{
    public:
        /** Loads a texture given the path. If already loaded, returns the shared_ptr
         */
        Texture _LoadFromFile ( const std::string& path ){ // already loaded, give shared_ptr
            auto it = mLoadedTextures.find(path);
            if ( it == mLoadedTextures.end()){
                mLoadedTextures[path] = std::make_shared<TextureWrapper>(path);
            }
            std::cout << "[TextureManager]" << "(size: " << mLoadedTextures.size() << ")"
                << " LoadFromFile() for " << *mLoadedTextures[path]
                << " (refs: " << mLoadedTextures[path].use_count() << ")"
            << std::endl;
            return Texture(mLoadedTextures[path], path);
        }

        std::shared_ptr<TextureWrapper> LoadFromFile(const std::string& path){
            auto it = mLoadedTextures.find(path);
            if ( it == mLoadedTextures.end()){
                mLoadedTextures[path] = std::make_shared<TextureWrapper>(path);
            }
            std::cout << "[TextureManager]" << "(size: " << mLoadedTextures.size() << ")"
                << " LoadFromFile() for " << *mLoadedTextures[path]
                << " (refs: " << mLoadedTextures[path].use_count() << ")"
            << std::endl;
            return mLoadedTextures[path];
        }

        Texture GetFromID ( const std::string& id) {
            auto it = mLoadedTextures.find(id);
            if( it == mLoadedTextures.end()){
                std::cout << "[TextureManager]" << "(size: " << mLoadedTextures.size() << ")"
                    << " GetFromID() for " << id << " FAILED (reason: not found)"
                << std::endl;
                return Texture(std::shared_ptr<TextureWrapper>(), id);
            }
            else{
                std::cout << "[TextureManager]" << "(size: " << mLoadedTextures.size() << ")"
                    << " GetFromID() for " << *mLoadedTextures[id]
                    << " (refs: " << mLoadedTextures[id].use_count() << ")"
                << std::endl;
                return Texture(mLoadedTextures[id], id);
            }

        }

        Texture CreateEmpty( int width, int height, const std::string& id){
            auto it = mLoadedTextures.find(id);
            if( it == mLoadedTextures.end()){
                mLoadedTextures[id] = std::make_shared<TextureWrapper>(width, height, id);
                std::cout << "[TextureManager]" << "(size: " << mLoadedTextures.size() << ")"
                    << " CreateEmpty() for " << *mLoadedTextures[id]
                    << " (refs: " << mLoadedTextures[id].use_count() << ")"
                << std::endl;
            }
            else{
                std::cout << "[TextureManager]" << "(size: " << mLoadedTextures.size() << ")"
                    << " CreateEmpty() for " << id << " FAILED (reason: already exists)"
                << std::endl;
            }

            return Texture(mLoadedTextures[id], id);
        }


        void Release ( std::string path, bool onlyIfUnused = true ) {
            auto it = mLoadedTextures.find(path);
            if ( it != mLoadedTextures.end()){
                TextureWrapper& texRef = *(*it).second;

                if( !onlyIfUnused || (*it).second.unique()){
                    std::cout << "[TextureManager]" << "(size: " << mLoadedTextures.size() << ")"
                        << " Release() for " << texRef
                    << std::endl;

                    (*it).second->Free();
                    mLoadedTextures.erase(it);
                }
                else{
                    std::cout << "[TextureManager]" << "(size: " << mLoadedTextures.size() << ")"
                        << " Release() for  " << texRef << " SKIPPED (reason: resource is shared)"
                        << " (refs: " << (*it).second.use_count()-1 << ")"
                    << std::endl;
                }
            }
        }

        void ReleaseAll(bool onlyIfUnused = true){
            auto it = mLoadedTextures.begin();
            while(it != mLoadedTextures.end()){
                TextureWrapper& texRef = *(*it).second;
                if( !onlyIfUnused || (*it).second.unique()){
                    std::cout << "[TextureManager]" << "(size: " << mLoadedTextures.size() << ")"
                        << " releasing " << texRef
                    << std::endl;

                    (*it).second->Free();
                    it = mLoadedTextures.erase(it);
                }
                else{
                    std::cout << "[TextureManager]" << "(size: " << mLoadedTextures.size() << ")"
                        << " cannot release " << texRef
                        << " (refs: " << (*it).second.use_count()-1 << ")"
                    << std::endl;
                    it++;
                }
            }
        }

    private:
        std::map < std::string, std::shared_ptr< TextureWrapper > > mLoadedTextures;
};

} // namespace safe



#endif // STEXTUREMANAGER_H
