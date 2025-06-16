#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

//używany do wczytywania tekstur i nie powtarzania tego procesu wielokrotnie (+ jest bardziej "fancy" :) )

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>

using namespace std;
using namespace sf;

class TextureManager {
public:
    static TextureManager& getInstance() {
        static TextureManager instance;
        return instance;
    }

    Texture& getTexture(const string& path) {
        auto it = textures.find(path);
        if (it == textures.end()) {
            Texture tex;
            if (!tex.loadFromFile(path)) {
                cout << "Failed to load texture: " << path << endl;
                exit(1);
            }
            tex.setSmooth(true);
            auto [inserted_it, _] = textures.emplace(path, std::move(tex));
            return inserted_it->second;
        }
        return it->second;
    }

private:
    unordered_map<string, Texture> textures;

    TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
};

#endif // TEXTUREMANAGER_H
