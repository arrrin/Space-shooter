#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include<math.h>
#include<vector>
#include"dArr.h"


using namespace sf;

class TextTag
{
private:
	float dtMultipiler;

	Font* font;
	Text text;

	float speed;
	Vector2f direction;
	float timerMax;
	float timer;
	bool follow;


public:
	TextTag(Font* font, std::string text, Color color,
		Vector2f position, unsigned int size,
		float timerMax
	);
	virtual ~TextTag();

	//accessory
	inline const float& getTimer()const { return this->timer; }
	

	void Update(const float& dt);
	void Draw(RenderTarget& target);
};

