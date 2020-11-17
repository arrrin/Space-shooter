#pragma once
#include"Upgrade.h"

class Pickup
{
private:
	float dtMultipiler;
	int type;

	dArr<Texture>* textures;
	Sprite sprite;

	float aliveTimerMax;
	float aliveTimer;

public:
	Pickup(
		dArr<Texture> &textures,
		Vector2f position,
		int type,
		float duration
	);
	virtual ~Pickup();

	inline const int& getType()const { return this->type; }
	inline const bool canDelete()const { return this->aliveTimer >= aliveTimerMax; }

	bool checkColllision( FloatRect rect);

	void Update(const float& dt);
	void Draw(RenderTarget& target);
};

