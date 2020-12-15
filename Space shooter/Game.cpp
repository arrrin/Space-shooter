#include "Game.h"

enum textures { player = 0, laser01,missile01, mainGun01  };

Game::Game(RenderWindow *window)
{
	this->window = window;
	this->window->setFramerateLimit(120);
	this->dtMultiplier = 60;
	this->scoreMultiplier = 1;
	this->score = 0;
	this->multiplierAdderMax = 10;
	this->multiplierAdder = 0;
	this->multiplierTimerMax =200.f;
	this->multiplierTimer = this->multiplierTimerMax;

	//Upgrades counter
	this->keyTimeDoubleRayMax = 1201.f;
	this->keyTimeDoubleRay = this->keyTimeDoubleRayMax;

	this->keyTimeTripleRayMax =701.f;
	this->keyTimeTripleRay = this->keyTimeTripleRayMax;

	this->keyTimePiercingShotMax =601.f;
	this->keyTimePiercingShot = this->keyTimePiercingShotMax;

	this->keyTimeShieldMax = 401.f;
	this->keyTimeShield = this->keyTimeShieldMax;

	this->keyTimeMachineGunMax = 501.f;
	this->keyTimeMachineGun = this->keyTimeMachineGunMax;

	this->keyTimeMax = 10.f;
	this->keyTime = this->keyTimeMax;
	this->paused = true;
	this->isMusicPlay = true;
	this->canHighScoreGet = false;
	
	/*this->canGetHighScore = false;*/

	//init texture
	this->initTextures();
	
	//init player
	this->players.add(Player(this->textures , 
		this->lWingTextures,this->rWingTextures,
		this->cPitTextures,this->auraTextures));

	/*this->players.add(Player(this->textures,
		this->lWingTextures, this->rWingTextures,
		this->cPitTextures, this->auraTextures,
		Keyboard::Key::Up, Keyboard::Key::Down, 
		Keyboard::Key::Left, Keyboard::Key::Right, Keyboard::Key::Enter ));*/

	this->playersAlive = this->players.size();	

	this->enemySpawnTimerMax = 40;
	this->enemySpawnTimer = this->enemySpawnTimerMax;

	//Init boss
	this->bossEncounter = false;

	this->initUI();
}

Game::~Game()
{

}

void Game::initTextures()
{
	//normal texture
	this->textures.push_back(Texture());
	this->textures[player].loadFromFile("Textures/ship.png");
	this->textures.push_back(Texture());
	this->textures[laser01].loadFromFile("Textures/Guns/rayTex01.png");
	this->textures.push_back(Texture());
	this->textures[missile01].loadFromFile("Textures/Guns/missileTex01.png");
	this->textures.push_back(Texture());
	this->textures[mainGun01].loadFromFile("Textures/Guns/gun01.png");

	//Map texture
	Texture temp;
	this->mapTexture.loadFromFile("Textures/spacemap.png");
	this->map.setTexture(mapTexture);

	//pickup
	temp.loadFromFile("Textures/Pickups/hpSupply.png");
	this->pickupTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Pickups/missileSupply.png");
	this->pickupTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Pickups/missileHSupply.png");
	this->pickupTextures.add(Texture(temp));

	//upgrade
	temp.loadFromFile("Textures/Upgrades/doubleray.png");
	this->upgradeTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/tripleray.png");
	this->upgradeTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/piercingshot.png");
	this->upgradeTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/shield.png");
	this->upgradeTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/healthtank.png");
	this->upgradeTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/statpoint.png");
	this->upgradeTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/powerupRF.png");
	this->upgradeTextures.add(Texture(temp));


	//enemies

	temp.loadFromFile("Textures/enemyMoveLeft.png");
	this->enemyTextures.add(Texture(temp));
	temp.loadFromFile("Textures/enemyFollow.png");
	this->enemyTextures.add(Texture(temp));
	temp.loadFromFile("Textures/enemyMoveLeftShoot.png");
	this->enemyTextures.add(Texture(temp));

	//enemy bullet
	temp.loadFromFile("Textures/Guns/roundBulletRed.png");
	this->enemyBulletTextures.add(Texture(temp));

	//Boss texture
	temp.loadFromFile("Textures/Bosses/Bodies/BossBody01.png");
	this->bossBodyTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Bosses/Guns/BossGun01.png");
	this->bossGunTextures.add(Texture(temp)); 
	temp.loadFromFile("Textures/Bosses/Bullets/BossBullet01.png");
	this->bossBulletTextures.add(Texture(temp));


	//init texture folders accesory
	std::ifstream in;

	//lwings text file
	in.open("Textures/Accessories/leftwings.txt");
	std::string fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			Texture temp;
			temp.loadFromFile(fileName);
			this->lWingTextures.add(Texture(temp));
		}
	}
	in.close();

	//rwings text file
	in.open("Textures/Accessories/rightwings.txt");
	fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			Texture temp;
			temp.loadFromFile(fileName);
			this->rWingTextures.add(Texture(temp));
		}
	}
	in.close();

	//cockpit
	in.open("Textures/Accessories/cockpits.txt");
	fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			Texture temp;
			temp.loadFromFile(fileName);
			this->cPitTextures.add(Texture(temp));
		}
	}
	in.close();

	//aura
	in.open("Textures/Accessories/auras.txt");
	fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			Texture temp;
			temp.loadFromFile(fileName);
			this->auraTextures.add(Texture(temp));
		}
	}
	in.close();

	//Particle
	Particle::initTextures();

}

