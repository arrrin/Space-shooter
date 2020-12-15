#include "Bullet.h"

Bullet::Bullet(Texture *texture,Vector2f position,
	Vector2f scale,
	float MaxVelo , float initialVelo, Vector2f direction, float acceleration,int damage )
{
	this->dtMultiplier = 60.f;

	this->texture = texture;
	this->sprite.setTexture(*this->texture);
	this->MaxVelo = MaxVelo;
	this->acceleration = acceleration;
	this->direction = direction;
	this->currentVelocity = Vector2f(
		initialVelo * this->direction.x,
		initialVelo * this->direction.y
		);

	this->damage = damage;

	this->sprite.setOrigin(
		this->sprite.getGlobalBounds().width / 2,
		this->sprite.getGlobalBounds().height / 2
	);

	this->sprite.setScale(scale);
	this->sprite.setPosition(position);
	this->sprite.setRotation(atan2(this->direction.y, this->direction.x) * 180 /3.1415926 +180);
}

Bullet::~Bullet()
{

}

void Bullet::movement(const float& dt)
{
	if (this->acceleration > 0.f)
	{
		if (this->currentVelocity.x < this->MaxVelo)
			this->currentVelocity.x += this->acceleration * this->direction.x*
			dt* this->dtMultiplier;
		if (this->currentVelocity.y < this->MaxVelo)
			this->currentVelocity.y += this->acceleration * this->direction.y*
			dt * this->dtMultiplier;
	}
	else
	{
		this->currentVelocity = Vector2f(
			this->MaxVelo*this->direction.x,
			this->MaxVelo*this->direction.y);
	}
	this->sprite.move(this->currentVelocity*dt * this->dtMultiplier);
}

void Bullet::Update(const float &dt)
{
	this->movement(dt);
}

void Bullet::Draw(RenderTarget& target)
{
	target.draw(this->sprite);
}
