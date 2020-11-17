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

	
	//init font
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	//init texture
	this->initTextures();
	
	//init player
	this->players.add(Player(this->textures , 
		this->lWingTextures,this->rWingTextures,
		this->cPitTextures,this->auraTextures));

	this->playersAlive = this->players.size();	

	this->enemySpawnTimerMax = 25;
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
	temp.loadFromFile("Textures/enemyMoveLeftShoot.png");
	this->enemyTextures.add(Texture(temp));


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


	//enemy bullet
	temp.loadFromFile("Textures/Guns/roundBulletRed.png");
	this->enemyBulletTextures.add(Texture(temp));

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

	this->scoreText.setFont(this->font);
	this->scoreText.setFillColor(Color(200,200,200,150));
	this->scoreText.setCharacterSize(30);
	this->scoreText.setString("Score : 0");
	this->scoreText.setPosition(10.f,10.f);


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
		this->enemies[index].getPosition().y- 
		this->enemies[index].getGlobalBounds().height);

	this->enemyText.setString(
		std::to_string(
			this->enemies[index].getHP()) + " / " +
		std::to_string(
			this->enemies[index].getHPMax()));

}

void Game::Update(const float& dt)
{
	if (this->playersAlive > 0)
	{
		//updateTimer
		if (this->enemySpawnTimer < this->enemySpawnTimerMax)
			this->enemySpawnTimer += 1.f * dt * this->dtMultiplier;

		//score timer multiply
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
		//upgrade

		//Spawn enemies
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

					//enemy collison
					for (size_t j = 0; j < this->enemies.size(); j++)
					{

						if (this->players[i].getBullets(k).getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
						{

							//piercing
							if(!this->players[i].getPiercingShot())
								this->players[i].removeBullet(k);
							//take dmg
							int damage = this->players[i].getDamage();
							if (this->enemies[j].getHP() > 0)
							{
								this->enemies[j].takeDamage(damage);

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
		
									if (dropChange < 10) 
									{
										this->pickups.add(Pickup(
											this->pickupTextures,
											this->enemies[j].getPosition(),
											0,
											150.f)
										);
									}
									else
									{
										dropChange = rand() % 100 + 1;

										if (dropChange > 90) 
											{
											this->upgrades.add(Upgrade(
												this->upgradeTextures,
												this->enemies[j].getPosition(),
												rand() % 5 , 150.f)
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
			this->score += players[i].getScore();
			this->scoreText.setString(
				"Score: " + std::to_string(this->score) +
				"\nMultipiler : " + std::to_string(this->scoreMultiplier) + "x" +
				"\nMultipliler Timer: " + std::to_string((int)this->multiplierTimer) +
				"\nNew Multipiler: " + std::to_string(this->multiplierAdder)
				+ " / " +
				std::to_string(this->multiplierAdderMax)
			);

		}
		//update enemies
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

		//upgrade update
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
						if(this->players[k].getGunLevel()<1)
						this->players[k].setGunlevel(1);

						break;

					case 1: // tripleray
						if (this->players[k].getGunLevel() < 2)
						this->players[k].setGunlevel(2);
						break;

					case 2: // piercing
						this->players[k].enablePiercingShot();
						break;

					case 3: // shield
						this->players[k].enableShield();
						break;

					case 4: // healthtank
						this->players[k].upgradeHP();
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

		//pickup update
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

						break;

					default:
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
}
void Game::DrawUI()
{


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

	//score
	this->window->draw(this->scoreText);
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

	//draw pickups
	for (size_t i = 0; i <this->pickups.size(); i++)
	{
		this->pickups[i].Draw(*this->window);

	}

	//draw upgrade
	for (size_t i = 0; i < this->upgrades.size(); i++)
	{
		this->upgrades[i].Draw(*this->window);
	}

	this->DrawUI();

	this->window->display();
}
