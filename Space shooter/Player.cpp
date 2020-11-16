#include "Player.h"

unsigned Player::players = 0;

enum controls {up=0,down,left,right,shoot};
enum weapon { LASER = 0, MISSILE01, MISSILE02 };


Player::Player(std::vector<Texture>& textures,
	int up, int down,
	int left, int right,
	int shoot 			)
	:level(1),exp(0),expNext(10),
	hp(15),hpMax(15),
	statPoint(0),cooling(0), plating(0),wiring(0),power(0),
	damage(1),damageMax(2),
	score(0)
{
	//dt

	this->dtMultipiler = 60.f;

	//stats
	this->expNext = 20+static_cast<int>(
			(50 / 3) * ((pow(level, 3) - 6 * pow(level, 2)) + 17 * level - 12));

	//update position
	this->playerCenter.x = this->sprite.getPosition().x +
		this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y +
		this->sprite.getGlobalBounds().height / 2;

	//Textures & sprite
	this->sprite.setTexture(textures[0]);
	this->sprite.setScale(0.13f, 0.13f);

	this->laserTexture = &textures[1];
	this->missile01Texture = &textures[2];

	this->mainGunSprite.setTexture(textures[3]);
	this->mainGunSprite.setOrigin(
		this->mainGunSprite.getGlobalBounds().width / 2,
		this->mainGunSprite.getGlobalBounds().height / 2
			);
	this->mainGunSprite.rotate(90);

	// set position gun to follow player
	this->mainGunSprite.setPosition(
		this->playerCenter.x + 20.f,
		this->playerCenter.y
	);

	//Timers
	this->shootTimerMax = 25.f;
	this->shootTimer = this->shootTimerMax;

	this->damageTimerMax = 10.f;
	this->damageTimer = this->damageTimerMax;

	//controls
	this->controls[controls::up] = up;
	this->controls[controls::down] = down;
	this->controls[controls::left] = left; 
	this->controls[controls::right] = right;
	this->controls[controls::shoot] = shoot;

	//velo
	this->maxVelo = 30.f;
	this->acceleration = 1.f;
	this->stabilizerForce = 0.6f;

	//guns
	this->currentWeapons = MISSILE01;

	//upgrade
	this->mainGunLevel = 0;
	this->dualMissiles01 = true;
	this->dualMissiles02 = false;

	// Co op
	this->playerNr = Player::players;
	Player::players++;
	
}

Player::~Player()
{

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
bool Player::UpdateLeveling()
{
	if (this->exp >= this->expNext)
	{
		this->level++;
		this->statPoint++;
		this->exp	-= this->expNext;
		this->expNext=static_cast<int>(
				(50 / 3) * ((pow(level, 3) - 6 * pow(level, 2)) + 17 * level - 12));
		
		this->hp = hpMax;
		
		return true;
	}
	return false;
}

void Player::UpdateAccessories(const float &dt)
{
	// set position gun to follow player
	this->mainGunSprite.setPosition(
		this->mainGunSprite.getPosition().x ,
		this->playerCenter.y);

	//main gun animate after firing
	if (this->mainGunSprite.getPosition().x < this->playerCenter.x + 20)
	{
		this->mainGunSprite.move(2.f *dt * this->dtMultipiler + 
			this->currentVelo.x * dt * this->dtMultipiler, 0.f);
	}
	if (this->mainGunSprite.getPosition().x > this->playerCenter.x + 20)
	{
		this->mainGunSprite.setPosition(
			this->playerCenter.x +20.f,
			this->playerCenter.y
		);
	}
}

void Player::Movement(const float& dt)
{
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
				this->bullets.push_back(Bullet(laserTexture,
					Vector2f(this->playerCenter.x + 50.f, this->playerCenter.y),
					Vector2f(0.2f, .2f),
					70.f, 20.f,
					Vector2f(1.f, 0.f),
					7.f));
			}
			else if (this->mainGunLevel == 1) {
				this->bullets.push_back(Bullet(laserTexture,
					Vector2f(this->playerCenter.x + 50.f, this->playerCenter.y),
					Vector2f(0.2f, .2f),
					70.f, 20.f,
					Vector2f(1.f, 0.f),
					5.f));
			}
			else if (this->mainGunLevel == 2) {
				this->bullets.push_back(Bullet(laserTexture,
					Vector2f(this->playerCenter.x + 50.f, this->playerCenter.y),
					Vector2f(0.2f, .2f),
					70.f, 20.f,
					Vector2f(1.f, 0.f),
					5.f));
			}
			//animation
			this->mainGunSprite.move(-40.f, 0.f);

		}
		else if (this->currentWeapons == MISSILE01)
		{
			//create bullet
				this->bullets.push_back(Bullet(missile01Texture,
					Vector2f(this->playerCenter.x, this->playerCenter.y - 25.f),
					Vector2f(0.05f, 0.05f),
					50.f, 2.f,
					Vector2f(1.f, 0.f),
					1.f));

			if (this->dualMissiles01)
			{
				//create bullet 2
				this->bullets.push_back(Bullet(missile01Texture,
					Vector2f(this->playerCenter.x, this->playerCenter.y + 25.f),
					Vector2f(0.05f, 0.05f),
					50.f, 2.f,
					Vector2f(1.f, 0.f),
					1.f));
			}
		}
		else if (this->currentWeapons == MISSILE02)
		{
			if (this->dualMissiles02)
			{

			}
		}
		
	
		this->shootTimer = 0; // reset timer
	}
}

void Player::Update(Vector2u windowBounds, const float& dt)
{
	//update timer
	if (this->shootTimer < this->shootTimerMax)
		this->shootTimer+= 1.f * dt * this->dtMultipiler;
	if (this->damageTimer < this->damageTimerMax)
		this->damageTimer+=1.f *dt * this->dtMultipiler;

	this->Movement(dt);
	this->UpdateAccessories(dt);
	this->Combat(dt);

}
void Player::Draw(RenderTarget &target)
{
	for (size_t i = 0; i < this->bullets.size(); i++)
	{
		this->bullets[i].Draw(target);
	}
	target.draw(this->mainGunSprite);
	target.draw(this->sprite);

}