#pragma once
#include "libs.h"

class Texture
{
private:
	GLuint id;
	int height;
	int width;
    unsigned int type;
    GLint textureUnit;

    bool forceSRGB = true;
    GLenum internalFormat = forceSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;


public:
    Texture()
    {
        glGenTextures(1, &this->id);
    }

	Texture(const char* filename, GLenum type, GLint texture_unit)
	{
        this->type = type;
        this->textureUnit = texture_unit;
        // Texture init
        glGenTextures(1, &this->id);
        glBindTexture(type, this->id);

        // Texture 0 // Load image
        unsigned char* image = stbi_load(filename, &this->width, &this->height, nullptr, STBI_rgb_alpha);

        if (image)
        {
            glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(type, 0, internalFormat, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(type);
            stbi_image_free(image);
        }
        else
        {
            std::cout << "Error texture load failed for : " << filename << "\n";
        }

        glActiveTexture(0);
        glBindTexture(type, 0);
	}

	~Texture()
	{
		glDeleteTextures(GL_TEXTURE_2D, &this->id);
	}

    inline GLuint getID() const
    {
        return this->id;
    }

    void bind()
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(this->type, this->id);
    }

    void unBind()
    {
        glActiveTexture(0);
        glBindTexture(this->type, 0);
    }

    void loadImage(const char* filename, GLenum type, GLint texture_unit)
    {
        // Supression si déjà existant
        if (this->id)
        {
            glDeleteTextures(1, &this->id);

        }

        // Texture init
        this->type = type;
        this->textureUnit = texture_unit;
        glGenTextures(1, &this->id);
        glBindTexture(type, this->id);

        // Texture 0 // Load image
        unsigned char* image = stbi_load(filename, &this->width, &this->height, nullptr, STBI_rgb_alpha);

        if (image)
        {
            glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(type, 0, internalFormat, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(type);
            stbi_image_free(image);
        }
        else
        {
            std::cout << "Error texture load failed for : " << filename << "\n";
        }

        glActiveTexture(0);
        glBindTexture(type, 0);
    }

    void changeImage(const char* filename)
    {
        // Supression si déjà existant
        if (this->id)
        {
            glDeleteTextures(1, &this->id);

        }

        glGenTextures(1, &this->id);
        glBindTexture(type, this->id);

        // Texture 0 // Load image
        unsigned char* image = stbi_load(filename, &this->width, &this->height, nullptr, STBI_rgb_alpha);

        if (image)
        {
            glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(this->type, 0, internalFormat, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(this->type);
            stbi_image_free(image);
        }
        else
        {
            std::cout << "Error texture replacement failed for : " << filename << "\n";
        }

        glActiveTexture(0);
        glBindTexture(this->type, 0);
    }

    GLint getTextureUnit() const
    {
        return this->textureUnit;
    }
};