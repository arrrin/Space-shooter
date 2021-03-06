#include "Enemy.h"

enum eTypes { MOVELEFT = 0, FOLLOW,MOVELEFTSHOOT,MOVELEFTSHOOT1, FOLLOWSHOOT };
enum bulletType { BULLET=0, MISSILE};
//
dArr<Bullet> Enemy::enemyBullets;

Enemy::Enemy(dArr<Texture>& textures,
	dArr<Texture>& bulletTextures,
	Vector2u windowBounds,
	Vector2f position,
	Vector2f moveDirection,
	int type, 
	int scalar
	,int playerFollow)
{
	this->dtMultipiler = 60;

	this->textures = &textures;
	this->bulletTextures = &bulletTextures;
	this->type = type;

	if (this->type >= (*this->textures).size())
	{
		std::cout << "NO TEXTURE FOR THIS TYPE!!" << "\n";
	}
	else
		this->sprite.setTexture((*this->textures)[this->type]);

		this->sprite.setOrigin
		(
			this->sprite.getGlobalBounds().width / 2,
			this->sprite.getGlobalBounds().height / 2
		);

	this->windowBounds = windowBounds;
	
	this->nrOfBullet = 0;

	this->damagetimerMax = 6.f;
	this->damageTimer = 0;
	
	this->shootTimerMax = 100.f;
	this->shootTimer = this->shootTimerMax;

	this->moveDirection = moveDirection;

	switch (this->type)
	{
		
	case MOVELEFT:
	
		this->sprite.setScale(Vector2f(0.125f,0.125f));

		this->hpMax = (rand() % 5 +1 ) * scalar;
		this->hp = this->hpMax;

		this->damageMax = (rand() % 3 + (2 * scalar)) * scalar;
		this->damageMin = (rand() % 1 + (1 * scalar)) * scalar; ;

		this->maxVelocity = rand() % 20 + 7 ;

		break;

	case FOLLOW:

		this->sprite.setScale(Vector2f(0.1f, 0.1f));

		this->hpMax = (rand() % 3 + 1) * scalar;
		this->hp = this->hpMax;

		this->damageMax = (rand() % 2 + (1 * scalar)) * scalar;
		this->damageMin = (rand() % 1 + (1 * scalar)) * scalar;

		this->maxVelocity = rand() % 7 + 5;

		break;

	case MOVELEFTSHOOT:

		this->sprite.setScale(Vector2f(0.13f, 0.13f));

		this->hpMax = (rand() % 3 + (1 * scalar)) * scalar;
		this->hp = this->hpMax;

		this->damageMax = (rand() % 2 + (1 * scalar)) * scalar;
		this->damageMin = (rand() % 1 + (1 * scalar)) * scalar;

		this->maxVelocity = rand() % 10  + 5;

		this->nrOfBullet = 5;

		this->shootTimerMax = 80.f;
		this->shootTimer = 0.f;

		break;
	case MOVELEFTSHOOT1:

		this->sprite.setScale(Vector2f(0.11f, 0.11f));
		this->sprite.setRotation(270);

		this->hpMax = (rand() % 3 + (1 * scalar)) * scalar;
		this->hp = this->hpMax;

		this->damageMax = (rand() % 2 + (1 * scalar)) * scalar;
		this->damageMin = (rand() % 1 + (1 * scalar)) * scalar;

		this->maxVelocity = rand() % 10 + 5;
		this->nrOfBullet = 3;

		this->shootTimerMax = 8.f;
		this->shootTimer = 0.f;

		break;

	default:
		this->hpMax = (rand() % 3 + 1) * scalar;
		this->hp = this->hpMax;

		this->damageMax = (rand() % 2 + 1) * scalar;
		this->damageMin = (rand() % 1 + 1) * scalar;

		this->maxVelocity = rand() % 10 + 3;
		break;
	}

	this->maxVelocity = maxVelocity;

	this->playerFollow = playerFollow;

	this->sprite.setPosition(windowBounds.x, (rand() % windowBounds.y) - this->sprite.getGlobalBounds().height);
	
}

Enemy::~Enemy()
{

}

