//#pragma once

#include<iostream>
#include<string>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<SOIL/SOIL.h>

class GenTexture
{
private:
	GLuint id;
	int width;
	int height;
	unsigned char* image;
	unsigned int type;

public:


	GenTexture(const char* fileName, GLenum type)
	{
		GLenum err;
		this->type = type;

		image = SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

		glGenTextures(1, &this->id);
		glBindTexture(this->type, this->id);

		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cerr << "OpenGL error in texture generation 0: " << err << std::endl;
		}


		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cerr << "OpenGL error in texture generation 1: " << err << std::endl;
		}

		if (image)
		{
			glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(this->type);
		}
		else
		{
			std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << fileName <<"\n";
		}

		//glActiveTexture(0);
		glBindTexture(this->type, 0);
		SOIL_free_image_data(image);

		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cerr << "OpenGL error in texture generation 2: " << err << std::endl;
		}
	}




	~GenTexture()
	{
		glDeleteTextures(1, &this->id);
	}

	int getHeight()
	{
		return height;
	}

	int getWidth()
	{
		return width;
	}

	GLuint getID()
	{
		return id;
	}

	unsigned char* GetData()
	{
		return image;
	}

	inline GLuint getID() const { return this->id; }

	void bind(const GLint texture_unit)
	{
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(this->type, this->id);
	}

	void bind(const GLuint target, const GLint texture_unit)
	{
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(target, this->id);
	}



	void unbind()
	{
		glActiveTexture(0);
		glBindTexture(this->type, 0);
	}

	void loadFromFile(const char* fileName)
	{
		if (this->id)
		{
			glDeleteTextures(1, &this->id);
		}

		unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

		glGenTextures(1, &this->id);
		glBindTexture(this->type, this->id);

		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		if (image)
		{
			glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(this->type);
		}
		else
		{
			std::cout << "ERROR::TEXTURE::LOADFROMFILE::TEXTURE_LOADING_FAILED: " << fileName << "\n";
		}

		glActiveTexture(0);
		glBindTexture(this->type, 0);
		SOIL_free_image_data(image);
	}
};
