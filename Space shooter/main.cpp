#include "Game.h"

int main()
{
	srand(time(NULL));

	RenderWindow window(VideoMode(1600, 900), "Wingman", Style::Default);
	
	Clock clock;
	float dt = 0.f;

	Game game(&window);


	dArr<int> ints;


	// Game loop
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)			
				window.close();			
			if(event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)	
				window.close();
		}

		dt = clock.restart().asSeconds();

		game.Update(dt);
		game.Draw();
	
	}

}