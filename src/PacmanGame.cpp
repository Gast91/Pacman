#include "Pacman.h"
#include "GhostPersonas.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(Config::WIDTH, Config::HEIGHT), "Pacman", sf::Style::Close);  // DECIDE NUMBERS! JUST INTS?
	window.setFramerateLimit(10); // less?

	Level level;
    Pacman pacman(&level, { 1, 1 });
    //Ghost  blinky(&level, { 26, 1 }, { 1, 1 }, { 11, 13 });     // start pos for most is home/frightened right?
    Clyde  clyde(&level, { 26, 1 }, { 26, 29 }, { 16, 15 });
    //Pinky   pinky(&level, { 26, 1 }, { 26, 1 }, { 16, 13 });

    level.registerPacman(&pacman);                                // POLY FOR ALL REGISTERS,MOVEMENTS, DRAWS!!
    //level.registerObserver(&blinky);
    //level.registerObserver(&pinky);
    //level.registerObserver(&inky);
    //inky.registerObserver(&blinky);   <----
    level.registerObserver(&clyde);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
                case Config::Keybinds::ESC:
					window.close();
					break;
				case Config::Keybinds::UP:
					pacman.changeDirection(NORTH);
					break;
				case Config::Keybinds::DOWN:
					pacman.changeDirection(SOUTH);
					break;
				case Config::Keybinds::LEFT:
					pacman.changeDirection(WEST);
					break;
				case Config::Keybinds::RIGHT:
					pacman.changeDirection(EAST);
					break;
				}
				break;
			default:
				break;
			}
		}

        if (!level.gameOver())  // needs work
        {
            pacman.move();
            level.update();
            //blinky.move();
            //pinky.move();
            //inky.move();
            clyde.move();
        }

		window.clear();
		window.draw(level);
		window.draw(pacman);
		//window.draw(blinky);
        //window.draw(pinky);
        //window.draw(inky);
        window.draw(clyde);
#ifndef CLASSIC
        window.draw(clyde.debugLines()); // experimental debug pathfind lines
#endif // !CLASSIC
		window.display();
	}
	return 0;
}