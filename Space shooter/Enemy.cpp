#include "Enemy.h"

enum eTypes { MOVELEFT = 0, FOLLOW, FOLLOWFAST, FOLLOWSHOOT };
Enemy::Enemy(dArr<Texture>& textures,Vector2u windowBounds,
	Vector2f position,
	Vector2f direction, Vector2f scale,
	int type, int hpMax, 
	int damageMax, int damageMin
,int playerFollow)
{
	this->textures = &textures;
	this->type = type;
	this->sprite.setTexture((*this->textures)[this->type]);
	this->sprite.setScale(scale);
	this->windowBounds = windowBounds;
	
	this->dtMultipiler = 60;

	this->direction = direction;
	this->sprite.setPosition(windowBounds.x, (rand() % windowBounds.y) - this->sprite.getGlobalBounds().height);

	this->playerFollow = playerFollow;

	this->hpMax = hpMax;
	this->hp = this->hpMax;

	this->damageMax = damageMax;
	this->damageMin = damageMin;

	this->damagetimerMax = 6.f;
	this-> damageTimer =0;
}

Enemy::~Enemy()
{

}

void Enemy::takeDamage(int damage)
{
	this->hp -= damage;

	this->damageTimer = this->damagetimerMax;

	if (this->hp <= 0)
	{
		this->hp = 0;
	}
}

void Enemy::Update(const float& dt,Vector2f playerPosition)
{
	Vector2f normalizedDir;

	switch (this->type)
	{
	case MOVELEFT:
		this->sprite.move(this->direction.x *10.f *dt *this->dtMultipiler,
			this->direction.y* 10.0f * dt * this->dtMultipiler);
		break;

	case FOLLOW:

		if (this->sprite.getPosition().x > playerPosition.x) 
		{
			direction.x = playerPosition.x - this->sprite.getPosition().x;
			direction.y = playerPosition.y - this->sprite.getPosition().y;
		}
		normalizedDir = normalize(direction, vectorLength(direction));

		if (normalizedDir.y > 0.3)
		{
			normalizedDir.y = 0.3f;
		}
		else if (normalizedDir.y < -0.3f)
		{
			normalizedDir.y = -0.3f;
		}

		if (normalizedDir.x > -0.7)
		{
			normalizedDir.x = -0.7f;
		}

		this->sprite.setRotation(atan2(normalizedDir.y, normalizedDir.x) *180 / 3.1415926f +180);

		this->sprite.move(normalizedDir.x * 3.0f * dt * dtMultipiler,
			normalizedDir.y * 3.f * dt * dtMultipiler);
		break;

	default:
		break;
	}
	
	 // attack timer 
	
	if (this->damageTimer > 0.f)
	{
		this->damageTimer -= 1.f * dt * dtMultipiler;

		this->sprite.setColor(Color::Red);

		//attack push
		this->sprite.move(9.f * this->damageTimer *dt*dtMultipiler, 0.f);
	}
	else
		this->sprite.setColor(Color::White);

}

void Enemy::draw(RenderTarget& target)
{
	target.draw(this->sprite);
}

