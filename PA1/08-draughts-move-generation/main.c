#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 1000

typedef struct {
    char type; // 0 - nothing // 1 - stage // b,B,w,W - ...
    char coordinateColumn;
    int coordinateRow;
    int x;
    int y;
} TTile;

TTile makeTile(int tileType, char tileCoordinateColumn, int tileCoordinateRow, int x, int y) {
    TTile res;
    res.type = tileType;
    res.coordinateColumn = tileCoordinateColumn;
    res.coordinateRow = tileCoordinateRow;
    res.x = x;
    res.y = y;
    return res;
}

int getTileIndex(TTile area[], int areaSize, int x, int y) {
    return (x * areaSize + y);
}

bool checkInputValidity(TTile area[], int areaSize, char tileType, char space, char tileCoordinateColumn, int tileCoordinateRow, char endLine) {
    if (tileType != 'w' && tileType != 'W' && tileType != 'b' && tileType != 'B') return false;
    if (space != ' ' || endLine != '\n') return false;
    if (tileCoordinateRow < 1 || tileCoordinateRow > areaSize) return false;
    if (tileCoordinateColumn < 'a' || tileCoordinateColumn >= 'a' + areaSize) return false;
    if (area[(tileCoordinateRow - 1) * areaSize + (tileCoordinateColumn - 'a')].type != 0) return false;
    if ((tileCoordinateRow % 2 == 0 && (tileCoordinateColumn - 'a') % 2 == 0) || (tileCoordinateRow % 2 != 0 && (tileCoordinateColumn - 'a') % 2 != 0)) return false;
    return true;
}

int isValidPos(TTile area[], int areaSize, int x, int y) {
    if (x < 0 || x >= areaSize || y < 0 || y >= areaSize || area[getTileIndex(area, areaSize, x, y)].type == 'w' || area[getTileIndex(area, areaSize, x, y)].type == 'W') return 2;
    if (area[getTileIndex(area, areaSize, x, y)].type != 0) return 1;
    return 0;
}

static int stepsSum = 0;

void printMovesSupfuncw(TTile area[], int areaSize, TTile white, int steps, int captured, TTile cache[]) {

    int directions[2][2] = {
        {1, 1},
        {1, -1}
    };
    for (int i = 0; i < 2; i++) {
        if (captured <= 0) {
            if (isValidPos(area, areaSize, white.x + directions[i][0], white.y + directions[i][1]) == 0) {
                printf("%c%d -> %c%d\n", white.coordinateColumn, white.coordinateRow, area[getTileIndex(area, areaSize, white.x + directions[i][0], white.y + directions[i][1])].coordinateColumn, area[getTileIndex(area, areaSize, white.x + directions[i][0], white.y + directions[i][1])].coordinateRow);
                stepsSum++;
            }
        }
        if (isValidPos(area, areaSize, white.x + directions[i][0], white.y + directions[i][1]) == 1) {
            if (isValidPos(area, areaSize, white.x + directions[i][0] * 2, white.y + directions[i][1] * 2) == 0) {
                cache[steps] = white;
                for (int i = 0; i <= steps; i++) {
                    printf("%c%d -> ", cache[i].coordinateColumn, cache[i].coordinateRow);
                }
                printf("%c%d", area[getTileIndex(area, areaSize, white.x + directions[i][0] * 2, white.y + directions[i][1] * 2)].coordinateColumn, area[getTileIndex(area, areaSize, white.x + directions[i][0] * 2, white.y + directions[i][1] * 2)].coordinateRow);
                printf(" +%d\n", captured + 1);
                stepsSum++;
                printMovesSupfuncw(area, areaSize, area[getTileIndex(area, areaSize, white.x + directions[i][0] * 2, white.y + directions[i][1] * 2)], steps + 1, captured + 1, cache);
            }
        }
    }

}

