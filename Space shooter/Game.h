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
	bool fullscreen;
	//audio

	bool paused;
	bool isMusicPlay;

	//score function
	Text highScoreText;
	int highScoreSet[5];
	Text highScore1;
	Text highScore2;
	Text highScore3;
	Text highScore4;
	Text highScore5;

	float keyTime;
	float keyTimeMax;
	int saveGameState = 1;

	//upgrades keyTime
	float keyTimeDoubleRay;
	float keyTimeDoubleRayMax;
	float keyTimeTripleRay;
	float keyTimeTripleRayMax;
	float keyTimePiercingShot;
	float keyTimePiercingShotMax;
	float keyTimeShield;
	float keyTimeShieldMax;
	float keyTimeMachineGun;
	float keyTimeMachineGunMax;

	int difficulty;
	float difficultyTimer;
	
	Clock bossTimer;

	//Texts
	Font font;
	Font font1;
	Font font2;
	Font font3;

	Text nameOfGame;
	Text name;
	Text followPlayerText;
	Text staticPlayerText;
	Text enemyText;
	Text gameOverText;
	Text scoreText;
	Text controlsText;
	Text playerStatsText;
		//upgrade Texts
		Text doubleRayText;
		Text tripleRayText;
		Text piercingShotText;
		Text shieldText;
	
	//UI
	RectangleShape shieldBar;
	RectangleShape doubleRayBar;
	RectangleShape tripleRayBar;
	RectangleShape piercingShotBar;
	RectangleShape shieldOutline;
	RectangleShape doubleRayOutline;
	RectangleShape tripleRayOutline;
	RectangleShape piercingOutline;
	//bar
	RectangleShape playerExpBar; 
	RectangleShape playerHpbar;

	RectangleShape playerStatsTextBack;

	//sound
	Music music;
	Sound killEnemy;
	SoundBuffer killEnemyBuffer;


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

	//particle
	dArr<Particle> particles;

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
	void initSound();
	void initFont();
	void initUI();
	void initHighScoreText();
	void initUpgradesUI();
	void toggleFullscreen();
	void UpdateUIPlayer(int index);
	void UpdateUIEnemy(int index);
	void updateKeytime(const float&dt);
	void pauseUpdate();
	void updateWhilePause(const float& dt);
	void enemyTimerUpdate(const float&dt);
	void upgradesTimerUpdate(const float&dt);
	void difficultyUpdate(const float& dt);
	void scoreUpdate(const float& dt);
	void highScore();
	void enemiesSpawnUpdate(const float &dt);
	void playerUpdate(const float& dt);
	void enemyBulletUpdate(const float &dt);
	void enemiesUpdate(const float& dt);
	void particleUpdate(const float& dt);
	void textTagsUpdate(const float& dt);
	void upgradesUpdate(const float& dt);
	void pickupsUpdate(const float&dt);
	void gameOver();
	void Restart();
	void Update(const float& dt);
	void DrawUI();
	void Draw();
};

