#include "Game.h"

enum textures { player = 0, laser01,missile01, mainGun01  };

Game::Game(RenderWindow *window)
{
	this->window = window;
	this->window->setFramerateLimit(120);
	this->dtMultiplier = 60;
	
	//init font
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	//init texture
	this->initTextures();
	
	//init player
	this->players.add(Player(this->textures , 
		this->lWingTextures,this->rWingTextures,
		this->cPitTextures,this->auraTextures));

	this->playersAlive = this->players.size();	

	this->enemySpawnTimerMax = 30;
	this->enemySpawnTimer = this->enemySpawnTimerMax;


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


	Texture temp;
	temp.loadFromFile("Textures/enemyMoveLeft.png");
	this->enemyTextures.add(Texture(temp));
	temp.loadFromFile("Textures/enemyFollow.png");
	this->enemyTextures.add(Texture(temp));

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

}

void Game::initUI()
{

		//Follow text
		this->followPlayerText.setFont(font);
		this->followPlayerText.setCharacterSize(15);

		//static text
		this->staticPlayerText.setFont(font);
		this->staticPlayerText.setCharacterSize(15);


	this->playerExpBar.setSize(Vector2f(90.f, 10.f));
	this->playerExpBar.setFillColor(Color(0.f, 90.f, 200.f, 200.f));

	this->enemyText.setFont(this->font);
	this->enemyText.setCharacterSize(14);

	this->gameOverText.setFont(this->font);
	this->gameOverText.setFillColor(Color::Red);
	this->gameOverText.setCharacterSize(40);
	this->gameOverText.setString("GAME OVER!!");
	this->gameOverText.setPosition( 100.f,this->window->getSize().y /2);

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
	
}

void Game::UpdateUIEnemy(int index)
{
	this->enemyText.setPosition(
		this->enemies[index].getPosition().x,
		this->enemies[index].getPosition().y- 13.f);

	this->enemyText.setString(
		std::to_string(
			this->enemies[index].getHP()) + " / " +
		std::to_string(
			this->enemies[index].getHPMax()));

}

void Game::Update(const float &dt)
{
	if (this->playersAlive >0) 
	{
		//updateTimer
		if (this->enemySpawnTimer < this->enemySpawnTimerMax)
			this->enemySpawnTimer+= 1.f* dt * this->dtMultiplier;

		//Spawn enemies
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->enemies.add(Enemy(
				this->enemyTextures, this->window->getSize(),
				Vector2f(0.f, 0.f)
				, Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f),
				rand()%2, rand() % 3 + 1, 2, 1 ,rand()%this->playersAlive
			));

			this->enemySpawnTimer = 0;
		}


		for (size_t i = 0; i < this->players.size(); i++)
		{
			if (this->players[i].isAlive())
			{
				//Update player
				this->players[i].Update(this->window->getSize(),dt);



				//bullet update
				for (size_t k = 0; k < this->players[i].getBulletsSize(); k++)
				{
					this->players[i].getBullets(k).Update(dt);

					//enemy collison
					for (size_t j = 0; j < this->enemies.size(); j++)
					{

						if (this->players[i].getBullets(k).getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
						{
							this->players[i].removeBullet(k);
							//take dmg
							int damage = this->players[i].getDamage();
							if (this->enemies[j].getHP() > 0)
							{
								this->enemies[j].takeDamage(damage);

								//text when shoot
								this->textTags.add(
									TextTag(
										&this->font,"-" + std::to_string(damage),
										Color::Red,
										Vector2f(this->enemies[j].getPosition().x + 20.f,
											this->enemies[j].getPosition().y - 20.f),
										Vector2f(1.f,0.f),
										26, 17.f,
										true)
								);
							}

							//dead
							if (this->enemies[j].getHP() <= 0)
							{
								//exp gain
								int exp = this->enemies[j].getHPMax() +
									(rand() % this->enemies[j].getHPMax() + 1);

								//score 
								int score = this->enemies[j].getHPMax();
								this->players[i].gainScore(score);

								if (this->players[i].gainExp(exp))
								{

									//text level up
									this->textTags.add(
										TextTag(
											&this->font, "LEVEL UP!" ,
											Color::Cyan,
											Vector2f(this->players[i].getPosition().x + 20.f,
												this->players[i].getPosition().y - 20.f),
											Vector2f(0.f, -1.f),
											36, 
											34.f,true)
									);
								}
								this->enemies.remove(j);

								//gain exp
								this->textTags.add(
									TextTag(
										&this->font, "+" + std::to_string(exp) +" xp",
										Color::Cyan,
										Vector2f(this->players[i].getPosition().x + 20.f,
											this->players[i].getPosition().y - 20.f),
										Vector2f(0.f, 1.f),
										26, 30.f
									,true)
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
		}
		//update enemies
		for (size_t i = 0; i < this->enemies.size(); i++)
		{
			this->enemies[i].Update(dt, this->players[this->enemies[i].getPlayerFollow()].getPosition());

			//collision w/ player
			for (size_t k = 0; k < this->players.size(); k++)
			{
				if (this->players[k].isAlive())
				{
					if (this->players[k].getGlobalBounds().intersects(this->enemies[i].getGlobalBounds())
						&& !this->players[k].isDamagedCooldown())
					{
						int damage = this->enemies[i].getDamage();


						this->players[k].takeDamage(damage);


						//text when collosion
						this->textTags.add(
							TextTag(
								&this->font,"-"+std::to_string(damage), 
								Color::Red,
									Vector2f(this->players[k].getPosition().x+20.f,
										this->players[k].getPosition().y-20.f),
								Vector2f(-1.f, 0.f),
								30,20.f,true)
						);

						//player dead
						if (!this->players[k].isAlive())
							this->playersAlive--;

						return;
					}
				}
			}

			if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGlobalBounds().width)
			{
				this->enemies.remove(i);
				return;
			}
		}

		//text tag update
		for (size_t i = 0; i < this->textTags.size(); i++)
		{
			this->textTags[i].Update(dt);

			if (this->textTags[i].getTimer() <= 0.f)
			{
				this->textTags.remove(i);
			}
		}
	}
}

void Game::DrawUI()
{

}

void Game::Draw()
{
	this->window->clear();

	//draw player
	for (size_t i = 0; i < this->players.size(); i++)
	{
		if (this->players[i].isAlive())
		{
				
			this->players[i].Draw(*this->window);

			//Ui
			this->UpdateUIPlayer(i);
			this->window->draw(this->followPlayerText); //UI
			this->window->draw(this->playerExpBar);
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

	//draw text tag
	for (size_t i = 0; i < this->textTags.size(); i++)
	{
		this->textTags[i].Draw(*this->window);
	}

	// over text!!
	if (this->playersAlive <= 0)
	{
		this->window->draw(this->gameOverText);
	}

	this->window->display();
}
