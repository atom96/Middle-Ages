/** @file
* Interface of game engine.
*/

#ifndef ENGINE_H
#define ENGINE_H

#define ERROR 2
#define END_GAME 1
#define CONTINUE_GAME 0
#define PLAYER_1 1
#define PLAYER_2 2


/**
 * Initializes a game. Needed before first INIT.
 */
void startGame();

/**
 * Frees memory. Needed after finishing game.
 */
void endGame();

/**
 * Initializes a game with size of a board, number of rounds and positions of
 * kings.
 * @param[in] boardSize size of a board
 * @param[in] maxNumberOfRounds maximal number of rounds which can be played
 * @param[in] xKingPlayer1 Number of a column in which Player1's King will be
 * located
 * @param[in] xKingPlayer1 Number of a row in which Player1's King will be
 * located
 * @param[in] xKingPlayer2 Number of a column in which Player2's King will be
 * located
 * @param[in] xKingPlayer2 Number of a row in which Player2's King will be
 * located
 * @return
 * CONTINUE_GAME if game has to be continued <br>
 * END_GAME if game ended in standard way (player won or it is a draw).
 * Proper communicate has been already printed to stderr <br>
 * ERROR if it is illegal to make this operation
 */

int init(long int boardSize, long int maxNumberOfRounds, long int xKingPlayer1,
		long int yKingPlayer1, long int xKingPlayer2, long int yKingPlayer2);

/**
 * Makes a move.
 * @param[in] xCurrent Column number before a move.
 * @param[in] yCurrent Row number before a move.
 * @param[in] xDestined Column number after a move.
 * @param[in] yDestined Row number before a move.
 * @return
 * CONTINUE_GAME if game has to be continued <br>
 * END_GAME if game ended in standard way (player won or it is a draw).
 * Proper communicate has been already printed to stderr <br>
 * ERROR if it is illegal to make this operation
 */
int move(long int xCurrent, long int yCurrent, long int xDestined, long int
	yDestined);

/**
 * Allows Peasant in column xCurrent and row yCurrent to produce Knight
 * in column xDestined and row yDestined
 * @param[in] xCurrent Column number of a Peasant.
 * @param[in] yCurrent Row number before of a Peasant.
 * @param[in] xDestined Column number of Knight which has to be produced.
 * @param[in] yDestined Row number of Knight which has to be produced.
 * @return
 * CONTINUE_GAME if game has to be continued <br>
 * END_GAME if game ended in standard way (player won or it is a draw).
 * Proper communicate has been already printed to stderr <br>
 * ERROR if it is illegal to make this operation
 */

int produceKnight(long int xCurrent, long int yCurrent, long int xDestined,
		long int yDesined);

/**
* Allows Peasant in column xCurrent and row yCurrent to produce Peasant
* in column xDestined and row yDestined.
* @param[in] xCurrent Column number of a Peasant.
* @param[in] yCurrent Row number before of a Peasant.
* @param[in] xDestined Column number of Peasant which has to be produced.
* @param[in] yDestined Row number of Peasant which has to be produced.
* @return
* CONTINUE_GAME if game has to be continued <br>
* END_GAME if game ended in standard way (player won or it is a draw).
* Proper communicate has been already printed to stderr <br>
* ERROR if it is illegal to make this operation
*/

int producePeasant(long int xCurrent, long int yCurrent, long int xDestined,
		long int yDestined);

/**
* Ends turn for current player
* @return
* CONTINUE_GAME if game has to be continued; <br>
* END_GAME if game ended in standard way (it is a draw).
* Proper communicate has been already printed to stderr <br>
*/

int endTurn();

/**
 * Prints (into stdout) top-left corner of the board of size m x m where
 * m = min(n,10).
 */

void printTopleft();

#endif /* ENGINE_H */
