#include "Player.h"

unsigned Player::players = 0;
dArr<Texture> Player::mainGunTextures;


enum controls {up=0,down,left,right,shoot};
enum weapon { LASER = 0, MISSILE01, MISSILE02 };


Player::Player(std::vector<Texture>& textures,
	dArr<Texture>& lWingTextures,
	dArr<Texture>& rWingTextures,
	dArr<Texture>& cPitTextures,
	dArr<Texture>& auraTextures,
	int up , int down ,
	int left , int right ,
	int shoot )

	:level(1),exp(0),expNext(10),
	hp(10),hpMax(10),
	statPoint(0),cooling(0), plating(0),wiring(0),power(0),
	damage(1),damageMax(2),   
	score(0)
{
	//dt
	this->dtMultipiler = 60.f;

	//keytime
	this->keyTimeMax = 80.f;
	this->keyTime = this->keyTimeMax;

	//stats
	this->expNext = 20+static_cast<int>(
			(50 / 3) * ((pow(level, 3) - 6 * pow(level, 2)) + 17 * level - 12));

	//update position
	this->playerCenter.x = this->sprite.getPosition().x +
		this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y +
		this->sprite.getGlobalBounds().height / 2;

	//Textures & sprite
	//Player guns
	Texture temp;
	temp.loadFromFile("Textures/Guns/gun01.png");
	Player::mainGunTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Guns/gun02.png");
	Player::mainGunTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Guns/gun03.png");
	Player::mainGunTextures.add(Texture(temp));

	this->lWingTextures = &lWingTextures;
	this->rWingTextures = &rWingTextures;
	this->cPitTextures = &cPitTextures;
	this->auraTextures = &auraTextures;
	
	this->sprite.setTexture(textures[0]);
	this->sprite.setScale(0.09f, 0.09f);
	this->sprite.setColor(Color(10, 10, 10, 255));
	this->sprite.setPosition(100.f, 520.f);
	

	this->laserTexture = &textures[1];
	this->missile01Texture = &textures[2];

	this->mainGunSprite.setTexture(Player::mainGunTextures[0]);
	this->mainGunSprite.setOrigin(
		this->mainGunSprite.getGlobalBounds().width / 2,
		this->mainGunSprite.getGlobalBounds().height / 2
			);
	this->mainGunSprite.rotate(90);
	this->mainGunSprite.setScale(0.9f,0.9f);

	// set position gun to follow player
	this->mainGunSprite.setPosition(
		this->playerCenter.x + 20.f,
		this->playerCenter.y
	);


	//accessory


	//select accessory
	this->lWingSelect = 10;
	this->rWingSelect =10;
	this->cPitSelect =10;
	this->auraSelect = 0;

	//acessory texture
	this->lWing.setTexture((*this->lWingTextures)[lWingSelect]);
	this->rWing.setTexture((*this->rWingTextures)[rWingSelect]);
	this->cPit.setTexture((*this->cPitTextures)[cPitSelect]);
	this->aura.setTexture((*this->auraTextures)[auraSelect]);

	//init accessory
	this->lWing.setOrigin(
		this->lWing.getGlobalBounds().width / 2,
		this->lWing.getGlobalBounds().height / 2);

			this->lWing.setPosition(this->playerCenter);
			this->lWing.setRotation(90.f);
			this->lWing.setScale(0.8f, 0.8f);

	this->rWing.setOrigin(
		this->rWing.getGlobalBounds().width / 2,
		this->rWing.getGlobalBounds().height / 2);

			this->rWing.setPosition(this->playerCenter);
			this->rWing.setRotation(90.f);
			this->rWing.setScale(0.8f, 0.8f);

	this->cPit.setOrigin(
		this->cPit.getGlobalBounds().width / 2,
		this->cPit.getGlobalBounds().height / 2);

			this->cPit.setPosition(this->playerCenter);
			this->cPit.setRotation(90.f);
			this->cPit.setScale(0.8f, 0.8f);

	this->aura.setOrigin(
		this->aura.getGlobalBounds().width / 2,
		this->aura.getGlobalBounds().height / 2);

			this->aura.setPosition(this->playerCenter);
			this->aura.setRotation(90.f);
			this->aura.setScale(0.8f, 0.8f);

	

	//Timers
	this->shootTimerMax = 25.f;
	this->shootTimer = this->shootTimerMax;

	this->damageTimerMax = 35.f;
	this->damageTimer = this->damageTimerMax;

	//controls
	this->controls[controls::up] = up;
	this->controls[controls::down] = down;
	this->controls[controls::left] = left; 
	this->controls[controls::right] = right;
	this->controls[controls::shoot] = shoot;

	//velo
	this->maxVelo = 35.f;
	this->acceleration = 1.f;
	this->stabilizerForce = 0.55f;

	//guns
	this->currentWeapons = LASER;

	//upgrade
	this->mainGunLevel = 0;
	this->piercingShot = false;
	this->shield = false;
	this->dualMissiles01 = false;
	this->dualMissiles02 = false;

	this->setGunlevel(0);

	// Co op
	this->playerNr = Player::players;
	Player::players++;
	
}

