#include "Pickup.h"

Pickup::Pickup(
	dArr<Texture> &textures,
	Vector2f position,
	int type,
	float duration)
{
	this->dtMultipiler = 60.f;

	this->textures = &textures;

	this->type = type;

	if (this->type < (*this->textures).size())
		this->sprite.setTexture((*this->textures)[this->type]);

	this->sprite.setOrigin(
		this->sprite.getGlobalBounds().width / 2,
		this->sprite.getGlobalBounds().height / 2
	);

	this->sprite.setPosition(position);

	this->sprite.setColor(Color(255, 255, 255, 200));

	this->aliveTimerMax = duration;
	this->aliveTimer = 0.f;

}

Pickup::~Pickup()
{
}

bool Pickup::checkColllision(FloatRect rect)
{	
	if (this->sprite.getGlobalBounds().intersects(rect))
		return true;


	return false;
}

void Pickup::Update(const float& dt)
{
	if (this->aliveTimer < this->aliveTimerMax)
		this->aliveTimer += 1.f * dt * this->dtMultipiler;

	this->sprite.rotate(5.f * dt * this->dtMultipiler);
}

void Pickup::Draw(RenderTarget& target)
{
	target.draw(this->sprite);
}
