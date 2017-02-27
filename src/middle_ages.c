/** @file
* Source code of main game file.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parse.h"
#include "engine.h"

#define ERROR_CODE 42
#define END_GAME_CODE 0

int main(void) {
    int seenInit = 0;
    int whatToDoWithGame;
    int inputCorectness;

    command lastInit;
    command *newCommand;

    startGame();

    while (true) {
        inputCorectness = parseCommand(&newCommand);
        if (inputCorectness != CORRECT_INPUT) {
            whatToDoWithGame = ERROR;
        }
        else {
            if (strcmp(newCommand->name, "INIT") == 0) {
                if (seenInit == 0) {
                    int i;
                    whatToDoWithGame = init(
                            newCommand->data[0],
                            newCommand->data[1],
                            newCommand->data[3],
                            newCommand->data[4],
                            newCommand->data[5],
                            newCommand->data[6]);

                    for (i = 0; i < 7; ++i) {
                        lastInit.data[i] = newCommand->data[i];
                    }

                    ++seenInit;

                    if (newCommand->data[2] == PLAYER_1) {
                        lastInit.data[2] = PLAYER_2;
                    } else if (newCommand->data[2] == PLAYER_2) {
                        lastInit.data[2] = PLAYER_1;
                    } else {
                        whatToDoWithGame = ERROR;
                    }

                    if (whatToDoWithGame != ERROR) {
                        printTopleft();
                    }

                } else if (seenInit == 1) {
                    int i;
                    whatToDoWithGame = CONTINUE_GAME;

                    ++seenInit;

                    for (i = 0; i < 7; ++i) {
                        if (lastInit.data[i] != newCommand->data[i]) {
                            whatToDoWithGame = ERROR;
                        }
                    }

                    if (whatToDoWithGame != ERROR) {
                        printTopleft();
                    }
                } else {
                    whatToDoWithGame = ERROR;
                }
            }
            else if (seenInit == 2) {
                if (strcmp(newCommand->name, "MOVE") == 0) {
                    whatToDoWithGame = move(
                            newCommand->data[0],
                            newCommand->data[1],
                            newCommand->data[2],
                            newCommand->data[3]);
                    if (whatToDoWithGame != ERROR) {
                        printTopleft();
                    }
                }
                else if (strcmp(newCommand->name, "PRODUCE_KNIGHT") == 0) {
                    whatToDoWithGame = produceKnight(
                            newCommand->data[0],
                            newCommand->data[1],
                            newCommand->data[2],
                            newCommand->data[3]);
                    if (whatToDoWithGame != ERROR) {
                        printTopleft();
                    }
                }
                else if (strcmp(newCommand->name, "PRODUCE_PEASANT") == 0) {
                    whatToDoWithGame = producePeasant(
                            newCommand->data[0],
                            newCommand->data[1],
                            newCommand->data[2],
                            newCommand->data[3]);
                    if (whatToDoWithGame != ERROR) {
                        printTopleft();
                    }
                }
                else if (strcmp(newCommand->name, "END_TURN") == 0) {
                    whatToDoWithGame = endTurn();

                }
                else {
                    whatToDoWithGame = ERROR;
                }
            } else {
                whatToDoWithGame = ERROR;
            }

            free(newCommand);
        }

        switch (whatToDoWithGame) {
            case CONTINUE_GAME:
                break;
            case END_GAME:
                endGame();
                exit(END_GAME_CODE);
            default:
                /*Either function returned END_GAME or some undefined value.
                 * Anyway, it means that an error has occurred.*/
                fprintf(stderr, "input error\n");
                endGame();
                exit(ERROR_CODE);
        }

    }

    return 0;
}