void Game::initSound()
{
	this->music.openFromFile("Musics/mainGameSong.wav");

	if (!this->music.openFromFile("Musics/mainGameSong.wav"))
	{
		std::cout << "Error Cannot load Musc!!" << "\n";
	}
	//if (isMusicPlay==true)
	//{
	//	music.play();
	//	isMusicPlay = false;
	//	std::cout << "Play Music!!" << "\n";
	//}
	
}

void Game::initFont()
{
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");
	this->font1.loadFromFile("Fonts/ethnocentric.ttf");
	this->font2.loadFromFile("Fonts/airstrikeb3d.ttf");
}

void Game::initUI()
{
	//Font
	this->initFont();

	//Main game name text
	this->nameOfGame.setFont(font2);
	this->nameOfGame.setCharacterSize(80);
	this->nameOfGame.setFillColor(Color::White);
	this->nameOfGame.setString("AIR STRIKE");
	this->nameOfGame.setPosition(1350.f, 5.f);

	//Follow text
	this->followPlayerText.setFont(font);
	this->followPlayerText.setCharacterSize(15);

	//static text
	this->staticPlayerText.setFont(font);
	this->staticPlayerText.setCharacterSize(15);

	//upgrades text
	this->initUpgradesUI();

	//Exp bar
	this->playerExpBar.setSize(Vector2f(90.f, 10.f));
	this->playerExpBar.setFillColor(Color(0.f, 90.f, 200.f, 200.f));

	this->enemyText.setFont(this->font);
	this->enemyText.setCharacterSize(14);

	this->gameOverText.setFont(this->font);
	this->gameOverText.setFillColor(Color::Red);
	this->gameOverText.setCharacterSize(40);
	this->gameOverText.setString("GAME OVER!!");
	this->gameOverText.setPosition( 100.f, this->window->getSize().y /2);

	this->scoreText.setFont(this->font);
	this->scoreText.setFillColor(Color(200,200,200,150));
	this->scoreText.setCharacterSize(30);
	this->scoreText.setString("Score : 0");
	this->scoreText.setPosition(10.f,10.f);

	this->controlsText.setFont(this->font);
	this->controlsText.setCharacterSize(40);
	this->controlsText.setString(
		(std::string("Controls\nW,A,S,D : Movement\nSpace bar : Shoot\nEsc : Pause&Resume\nX : Exit game" 
		)
			));
	this->controlsText.setPosition(10.f, 700.f);

	this->playerStatsText.setFont(font);
	this->playerStatsText.setFillColor(Color::White);
	this->playerStatsText.setCharacterSize(16);
	this->playerStatsText.setString("NONE");

	this->playerStatsTextBack.setFillColor(Color(50, 50, 50, 100));
	this->playerStatsTextBack.setOutlineThickness(1.f);
	this->playerStatsTextBack.setOutlineColor(Color(255, 255, 255, 200));
}

void Game::initUpgradesUI()
{
	//text
	this->doubleRayText.setFont(this->font1);
	this->doubleRayText.setCharacterSize(15);
	this->doubleRayText.setFillColor(Color::Black);
	this->doubleRayText.setString("DOUBLE RAY");
	this->doubleRayText.setPosition(1700.f, 20.f);

	this->tripleRayText.setFont(this->font1);
	this->tripleRayText.setCharacterSize(15);
	this->tripleRayText.setFillColor(Color::Black);
	this->tripleRayText.setString("TRIPLE RAY");
	this->tripleRayText.setPosition(1700.f, 60.f);

	this->piercingShotText.setFont(this->font1);
	this->piercingShotText.setCharacterSize(15);
	this->piercingShotText.setFillColor(Color::Black);
	this->piercingShotText.setString("PIERCING SHOT");
	this->piercingShotText.setPosition(1700.f, 100.f);

	this->shieldText.setFont(this->font1);
	this->shieldText.setCharacterSize(15);
	this->shieldText.setFillColor(Color::Black);
	this->shieldText.setString("INVISIBLE");
	this->shieldText.setPosition(1700.f, 140.f);

	//bar
	this->shieldBar.setSize(Vector2f(200.f, 20.f));
	this->shieldBar.setFillColor(Color::Cyan);
	this->shieldOutline.setSize(Vector2f(200.f, 20.f));
	this->shieldOutline.setOutlineThickness(2);
	this->shieldOutline.setFillColor(Color(100, 100, 100, 0));
	this->shieldOutline.setOutlineColor(Color::White);

	this->doubleRayBar.setSize(Vector2f(200.f, 20.f));
	this->doubleRayBar.setFillColor(Color::Magenta);
	this->doubleRayOutline.setSize(Vector2f(200.f, 20.f));
	this->doubleRayOutline.setOutlineThickness(2);
	this->doubleRayOutline.setFillColor(Color(100, 100, 100, 0));
	this->doubleRayOutline.setOutlineColor(Color::White);

	this->tripleRayBar.setSize(Vector2f(200.f, 20.f));
	this->tripleRayBar.setFillColor(Color::Red);
	this->tripleRayOutline.setSize(Vector2f(200.f, 20.f));
	this->tripleRayOutline.setOutlineThickness(2);
	this->tripleRayOutline.setFillColor(Color(100, 100, 100, 0));
	this->tripleRayOutline.setOutlineColor(Color::White);

	this->piercingShotBar.setSize(Vector2f(200.f, 20.f));
	this->piercingShotBar.setFillColor(Color::Yellow);
	this->piercingOutline.setSize(Vector2f(200.f, 20.f));
	this->piercingOutline.setOutlineThickness(2);
	this->piercingOutline.setFillColor(Color(100, 100, 100, 0));
	this->piercingOutline.setOutlineColor(Color::White);
}

