Middle ages - board game. This software is a judge - it will write proper text to stderr 
when one of the players will win. 

Author: Arkadiusz Tomczak</br>
Version: 1.0</br>
Date: 28-04-2016</br>
Language:
 - Program language: English
 - Variable names: English
 - Comments: English

It's simple board game. We have 3 types of units - King, Knight and Peasant. Each player can has only 
one King. The main goal is to kill other players King. Each unit is able to move 1 step in every
direction (also diagonally). Peasant, instead of moving, can produce another Peasant or Knight, but only once 
in 3 rounds and in the immediate vicinity of himself.
If unit steps on field with another unit (second unit must belong to other player) there is a battle.
Rules are simple: King wins with Knight and Peasant, Knight wins with Peasant, if units types are the same, both die.
Game can end with draw if there would be more than k rounds (user can specify k value).
Program accepts those commands in the input:
- `INIT n k p x1 y1 x2 y2` (n - board size (board is a square), k - max num. of rounds, p - who starts, xi, yi - king position)
- `MOVE x1 y1 x2 y2` (move unit from x1 y1 to x2 y2)
- `PRODUCE_KNIGHT x1 y1 x2 y2` (Peasant in position x1 y1 has to produce Knight in field x2 y2)
- `PRODUCE_PEASANT x1 y1 x2 y2` (Same as above, but producing Peasant instead of Knight)
- `END_TURN` - ends turn of player, who sent this command

After every move program prints chunk of board sized 10*10 (but the gameboard can be a lot bigger).
At the beginnig, each player has one King, two Knights and one Peasant (who are located on board like this: QKKP
where Q means the King).

If input is incorrect, program will end with error code 42.
More specyfic info is in doxygen documentation (in Polish).
