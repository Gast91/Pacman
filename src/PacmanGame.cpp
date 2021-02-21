#pragma once
#include <thread>
#include "Pacman.h"
#include "GhostPersonas.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(Config::WIDTH, Config::HEIGHT), "Pacman", sf::Style::Close);
    window.setFramerateLimit(10); // less?

    // Instantiate level, pacman and all the ghosts
    std::unique_ptr<Level> level = std::make_unique<Level>();
    const AStar aStar{ level.get() };
    std::unique_ptr<Pacman> pacman = std::unique_ptr<Pacman>(new Pacman(level.get(), { 13, 17 }));
    std::array<std::unique_ptr<Ghost>, 4> ghosts = {
        std::unique_ptr<Ghost>(new Ghost(Config::sprites::blinky, &aStar, { 13, 11 }, { 1, 1 }, { 11, 13 })),
        std::unique_ptr<Ghost>(new Pinky(&aStar, { 16, 13 }, { 26, 1 })),
        // Inky's movement depends on another ghost (Blinky by default)
        std::unique_ptr<Ghost>(new Inky (&aStar, ghosts[0].get(), { 11, 15 }, { 7 , 29})),
        std::unique_ptr<Ghost>(new Clyde(&aStar, { 16, 15 }, { 20, 29})),
    };

    // Register ghosts as level observers
    level->registerPacman(pacman.get());
    for (const auto& ghost : ghosts) level->registerObserver(ghost.get());

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
                case Config::Keybinds::START:
                    if (!level->hasStarted()) level->begin();
                    break;
                }
                break;
            default: break;
            }
        }

        pacman->move();
        level->update();
        for (auto& ghost : ghosts) ghost->move();

        window.clear();
        window.draw(*level);
        window.draw(*pacman);
        if (!level->gameOver())
        {
            for (const auto& ghost : ghosts)
            {
                window.draw(*ghost);
                // Draw debug pathfinding lines
                window.draw(ghost->debugLines(sf::Color::Red));
            }
        }
        window.display();
        if (level->isPaused()) std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    //_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
    return 0;
}