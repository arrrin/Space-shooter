#pragma once
#include"Bullet.h"
class Player
{
private:
	unsigned playerNr;

	float dtMultipiler;

	Vector2f playerCenter;

	float shootTimer;
	float shootTimerMax;
	float damageTimer;
	float damageTimerMax;

	Sprite sprite;	
	RectangleShape hitbox;

	//accessory
	Sprite mainGunSprite;
	std::vector<Bullet> bullets;
	Texture* laserTexture;
	Texture* missile01Texture;
	Texture* missile02Texture;
	
	int controls[5];
	Vector2f currentVelo;
	float maxVelo;
	float acceleration;
	Vector2f direction;
	float stabilizerForce;

	int level;
	int exp;
	int expNext;
	int statPoint;

	int cooling; //  hp
	int plating; // armor
	int power; 
	int wiring; // speed

	int hp;
	int hpMax;

	int damage;
	int damageMax;

	int score;

	int currentWeapons;

	//Upgrades
	int mainGunLevel;
	bool dualMissiles01;
	bool dualMissiles02;

public:
	Player(std::vector<Texture> &textures,
		int up = 22 , int down=18,
		int left=0,int right =3 ,
		int shoot=57);
	virtual ~Player();

	//accessory
	inline std::vector<Bullet>& getBullets() { return this->bullets; }
	inline const Vector2f& getPosition()const { return this->sprite.getPosition(); }
	inline const String getHpAsString()const { return std::to_string(this->hp) + "/" + std::to_string(this->hpMax); }
	int getDamage()const;
	inline FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline const int& getHP()const { return this->hp; }
	inline const int& getHPMax()const { return this->hpMax; }
	inline void takeDamage(int damage) { this->hp -= damage; }
	inline bool isAlive()const {return this->hp > 0; }
	inline const int& getPlayernr()const { return this->playerNr; }
	inline const int& getLevel()const { return this->level; }
	inline const int& getExp()const { return this->exp; }
	inline const int& getExpNext()const { return this->expNext; }
	inline bool gainExp(int exp) { this->exp += exp; return this->UpdateLeveling(); }

	//function
	bool UpdateLeveling();
	void UpdateAccessories(const float& dt);
	void Movement(const float& dt);
	void Combat(const float& dt);
	void Update(Vector2u windowBounds, const float& dt);
	void Draw(RenderTarget& target);

	//static
	static unsigned players;
};