Player::~Player()
{

}

Bullet& Player::getBullets(unsigned index)
{
	if (index < 0 || index >this->bullets.size())
		throw"OUT OF BOUND! PLAYer::BULLET";
	
	return this->bullets[index];

}

void Player::removeBullet(unsigned index)
{
	if (index < 0 || index >this->bullets.size())
		throw"OUT OF BOUND! PLAYer::removeBULLET";

	this->bullets.remove(index);
}

int Player::getDamage()const
{
	int damage = 0;
	switch (this->currentWeapons)
	{
	case LASER:
		damage = rand() % this->damageMax + this->damage;
		break;

	case MISSILE01:
		damage = rand() % this->damageMax + this->damage;
		damage *= 2;
		break;

	case MISSILE02:
		damage = rand() % this->damageMax + this->damage;
		damage *= 4;
		break;
	default:
		damage = rand() % this->damageMax + this->damage;
		break;
	}


	return damage;
}

void Player::takeDamage(int damage)
{
	this->hp -= damage;

	this->damageTimer = 0;

	this->currentVelo.x += -(this->normDir.x) * 10.f;
	this->currentVelo.y += -(this->normDir.y) * 10.f;
}

void Player::gainHP(int hp)
{
	this->hp += hp;
	if (this->hp > this->hpMax)
		this->hp = this->hpMax;
}

void Player::setGunlevel(int gunLevel)
{
	this->mainGunLevel = gunLevel;

	if (this->mainGunLevel < Player::mainGunTextures.size())
		this->mainGunSprite.setTexture(Player::mainGunTextures[this->mainGunLevel]);
	else
		std::cout << "NO TEXTURE FOR THAT MAIN GUN!" << "\n";
}

void Player::addStatPointRandom()
{
	int temp = rand() % 4;
	switch (temp)
	{
	case 0:
		this->power++;
		break;
	case 1 :
		this->wiring++;
		break;
	case 2 :
		this->cooling++;
		break;
	case 3:
		this->plating++;
		break;
	default:
		break;
	}

	this->UpdateStats();
}

void Player::Reset()
{
	this->hpMax = 10;
	this->hp = this->hpMax;
	this->sprite.setPosition(Vector2f(100.f, 100.f));
	this->bullets.clear();
	this->setGunlevel(0);
	this->wiring = 0;
	this->cooling = 0;
	this->power = 0;
	this->plating = 0;
	this->dualMissiles01 = false;
	this->dualMissiles02 = false;
	this->shield = false;
	this->piercingShot = false;
	this->currentVelo.x = 0 ;
	this->currentVelo.y = 0;
	this->level = 1;
	this->exp = 0;
	this->expNext = 20;
	this->currentWeapons = LASER;
	this->statPoint = 0;
	this->shootTimer = this->shootTimerMax;
	this->score = 0;

}

