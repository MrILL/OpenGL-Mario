#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Graphic_Square.h"
#include <string>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#define GLUT_DISABLE_ATEXIT_HACK

#include "Player.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const unsigned int block_size = 32; // при изменении проиходит дичь, надо поправить

Player *obj;
double timeD;
double timeKoef = 1;

const int tile_q = 4;
Graphic_Square *tile[tile_q];

const int H = 17;
const int W = 150;
std::string TileMap[H] = {
"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
"0                                                                                                                                                    0",
"0                                                                                    w                                                               0",
"0                   w                                  w                   w                                                                         0",
"0                                      w                                       kk                                                                    0",
"0                                                                             k  k    k    k                                                         0",
"0                      c                                                      k      kkk  kkk  w                                                     0",
"0                                                                       r     k       k    k                                                         0",
"0                                                                      rr     k  k                                                                   0",
"0                                                                     rrr      kk                                                                    0",
"0               c    kckck                                           rrrr                                                                            0",
"0                                      00              00           rrrrr                                                                            0",
"0G                                     00              00          rrrrrr            G                                                               0",
"0           d    g       d             t0              t0         rrrrrrr                                                                            0",
"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
};

/*
Tiles ID:
0 - P | Brick1
1 - c | ?
2 - k | Brick2
3 - r | Brick3
4 - t | Vertical tube decoration

*/


int main()
{
	/// different initializating staff

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}



	/// preraring command for main loop

	std::reverse(&TileMap[0], &TileMap[H]);
	obj = new Player("Texture/mario_1.jpg", 200, 200, block_size, block_size);
	obj->Init(TileMap, H);
	obj->add_texture("Texture/mario_2.jpg");
	obj->add_texture("Texture/mario_3.jpg");
	obj->add_texture("Texture/mario_4.jpg");

	tile[0] = new Graphic_Square("Texture/1.jpg", 0, 0, block_size, block_size);
	tile[1] = new Graphic_Square("Texture/2.jpg", 0, 0, block_size, block_size);
	tile[2] = new Graphic_Square("Texture/3.jpg", 0, 0, block_size, block_size);
	tile[3] = new Graphic_Square("Texture/4.jpg", 0, 0, block_size, block_size);
	tile[4] = new Graphic_Square("Texture/5.jpg", 0, 0, 2*block_size, 3*block_size);


	/// main loop

	double time = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		timeD = glfwGetTime() - time;
		if (timeD > 20) timeD = 20;
		time = glfwGetTime();
		timeD *= 2700;
		float offsetX = 10;
		float offsetY = 10;
	//	std::cout << timeD << '\t' << glfwGetTime() << '\t' << time << std::endl;

		processInput(window);

		glClearColor(103.0f / 255.0f, 143.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (obj->x > SCR_WIDTH / 2)
			offsetX = obj->x - SCR_WIDTH / 2;
		else
			offsetX = 0;
		if (obj->y > SCR_HEIGHT / 2 + obj->height)
			offsetY = obj->y - SCR_HEIGHT / 2 - obj->height;
		else
			offsetY = 0;
	//	offsetX = 0;
	//	offsetY = 0;

		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++)
			{
				if (TileMap[i][j] == 't') {
					tile[4]->x = float(j) * block_size;
					tile[4]->y = float(i) * block_size;
					tile[4]->draw(offsetX, offsetY);
					continue;
				}
				if (TileMap[i][j] == 'P') {
					tile[0]->x = float(j) * block_size;
					tile[0]->y = float(i) * block_size;
					tile[0]->draw(offsetX, offsetY);
					continue;
				}
				if (TileMap[i][j] == 'c') {
					tile[1]->x = float(j) * block_size;
					tile[1]->y = float(i) * block_size;
					tile[1]->draw(offsetX, offsetY);
					continue;
				}
				if (TileMap[i][j] == 'k') {
					tile[2]->x = float(j) * block_size;
					tile[2]->y = float(i) * block_size;
					tile[2]->draw(offsetX, offsetY);
					continue;
				}
				if (TileMap[i][j] == 'r') {
					tile[3]->x = float(j) * block_size;
					tile[3]->y = float(i) * block_size;
					tile[3]->draw(offsetX, offsetY);
					continue;
				}
//				if (TileMap[i][j] == '0');

				if (TileMap[i][j] == ' ') continue;
			}

		(*obj).draw(offsetX, offsetY);
		obj->update(float(timeD));

		glfwSwapBuffers(window);
		glfwPollEvents();
	//	system("pause");
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		obj->dx = 0.17f*block_size / 32;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		obj->dx = -0.17f*block_size / 32;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		if (obj->onGround) {
			obj->dy = 0.37f*block_size/32;
			obj->onGround = false;
		}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}