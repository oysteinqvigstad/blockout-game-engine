#include "TextureManager.h"

bool TextureManager::LoadTexture2D(const std::string& name, const std::string& filename, GLuint unit, bool mipMap)
{
    Texture texture(name, filename, unit, mipMap, Texture2D);
    bool RGBA = filename.substr(filename.find_last_of('.') + 1) == "png";
    auto data = LoadTextureImage(TEXTURES_DIR + texture.filename, texture.width, texture.height, texture.bpp, (RGBA ? STBI_rgb_alpha : STBI_rgb));


    if (!data) {
        return false;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0 + unit); // Texture Unit
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, (RGBA ? GL_RGBA8 : GL_RGB8), texture.width, texture.height, 0, (RGBA ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);

    if (mipMap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    this->Textures.push_back(texture);

    FreeTextureImage(data);

    return true;
}

bool TextureManager::LoadCubeMap(const std::string& name, const std::vector<std::string>& filenames, GLuint unit, bool mipMap)
{
    Texture texture(name, filenames[0], unit, mipMap, CubeMap);
    unsigned char *data;
    bool RGBA;
    GLuint tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0 + unit); // Texture Unit
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    for (unsigned int i = 0; i < 6; i++) {
        if (i == 0 || filenames.size() == 6) {
            auto filePath = std::string(TEXTURES_DIR) + filenames[i];
            RGBA = filenames[i].substr(filenames[i].find_last_of('.') + 1) == "png";
            data = LoadTextureImage(filePath, texture.width,
                                          texture.height, texture.bpp,
                                          (RGBA ? STBI_rgb_alpha : STBI_rgb));
            if (!data)
                return false;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, (RGBA ? GL_RGBA8 : GL_RGB8), texture.width, texture.height, 0, (RGBA ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
    }

    if (mipMap) {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    // Wrapping
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // Filtering
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    this->Textures.push_back(texture);
    FreeTextureImage(data);

    return true;
}


GLint TextureManager::GetUnitByName(const std::string& name) const
{
    for(const auto& texture: this->Textures)
    {
        if (texture.name == name)
        {
            return texture.unit;
        }
    }
    return -1;
}

unsigned char* TextureManager::LoadTextureImage(const std::string& filepath, int& width, int& height, int& bpp, int format)
{
    return stbi_load(filepath.c_str(), &width, &height, &bpp, format);
}


void TextureManager::FreeTextureImage(unsigned char* data)
{
    if (data)
    {
        stbi_image_free(data);
    }
}
