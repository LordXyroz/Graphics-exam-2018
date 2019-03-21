#include "src/headers/Texture.h"
#include "src/headers/logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "src/headers/stb_image.h"

#include <SDL2/include/SDL_surface.h>


// Only used for textbox texture, would cause memory leak if used in createTextureFromText func
Texture::Texture()
{
    glGenTextures(1, &m_texture_number);                    // Generates unique texture ID
}

// Default constructor we use for normal entities
Texture::Texture(const char* path)
{
    // Standard stbi image loading
    stbi_set_flip_vertically_on_load(true);                 // Makes sure the image is not upside down

    int width;
    int height;
    int nrChannels;

    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

    glGenTextures(1, &m_texture_number);                    // Generates unique texture ID

    glBindTexture(GL_TEXTURE_2D, m_texture_number);         // Binds texture for use

    if (data)
    {
        // Creates texture from data from stbi image loader
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        LOG_ERROR("ERROR::TEXTURE::LOADING..... From path: %s", path);
    }

    stbi_image_free(data);                                  // Frees up memory after texture is loaded
}

// Function for creating texture for text on screen
int Texture::createTextureFromText(TTF_Font* font, std::string text, SDL_Color color)
{
    // Creates texture data based on string
    message = TTF_RenderText_Blended(font, text.c_str(), color);

    glBindTexture(GL_TEXTURE_2D, m_texture_number);          // Binds texture for use

    // Creates texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, message->w, message->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, message->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    int ret = message->w;                                   // return variabel

    SDL_FreeSurface(message);                               // Frees up memory after texture is created
    return ret;
}

GLuint Texture::getNo()
{
    return m_texture_number;
}
