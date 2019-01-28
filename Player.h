#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Graphic_Square.h"
#include <time.h>

#include <iostream>

class Player: public Graphic_Square {
private:
	unsigned TileMap_h;
	std::string *TileMap;
	unsigned block_size;
	float current_texture;
public:
	float dx, dy;
	bool onGround;
	unsigned int textures[6];
	using Graphic_Square::Graphic_Square;
	void Init(std::string *map, unsigned map_h) {
		TileMap_h = map_h;
		TileMap = map;
		dx = dy = 0.1f;
		onGround = true;
		block_size = width;
	}
	void update(float timeD) {
		if (dx) {
			current_texture += 0.005*timeD;
			if (current_texture >= get_max_t() + 1) {
				current_texture -= get_max_t() + 1;
			}
			set_texture(current_texture);
		}
		x += dx * timeD;
		Collision(0);

		if (!onGround)
			dy -= 0.0005f*timeD;
		y += dy * timeD;
		onGround = false;
		Collision(1);

		if (y < 0) {
			y = 0;
			dy = 0;
			onGround = true;
		}
		dx = 0;
	}
	void Collision(int dir) {
		for (int i = int(y) / block_size; i < (int(y) + height) / block_size; i++) {
			for (int j = int(x) / block_size; j < (int(x) + width) / block_size; j++) {
				if (
					TileMap[i][j] == 'P' ||
					TileMap[i][j] == 'c' ||
					TileMap[i][j] == 'k' ||
					TileMap[i][j] == 'r' ||
					TileMap[i][j] == 't' ||
					TileMap[i][j] == '0'
					) {
					if ((dx > 0) && (dir == 0)) {
						x = (j) * block_size - width;
					}
					if ((dx < 0) && (dir == 0)) {
						x = float(j) * block_size + block_size;
					}
					if ((dy < 0) && (dir == 1)) { y = float(i) * block_size + height;  dy = 0;   onGround = true; }
					if ((dy > 0) && (dir == 1)) { y = float(i) * block_size - block_size;   dy = 0; }
				}
			}
		}
	}
};