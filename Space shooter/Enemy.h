#pragma once
#include"Bullet.h"

class Enemy
{
private:

	float dtMultipiler;

	dArr<Texture>* textures;
	Sprite sprite;
	Vector2u windowBounds;
	Vector2f moveDirection;
	Vector2f normalizedMoveDir;
	Vector2f lookDirection;
	Vector2f normalizedLookDir;
	float maxVelocity;

	dArr<Texture>* bulletTextures;
	dArr<Bullet> bullets;


	float damagetimerMax;
	float damageTimer;

	float shootTimerMax;
	float shootTimer;

	int type;
	int hp;
	int hpMax;
	int damageMin;
	int damageMax;

	int playerFollow;

public:
	Enemy(
		dArr<Texture>&textures,
		dArr<Texture>& bulletTextures,
		Vector2u windowBounds,
		Vector2f position, 
		Vector2f direction
		,int type,
		int scalar,
		int playerFollow);
	virtual ~Enemy();

	inline const int getDamage()const { return rand() % this->damageMax + this->damageMin; }
	inline const int getHP()const { return this->hp; }
	inline const int getHPMax()const { return this->hpMax; }
	inline const bool isDead()const { return this->hp <= 0; }
	inline  FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline  Vector2f getPosition()const { return this->sprite.getPosition(); }
	inline const int& getPlayerFollow()const { return this-> playerFollow; }
	inline dArr<Bullet>& getBullets() { return this->bullets; }

	//function
	void collision();
	void takeDamage(int damage);
	void Update(const float& dt,Vector2f playerPosition);
	void draw(RenderTarget &target);


	static dArr<Bullet> enemyBullets;

	float vectorLength(Vector2f v)
	{
		return sqrt(pow(v.x, 2) + pow(v.y, 2));
	}
	Vector2f normalize(Vector2f v, float length)
	{
		if (length == 0)
			return Vector2f(0.f, 0.f);
		else
			return v / length;
	}
};

