#include "Game.h"

int main()
{
	srand(time(NULL));
	RenderWindow window(VideoMode(1920, 1080), "AIRSTRIKE", Style::Default);
	Clock clock;
	float dt = 0.f;
	Music music;

	/*music.openFromFile("Musics/mainGameSong.wav");
	music.setVolume(1.f);
	music.play();*/
	Game game(&window);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)			
				window.close();			
			if(event.type == Event::KeyPressed && event.key.code == Keyboard::X)	
				window.close(); 
		}
		dt = clock.restart().asSeconds();
		
			game.Update(dt);
			game.Draw();
	}
}