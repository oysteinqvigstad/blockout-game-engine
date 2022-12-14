#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

// External libraries
#include <glad/glad.h>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

// STD includes
#include <string>
#include <utility>
#include <vector>

class TextureManager
{
public:

    enum TextureType {Texture2D, CubeMap};

    struct Texture
    {
        [[maybe_unused]] bool mipMap;
        int width, height, bpp;
        std::string name;
        std::string filename;
        GLuint unit;
        TextureManager::TextureType type;
        Texture(std::string name, std::string filenames, const GLuint unit,
                bool mipMap, TextureManager::TextureType type)
        : name(std::move(name)), filename(std::move(filenames)), unit(unit),
          mipMap(mipMap), type(type), width(0), height(0), bpp(0) {};
    };

public:
    static TextureManager* GetInstance() {
        return TextureManager::Instance != nullptr ? TextureManager::Instance
                            : TextureManager::Instance = new TextureManager();
    }

public:
    bool LoadTexture2D(const std::string& name, const std::string& filename, GLuint unit, bool mipMap= true);
    bool LoadCubeMap(const std::string& name, const std::vector<std::string>& filenames, GLuint unit, bool mipMap= true);
    [[nodiscard]] GLint GetUnitByName(const std::string& name) const;

private:
    static unsigned char* LoadTextureImage(const std::string& filepath, int& width, int& height, int& bpp, int format);
    static void FreeTextureImage(unsigned char* data) ;

private:
    TextureManager()= default;
public:
    TextureManager(const TextureManager&) = delete;
    void operator=(const TextureManager&) = delete;

private:
    inline static TextureManager* Instance = nullptr;

private:
    std::vector<TextureManager::Texture> Textures;
};

#endif // TEXTUREMANAGER_H_
