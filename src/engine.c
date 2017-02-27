/** @file
* Source code of game engine
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define KING 'K'
#define KNIGHT 'R'
#define PEASANT 'C'
#define CORNER_SIZE 10

#define ERROR 2
#define END_GAME 1
#define CONTINUE_GAME 0

#define MIN_BOARD_SIZE 8
#define MIN_NUM_OF_ROUNDS 0
#define MAX_NUM_OF_ROUNDS 2147483648
#define MIN_DISTANCE 8

#define EMPTY_FIELD '.'
#define KNIGHT_PL1 'R'
#define KNIGHT_PL2 'r'
#define KING_PL1 'K'
#define KING_PL2 'k'
#define PEASANT_PL1 'C'
#define PEASANT_PL2 'c'

#define PLAYER_1 1
#define PLAYER_2 2

struct unitNode;

typedef struct unitNode unitNode;

struct unitNode {
    char type;
    long int x;
    long int y;
    int playerWhoOwns;
    long int roundWhenMoved;
    unitNode *nextUnit;
};

struct board {
    unitNode *unitsOnBoard;
    char topLeft[CORNER_SIZE][CORNER_SIZE];
    long int topLeftPrintSize;
    long int boardSize;
    long int maxNumberOfRounds;
    long int currentRound;
    int currentPlayer;
};

typedef struct board board;

static board *mainBoard;

static long int absoluteValue(long int x) {
    if (x >= 0) {
        return x;
    }
    else {
        return -x;
    }
}

static long int maxValue(long int number1, long int number2) {
    if (number1 >= number2) {
        return number1;
    }
    else {
        return number2;
    }
}

static long int minValue(long int number1, long int number2) {
    if (number1 <= number2) {
        return number1;
    }
    else {
        return number2;
    }
}

static long int maximumMetric(long int x1, long int y1, long int x2,
                              long int y2) {

    return maxValue(absoluteValue(x1 - x2), absoluteValue(y1 - y2));
}

static void endAsDraw() {
    fprintf(stderr, "draw\n");
}

static void endAsWin(int playerWhoWon) {
    fprintf(stderr, "player %d won\n", playerWhoWon);
}

static char unitCharRepresentation(char unitType, int playerWhoOwns) {
    if (playerWhoOwns == PLAYER_1) {
        switch (unitType) {
            case KING:
                return KING_PL1;
            case KNIGHT:
                return KNIGHT_PL1;
            case PEASANT:
                return PEASANT_PL1;
            default:
                assert(false);
        }
    }
    else {
        switch (unitType) {
            case KING:
                return KING_PL2;
            case KNIGHT:
                return KNIGHT_PL2;
            case PEASANT:
                return PEASANT_PL2;
            default:
                assert(false);
        }
    }
}

static void addUnit(char unitType, long int x, long int y, int playerWhoOwns) {
    unitNode *newUnit = malloc(sizeof *newUnit);

    newUnit->type = unitType;
    newUnit->x = x;
    newUnit->y = y;
    newUnit->playerWhoOwns = playerWhoOwns;
    newUnit->roundWhenMoved = mainBoard->currentRound - 1;
    newUnit->nextUnit = mainBoard->unitsOnBoard;
    mainBoard->unitsOnBoard = newUnit;

    if (x <= CORNER_SIZE && y <= CORNER_SIZE) {
        mainBoard->topLeft[y - 1][x - 1] = unitCharRepresentation(
                unitType,
                playerWhoOwns
        );
    }
}

static unitNode *findUnit(long int x, long int y) {
    unitNode *currentNode = mainBoard->unitsOnBoard;
    while (currentNode != NULL) {
        if (currentNode->x == x && currentNode->y == y) {
            return currentNode;
        }
        currentNode = currentNode->nextUnit;
    }

    return NULL;
}

static void removeUnit(long int x, long int y) {
    unitNode *listGuardian = malloc(sizeof *listGuardian);
    unitNode *previousNode = listGuardian;
    unitNode *currentNode = mainBoard->unitsOnBoard;

    listGuardian->nextUnit = currentNode;

    while (currentNode != NULL) {
        if (currentNode->x == x && currentNode->y == y) {
            previousNode->nextUnit = currentNode->nextUnit;

            if (x <= CORNER_SIZE && y <= CORNER_SIZE) {
                mainBoard->topLeft[y - 1][x - 1] = EMPTY_FIELD;
            }

            break;
        }
        previousNode = currentNode;
        currentNode = currentNode->nextUnit;
    }

    assert (currentNode != NULL);

    free(currentNode);
    mainBoard->unitsOnBoard = listGuardian->nextUnit;
    free(listGuardian);
}

extern void startGame() {
    int i, j;
    mainBoard = malloc(sizeof *mainBoard);
    mainBoard->currentRound = 1;
    mainBoard->currentPlayer = PLAYER_1;
    mainBoard->unitsOnBoard = NULL;

    for (i = 0; i < CORNER_SIZE; ++i) {
        for (j = 0; j < CORNER_SIZE; ++j) {
            mainBoard->topLeft[i][j] = EMPTY_FIELD;
        }
    }
}

extern void endGame() {
    unitNode *currentList = mainBoard->unitsOnBoard;
    unitNode *nodeToDelete;

    while (currentList != NULL) {
        nodeToDelete = currentList;
        currentList = currentList->nextUnit;
        free(nodeToDelete);
    }

    free(mainBoard);
}

extern void printTopleft() {
    int i, j;

    for (i = 0; i < mainBoard->topLeftPrintSize; ++i) {
        for (j = 0; j < mainBoard->topLeftPrintSize; ++j) {
            putchar(mainBoard->topLeft[i][j]);
        }

        putchar('\n');
    }

    putchar('\n');
}

extern int init(long int boardSize, long int maxNumberOfRounds,
                long int xKingPlayer1, long int yKingPlayer1,
                long int xKingPlayer2,
                long int yKingPlayer2) {

    if (
            boardSize <= MIN_BOARD_SIZE ||
            maxNumberOfRounds <= MIN_NUM_OF_ROUNDS ||
            maxNumberOfRounds >= MAX_NUM_OF_ROUNDS ||
            xKingPlayer1 <= 0 ||
            xKingPlayer2 <= 0 ||
            yKingPlayer1 <= 0 ||
            yKingPlayer2 <= 0 ||
            xKingPlayer1 > boardSize - 3 ||
            xKingPlayer2 > boardSize - 3 ||
            yKingPlayer1 > boardSize ||
            yKingPlayer2 > boardSize ||
            maximumMetric(
                    xKingPlayer1,
                    yKingPlayer1,
                    xKingPlayer2,
                    yKingPlayer2
            ) < MIN_DISTANCE) {
        return ERROR;
    }

    mainBoard->boardSize = boardSize;
    mainBoard->maxNumberOfRounds = maxNumberOfRounds;
    mainBoard->topLeftPrintSize = minValue(boardSize, CORNER_SIZE);

    addUnit(KING, xKingPlayer1, yKingPlayer1, PLAYER_1);
    addUnit(PEASANT, xKingPlayer1 + 1, yKingPlayer1, PLAYER_1);
    addUnit(KNIGHT, xKingPlayer1 + 2, yKingPlayer1, PLAYER_1);
    addUnit(KNIGHT, xKingPlayer1 + 3, yKingPlayer1, PLAYER_1);

    addUnit(KING, xKingPlayer2, yKingPlayer2, PLAYER_2);
    addUnit(PEASANT, xKingPlayer2 + 1, yKingPlayer2, PLAYER_2);
    addUnit(KNIGHT, xKingPlayer2 + 2, yKingPlayer2, PLAYER_2);
    addUnit(KNIGHT, xKingPlayer2 + 3, yKingPlayer2, PLAYER_2);

    return CONTINUE_GAME;
}

static unitNode *whoWinBattle(unitNode *unit1, unitNode *unit2) {
    if (unit1->type == unit2->type) {
        return NULL;
    }

    switch (unit1->type) {
        case KNIGHT:
            return unit1;
        case PEASANT:
            return unit2;
        case KING:
            if (unit2->type == KNIGHT) {
                return unit2;
            } else {
                return unit1;
            }
        default:
            assert(false);
    }
}

static void moveTo(unitNode *unit, long int x, long int y) {
    if (unit->x <= CORNER_SIZE && unit->y <= CORNER_SIZE) {
        mainBoard->topLeft[unit->y - 1][unit->x - 1] = EMPTY_FIELD;
    }

    unit->x = x;
    unit->y = y;
    unit->roundWhenMoved = mainBoard->currentRound;

    if (x <= CORNER_SIZE && y <= CORNER_SIZE) {
        mainBoard->topLeft[y - 1][x - 1] = unitCharRepresentation(
                unit->type,
                unit->playerWhoOwns
        );
    }
}

extern int move(long int xCurrent, long int yCurrent, long int xDestined,
                long int yDestined) {

    unitNode *currentUnit = findUnit(xCurrent, yCurrent);
    unitNode *unitOnField = findUnit(xDestined, yDestined);

    if (
            xCurrent < 0 ||
            xDestined < 0 ||
            yCurrent < 0 ||
            yDestined < 0 ||
            xCurrent > mainBoard->boardSize ||
            xDestined > mainBoard->boardSize ||
            yCurrent > mainBoard->boardSize ||
            yDestined > mainBoard->boardSize ||
            maximumMetric(xCurrent, yCurrent, xDestined, yDestined) != 1 ||
            currentUnit == NULL ||
            currentUnit->roundWhenMoved == mainBoard->currentRound) {
        return ERROR;
    }

    if (unitOnField != NULL && unitOnField->playerWhoOwns
                               == currentUnit->playerWhoOwns) {

        return ERROR;
    }

    if (unitOnField == NULL) {
        moveTo(currentUnit, xDestined, yDestined);
    } else {
        unitNode *unitWhichWon = whoWinBattle(currentUnit, unitOnField);

        if (unitWhichWon == NULL) {
            char unitType = currentUnit->type;

            removeUnit(currentUnit->x, currentUnit->y);
            removeUnit(unitOnField->x, unitOnField->y);

            if (unitType == KING) {
                endAsDraw();
                return END_GAME;
            }
        }
        else if (unitWhichWon == currentUnit) {
            char unitType = unitOnField->type;

            removeUnit(unitOnField->x, unitOnField->y);
            moveTo(currentUnit, xDestined, yDestined);
            if (unitType == KING) {
                endAsWin(currentUnit->playerWhoOwns);
                return END_GAME;
            }
        }
        else if (unitWhichWon == unitOnField) {
            char unitType = currentUnit->type;

            removeUnit(currentUnit->x, currentUnit->y);

            if (unitType == KING) {
                endAsWin(unitOnField->playerWhoOwns);
                return END_GAME;
            }
        }
    }
    return CONTINUE_GAME;
}

extern int produceKnight(long int xCurrent, long int yCurrent,
                         long int xDestined, long int yDesined) {

    unitNode *currentPeasant = findUnit(xCurrent, yCurrent);

    if (
            currentPeasant == NULL ||
            currentPeasant->type != PEASANT ||
            currentPeasant->playerWhoOwns != mainBoard->currentPlayer ||
            currentPeasant->roundWhenMoved + 3 > mainBoard->currentRound ||
            maximumMetric(xCurrent, yCurrent, xDestined, yDesined) != 1 ||
            findUnit(xDestined, yDesined) != NULL ||
            xDestined > mainBoard->boardSize ||
            yDesined > mainBoard->boardSize) {
        return ERROR;
    }

    addUnit(KNIGHT, xDestined, yDesined, currentPeasant->playerWhoOwns);
    return CONTINUE_GAME;
}

extern int producePeasant(long int xCurrent, long int yCurrent,
                          long int xDestined, long int yDestined) {
    unitNode *currentPeasant = findUnit(xCurrent, yCurrent);

    if (
            currentPeasant == NULL ||
            currentPeasant->type != PEASANT ||
            currentPeasant->playerWhoOwns != mainBoard->currentPlayer ||
            currentPeasant->roundWhenMoved + 3 > mainBoard->currentRound ||
            maximumMetric(xCurrent, yCurrent, xDestined, yDestined) != 1 ||
            findUnit(xDestined, yDestined) != NULL ||
            xDestined > mainBoard->boardSize ||
            yDestined > mainBoard->boardSize) {
        return ERROR;
    }

    addUnit(PEASANT, xDestined, yDestined, currentPeasant->playerWhoOwns);
    return CONTINUE_GAME;
}

extern int endTurn() {
    if (mainBoard->currentPlayer == PLAYER_1) {
        mainBoard->currentPlayer = PLAYER_2;
    }
    else {
        mainBoard->currentPlayer = PLAYER_1;
        mainBoard->currentRound += 1;
        if (mainBoard->currentRound > mainBoard->maxNumberOfRounds) {
            endAsDraw();
            return END_GAME;
        }
    }
    return CONTINUE_GAME;
}