void Enemy::collision()
{
	this->damageTimer = this->damagetimerMax;
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

	switch (this->type)
	{
	case MOVELEFT:
		this->sprite.move(	this->moveDirection.x *this->maxVelocity *dt *this->dtMultipiler,
							this->moveDirection.y* this->maxVelocity * dt * this->dtMultipiler);

		this->normalizedMoveDir = normalize(this->moveDirection, vectorLength(this->moveDirection));
		break;

	case FOLLOW:

		if (this->sprite.getPosition().x > playerPosition.x) 
		{
			this->moveDirection.x = playerPosition.x - this->sprite.getPosition().x;
			this->moveDirection.y = playerPosition.y - this->sprite.getPosition().y;
		}

		this->normalizedMoveDir = normalize(this->moveDirection, vectorLength(this->moveDirection));

		if (this->normalizedMoveDir.y > 0.3)
		{
			this->normalizedMoveDir.y = 0.3f;
		}
		else if (this->normalizedMoveDir.y < -0.3f)
		{
			this->normalizedMoveDir.y = -0.3f;
		}

		if (this->normalizedMoveDir.x > -0.7)
		{
			this->normalizedMoveDir.x = -0.7f;
		}

		this->sprite.setRotation(atan2(this->normalizedMoveDir.y, this->normalizedMoveDir.x) *180 / 3.1415926f +180);

		this->sprite.move(	this->normalizedMoveDir.x * this->maxVelocity * dt * dtMultipiler,
							this->normalizedMoveDir.y * this->maxVelocity * dt * dtMultipiler);
		break;

	case MOVELEFTSHOOT:
		this->shootTimerMax = 80.f;
		if (shootTimer < this->shootTimerMax)
			this->shootTimer += 1.f * dt * this->dtMultipiler;


	
		this->lookDirection.x = playerPosition.x - this->sprite.getPosition().x;
		this->lookDirection.y = playerPosition.y - this->sprite.getPosition().y;

		this->normalizedLookDir = normalize(this->lookDirection, vectorLength(this->lookDirection));

		this->sprite.setRotation(atan2(this->normalizedLookDir.y, this->normalizedLookDir.x)*180 /3.1415926+180);

		this->sprite.move(	this->moveDirection.x * this->maxVelocity * dt * this->dtMultipiler,
							this->moveDirection.y * this->maxVelocity * dt * this->dtMultipiler);

		this->normalizedMoveDir = normalize(this->moveDirection, vectorLength(this->moveDirection));

		//enemy shoot
		if (this->shootTimer >= this->shootTimerMax && this->nrOfBullet>0)
		{
			this->enemyBullets.add(
				Bullet(&(*this->bulletTextures)[BULLET],
					Vector2f(this->sprite.getPosition()),
					Vector2f(0.2f, .2f),
					5.f, 2.0f,
					this->normalizedLookDir,
					.5f, 
					this->getDamage()));
			this->shootTimer = 0.f;
			this->nrOfBullet--;
		}

		break;
	case MOVELEFTSHOOT1:
		this->shootTimerMax = 8.f;
		if (shootTimer < this->shootTimerMax)
			this->shootTimer += 1.f * dt * this->dtMultipiler;

		this->sprite.move(this->moveDirection.x * this->maxVelocity * dt * this->dtMultipiler,
			this->moveDirection.y * this->maxVelocity * dt * this->dtMultipiler);

		this->normalizedMoveDir = normalize(this->moveDirection, vectorLength(this->moveDirection));
		if (this->shootTimer >= this->shootTimerMax && this->nrOfBullet > 0)
		{
			this->enemyBullets.add(
				Bullet(&(*this->bulletTextures)[BULLET],
					Vector2f(this->sprite.getPosition()),
					Vector2f(0.2f, .2f),
					5.f, 2.0f,
					Vector2f(-1.f,0.f),
					.5f,
					this->getDamage()));
			this->shootTimer = 0.f;
			this->nrOfBullet--;
		}
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
		this->sprite.move(	9.f * -this->normalizedMoveDir.x* this->damageTimer *dt*dtMultipiler,
							9.f * -this->normalizedMoveDir.y * this->damageTimer * dt * dtMultipiler);
	}
	else
		this->sprite.setColor(Color::White);	

}

void Enemy::draw(RenderTarget& target)
{
	target.draw(this->sprite);

	for (size_t i = 0; i < this->bullets.size(); i++)
	{
		this->bullets[i].Draw(target);
	}
}

