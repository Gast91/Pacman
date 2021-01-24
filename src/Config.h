#pragma once

#define NORTH sf::Vector2i{ 0, -1}
#define SOUTH sf::Vector2i{ 0,  1}
#define WEST  sf::Vector2i{-1,  0}
#define EAST  sf::Vector2i{ 1,  0}

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
	const int ROWS = 28, COLS = 31;  // HACKED IN (31 x 28) - change to std::array?
}
// MAKE LEVEL CONST REFERENCE <-------------------

// COMPLETED:
// 1. Change way movement works:
//       Button presses change a member turnNext(?) variable
//       When pacman reaches a turnable tile (node implementation as special tile?) and can turn at the command specified->turn
//       Keep moving towards the current direction until you can turn or you hit a wall
//       Research animations when pacman unable to move and maybe further movement
//-----------------------------------------------------------------------------------------------------------------------------
// 2. Get a ghost in the game:
//       Make a ghost's spritesheet
//       Populate the ghost class
//-----------------------------------------------------------------------------------------------------------------------------
// 3. Start work on nodes:
//       Research whether the base node idea can work for pathfinding
//       Make changes to map to accommodate nodes (if needed) and implement it
//-----------------------------------------------------------------------------------------------------------------------------
// 4. Implement pathfinding:
//       A* class/algorithm
//       Output of A* goes to ghost movement
//       Ghost chases pacman
//-----------------------------------------------------------------------------------------------------------------------------

// TODO:
// 5. Get all the ghosts in the game:
//       Make the rest of the spritesheets
//       Implement personalities for the ghosts - REVISIT poly
//-----------------------------------------------------------------------------------------------------------------------------
// 6. Actual game logic:
//       Pacman states (alive - dead)
//       Ghost states (hunting - hunted - dead)
//       Win / Loss
//       Multiple lives
//-----------------------------------------------------------------------------------------------------------------------------
// 7. Final sprites / game updates:
//       Ghost hunted spritesheet
//       Ghost dead spritesheet
//       Lives counter
//-----------------------------------------------------------------------------------------------------------------------------
// 8. Text - Media:
//       Score counter (class like snake?)
//       Game sound
//-----------------------------------------------------------------------------------------------------------------------------
// 9. Cleanup:
//       Various codebase improvements (consts, pointers, refs...)
//       Cmd arguments
//       Error handling for resource files and error messages
//       .... :)