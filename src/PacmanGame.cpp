#include "Pacman.h"
#include "GhostPersonas.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(Config::WIDTH, Config::HEIGHT), "Pacman", sf::Style::Close);  // DECIDE NUMBERS! JUST INTS?
	window.setFramerateLimit(10); // less?

    // Instantiate level, pacman and all the ghosts
    std::unique_ptr<Level> level = std::make_unique<Level>();
    std::unique_ptr<Pacman> pacman = std::unique_ptr<Pacman>(new Pacman(level.get(), { 13, 17 }));
    std::array<std::unique_ptr<Ghost>, 4> ghosts = {        // start pos for most is home/frightened right?
        std::unique_ptr<Ghost>(new Ghost(Config::sprites::blinky, level.get(), { 1, 1 }, { 1, 1 }, { 11, 13 })),
        std::unique_ptr<Ghost>(new Pinky(level.get(), { 26, 1 }, { 26, 1 }, { 16, 13 })),
        std::unique_ptr<Ghost>(new Inky(level.get(), { 1, 29 }, { 1, 29 }, { 11, 15 })),
        std::unique_ptr<Ghost>(new Clyde(level.get(), { 26, 29 }, { 26, 29 }, { 16, 15 })),
    };

    // Register level observers (Inky also observes Blinky since its movement depends on it)
    level->registerPacman(pacman.get());
    for (auto& ghost : ghosts) level->registerObserver(ghost.get());
    ghosts[2]->registerObserver(ghosts[0].get());                       // <---- kinda meh

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
					pacman->changeDirection(NORTH);
					break;
				case Config::Keybinds::DOWN:
					pacman->changeDirection(SOUTH);
					break;
				case Config::Keybinds::LEFT:
					pacman->changeDirection(WEST);
					break;
				case Config::Keybinds::RIGHT:
					pacman->changeDirection(EAST);
					break;
				}
				break;
			default:
				break;
			}
		}

        if (!level->gameOver())  // needs work
        {
            pacman->move();
            level->update();
            for (auto& ghost : ghosts) ghost->move();
        }

		window.clear();
		window.draw(*level);
		window.draw(*pacman);
        for (auto& ghost : ghosts) window.draw(*ghost);
        // Display debug pathfinding lines
#ifndef CLASSIC
        for (auto& ghost : ghosts) window.draw(ghost->debugLines());
#endif // !CLASSIC
		window.display();
	}
    //_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
	return 0;
}