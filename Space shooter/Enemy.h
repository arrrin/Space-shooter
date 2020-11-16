#pragma once
#include"Bullet.h"

class Enemy
{
private:
	dArr<Texture>* textures;
	Sprite sprite;
	Vector2u windowBounds;
	Vector2f direction;

	float dtMultipiler ;
	
	float damagetimerMax;
	float damageTimer;

	int type;
	int hp;
	int hpMax;
	int damageMin;
	int damageMax;

	int playerFollow;

public:
	Enemy(dArr<Texture>&textures,Vector2u windowBounds,
		Vector2f position, 
		Vector2f direction,Vector2f scale,
		int type,int hpMax, 
		int damageMax, int damageMin,
		int playerFollow);
	virtual ~Enemy();

	inline const int getDamage()const { return rand() % this->damageMax + this->damageMin; }
	inline const int getHP()const { return this->hp; }
	inline const int getHPMax()const { return this->hpMax; }
	inline const bool isDead()const { return this->hp <= 0; }
	inline  FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline  Vector2f getPosition()const { return this->sprite.getPosition(); }
	inline const int& getPlayerFollow()const { return this-> playerFollow; }


	//function
	void takeDamage(int damage);
	void Update(const float& dt,Vector2f playerPosition);
	void draw(RenderTarget &target);


	float vectorLength(Vector2f v)
	{
		return sqrt(pow(v.x, 2) + pow(v.y, 2));
	}
	Vector2f normalize(Vector2f v, float length)
	{
		return v / length;
	}
};

