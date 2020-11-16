#include "TextTag.h"

TextTag::TextTag(Font *font ,std::string text,Color color,
	Vector2f position, unsigned int size,
	float timerMax)
{
	this->dtMultipiler= 60.f;

	this->text.setFont(*font);
	this->text.setCharacterSize(size);
	this->text.setPosition(position);
	this->text.setString(text);
	this->text.setFillColor(color);

	this->speed = 2.f;
	this->direction.x = 0.f;
	this->direction.y = -1.f;
	this->timerMax = timerMax;
	this->timer = this->timerMax;
}

TextTag::~TextTag()
{

}

void TextTag::Update(const float& dt)
{
	//timer
	if (this->timer > 0.f)
	{
		this->timer += 1.f * dt * this->dtMultipiler;
		
		this->text.move(this->direction.x * this->speed * dt * this->dtMultipiler,
			this->direction.y * this->speed * dt * this->dtMultipiler);
	}
	else
	{
		this->text.setFillColor(Color(0, 0, 0, 0));
	}

	//
}

void TextTag::Draw(RenderTarget &target)
{
	target.draw(this->text);
}
