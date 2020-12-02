#pragma once
#include"Player.h"
#include"Enemy.h"

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

	int difficulty;
	float difficultyTimer;

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

	//pickups
	dArr<Pickup> pickups;
	
	//upgrades
	dArr<Upgrade> upgrades;

	//texture
	std::vector<Texture> textures;
	dArr<Texture> enemyTextures;
	dArr<Texture> enemyBulletTextures;

	dArr<Texture> lWingTextures;
	dArr<Texture> rWingTextures;
	dArr<Texture> cPitTextures;
	dArr<Texture> auraTextures;
	
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
	void Update(const float& dt);
	void DrawUI();
	void Draw();
};