void Game::toggleFullscreen()
{
	if (Keyboard::isKeyPressed(Keyboard::F11) && this->keyTime >= this->keyTimeMax)
	{
		this->keyTime = 0.f;

		if (fullscreen)
		{
			this->fullscreen = false;
			this->window->close();
			this->window->create(sf::VideoMode(1920, 1080), "Wingman Game", Style::Default);
		}
		else
		{
			this->fullscreen = true;
			this->window->close();
			this->window->create(sf::VideoMode(1920, 1080), "Wingman Game", Style::Fullscreen);
		}
	}
}

void Game::UpdateUIPlayer(int index)
{
	if (index >= this->players.size())
	{

	}
	else  //follow text
	{
		this->followPlayerText.setPosition(
			this->players[index].getPosition().x - 10.f,
			this->players[index].getPosition().y - 60.f);

		this->followPlayerText.setString(
			std::to_string(
				this->players[index].getPlayernr()) +
			"						" +
			this->players[index].getHpAsString()
			+ "\n\n\n\n\n\n\n\n\n"
			+ std::to_string(this->players[index].getLevel())
		);
		
	}

	//static

	//bar
	this->playerExpBar.setPosition(
		this->players[index].getPosition().x + 15.f,
		this->players[index].getPosition().y + 115.f);
	this->playerExpBar.setScale(
		static_cast<float>(this->players[index].getExp())/this->players[index].getExpNext()
		, 1.f
	);
		//shield
	this->shieldBar.setPosition(1700.f, 140.f);
	this->shieldOutline.setPosition(1700.f, 140.f);
	this->shieldBar.setScale(
		static_cast<float>(this->keyTimeShield) / this->keyTimeShieldMax, 1.f);
		if ((int)(this->keyTimeShield) % 5 == 0)
		{
			this->shieldBar.setFillColor(Color::White);
		}
		else
		{
			this->shieldBar.setFillColor(Color::Cyan);
		}
		//double ray
	this->doubleRayBar.setPosition(1700.f, 20.f);
	this->doubleRayOutline.setPosition(1700.f, 20.f); 
	this->doubleRayBar.setScale(
		static_cast<float>(this->keyTimeDoubleRay) / this->keyTimeDoubleRayMax, 1.f
		);
		if ((int)(this->keyTimeDoubleRay) % 5 == 0)
		{
			this->doubleRayBar.setFillColor(Color::White);
		}
		else
		{
			this->doubleRayBar.setFillColor(Color::Magenta);
		}
		//triple ray
	this->tripleRayBar.setPosition(1700.f, 60.f);
	this->tripleRayOutline.setPosition(1700.f, 60.f);
	this->tripleRayBar.setScale(
		static_cast<float>(this->keyTimeTripleRay) / this->keyTimeTripleRayMax, 1.f
	);
		if ((int)(this->keyTimeTripleRay) % 5 == 0)
		{
			this->tripleRayBar.setFillColor(Color::White);
		}
		else
		{
			this->tripleRayBar.setFillColor(Color::Red);
		}
		//piercing 
	this->piercingShotBar.setPosition(1700.f, 100.f);
	this->piercingOutline.setPosition(1700.f, 100.f);
	this->piercingShotBar.setScale(
		static_cast<float>(this->keyTimePiercingShot) / this->keyTimePiercingShotMax, 1.f
	);
		if ((int)(this->keyTimePiercingShot) % 5 == 0)
		{
			this->piercingShotBar.setFillColor(Color::White);
		}
		else
		{
			this->piercingShotBar.setFillColor(Color::Yellow);
		}

		//stat text
		//STATS BOX WITH TEXT
		if (this->players[index].playerShowStatsIsPressed())
		{
			this->playerStatsText.setString(this->players[index].getStatsAsString());

			this->playerStatsTextBack.setPosition(
				this->players[index].getPosition().x,
				this->players[index].getPosition().y + 150.f
			);
			this->playerStatsTextBack.setSize(Vector2f(this->playerStatsText.getGlobalBounds().width, this->playerStatsText.getGlobalBounds().height));

			this->playerStatsText.setPosition(this->playerStatsTextBack.getPosition());
		}

}

void Game::UpdateUIEnemy(int index)
{
	this->enemyText.setPosition(
		this->enemies[index].getPosition().x,
		this->enemies[index].getPosition().y- 
		this->enemies[index].getGlobalBounds().height);

	this->enemyText.setString(
		std::to_string(
			this->enemies[index].getHP()) + " / " +
		std::to_string(
			this->enemies[index].getHPMax()));

}

void Game::updateKeytime(const float &dt)
{
	if (this->keyTime < this->keyTimeMax)
		this->keyTime += 1.f * dt * this->dtMultiplier;
}