void printMovesSupfuncW(TTile area[], int areaSize, TTile white, int steps, int captured, TTile cache[], int prevDirection) {
    int directions[4][2] = {
        {1, 1},
        {1, -1},
        {-1, -1},
        {-1, 1}
    };
    if (captured <= 0) {
        for (int i = 0; i < 4; i++) {
            for (int j = 1; j < areaSize; j++) {
                if (isValidPos(area, areaSize, white.x + directions[i][0] * j, white.y + directions[i][1] * j) == 0) {
                    printf("%c%d -> %c%d\n", white.coordinateColumn, white.coordinateRow, area[getTileIndex(area, areaSize, white.x + directions[i][0] * j, white.y + directions[i][1] * j)].coordinateColumn, area[getTileIndex(area, areaSize, white.x + directions[i][0] * j, white.y + directions[i][1] * j)].coordinateRow);
                    stepsSum++;
                }
                if (isValidPos(area, areaSize, white.x + directions[i][0] * j, white.y + directions[i][1] * j) == 1) {
                    if (isValidPos(area, areaSize, white.x + directions[i][0] * (j + 1), white.y + directions[i][1] * (j + 1)) != 0) {
                        break;
                    }
                    else {
                        TTile tempArea[MAX_SIZE];
                        memcpy(tempArea, area, MAX_SIZE * sizeof(TTile));
                        tempArea[getTileIndex(tempArea, areaSize, white.x + directions[i][0] * j, white.y + directions[i][1] * j)].type = 0;
                        cache[steps] = white;
                        tempArea[getTileIndex(tempArea, areaSize, cache[steps].x, cache[steps].y)].type = 0;
                        printMovesSupfuncW(tempArea, areaSize, tempArea[getTileIndex(tempArea, areaSize, white.x + directions[i][0] * (j + 1), white.y + directions[i][1] * (j + 1))], steps + 1, captured + 1, cache, i);
                        break;
                    }
                }
                if (isValidPos(area, areaSize, white.x + directions[i][0] * j, white.y + directions[i][1] * j) == 2) break;
            }
        }
    }
    else {
        int l = 0;
        while (isValidPos(area, areaSize, white.x + directions[prevDirection][0] * l, white.y + directions[prevDirection][1] * l) == 0) {
            for (int i = 0; i < steps; i++) {
                printf("%c%d -> ", cache[i].coordinateColumn, cache[i].coordinateRow);
            }
            printf("%c%d", area[getTileIndex(area, areaSize, white.x + directions[prevDirection][0] * l, white.y + directions[prevDirection][1] * l)].coordinateColumn, area[getTileIndex(area, areaSize, white.x + directions[prevDirection][0] * l, white.y + directions[prevDirection][1] * l)].coordinateRow);
            printf(" +%d\n", captured);
            stepsSum++;
            for (int i = 0; i < 4; i++) {
                if ((directions[i][0] != -directions[prevDirection][0] || directions[i][1] != -directions[prevDirection][1])) {
                    for (int k = 0; k < areaSize; k++) {
                        if (isValidPos(area, areaSize, white.x + directions[prevDirection][0] * l + directions[i][0] * k, white.y + directions[prevDirection][1] * l + directions[i][1] * k) == 1) {
                            if (isValidPos(area, areaSize, white.x + directions[prevDirection][0] * l + directions[i][0] * (k + 1), white.y + directions[prevDirection][1] * l + directions[i][1] * (k + 1)) == 0) {
                                TTile tempArea[MAX_SIZE];
                                memcpy(tempArea, area, MAX_SIZE * sizeof(TTile));
                                tempArea[getTileIndex(tempArea, areaSize, white.x + directions[prevDirection][0] * l + directions[i][0] * k, white.y + directions[prevDirection][1] * l + directions[i][1] * k)].type = 0;
                                cache[steps] = tempArea[getTileIndex(tempArea, areaSize, white.x + directions[prevDirection][0] * l, white.y + directions[prevDirection][1] * l)];
                                tempArea[getTileIndex(tempArea, areaSize, cache[steps].x, cache[steps].y)].type = 0;
                                printMovesSupfuncW(tempArea, areaSize, tempArea[getTileIndex(tempArea, areaSize, white.x + directions[prevDirection][0] * l + directions[i][0] * (k + 1), white.y + directions[prevDirection][1] * l + directions[i][1] * (k + 1))], steps + 1, captured + 1, cache, i);
                                break;
                            }
                            else break;
                        }
                        if (isValidPos(area, areaSize, white.x + directions[prevDirection][0] * l + directions[i][0] * k, white.y + directions[prevDirection][1] * l + directions[i][1] * k) == 2) break;
                    }
                }
            }
            l++;
        }
    }
}

void printMoves(TTile area[], int areaSize, TTile white) {
    TTile cache[MAX_SIZE];
    cache[0] = white;
    if (white.type == 'w')
        printMovesSupfuncw(area, areaSize, white, 0, 0, cache);
    if (white.type == 'W')
        printMovesSupfuncW(area, areaSize, white, 0, 0, cache, 0);
}

int main(void) {
    printf("Velikost hraci plochy:\n");
    int areaSize; char endLine;
    if (scanf("%d%c", &areaSize, &endLine) != 2 || areaSize < 3 || areaSize > 26 || endLine != '\n') {
        printf("Nespravny vstup.\n");
        return 1;
    }
    TTile area[MAX_SIZE];
    TTile whites[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) {
        area[i] = makeTile(0, 'a' + (i % areaSize), (i + areaSize) / areaSize, i / areaSize, i % areaSize);
        whites[i] = makeTile(0, 0, 0, 0, 0);
    }
    printf("Pozice kamenu:\n");
    char tileType, tileCoordinateColumn, space; int tileCoordinateRow, whitesNum = 0;
    TTile* tempTile;
    while (scanf("%c%c%c%d%c", &tileType, &space, &tileCoordinateColumn, &tileCoordinateRow, &endLine) == 5) {
        if (!checkInputValidity(area, areaSize, tileType, space, tileCoordinateColumn, tileCoordinateRow, endLine)) {
            printf("Nespravny vstup.\n");
            return 2;
        }
        tempTile = &(area[getTileIndex(area, areaSize, tileCoordinateRow - 1, tileCoordinateColumn - 'a')]);
        *tempTile = makeTile(tileType, tempTile->coordinateColumn, tempTile->coordinateRow, tempTile->x, tempTile->y);
        if (tileType == 'w' || tileType == 'W')
            whites[whitesNum++] = makeTile(tileType, tileCoordinateColumn, tileCoordinateRow, tileCoordinateRow - 1, tileCoordinateColumn - 'a');
    }
    if (!feof(stdin)) {
        printf("Nespravny vstup.\n");
        return 3;
    }

    for (int i = 0; i < whitesNum; i++) {
        printMoves(area, areaSize, whites[i]);
    }
    printf("Celkem ruznych tahu: %d\n", stepsSum);

    return 0;
}
