#pragma once
#include"TextTag.h"

class Bullet
{
private:
	float dtMultiplier;
	Texture* texture;
	Sprite sprite;


	
	Vector2f currentVelocity;
	float MaxVelo;
	float acceleration;
	Vector2f direction;

public:
	Bullet(Texture* texture, Vector2f position,Vector2f scale,
		float MaxVelo, float initialVelo,
		Vector2f direction, float acceleration);
	virtual ~Bullet();

	inline FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline const Vector2f& getPosition()const { return this->sprite.getPosition(); }
	inline void setPosition(Vector2f position) { this->sprite.setPosition(position); }


	void movement(const float& dt);

	void Update(const float& dt);
	void Draw(RenderTarget &target);
};