void Game::pauseUpdate()
{
	if (Keyboard::isKeyPressed(Keyboard::Escape) && this->keyTime >= this->keyTimeMax)
	{
		if (this->paused)
			this->paused = false;
		else
			this->paused = true;

		this->keyTime = 0.f;
	}
}

void Game::updateWhilePause(const float& dt)
{
	if (this->paused)
	{
		for (size_t i = 0; i < this->players.size(); i++)
		{
			if (this->players[i].isAlive())
			{
				this->players[i].changeAccessories(dt);
			}
		}
	}
}

void Game::enemyTimerUpdate(const float& dt)
{
	if (this->enemySpawnTimer < this->enemySpawnTimerMax)
		this->enemySpawnTimer += 1.f * dt * this->dtMultiplier;

	if (this->enemySpawnTimerMax > 5)
		this->difficultyTimer += 1.f * dt * this->dtMultiplier;

}

void Game::upgradesTimerUpdate(const float& dt)
{
	//Double ray keytime
	if (this->keyTimeDoubleRay < this->keyTimeDoubleRayMax)
	{
		this->keyTimeDoubleRay -= 1.f * dt * this->dtMultiplier;
		if (this->keyTimeDoubleRay <=1.f)
		{
			this->keyTimeDoubleRay = this->keyTimeDoubleRayMax;
			for (size_t i = 0; i < players.size(); i++)
			{
				this->players[i].setGunlevel(0);
			}
		}
		std::cout << "double ray time left : " << keyTimeDoubleRay << "\n";
	}
	//triple keytime
	if (this->keyTimeTripleRay < this->keyTimeTripleRayMax)
	{
		this->keyTimeTripleRay -= 1.f * dt * this->dtMultiplier;
		if (this->keyTimeTripleRay <=1.f)
		{
			this->keyTimeTripleRay = this->keyTimeTripleRayMax;
			for (size_t i = 0; i < players.size(); i++)
			{
				this->players[i].setGunlevel(0);
			}
		}
		std::cout << "triple ray time left : " << keyTimeTripleRay << "\n";
	}
	// piercing shot keytime
	if (this->keyTimePiercingShot < this->keyTimePiercingShotMax)
	{
		this->keyTimePiercingShot -= 1.f * dt * this->dtMultiplier;
		if (this->keyTimePiercingShot <=1.f)
		{
			this->keyTimePiercingShot =this->keyTimePiercingShotMax;
			for (size_t i = 0; i < players.size(); i++)
			{
				this->players[i].disablePiercingShot();
			}
		}
		std::cout << "Piercing shot left : " << keyTimePiercingShot << "\n";
	}
	//shield keyTime
	if (this->keyTimeShield < this->keyTimeShieldMax)
	{
		this->keyTimeShield -= 1.f * dt * this->dtMultiplier;
		if (this->keyTimeShield <=1.f)
		{
			this->keyTimeShield = this->keyTimeShieldMax;
			for (size_t i = 0; i < players.size(); i++)
			{
				this->players[i].disableShield();
			}
		}
		std::cout << "Shield left : " << keyTimeShield << "\n";
	}
	//machinegun keytime
	if (this->keyTimeMachineGun < this->keyTimeMachineGun)
	{
		this->keyTimeMachineGun -= 1.f * dt * this->dtMultiplier;
		if (this->keyTimeMachineGun <= 1.f)
		{
			this->keyTimeMachineGun = this->keyTimeMachineGunMax;
			for (size_t i = 0; i < players.size(); i++)
			{
				this->players[i].disableMachineGun();
			}
		}
		std::cout << "Machine gun left : " << keyTimeMachineGun << "\n";
	}
}

void Game::difficultyUpdate(const float& dt)
{
	if ((int)this->difficultyTimer % 1000 == 0 && this->enemySpawnTimer > 10)
	{
		if (this->enemySpawnTimerMax > 10)
		{
			this->enemySpawnTimerMax--;
		}

		this->difficulty++;
		this->difficultyTimer = 1.f;
	}
}

void Game::scoreUpdate(const float& dt)
{
	if (this->multiplierTimer > 0.f)
	{
		this->multiplierTimer -= 1.f * dt * this->dtMultiplier;

		if (this->multiplierTimer <= 0.f)
		{
			this->multiplierTimer = 0.f;
			this->multiplierAdder = 0;
			this->scoreMultiplier = 1;
		}
	}
	if (this->multiplierAdder >= this->multiplierAdderMax)
	{
		this->multiplierAdder = 0;
		this->scoreMultiplier++;
	}
}

void Game::highScore()
{
	//fp = fopen("./highScore.txt", "r");
	//for (int i = 0; i < 1; i++)
	//{
	//	fscanf(fp, "%d", &this->Lscore[i]);
	//	this->userScore.push_back(this->Lscore[i]);
	//}
	//std::sort(this->userScore.begin(), this->userScore.end());
	//fclose(fp);

	//fopen("./highScore.txt", "w");
	//
	//	fprintf(fp, "%d\n", this->userScore[1]);
	//fclose(fp);
}

void Game::enemiesSpawnUpdate(const float& dt)
{
	this->enemyTimerUpdate(dt);

	if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
	{
		this->enemies.add(Enemy(
			this->enemyTextures,
			this->enemyBulletTextures,
			this->window->getSize(),
			Vector2f(0.f, 0.f),
			Vector2f(-1.f, 0.f),
			rand() % 3,
			this->players[0].getLevel(),
			rand() % this->playersAlive)
		);

		this->enemySpawnTimer = 0;
	}
}

