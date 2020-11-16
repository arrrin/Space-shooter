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
	std::vector<TextTag> textTags;

	//Player
	std::vector<Player> players;
	int playersAlive;

	//Enemies
	std::vector<Enemy> enemies;
	std::vector<Enemy> enemiesSaved;
	float enemySpawnTimer;
	float enemySpawnTimerMax;

	//texture
	std::vector<Texture> textures;

public:
	Game(RenderWindow* window);
	virtual ~Game();
	//
	inline RenderWindow& getWindow() { return *this->window; }


	//function
	void initUI();
	void UpdateUIPlayer(int index);
	void UpdateUIEnemy(int index);
	void Update(const float& dt);
	void DrawUI();
	void Draw();
};

