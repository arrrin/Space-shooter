#include "Enemy.h"

enum eTypes { MOVELEFT = 0, FOLLOW, FOLLOWFAST, FOLLOWSHOOT };
Enemy::Enemy(Texture* texture,Vector2u windowBounds, 
	Vector2f position,
	Vector2f direction, Vector2f scale,
	int type, int hpMax, 
	int damageMax, int damageMin)
{
	this->texture = texture;
	this->sprite.setTexture(*this->texture);
	this->sprite.setScale(scale);
	this->windowBounds = windowBounds;
	
	this->dtMultipiler = 60;

	this->direction = direction;
	this->sprite.setPosition(windowBounds.x, (rand() % windowBounds.y) - this->sprite.getGlobalBounds().height);

	this->type = type;

	this->hpMax = hpMax;
	this->hp = this->hpMax;

	this->damageMax = damageMax;
	this->damageMin = damageMin;
}

Enemy::~Enemy()
{
}

void Enemy::takeDamage(int damage)
{
	this->hp -= damage;
	if (this->hp <= 0)
	{
		this->hp = 0;
	}
}

void Enemy::Update(const float& dt)
{
	switch (this->type)
	{
	case 0:
		this->sprite.move(this->direction.x *10.f *dt *this->dtMultipiler,
			this->direction.y* 10.0f * dt * this->dtMultipiler);
		break;
	default:
		break;
	}
}

void Enemy::draw(RenderTarget& target)
{
	target.draw(this->sprite);
}