void Game::playerUpdate(const float& dt)
{
	for (size_t i = 0; i < this->players.size(); i++)
	{
		if (this->players[i].isAlive())
		{
			//Update player
			this->players[i].Update(this->window->getSize(), dt);

			//bullet update
			for (size_t k = 0; k < this->players[i].getBulletsSize(); k++)
			{
				this->players[i].getBullets(k).Update(dt);

				//enemy collison with bullet
				for (size_t j = 0; j < this->enemies.size(); j++)
				{

					if (this->players[i].getBullets(k).getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
					{

						//not piercing
						if (!this->players[i].getPiercingShot())
						{
							this->players[i].removeBullet(k);
						}
						//pierce
						else
						{
							this->players[i].getBullets(k).setPosition(
								Vector2f(this->enemies[j].getPosition().x + this->enemies[j].getGlobalBounds().width + this->players[i].getBullets(k).getGlobalBounds().width / 2 + 1.f,
									this->players[i].getBullets(k).getPosition().y)
							);
						}

						//take dmg
						int damage = this->players[i].getDamage();
						if (this->enemies[j].getHP() > 0)
						{
							this->enemies[j].takeDamage(damage);

							//Add particles on damage
							int nrOfPart = rand() % 2 + 1;
							for (size_t l = 0; l < nrOfPart; l++)
							{
								this->particles.add(Particle(
									 this->enemies[j].getPosition(),
									0,
									Vector2f(1.f,0.f),
									rand() % 10 + 4,
									rand() % 20,
									30.f,
									Color(255, 255, 255, 255)
								));
							}
							//text when shoot
							this->textTags.add(
								TextTag(
									&this->font, "-" + std::to_string(damage),
									Color::Red,
									Vector2f(this->enemies[j].getPosition().x + 20.f,
										this->enemies[j].getPosition().y - 20.f),
									Vector2f(1.f, 0.f),
									26, 17.f,
									true)
							);
						}

						//enemy dead
						if (this->enemies[j].getHP() <= 0)
						{
							//Particle enemy dead
							int nrOfPart = rand() % 8 + 5;
							for (size_t l = 0; l < nrOfPart; l++)
							{
								this->particles.add(Particle(
									this->enemies[j].getPosition(),
									0,
									Vector2f(1.f,0.f),
									rand() % 12 + 6,
									rand() % 20,
									40.f,
									Color(255, 255, 255, 255)
								));
							}
							//exp gain
							int exp = this->enemies[j].getHPMax() +
								(rand() % this->enemies[j].getHPMax() + 1 *
									this->scoreMultiplier);

							//score & reset timer
							this->multiplierTimer = this->multiplierTimerMax;
							int score = this->enemies[j].getHPMax() * this->scoreMultiplier;
							this->multiplierAdder++;
							this->players[i].gainScore(score);

							//score text tag
							this->textTags.add(
								TextTag(
									&this->font,
									" + " + std::to_string(score) + "	( x"
									+ std::to_string(this->scoreMultiplier) + " )",
									Color::Yellow,
									Vector2f(100.f, 10.f),
									Vector2f(1.f, 0.f),
									30,
									40.f, true)
							);

							int dropChange = rand() & 100 + 1;
							// pick up drop
							if (dropChange < 50)
							{
								dropChange = rand() & 100 + 1;

								if (dropChange < 10)
								{
									this->pickups.add(Pickup(
										this->pickupTextures,
										this->enemies[j].getPosition(),
										0,
										150.f)
									);
								}
							}
							else
							{
								dropChange = rand() % 100 + 1;

								if (dropChange > 80)
								{
									this->upgrades.add(Upgrade(
										this->upgradeTextures,
										this->enemies[j].getPosition(),
										rand() % 6, 150.f)
									);
								}
							}


							if (this->players[i].gainExp(exp))
							{

								//text level up
								this->textTags.add(
									TextTag(
										&this->font, "LEVEL UP!",
										Color::Cyan,
										Vector2f(this->players[i].getPosition().x + 20.f,
											this->players[i].getPosition().y - 20.f),
										Vector2f(0.f, -1.f),
										36,
										40.f, true)
								);
							}
							this->enemies.remove(j);

							//gain exp
							this->textTags.add(
								TextTag(
									&this->font, "+" + std::to_string(exp)
									+ " ( x" + std::to_string(this->scoreMultiplier) + " ) "
									+ " xp",
									Color::Cyan,
									Vector2f(this->players[i].getPosition().x + 20.f,
										this->players[i].getPosition().y - 20.f),
									Vector2f(1.f, 0.f),
									30, 40.f
									, true)
							);



						}

						return;
					}

					//window bound check
					if (this->players[i].getBullets(k).getPosition().x > this->window->getSize().x)
					{
						this->players[i].removeBullet(k);
						return;
					}
				}
			}
		}
		//update score
		this->score = 0;
		for (size_t k = 0; k < players.size(); k++)
		{
			this->score += players[k].getScore();

			/*fp = fopen("./highScore.txt", "r");
			for (size_t i = 0; i < userScore.size(); i++)
			{
				fscanf(fp, "%d", &userScore[i]);
				userScore.push_back(this->Lscore[i]);
			}
			for (size_t i = 0; i < userScore.size(); i++)
			{
				if (this->score >= userScore[i])
				{
					canHighScoreGet = true;
				}
			}
			
			fclose(fp);*/
		}

		this->scoreText.setString(
			"Score: " + std::to_string(this->score) +
			"\nMultipiler : " + std::to_string(this->scoreMultiplier) + "x" +
			"\nMultipliler Timer: " + std::to_string((int)this->multiplierTimer) +
			"\nNew Multipiler: " + std::to_string(this->multiplierAdder)
			+ " / " +
			std::to_string(this->multiplierAdderMax) +
			"\nDifficulty: " +
			std::to_string(this->difficulty)
		);

	}
}

void Game::enemyBulletUpdate(const float& dt)
{
		//Enemy bullet update
		bool bulletRemoved = false;
		bool playerKilled = false;
	
		for (size_t i = 0; i < Enemy::enemyBullets.size() && !bulletRemoved; i++)
		{
			Enemy::enemyBullets[i].Update(dt);
			//Player collision check
			for (size_t k = 0; k < this->players.size() && !playerKilled; k++)
			{
				if (Enemy::enemyBullets[i].getGlobalBounds().intersects(this->players[k].getGlobalBounds())
					&& this->players[k].isAlive())
				{
					int damage = Enemy::enemyBullets[i].getDamage();
					std::cout << "collision check" << std::endl;
					//PLAYER TAKES BULLET DAMAGE
					if (!this->players[k].getShield())
					{
						this->players[k].takeDamage(damage);

						//PLAYER TAKE DAMAGE TAG
						this->textTags.add(
							TextTag(&this->font,
								"-" + std::to_string(damage),
								Color::Red,
								Vector2f(this->players[k].getPosition().x + 20.f,
									this->players[k].getPosition().y - 20.f),
								Vector2f(-1.f, 0.f),
								30,
								30.f,
								true
							)
						);

						//Player death
						if (!this->players[k].isAlive())
							playerKilled = true;
					}
					else
					{
						//Add particles on shielding
						int nrOfPart = rand() % 5 + 3;
						for (size_t l = 0; l < nrOfPart; l++)
						{
							this->particles.add(Particle(
								Enemy::enemyBullets[i].getPosition(),
								0,
								Enemy::enemyBullets[i].getVel(),
								rand() % 20 + 5,
								rand() % 20,
								30.f,
								Color(0, 50, 255, 255)
							));
						}

						//PLAYER SHIELDED TAG
						this->textTags.add(
							TextTag(&this->font,
								"-" + std::to_string(0),
								Color::Cyan,
								Vector2f(this->players[k].getPosition().x + 20.f,
									this->players[k].getPosition().y - 20.f),
								Vector2f(-1.f, 0.f),
								30,
								30.f,
								true
							)
						);
					}

					bulletRemoved = true;
				}

				if (playerKilled)
					this->playersAlive--;
			}
			
			if (bulletRemoved)
				Enemy::enemyBullets.remove(i);
		}
}

void Game::enemiesUpdate(const float& dt)
{
	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].Update(dt, this->players[this->enemies[i].getPlayerFollow()].getPosition());

		//enemy bullet update
		for (size_t k = 0; k < this->enemies[i].getBullets().size(); k++)
		{
			this->enemies[i].getBullets()[k].Update(dt);
		}

		//collision w/ player
		for (size_t k = 0; k < this->players.size(); k++)
		{
			if (!this->players[k].getShield()) {
				if (this->players[k].isAlive())
				{
					if (this->players[k].getGlobalBounds().intersects(this->enemies[i].getGlobalBounds())
						&& !this->players[k].isDamagedCooldown())
					{
						int damage = this->enemies[i].getDamage();

						this->players[k].takeDamage(damage);

						this->enemies[i].collision();

						//text when collosion
						this->textTags.add(
							TextTag(
								&this->font, "-" + std::to_string(damage),
								Color::Red,
								Vector2f(this->players[k].getPosition().x + 20.f,
									this->players[k].getPosition().y - 20.f),
								Vector2f(-1.f, 0.f),
								30, 20.f, true)
						);

						//player dead
						if (!this->players[k].isAlive())
							this->playersAlive--;

						return;
					}
				}
			}
		}
		//enemy out of screen
		if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGlobalBounds().width)
		{
			this->enemies.remove(i);
			return;
		}

	}

	this->enemyBulletUpdate(dt);
}

