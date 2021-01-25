#include "Pacman.h"
#include "Ghost.h"

constexpr const char* blinkySpritesheet = "resources/sprites/blinky_spritesheet.png";   // Move each to the new derived class
constexpr const char* pinkySpritesheet  = "resources/sprites/pinky_spritesheet.png";
constexpr const char* inkySpritesheet   = "resources/sprites/inky_spritesheet.png";
constexpr const char* clydeSpritesheet  = "resources/sprites/clyde_spritesheet.png";
constexpr const char* huntedSpritesheet = "resources/sprites/ghost_spritesheet.png";   // move to base class?

int main()
{
	sf::RenderWindow window(sf::VideoMode(Config::WIDTH, Config::HEIGHT), "Pacman", sf::Style::Close);  // DECIDE NUMBERS! JUST INTS?
	window.setFramerateLimit(10); // less?

	Level level;
    Pacman pacman(&level, { 1, 1 });
    Ghost  blinky(blinkySpritesheet, huntedSpritesheet, &level, { 26, 1 }, { 1, 1 }, { 11, 13 });
    //Ghost  pinky(pinkySpritesheet, huntedSpritesheet, &level, { 26, 1 }, {26, 1}, { 16, 13 });

    level.registerPacman(&pacman);
    level.registerObserver(&blinky);

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
            blinky.move(); // needs some refactor preferably
        }

        // move this into ghost move? available during debug? drawn along with blinky? or better in a debug object? ALSO ONLY WHEN ASTAR IS HAPPENING
#ifndef CLASSIC
		sf::VertexArray lines(sf::LineStrip, blinky.path.size()); 
		for (int i = 0; i < blinky.path.size(); i++)
		{
			// make it a utility function?
			lines[i].position = sf::Vector2f(blinky.path[i]->gridPosition.x * Config::ENTITY_SIZE + Config::SCALED_OFFSET, 
                                             blinky.path[i]->gridPosition.y * Config::ENTITY_SIZE + Config::SCALED_OFFSET);
			lines[i].color = sf::Color::Red;
		}
#endif // !CLASSIC

		window.clear();
		window.draw(level);
		window.draw(pacman);
		window.draw(blinky);
#ifndef CLASSIC
        window.draw(lines); // experimental debug pathfind lines
#endif // !CLASSIC
		window.display();
	}
	return 0;
}