bool Player::UpdateLeveling()
{
	if (this->exp >= this->expNext)
	{
		this->level++;
		this->statPoint++;
		this->exp	-= this->expNext;
		this->expNext=static_cast<int>(
				(50 / 3) * ((pow(level, 3) - 6 * pow(level, 2)) + 17 * level - 12));
		
		this->cooling++;
		this->wiring++;
		this->power++;
		this->plating++;

		this->UpdateStats();

		this->hp = hpMax;
		
		return true;
	}
	return false;
}

void Player::UpdateStats()
{
	this->hpMax += plating * 5.f;
	this->damageMax += power * 2;
	this->damage += power;
	this->shootTimerMax -= (cooling*0.05);
}

void Player::changeAccessories(const float& dt)
{
	if (Keyboard::isKeyPressed(Keyboard::Up) && this->keyTime >= keyTimeMax)
	{
		if (lWingSelect < (*this->lWingTextures).size() - 1 && 
			rWingSelect < (*this->rWingTextures).size() - 1 &&
			cPitSelect < (*this->cPitTextures).size()-1)
		{
		lWingSelect++;
		rWingSelect++;
		cPitSelect++;
		}
		else 
		{
		lWingSelect = 0;
		rWingSelect = 0;
		cPitSelect = 0;
		}

		this->lWing.setTexture((*this->lWingTextures)[lWingSelect]);
		this->rWing.setTexture((*this->rWingTextures)[rWingSelect]);
		this->cPit.setTexture((*this->cPitTextures)[cPitSelect]);
		this->keyTime = 0;
	}
	//down key
	//else if (Keyboard::isKeyPressed(Keyboard::Down) && this->keyTime >= keyTimeMax)
	//{
	//	if (lWingSelect < (*this->lWingTextures).size() - 1 &&
	//		rWingSelect < (*this->rWingTextures).size() - 1)
	//	{
	//		lWingSelect--;
	//		rWingSelect--;
	//	}
	//	else
	//	{
	//		lWingSelect = 11;
	//		rWingSelect = 11;
	//	}

	//	this->lWing.setTexture((*this->lWingTextures)[lWingSelect]);
	//	this->rWing.setTexture((*this->rWingTextures)[rWingSelect]);
	//	this->keyTime = 0;
	//}


}

void Player::UpdateAccessories(const float& dt)
{
	// set position gun to follow player
		this->mainGunSprite.setPosition(
			this->mainGunSprite.getPosition().x,
			this->playerCenter.y);
	
	//main gun animate after firing
	if (this->mainGunSprite.getPosition().x < this->playerCenter.x + 20)
	{
		this->mainGunSprite.move(5.f *dt * this->dtMultipiler + 
			this->currentVelo.x * dt * this->dtMultipiler, 0.f);
	}
	if (this->mainGunSprite.getPosition().x > this->playerCenter.x + 20)
	{
		this->mainGunSprite.setPosition(
			this->playerCenter.x +20.f,
			this->playerCenter.y
		);
	}

	//leftwing
	this->lWing.setPosition(playerCenter.x + -abs(this->currentVelo.x), 
							playerCenter.y + -abs(this->currentVelo.x/2)+this->currentVelo.y/2);

	//rightwing
	this->rWing.setPosition(playerCenter.x + -abs(this->currentVelo.x),
							playerCenter.y +  abs(this->currentVelo.x / 2)+ this->currentVelo.y / 2);

	//cockpit
	this->cPit.setPosition( playerCenter.x + this->currentVelo.x,
							playerCenter.y);
	
	//aura
	this->aura.setPosition(playerCenter);
	this->aura.rotate(.1f * dt * this->dtMultipiler);

}

