#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SPAWN_TRY 1000

#define bool _Bool
#define false 0
#define true 1

#define DATA_WALL 0
#define DATA_TAIL 1
#define DATA_AIR 2
#define DATA_APPLE 3
#define DATA_HEAD 4

#define RENDER_AIR ' '
#define RENDER_WALL '#'
#define RENDER_TAIL '*'
#define RENDER_HEAD_UP '^'
#define RENDER_HEAD_DOWN 'v'
#define RENDER_HEAD_RIGHT '>'
#define RENDER_HEAD_LEFT '<'
#define RENDER_APPLE '@'

#define LENGTH 20
#define APPLE_NUMBER 5

void PrintMap(const int * Map, bool Up, bool Down, bool Right, bool Left) {
    system("clear");
    for (int PosX = 0; PosX < LENGTH; PosX++) {
        for (int PosY = 0; PosY < LENGTH; PosY++) {
            switch (*(Map + PosX + PosY * LENGTH)) {
                case DATA_WALL:
                    printf("%c", RENDER_WALL);
                    break;
                case DATA_AIR:
                    printf("%c", RENDER_AIR);
                    break;
                case DATA_APPLE:
                    printf("%c", RENDER_APPLE);
                    break;
                case DATA_TAIL:
                    printf("%c", RENDER_TAIL);
                    break;
                case DATA_HEAD:
                    if (Up)
                    {
                        printf("%c", RENDER_HEAD_UP);
                    }
                    else if (Down)
                    {
                        printf("%c", RENDER_HEAD_DOWN);
                    }
                    else if (Right)
                    {
                        printf("%c", RENDER_HEAD_RIGHT);
                    }
                    else if (Left)
                    {
                        printf("%c", RENDER_HEAD_LEFT);
                    }
                    break;
            }
        }
        printf("\n");
    }
}

void NextFrame(int * Map,int * Snake,bool Up,bool Down,bool Right,bool Left,int * SnakeLength, bool * GameOver, int * AppleNumber) {
    int PosX = 0;
    int PosY = 0;
    int PosXApple;
    int PosYApple;
    int SpawnTry = 0;

    while (*(Map + PosX + PosY * LENGTH) != DATA_HEAD) {
        PosX = PosX + 1;
        if (PosX > LENGTH - 1) {
            PosY = PosY + 1;
        }
        PosX = PosX % LENGTH;
        if (PosY > LENGTH - 1) {
            exit(-1);
        }
    }
    if (Up) {
        PosX = PosX - 1;
    } else if (Down) {
        PosX = PosX + 1;
    } else if (Right) {
        PosY = PosY + 1;
    } else if (Left) {
        PosY = PosY - 1;
    }

    if (*(Map + PosX + PosY * LENGTH) == DATA_APPLE) {
        *SnakeLength = *SnakeLength + 1;
        *AppleNumber = *AppleNumber - 1;
    }

    if (*(Map + PosX + PosY * LENGTH) == DATA_WALL) {
        *GameOver = true;
        return;
    }

    if (*(Map + PosX + PosY * LENGTH) == DATA_TAIL) {
        *GameOver = true;
        return;
    }

    for (int X = 0; X < LENGTH; X++) {
        for (int Y = 0; Y < LENGTH; Y++) {
            if (*(Snake + X + Y * LENGTH) > 0) {
                *(Snake + X + Y * LENGTH) += 1;
            }
        }
    }

    *(Snake + PosX + PosY * LENGTH) = 1;
    *(Map + PosX + PosY * LENGTH) = DATA_HEAD;

    for (int X = 0; X < LENGTH; X++) {
        for (int Y = 0; Y < LENGTH; Y++) {
            if (*(Snake + X + Y * LENGTH) > *SnakeLength) {
                *(Snake + X + Y * LENGTH) = 0;
            }
        }
    }

    for (int X = 0; X < LENGTH; X++) {
        for (int Y = 0; Y < LENGTH; Y++) {
            if (*(Map + X + Y * LENGTH) != DATA_APPLE && *(Map + X + Y * LENGTH) != DATA_WALL && *(Snake + X + Y * LENGTH) == 0) {
                *(Map + X + Y * LENGTH) = DATA_AIR;
            }
            else if (*(Map + X + Y * LENGTH) != DATA_APPLE && *(Map + X + Y * LENGTH) != DATA_WALL && *(Snake + X + Y * LENGTH) > 1) {
                *(Map + X + Y * LENGTH) = DATA_TAIL;
            }
        }
    }

    while (*AppleNumber < APPLE_NUMBER) {
        SpawnTry = SpawnTry + 1;
        PosXApple = rand() % LENGTH;
        PosYApple = rand() % LENGTH;
        if (*(Map + PosXApple + PosYApple * LENGTH) == DATA_AIR) {
            *AppleNumber = *AppleNumber + 1;
            *(Map + PosXApple + PosYApple * LENGTH) = DATA_APPLE;
        }
        if (SpawnTry > MAX_SPAWN_TRY) {
            *GameOver = true;
            printf("You win !!!!!\n");
            break;
        }
    }
}