void Game::particleUpdate(const float& dt)
{
	bool particleRemoved = false;
	for (size_t i = 0; i < this->particles.size() && !particleRemoved; i++)
	{
		this->particles[i].update(dt);

		if (this->particles[i].readyToDel())
			particleRemoved = true;

		if (particleRemoved)
			this->particles.remove(i);
	}
}

void Game::textTagsUpdate(const float& dt)
{
	for (size_t i = 0; i < this->textTags.size(); i++)
	{
		this->textTags[i].Update(dt);

		if (this->textTags[i].getTimer() <= 0.f)
		{
			this->textTags.remove(i);
		}
	}
}

void Game::upgradesUpdate(const float& dt)
{
	for (size_t i = 0; i < this->upgrades.size(); i++)
	{
		this->upgrades[i].Update(dt);

		for (size_t k = 0; k < this->players.size(); k++)
		{
			if (this->upgrades[i].checkCollision(this->players[k].getGlobalBounds()))
			{
				switch (this->upgrades[i].getType())
				{
				case 0: // doubleray
					if (this->players[k].getGunLevel() <= 1)
					{
						this->players[k].setGunlevel(1);
						this->keyTimeDoubleRay = this->keyTimeDoubleRayMax-1;

						//DOUBLE RAY TEXT TAGs
						this->textTags.add(
							TextTag(
								&this->font, "Double Ray!",
								Color::Blue,
								Vector2f(this->players[k].getPosition().x + 20.f,
									this->players[k].getPosition().y - 20.f),
								Vector2f(0.f, -1.f),
								40, 100.f
								, true));

						this->textTags.add(
							TextTag(
								&this->font, "DOUBLE RAY!",
								Color::White,
								Vector2f(1600.f,15.f),
								Vector2f(-1.f, 0.f),
								30, 75.f
								, false));
					}

					break;

				case 1: // tripleray
					if (this->players[k].getGunLevel() <= 2)
					{
						this->players[k].setGunlevel(2);
						this->keyTimeTripleRay =this->keyTimeTripleRayMax-1;
						this->keyTimeDoubleRay = this->keyTimeDoubleRayMax;
						//TRIPLE RAY TEXT TAGs
						this->textTags.add(
							TextTag(
								&this->font, "Triple Ray!",
								Color::Magenta,
								Vector2f(this->players[k].getPosition().x + 20.f,
									this->players[k].getPosition().y - 20.f),
								Vector2f(0.f, -1.f),
								40, 100.f
								, true));

						this->textTags.add(
							TextTag(
								&this->font, "TRIPLE RAY!",
								Color::White,
								Vector2f(1600.f, 50.f),
								Vector2f(-1.f, 0.f),
								30, 75.f
								, false));
					}
					break;

				case 2: // piercing
					this->players[k].enablePiercingShot();
					this->keyTimePiercingShot=this->keyTimePiercingShotMax-1;
					//PIERCING SHOT TEXT TAGs
					this->textTags.add(
						TextTag(
							&this->font, "PIERCING SHOT!",
							Color::Yellow,
							Vector2f(this->players[k].getPosition().x + 20.f,
								this->players[k].getPosition().y - 20.f),
							Vector2f(0.f, -1.f),
							40, 100.f
							, true));
					this->textTags.add(
						TextTag(
							&this->font, "PIERCING SHOT!",
							Color::White,
							Vector2f(1600.f, 90.f),
							Vector2f(-1.f, 0.f),
							30, 75.f
							, false));

					break;

				case 3: // shield
					this->players[k].enableShield();
					this->keyTimeShield = this->keyTimeShieldMax-1;
					//Sheild TEXT TAGs
					this->textTags.add(
						TextTag(
							&this->font, "INVISIBLE SHIELD!",
							Color::Yellow,
							Vector2f(this->players[k].getPosition().x + 20.f,
								this->players[k].getPosition().y - 20.f),
							Vector2f(0.f, -1.f),
							40, 50.f
							, true));

					this->textTags.add(
						TextTag(
							&this->font, "INVISIBLE!",
							Color::White,
							Vector2f(1600.f, 120.f),
							Vector2f(-1.f, 0.f),
							30, 75.f
							, false));

					break;


				case 4: // healthtank
					this->players[k].upgradeHP();

					this->textTags.add(
						TextTag(
							&this->font, "HP MAX INCREASE!",
							Color(255, 165, 0),
							Vector2f(this->players[k].getPosition().x + 20.f,
								this->players[k].getPosition().y - 20.f),
							Vector2f(0.f, -1.f),
							40, 45.f
							, true));

					break;

				case 5: // stat point upgrade
					this->players[k].addStatPointRandom();


					this->textTags.add(
						TextTag(
							&this->font, "Stat point INCREASE!",
							Color(255, 165, 0),
							Vector2f(this->players[k].getPosition().x + 20.f,
								this->players[k].getPosition().y - 20.f),
							Vector2f(0.f, -1.f),
							40, 45.f
							, true));
					break;

				case 6: // Laser maching gun
					this->players[k].enableMachineGun();

					this->keyTimeMachineGun - 1.f;
					this->textTags.add(
						TextTag(
							&this->font, "Laser Machine Gun!",
							Color(255, 165, 0),
							Vector2f(this->players[k].getPosition().x + 20.f,
								this->players[k].getPosition().y - 20.f),
							Vector2f(0.f, -1.f),
							40, 45.f
							, true));
					break;
				}
				this->upgrades.remove(i);
				return;
			}

		}
		if (this->upgrades[i].canDelete())
		{
			this->upgrades.remove(i);
			break;
		}
	}
}

