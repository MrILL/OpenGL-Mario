#pragma once
#include "Shader.h"
#include <string>
#include <vector>
#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Graphic_Square {
private:
	Shader ourShader;
	unsigned int VAO;				
	unsigned int VBO;				
	unsigned int EBO;				
	unsigned int texture_tmp;		
	std::vector<unsigned> texture;	
	unsigned int max_t;				
	unsigned current_t;				//can set with set_texture()
public:
	float width_f, height_f;
	float width, height;
	float x, y;
	Graphic_Square(std::string path = "", float xPos = 0.0f, float yPos = 0.0f, float w = 32.0f, float h = 32.0f) :
		ourShader("3.3.shader.vs", "3.3.shader.fs"),
		width_f(float(w) * 2 / 800),
		height_f(float(h) * 2 / 600),
		width(w),
		height(h),
		x(xPos),
		y(yPos),
		max_t(0),
		current_t(0),
		texture_tmp(0)
		{
		float vertices[] = {
		 width_f,	height_f,	0.0f,	1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 width_f,	0.0f,		0.0f,	0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		 0.0f,		0.0f,		0.0f,	0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		 0.0f,		height_f,	0.0f,	1.0f, 1.0f, 0.0f,   0.0f, 1.0f // top left 
		};
		unsigned int indices[] = {
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO); // that's it
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		/// begin part of texture_tmp
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		
		///adding texture

		if (!path.empty()) {
			glGenTextures(1, &texture_tmp);
			glBindTexture(GL_TEXTURE_2D, texture_tmp);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			int wid, hei, nrChannels;
			stbi_set_flip_vertically_on_load(true);
			unsigned char *data = stbi_load(path.c_str(), &wid, &hei, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wid, hei, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				texture.push_back(texture_tmp);
				max_t++;
			}
			else
			{
				std::cout << "Failed to load texture " << path.c_str() << std::endl;
			}
			stbi_image_free(data);
		}
	}

	void add_texture(std::string path) {
		glGenTextures(1, &texture_tmp);
		glBindTexture(GL_TEXTURE_2D, texture_tmp);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		int wid, hei, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(path.c_str(), &wid, &hei, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wid, hei, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture " << path.c_str() << std::endl;
			stbi_image_free(data);
			return;
		}
		stbi_image_free(data);
		max_t++;
		texture.push_back(texture_tmp);
	}

	void set_texture(unsigned num) {
		if (num >= max_t) {
			current_t = max_t-1;
		}
		else {
			current_t = num;
		}
	}

	inline unsigned get_max_t() {
		return max_t;
	}

	~Graphic_Square() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void draw(float offsetX = 0, float offsetY = 0) {
		ourShader.use();
		ourShader.setFloat("x", (x-offsetX) * 2 / 800);
		ourShader.setFloat("y", (y-offsetY) * 2 / 600);
		glBindTexture(GL_TEXTURE_2D, texture[current_t]);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void move(float xPos, float yPos) {
		x += xPos;
		y += yPos;
	}
};