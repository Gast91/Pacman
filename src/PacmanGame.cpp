#include "Pacman.h"
#include "Ghost.h"

//Keybinds  - move to config  (inside another namespace?)
constexpr sf::Keyboard::Key UP          = sf::Keyboard::Key::W;
constexpr sf::Keyboard::Key DOWN        = sf::Keyboard::Key::S;
constexpr sf::Keyboard::Key LEFT        = sf::Keyboard::Key::A;
constexpr sf::Keyboard::Key RIGHT       = sf::Keyboard::Key::D;
constexpr sf::Keyboard::Key ESC         = sf::Keyboard::Key::Escape;

constexpr const char* blinkySpriteSheet = "resources/sprites/blinky_spritesheet.png";   // Move to config??
constexpr const char* huntedSpritesheet = "resources/sprites/ghost_spritesheet.png";

int main()
{
	sf::RenderWindow window(sf::VideoMode(Config::WIDTH, Config::HEIGHT), "Pacman", sf::Style::Close);  // DECIDE NUMBERS! JUST INTS?
	window.setFramerateLimit(10); // less?

	Level level;
    Pacman pacman(&level, { 1, 1 });  // a design pattern for level and entities (observer?)
    Ghost  blinky(blinkySpriteSheet, huntedSpritesheet, &level, { 26, 1 }, { 1, 1 }, { 11, 13 });

	// no blinky class but rather different pathfinding algorithms(personalities)
    // ghost constructor takes starting position and pathfinding/personality algorithm
    // Pinky(Pink) and Inky(Cyan) try to position themselves in front of Pac-Man, usually by cornering him, 
    // and Clyde(Orange) will switch between chasing Pac-Man and fleeing from him.

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
				case ESC:
					window.close();
					break;
				case UP:
					pacman.changeDirection(NORTH);
					break;
				case DOWN:
					pacman.changeDirection(SOUTH);
					break;
				case LEFT:	
					pacman.changeDirection(WEST);
					break;
				case RIGHT:
					pacman.changeDirection(EAST);
					break;
                case sf::Keyboard::Key::Space:
                    blinky.debugTargeting();
                    break;
				}
				break;
			default:
				break;
			}
		}

        pacman.move();
        blinky.move(); // needs some refactor preferably

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