void Game::pickupsUpdate(const float& dt)
{
	for (size_t i = 0; i < this->pickups.size(); i++)
	{
		this->pickups[i].Update(dt);

		for (size_t k = 0; k < this->players.size(); k++)
		{
			if (this->pickups[i].checkColllision(this->players[k].getGlobalBounds()))
			{

				switch (this->pickups[i].getType())
				{

				case 0: // hp

					this->players[k].gainHP(this->players[k].getHP() / 5);

					this->textTags.add(
						TextTag(
							&this->font, "HP GAIN + " + std::to_string(this->players[k].getHP() / 5),
							Color::Green,
							Vector2f(this->players[k].getPosition().x + 20.f,
								this->players[k].getPosition().y - 20.f),
							Vector2f(0.f, -1.f),
							30, 40.f
							, true));

					break;

				}

				this->pickups.remove(i);
				return;
			}
		}
		if (this->pickups[i].canDelete())
		{
			this->pickups.remove(i);
			break;
		}
	}

}

void Game::gameOver()
{

	this->enemies.clear();
	this->upgrades.clear();
	this->pickups.clear();
	this->textTags.clear();
	this->gameOverText.setString
	(
		std::string("GAME OVER\nScore:" +
			std::to_string(this->score) + "\n" +
			"F1 to RESTART"
		)
	);
}

