#pragma once
#include"Player.h"
#include"Enemy.h"

class Game
{
private:
	RenderWindow *window;
	float dtMultiplier;

	//Text
	Font font;
	Text followPlayerText;
	Text staticPlayerText;
	Text enemyText;
	Text gameOverText;
	
	//UI

	RectangleShape playerExpBar; //bar

	//tags
	dArr<TextTag> textTags;

	//Player
	dArr<Player> players;
	int playersAlive;

	//Enemies
	dArr<Enemy> enemies;
	std::vector<Enemy> enemiesSaved;
	float enemySpawnTimer;
	float enemySpawnTimerMax;

	//texture
	std::vector<Texture> textures;
	dArr<Texture> enemyTextures;

	dArr<Texture> lWingTextures;
	dArr<Texture> rWingTextures;
	dArr<Texture> cPitTextures;
	dArr<Texture> auraTextures;


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

