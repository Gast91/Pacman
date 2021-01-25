#pragma once

#define NORTH sf::Vector2i{ 0, -1}
#define SOUTH sf::Vector2i{ 0,  1}
#define WEST  sf::Vector2i{-1,  0}
#define EAST  sf::Vector2i{ 1,  0}

enum GhostState { Chase, Scatter, Frightened, Dead };

namespace Config 
{
  // Overall Scale of the game - Change ONLY this to adjust game size
	const float SCALE = 1.5f;  // No limit atm or dynamic (cmd also?)
  //-----------------------------------------------------------------
  //--------------------GAME WINDOW SIZE-----------------------------
  //-----------------------------------------------------------------
	const float WIDTH  = 448 * SCALE;
	const float HEIGHT = 496 * SCALE;
  //-----------------------------------------------------------------
  //--------------------GAME ENTITIES SIZES--------------------------
  //-----------------------------------------------------------------
	const float ENTITY_SIZE = 16.0f * SCALE;
	const float SCALED_OFFSET = ENTITY_SIZE / ( 3 * SCALE );
	const float DOT_SIZE    = 2.5f  * SCALE;
	const float B_DOT_SIZE  = 5.0f  * SCALE;
  //-----------------------------------------------------------------
	const int ROWS = 28, COLS = 31;  // change to std::array?
}
// MAKE LEVEL CONST REFERENCE <-------------------

// TODO:
// 5. Get all the ghosts in the game:
//       Implement personalities for the ghosts - REVISIT poly
//-----------------------------------------------------------------------------------------------------------------------------
// 6. Actual game logic:
//       Pacman states (alive - dead)
//       Win / Loss
//       Multiple lives
//-----------------------------------------------------------------------------------------------------------------------------
// 7. Text - Media:
//       Score counter (class like snake?)
//       Lives counter
//       Game sound
//-----------------------------------------------------------------------------------------------------------------------------
// 8. Cleanup:
//       Various codebase improvements (consts, pointers, refs...)
//       Cmd arguments
//       Error handling for resource files and error messages
//       .... :)