void Player::Movement(Vector2u windowBound, const float& dt)
{
	//update normal dir

	this->normDir = normalize(this->currentVelo, vectorLength(this->currentVelo));


	//up
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::up])))
	{
		direction.x = 0.f;
		direction.y = -1.f;

		if(this->currentVelo.y > -this->maxVelo && direction.y < 0)
			this->currentVelo.y += this->direction.y * this->acceleration
			* dt * this->dtMultipiler;
	}

	//down
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::down])))
	{
		direction.x = 0.f;
		direction.y = 1.f;

		if (this->currentVelo.y < this->maxVelo && direction.y> 0)
			this->currentVelo.y += this->direction.y * this->acceleration
			* dt * this->dtMultipiler;
	}
	
	//left
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::left])))
	{
		direction.x = -1.f;
		direction.y = 0.f;

		if (this->currentVelo.x > -this->maxVelo && direction.x< 0)
			this->currentVelo.x += this->direction.x * this->acceleration
			* dt * this->dtMultipiler;
	}

	//right
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::right])))
	{
		direction.x = 1.f;
		direction.y = 0.f;

		if (this->currentVelo.x < this->maxVelo && direction.x> 0)
			this->currentVelo.x += this->direction.x * this->acceleration
			* dt * this->dtMultipiler;
	}
	//drag
	if (this->currentVelo.x > 0)
	{
		this->currentVelo.x -= this->stabilizerForce
			* dt * this->dtMultipiler;

		if (this->currentVelo.x < 0)
			this->currentVelo.x = 0;
	}
	else if (this->currentVelo.x < 0)
	{
		this->currentVelo.x += this->stabilizerForce
			* dt * this->dtMultipiler;
		if (this->currentVelo.x > 0)
			this->currentVelo.x = 0;
	}
	if (this->currentVelo.y > 0)
	{
		this->currentVelo.y -= this->stabilizerForce
			* dt * this->dtMultipiler;
		if (this->currentVelo.y < 0)
			this->currentVelo.y = 0;
	}
	else if (this->currentVelo.y < 0)
	{
		this->currentVelo.y += this->stabilizerForce
			* dt * this->dtMultipiler;
		if (this->currentVelo.y > 0)
			this->currentVelo.y = 0;
	}

	//final move
	this->sprite.move(this->currentVelo.x * dt * this->dtMultipiler,
		this->currentVelo.y * dt * this->dtMultipiler);

	// update position
	this->playerCenter.x = this->sprite.getPosition().x +
		this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y +
		this->sprite.getGlobalBounds().height / 2;

	//window collision
	
	if (this->getPosition().x <= 0)
	{
		this->sprite.setPosition(0.f, this->sprite.getPosition().y);
		this->currentVelo.x = 0.f;
	}

	else if (this->getPosition().y <= 0)
	{
		this->sprite.setPosition(this->sprite.getPosition().x, 0.f);
		this->currentVelo.y = 0.f;
	}

	else if (this->getPosition().x + this->getGlobalBounds().width>= windowBound.x)
	{
		this->sprite.setPosition(windowBound.x - this->getGlobalBounds().width, this->sprite.getPosition().y );
		this->currentVelo.x = 0.f;
	}
	else if (this->getPosition().y  + this->getGlobalBounds().height >= windowBound.y)
	{
		this->sprite.setPosition(this->sprite.getPosition().x , windowBound.y - this->getGlobalBounds().height);
		this->currentVelo.y = 0.f;
	}

		
}