void Game::Restart()
{
	if (Keyboard::isKeyPressed(Keyboard::F1))
	{
		for (size_t i = 0; i < players.size(); i++)
		{
			this->players[i].Reset();
		}

		this->playersAlive = this->players.size();
		this->score = 0;
		this->scoreMultiplier = 1;
		this->multiplierAdder = 0;
		this->difficulty = 0;
		this->bossEncounter = false;
		this->enemySpawnTimerMax = 30.f;
		this->keyTimeDoubleRay = this->keyTimeDoubleRayMax;
		this->keyTimePiercingShot = this->keyTimePiercingShotMax;
		this->keyTimeShield = this->keyTimeShieldMax;
		this->keyTimeTripleRay = this->keyTimeTripleRayMax;

		//clear element
		this->enemies.clear();
		this->upgrades.clear();
		this->pickups.clear();
		this->textTags.clear();
		this->bosses.clear();
		this->particles.clear();
	}
}

void Game::Update(const float& dt)
{
	/*this->initSound();*/

	this->updateKeytime(dt);
	
	this->pauseUpdate();
	
	//fullscreen
	this->toggleFullscreen();

	//Can do while pause 
	this->updateWhilePause(dt);

	//Game running
	if (this->playersAlive > 0 && !this->paused)
	{
		//updateTimer
		this->upgradesTimerUpdate(dt);

		this->difficultyUpdate(dt);

		//score timer multiply
		this->scoreUpdate(dt);

		//Spawn enemies with timer
		this->enemiesSpawnUpdate(dt);

		// bullet & collision enemies w/bullet & enemy dead & drop pickups and  upgrades
		this->playerUpdate(dt);

		//update enemies / Collison with player / out of bound
		this->enemiesUpdate(dt);

		

		//Particle update
		this->particleUpdate(dt);

		//text tag update
		this->textTagsUpdate(dt);

		//upgrade update
		this->upgradesUpdate(dt);

		//pickup update
		this->pickupsUpdate(dt);
	}

	//Game over
	else if (this->playersAlive <= 0)
	{
		this->gameOver();
		this->Restart();
	}

}

void Game::DrawUI()
{
	if (this->paused)
	{
		this->window->draw(this->nameOfGame);
	}

	if (!this->paused)
	{
		//draw text tag
		for (size_t i = 0; i < this->textTags.size(); i++)
		{
			this->textTags[i].Draw(*this->window);
		}
	}
	// over text!!
	if (this->playersAlive <= 0)
	{
		this->window->draw(this->gameOverText);
	}
	
	//score
	this->window->draw(this->scoreText);

	
	//pause
	if (this->paused)
		this->window->draw(this->controlsText);

}

void Game::Draw()
{
	this->window->clear();
	this->window->draw(this->map);
	//draw player
	if (!this->paused) {
		for (size_t i = 0; i < this->players.size(); i++)
		{
			if (this->players[i].isAlive())
			{

				//Ui
				this->UpdateUIPlayer(i);
				this->players[i].Draw(*this->window);
				this->window->draw(this->followPlayerText); //UI

				this->window->draw(this->shieldBar);
				this->window->draw(this->shieldOutline);
				this->window->draw(this->shieldText);

				this->window->draw(this->piercingShotBar);
				this->window->draw(this->piercingOutline);
				this->window->draw(this->piercingShotText);

				this->window->draw(this->doubleRayBar);
				this->window->draw(this->doubleRayOutline);
				this->window->draw(this->doubleRayText);

				this->window->draw(this->tripleRayBar);
				this->window->draw(this->tripleRayOutline);
				this->window->draw(this->tripleRayText);

				this->window->draw(this->playerExpBar);
				if (this->players[i].playerShowStatsIsPressed())
				{
					this->window->draw(this->playerStatsTextBack);
					this->window->draw(this->playerStatsText);
				}
			}
		}

		//draw enemy
		for (size_t i = 0; i < this->enemies.size(); i++)
		{

			this->enemies[i].draw(*this->window);

			//Ui
			this->UpdateUIEnemy(i);
			this->window->draw(this->enemyText);
		}

		for (size_t i = 0; i < Enemy::enemyBullets.size(); i++)
		{
			Enemy::enemyBullets[i].Draw(*this->window);
		}

		//draw pickups
		for (size_t i = 0; i < this->pickups.size(); i++)
		{
			this->pickups[i].Draw(*this->window);

		}

		//draw upgrade
		for (size_t i = 0; i < this->upgrades.size(); i++)
		{
			this->upgrades[i].Draw(*this->window);
		}

		//draw particles
		for (size_t i = 0; i < this->particles.size(); i++)
		{
			this->particles[i].draw(*this->window);
		}
	}

	this->DrawUI();

	this->window->display();
}