int main() {
    srand(time(NULL));

    bool GameOver = false;
    bool Left = false;
    bool Right = false;
    bool Up = true;
    bool Down = false;

    int * Map;
    int * Snake;
    int AppleNumber = 0;
    int PosX;
    int PosY;
    int PosXHead = 0;
    int PosYHead = 0;
    int SnakeLength = 1;
    int Direction;

    Map = (int *) malloc(sizeof(int) * LENGTH * LENGTH);
    Snake = (int *) malloc(sizeof(int) * LENGTH * LENGTH);

    if (Map == NULL){
        return -1;
    }

    if (Snake == NULL){
        return -1;
    }

    for (int x = 0; x < LENGTH; x++) {
        for (int y = 0; y < LENGTH; y++) {
            if (x != 0 && x != LENGTH - 1 && y != 0 && y != LENGTH - 1) {
                *(Map + x + y * LENGTH) = DATA_AIR;
            } else {
                *(Map + x + y * LENGTH) = DATA_WALL;
            }
            *(Snake + x + y * LENGTH) = 0;
        }
    }

    while (AppleNumber < APPLE_NUMBER) {
        PosX = rand() % LENGTH;
        PosY = rand() % LENGTH;
        if (*(Map + PosX + PosY * LENGTH) == DATA_AIR) {
            AppleNumber = AppleNumber + 1;
            *(Map + PosX + PosY * LENGTH) = DATA_APPLE;
        }
    }

    while (*(Map + PosXHead + PosYHead * LENGTH) != DATA_HEAD)  {
        PosXHead = rand() % LENGTH;
        PosYHead = rand() % LENGTH;
        if (*(Map + PosXHead + PosYHead * LENGTH) == DATA_AIR) {
            *(Map + PosXHead + PosYHead * LENGTH) = DATA_HEAD;
            *(Snake + PosXHead + PosYHead * LENGTH) = 1;
        }
    }

    PrintMap(Map, Up, Down, Right, Left);

    while (GameOver != true) {
        Direction = getchar();
        getchar();
        switch (Direction) {
            default:
                break;
            case 'z':
                Up = true;
                Down = false;
                Right = false;
                Left = false;
                break;
            case 's':
                Up = false;
                Down = true;
                Right = false;
                Left = false;
                break;
            case 'd':
                Up = false;
                Down = false;
                Right = true;
                Left = false;
                break;
            case 'q':
                Up = false;
                Down = false;
                Right = false;
                Left = true;
                break;
        }
        NextFrame(Map, Snake, Up, Down, Right, Left, &SnakeLength, &GameOver, &AppleNumber);
        PrintMap(Map, Up, Down, Right, Left);
    }
    return 0;
}