void Player::Combat(const float& dt)
{
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::shoot]))
		&& this->shootTimer >= this->shootTimerMax)
	{
		if (this->currentWeapons == LASER)
		{
			//create bullet
			if (this->mainGunLevel == 0) {
				this->bullets.add(Bullet(laserTexture,
					Vector2f(this->playerCenter.x + 50.f, this->playerCenter.y),
					Vector2f(0.4f, .1f),
					60.f, 20.f,
					Vector2f(1.f, 0.f),
					5.f));
			}
			//double ray
			else if (this->mainGunLevel == 1) 
			{
				this->bullets.add(Bullet(laserTexture,
					Vector2f(this->playerCenter.x + 50.f, this->playerCenter.y-25),
					Vector2f(0.4f, .10f),
					60.f, 20.f,
					Vector2f(1.f, 0.f),
					5.f));
				this->bullets.add(Bullet(laserTexture,
					Vector2f(this->playerCenter.x + 50.f, this->playerCenter.y+25),
					Vector2f(0.4f, .10f),
					60.f, 20.f,
					Vector2f(1.f, 0.f),
					5.f));
			}
			else if (this->mainGunLevel == 2) 
				{
				this->bullets.add(Bullet(laserTexture,
					Vector2f(this->playerCenter.x + 50.f, this->playerCenter.y - 40),
					Vector2f(0.4f, .10f),
					60.f, 20.f,
					Vector2f(1.f, 0.f),
					5.f));
				this->bullets.add(Bullet(laserTexture,
					Vector2f(this->playerCenter.x + 50.f, this->playerCenter.y  ),
					Vector2f(0.4f, .10f),
					60.f, 20.f,
					Vector2f(1.f, 0.f),
					5.f));
				this->bullets.add(Bullet(laserTexture,
					Vector2f(this->playerCenter.x + 50.f, this->playerCenter.y +40),
					Vector2f(0.4f, .10f),
					60.f, 20.f,
					Vector2f(1.f, 0.f),
					5.f));
			}
			//animation
			this->mainGunSprite.move(-40.f, 0.f);

		}
		else if (this->currentWeapons == MISSILE01)
		{
			//create bullet
				this->bullets.add(Bullet(missile01Texture,
					Vector2f(this->playerCenter.x, this->playerCenter.y - 25.f),
					Vector2f(0.05f, 0.05f),
					50.f, 2.f,
					Vector2f(1.f, 0.f),
					1.f));

			if (this->dualMissiles01)
			{
				//create bullet 2
				this->bullets.add(Bullet(missile01Texture,
					Vector2f(this->playerCenter.x, this->playerCenter.y + 25.f),
					Vector2f(0.05f, 0.05f),
					50.f, 2.f,
					Vector2f(1.f, 0.f),
					1.f));
			}
		}
		
		
		this->shootTimer = 0; // reset timer
	}
	
	//dmg
	if (this->isDamagedCooldown())
	{
		if ((int)this->damageTimer % 2 == 0)
		{
			this->lWing.setColor(Color::Red);
			this->rWing.setColor(Color::Red);
			this->cPit.setColor(Color::Red);
		}
		else
		{
			this->lWing.setColor(Color::White);
			this->rWing.setColor(Color::White);
			this->cPit.setColor(Color::White);
		}
	}
	else	
	{
		this->lWing.setColor(Color::White);
		this->rWing.setColor(Color::White);
		this->cPit.setColor(Color::White);
	}
		
}

void Player::Update(Vector2u windowBounds, const float& dt)
{
	//update timer
	if (this->shootTimer < this->shootTimerMax)
		this->shootTimer+= 1.f * dt * this->dtMultipiler;
	if (this->damageTimer < this->damageTimerMax)
		this->damageTimer+=1.f *dt * this->dtMultipiler;
	if (this->keyTime < this->keyTimeMax)
		this->keyTime += 1.f * dt * this->dtMultipiler;

	this->Movement(windowBounds, dt);
	this->changeAccessories(dt);
	this->UpdateAccessories(dt);
	this->Combat(dt);

}

void Player::Draw(RenderTarget &target)
{

	target.draw(this->aura);

	for (size_t i = 0; i < this->bullets.size(); i++)
	{
		this->bullets[i].Draw(target);
	}

	target.draw(this->mainGunSprite);

	target.draw(this->sprite);

	target.draw(this->cPit);
	target.draw(this->lWing);
	target.draw(this->rWing);

}