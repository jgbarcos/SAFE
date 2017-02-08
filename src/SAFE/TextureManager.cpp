#include "SAFE/TextureManager.h"

namespace safe{

std::shared_ptr<Texture> TextureManager::LoadFromFile(const std::string& path){
    auto it = mLoadedTextures.find(path);
    if ( it == mLoadedTextures.end()){
        mLoadedTextures[path] = std::make_shared<Texture>(path);
    }
    std::cout << "[TextureManager]" << "(size: " << mLoadedTextures.size() << ")"
        << " LoadFromFile() for " << *mLoadedTextures[path]
        << " (refs: " << mLoadedTextures[path].use_count() << ")"
    << std::endl;
    return mLoadedTextures[path];
}

std::shared_ptr<Texture> TextureManager::GetFromID ( const std::string& id) {
    auto it = mLoadedTextures.find(id);
    if( it == mLoadedTextures.end()){
        std::cout << "[TextureManager]" << "(size: " << mLoadedTextures.size() << ")"
            << " GetFromID() for " << id << " FAILED (reason: not found)"
        << std::endl;
        return nullptr;
    }
    else{
        std::cout << "[TextureManager]" << "(size: " << mLoadedTextures.size() << ")"
            << " GetFromID() for " << *mLoadedTextures[id]
            << " (refs: " << mLoadedTextures[id].use_count() << ")"
        << std::endl;
        return mLoadedTextures[id];
    }

}

std::shared_ptr<Texture> TextureManager::CreateEmpty( int width, int height, const std::string& id){
    auto it = mLoadedTextures.find(id);
    if( it == mLoadedTextures.end()){
        mLoadedTextures[id] = std::make_shared<Texture>(width, height, id);
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

    return mLoadedTextures[id];
}


void TextureManager::Release ( std::string path, bool onlyIfUnused ) {
    auto it = mLoadedTextures.find(path);
    if ( it != mLoadedTextures.end()){
        Texture& texRef = *(*it).second;

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

void TextureManager::ReleaseAll(bool onlyIfUnused){
    auto it = mLoadedTextures.begin();
    while(it != mLoadedTextures.end()){
        Texture& texRef = *(*it).second;
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
  
} // namespace safe