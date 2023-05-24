# Breakout++
[Breakout](https://en.wikipedia.org/wiki/Breakout_(video_game)) remade in C++ with some additional features. Originally made for a university project.

## Team Members
- Buck4437
- ChatterCube
- minghinshi
- PeterOwO
- raZylAWY

## Quick Start (WIP)

0. Make sure the dimensions of the terminal is over 50 rows * 200 columns.

1. Check the `src/ncu.h` header file
    - the header should be 
    > `#include <curses.h>` 
    - if it is meant to be compiled in linux

2. Compile the game by using makefile
    > `make main`

3. Run the Program
    > `./main`


## Controls
### Menu Navigation
When you enter Breakout++, you will see a screen with a couple of options (Play, Leaderboard, Credits, How to Play, Exit). You can control the cursor by using the Up and Down key. To select them, press the Enter key.

### Gameplay
Use the left and right arrow key to move the paddle horizontally. 
- Note that if you hold down an arrow key, there will be a delay between the paddle constantly moving. This is to do with how key inputs are handled. If you want to move constantly you will need to press an arrow key repeatedly.

You can pause by pressing "P". Navigate the pause menu using the arrow keys.

To fire a missile, press "C".

## Game Overview
Breakout++ is a remake of Breakout with the addition of power-ups.

In this game, you will be controlling a paddle at the bottom of the screen. Your main objective is the destroy a wall of bricks at the top of the screen, by repeatedly bouncing a ball towards them using the paddle.

Occasionally, power-ups are spawned when you break a bricks. They can help or hinder your progress in the game.

At the start of the game, you have 3 lifes. When a ball reaches the bottom of the screen, it will disappear and you will lose the ball. If all balls are gone from the playing field, you lose a life. The game ends automatically when you lose all lifes.

When you reach a score milestone, you gain an extra life. The first bonus is at 5000 points, and then 20000, 45000 etc. _(The formula is given by `5000 * N ^ 2`, where N = # of extra lifes)_

The main objective is to break all the bricks in a level without losing all of the lives. The level becomes progressively more difficult as the player advances in the game.

If the player scores enough points, they will be ranked in the leaderboard, allowing them to compete with each other for the highest score.



## Features and Implementation
There are a number of features in our game, which will be listed here.

### Generation of random game sets or events
The type of power-up that spawns is determined by a loot table. When a power-up is spawned, its time is determined by randomly picking a type from the loot table.

### Data structures for storing game status
Game statistics data (e.g. Points, multiplier etc) are stored as member variables of GameStat (Class).  

Individual blocks are represented as RectBlock/RectWall objects.

Blocks are stored together in a set within the Level class.

### Dynamic memory management

Pointers are used to access other class members (e.g PlayingField, GameStat).
Game elements such as blocks are stored in STL sets.
Heap memory is also used to store in-game entities such as balls and missiles.

### File I/O
Levels (and loot table, if any) are loaded from *.bl(breakout level) files in the "data/" directory.
When the player starts the game, the program reads from data/index.txt to determine the order of level that the game should load. It then reads the corresponding .bl files to load the level (and lthe level's oot table).

The leaderboard data is stored in data/leaderboard.txt, which is updated whenever the player scores a spot in the leaderboard.

### Program code in multiple files

Each function of the game are specialized into different class objects, which are stored in separate files (.cpp/.h).





## Dependencies
Our game uses the `ncurses` library to render the game. It allows us to display graphics easily on the terminal and to also support I/O events, needed for players to navigate menu. 





## Game mechanics (In-depth)

### Power-ups
When you break a brick, power-ups are spawned occasionally, which can either help or hinder your progress. Power-ups are spawned at the location of the broken brick, and slowly falls from the top. The effect of a power-up will only activate when you collect it using the paddle. Otherwise, it will fall to the bottom and despawn. The type of power-up spawned is determined by a loot table, which can be modified depending on the level settings.

All dropped power-ups will despawn if the player loses a life or clears a level. Furthermore, any power-up effects will also expire (including _Shield_ and _Missile_).

All power-ups have colour indicators to indicate whether it is a buff or a debuff. Green represents a buff while red represents a debuff.

### _Pad Expand_

Power-up Symbol: `<=>`

Power-up Type: Buff

This power-up expands the pad by 2 pixels, making it easier to catch the balls and power-ups. 
This power-up can cancel out the effect from _Pad Shrink_.
The effect can be stacked up to two times (expanded by 4 pixels compared to the base size).

### _Pad Shrink_

Power-up Symbol: `>=<`

Power-up Type: Nerf

This power-up shrinks the pad by 2 pixels, making it harder to catch the balls and power-ups. 
This power-up can cancel out the effect from _Pad Expand_.
The effect can be applied once only (shrank by 2 pixels compared to the base size).

### _Fast Ball_

Power-up Symbol: `o>>`

Power-up Type: Nerf

This power-up speeds up the ball, making it harder to catch the balls and power-ups. 
This power-up can cancel out the effect from  _Slow Ball_.
The effect can be stacked to further speed up the ball.
This power-up automatically expires after 200 frames, unless another _Fast Ball_ / _Slow Ball_ power-up is collected, which resets the timer.

### _Slow Ball_

Power-up Symbol: `o<<`

Power-up Type: Buff

This power-up slows up the ball, making it easier to catch the balls and power-ups. 
This power-up can cancel out the effect from  _Fast Ball_.
The effect can be stacked to further slow up the ball.
This power-up automatically expires after 200 frames, unless another _Fast Ball_ / _Slow Ball_ power-up is collected, which resets the timer.

### _Multiball_

Power-up Symbol: `ooo`

Power-up Type: Buff

This power-up spawns two extra balls to the game field, making it easier to destroy bricks and score points.
The balls spawned will persist until the player loses them, or the level is cleared.

### _Multiplier_

Power-up Symbol: `x 2`

Power-up Type: Buff

This power-up grants a temporary multiplier to score gain, starting at x2 and increases linearly.
The multiplier increases when another _Multiplier_ power-up is collected within its duration.
This power-up automatically expires after 300 frames, unless another _Multiplier_ power-up is collected, which resets the timer.
This power-up makes it easier to score higher points, for extra lifes and getting on the leaderboard.

### _Shield_

Power-up Symbol: `###`

Power-up Type: Buff

This power-up creates a shield at the bottom of the level, which prevent balls from escaping the playing field.
When another _Shield_ power-up is collection while the shield is activated, it levels up instead, up to a maximum level of 3.

The appearance of the shield changes when it levels up:

| Level  | Appearance |
| :----: | :------:   |
| 1      | `--------` |
| 2      | `========` |
| 3 (Max)| `########` |

If a shield is active, it can rebounce a ball when a ball reaches the bottom. After rebouncing a ball, the shield either disappears (level 1), or the shield level decreases (level 2+).

Note that the shield only prevents balls from escaping. **It cannot collect power-ups**.

### _Missile_

Power-up Symbol: `>=>`

Power-up Type: Buff

This power-up adds a missile to the player's inventory, which is helpful for aiming and clearing blocks manually.
This power-up can be stacked, and the player can own multiple missiles at the same time.

To launch a missile, press "C" on the keyboard. Three missiles will be launched from the paddle, at the left, right and centre position (This only consumes 1 missile from the player). When the missile hits a block, it despawns and breaks the block, if it can be broken.

### Pity System

When the player has cleared a certain percentage of bricks in the level, the pity system is activated. Every 500 frames, the game will select a random "Pity" power-up from a loot table, and it will be spawned from the top of the screen. The power-up can help player clearing the remaining bricks in the level.

The type of power-up spawned is determined by a special "pity loot table", which can also be modified depending on the level settings. By default, the pity system will only spawn _Multiball_ and _Missile_ power-ups.