#pragma once
#include"Bullet.h"
class Player
{
private:
	unsigned playerNr;

	float dtMultipiler;
	float keyTimeMax;
	float keyTime;

	Vector2f playerCenter;

	float shootTimer;
	float shootTimerMax;
	float damageTimer;
	float damageTimerMax;

	Sprite sprite;	
	RectangleShape hitbox;

	//accessory
	Sprite mainGunSprite;
	dArr<Bullet> bullets;
	Texture* laserTexture;
	Texture* missile01Texture;
	Texture* missile02Texture;

	dArr<Texture>* lWingTextures;
	dArr<Texture>* rWingTextures;
	dArr<Texture>* cPitTextures;
	dArr<Texture>* auraTextures;
	Sprite lWing;
	Sprite rWing;
	Sprite cPit;
	Sprite aura;

	//sound
	Sound laserSound;
	SoundBuffer laserSoundBuffer;


	int lWingSelect;
	int rWingSelect;
	int cPitSelect;
	int auraSelect;

	int controls[5];
	Vector2f currentVelo;
	float maxVelo;
	float acceleration;
	Vector2f direction;
	float stabilizerForce;
	Vector2f normDir;
	
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
	bool piercingShot;
	bool shield;
	bool dualMissiles01;
	bool dualMissiles02;
	bool machineGun;


public:
	Player(std::vector<Texture> &textures,
		dArr<Texture>& lWingTextures,
		dArr<Texture>& rWingTextures,
		dArr<Texture>& cPitTextures,
		dArr<Texture>& auraTextures	,
		int up = 22, int down = 18,
		int left = 0, int right = 3,
		int shoot = 57 );
	virtual ~Player();

	//accessory
	Bullet& getBullets(unsigned index );
	void removeBullet(unsigned index);
	inline const int getBulletsSize()const { return this->bullets.size(); }
	inline const Vector2f& getPosition()const { return this->sprite.getPosition(); }
	inline const String getHpAsString()const { return std::to_string(this->hp) + "/" + std::to_string(this->hpMax); }
	int getDamage()const;
	inline FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline const int& getHP()const { return this->hp; }
	inline const int& getHPMax()const { return this->hpMax; }
	void takeDamage(int damage);
	inline bool isAlive()const {return this->hp > 0; }
	inline const int& getPlayernr()const { return this->playerNr; }
	inline const int& getLevel()const { return this->level; }
	inline const int& getExp()const { return this->exp; }
	inline const int& getExpNext()const { return this->expNext; }
	inline bool gainExp(int exp) { this->exp += exp; return this->UpdateLeveling(); }
	inline void gainScore(int score) { this->score += score; }
	inline const int getScore()const { return this->score; }
	inline bool isDamagedCooldown() { return this->damageTimer <this->damageTimerMax; }
	void gainHP(int hp);
	void setGunlevel(int gunLevel);
	inline void enablePiercingShot(){ this->piercingShot = true; }
	inline void disablePiercingShot() { this->piercingShot =false; }
	inline void enableDualMissile01() { this->dualMissiles01 = true; }
	inline void enableDualMissile02() { this->dualMissiles02 = true; }
	inline void enableShield() { this->shield = true; }
	inline void disableShield() { this->shield = false; }
	inline void upgradeHP() { 
		int temp = rand() % level +2;
		this->hpMax += temp; 
		this->hp += temp;
				}
	inline bool getPiercingShot()const { return this->piercingShot; }
	inline bool getShield()const { return this->shield; }
	inline const int& getGunLevel()const { return this->mainGunLevel; }

	inline const float getShootTimerMax()const { return this->shootTimerMax; }
	inline const float getShootTimer()const { return this->shootTimer; }
	inline void  enableMachineGun() { this->machineGun = true; }
	inline void  disableMachineGun() { this->machineGun = false; }
	inline bool getMachineGun()const { return this->machineGun; }

	inline void addStatPoint() { this->statPoint++; }
	void addStatPointRandom();
	
	
	bool playerShowStatsIsPressed();
	std::string getStatsAsString()const;

	//function
	void initSound();
	void Reset();
	bool UpdateLeveling();
	void UpdateStats();
	void changeAccessories(const float& dt);
	void UpdateAccessories(const float& dt);
	void Movement(Vector2u windowBound,const float& dt);
	void Combat(const float& dt);
	void Update(Vector2u windowBounds, const float& dt);
	void Draw(RenderTarget& target);

	//static
	static unsigned players;

	//static textures
	static dArr<Texture> mainGunTextures;


	//normal func
	float vectorLength(Vector2f v)
	{
		return sqrt(pow(v.x, 2) + pow(v.y, 2));
	}
	Vector2f normalize(Vector2f v, float length)
	{
		if (length == 0)
			return Vector2f(0.f,0.f);
		else
			return v / length;
	}
};

