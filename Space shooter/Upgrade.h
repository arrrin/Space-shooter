#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include<math.h>
#include<vector>
#include"dArr.h"
#include<fstream>

using namespace sf;

class Upgrade
{
private:
	float dtMultiplier;
	int type;

	float aliveTimerMax;
	float aliveTimer;

	dArr<Texture>* textures;
	Sprite sprite;

public:
	Upgrade(dArr<Texture>&textures
		, Vector2f position,
		int type,
		float duration);
	virtual ~Upgrade();

	inline const int& getType()const { return this->type; }
	inline bool canDelete() { return this->aliveTimer >= this->aliveTimerMax; }

	bool checkCollision(FloatRect rect);

	void Update(const float &dt);
	void Draw(RenderTarget& target);
};

