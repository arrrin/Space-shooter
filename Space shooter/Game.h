#pragma once
#include"Player.h"
#include"Enemy.h"
#include"Boss.h"
class Game
{
private:

	RenderWindow *window;
	float dtMultiplier;
	unsigned score;
	unsigned scoreMultiplier;
	float multiplierTimerMax;
	float multiplierTimer;
	int multiplierAdder;
	int multiplierAdderMax;
	
	bool paused;

	float keyTime;
	float keyTimeMax;

	//upgrades keyTime
	float keyTimeDoubleRay;
	float keyTimeDoubleRayMax;
	float keyTimeTripleRay;
	float keyTimeTripleRayMax;
	float keyTimePiercingShot;
	float keyTimePiercingShotMax;

	int difficulty;
	float difficultyTimer;
	
	Clock bossTimer;

	//Text
	Font font;
	Text followPlayerText;
	Text staticPlayerText;
	Text enemyText;
	Text gameOverText;
	Text scoreText;
	Text controlsText;
	
	//UI

	RectangleShape playerExpBar; //bar

	//tags
	dArr<TextTag> textTags;

	//Player
	dArr<Player> players;
	int playersAlive;

	//map
	Sprite map;
	Texture mapTexture;

	//Enemies
	dArr<Enemy> enemies;
	std::vector<Enemy> enemiesSaved;
	float enemySpawnTimer;
	float enemySpawnTimerMax;

	//bosses
	bool bossEncounter;
	dArr<Boss> bosses;

	//pickups
	dArr<Pickup> pickups;
	
	//upgrades
	dArr<Upgrade> upgrades;

	//player texture
	std::vector<Texture> textures;
		//wing texture
		dArr<Texture> lWingTextures;
		dArr<Texture> rWingTextures;
		dArr<Texture> cPitTextures;
		dArr<Texture> auraTextures;
		
	//enemy textures
	dArr<Texture> enemyTextures;
	dArr<Texture> enemyBulletTextures;

	//Boss textures
	dArr<Texture> bossBodyTextures;
	dArr<Texture> bossGunTextures;
	dArr<Texture> bossBulletTextures;

	//other textures
	dArr<Texture> pickupTextures;
	dArr<Texture> upgradeTextures;


public:
	Game(RenderWindow* window);
	virtual ~Game();
	//
	inline RenderWindow& getWindow() { return *this->window; }


	//function
	void initTextures();
	void initUI();
	void UpdateUIPlayer(int index);
	void UpdateUIEnemy(int index);
	void updateKeytime(const float&dt);
	void pauseUpdate();
	void updateWhilePause(const float& dt);
	void enemyTimerUpdate(const float&dt);
	void upgradesTimerUpdate(const float&dt);
	void difficultyUpdate(const float& dt);
	void scoreUpdate(const float& dt);
	void enemiesSpawnUpdate(const float &dt);
	void playerUpdate(const float& dt);
	void enemiesUpdate(const float& dt);
	void textTagsUpdate(const float& dt);
	void upgradesUpdate(const float& dt);
	void pickupsUpdate(const float&dt);
	void gameOver();
	void Restart();
	void Update(const float& dt);
	void DrawUI();
	void Draw();
};

