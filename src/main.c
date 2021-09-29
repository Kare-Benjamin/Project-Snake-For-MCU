#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>

// Graphics
#define head '*'
#define tail '#'
#define topBoarder '|_|'
#define middleBoarder '|'
#define food '$'
#define voidSpace ' '
#define bomb 'x'

// Game Options
#define startSpeed 100
#define xDim 30
#define yDim 30
#define minSpeed 200
#define maxSpeed 40
#define bombObstacleSize 2
#define playerSpawnPaddingFront 8
#define playerSpawnPaddingSides 1

/* Global game variables*/
char gameField[yDim][xDim];
int alive = 1;
int x;
int y;
int speed;
int score = 0;
int oldScore;
int dirX;
int dirY;
int oldBodyCor[yDim * xDim][2];
int body[yDim * xDim][2];
int numOfBody = 0;
int boarder = 2;
int bugsPrevented;
int oldBugsPrevented;
static int illegalSpawn;

// Prototypes
void makeMap(int xLength, int yLength);
void draw();
void move();
void genPlayer();
void genEnemy();
void hideCursor();
void keyInput();
void genDefDir();
void goToXY(int column, int line);
void grow();
void freeMove();
void reset();
void gameOverPrint();
void choseGameOverPath();
void printSnake();
void printSnake2();
void printSnake3();
void menuSystem();
int menuSystemLogic(int MenuChoice);

/* Main loop executing the game logic */
int main() {
    hideCursor();
    do {
        // Setup
        reset();
        menuSystem();
        // Clear screen
        system("cls");
        // Creates the map
        makeMap(xDim, yDim);
        // Generates the player and ensures a reasonable spawn
        genPlayer();
        while (illegalSpawn) {
            genPlayer();
        }
        // Direction
        genDefDir();
        // Generates Enemy and ensures a reasonable spawn
        genEnemy();
        while (illegalSpawn) {
            genPlayer();
        }
        // Main game active loop, ends when player is not alive any more
        while (alive) {
            // Delay untill the player can move
            Sleep(speed);
            // Take player key input
            keyInput();
            // Move the player
            move();
            // Draw again
            draw();
        }
        // Clear screen
        system("cls");
        // Game over menu
        gameOverPrint();
        // Let player browse the menu
        while (!alive) {
            choseGameOverPath();
        }
        // Back to start
    } while (alive);
}

void makeMap(int xLength, int yLength) {
    /* Creates the map filling in the gameField matrix. 
       It also creates obstacles with magnitude depending on the game settings*/
    int x, y;
    for (y = 0; y < yLength; ++y) {
        for (x = 0; x < xLength; ++x) {
            if (y == 0 || y == yLength - 1) {
                gameField[y][x] = topBoarder;
            }
            else if (x == 0 || x == xLength - 1) {
                gameField[y][x] = middleBoarder;
            }
            else if (x == xDim / 2 && y == yDim / 2) {
                for (int i = 0; i < bombObstacleSize; i++) {
                    gameField[y-i][x+i] = bomb;
                    gameField[y+i][x+i] = bomb;
                    gameField[y+i][x-i] = bomb;
                    gameField[y-i][x-i] = bomb;
                }
            }
            else if (gameField[y][x] != bomb) {
                gameField[y][x] = voidSpace;
            }
        }
    }
}
void grow() {
    /* The growth logic of the snake*/
    if (numOfBody == 0) {
        ++numOfBody;
        gameField[body[0][0] - dirY][body[0][1] - dirX] = tail;
        body[1][0] = body[0][0] - dirY;
        body[1][1] = body[0][1] - dirX;
        return;
    }
    for (int i = 0; i <= numOfBody; ++i) {
        if (i == numOfBody) {
            body[i + 1][0] = oldBodyCor[i][0];
            body[i + 1][1] = oldBodyCor[i][1];
            gameField[body[i + 1][0]][body[i + 1][1]] = tail;
            ++numOfBody;
            return;
        }
    }
    return;
}
void genDefDir() {
    /* Movement logic */
    srand(time(NULL));
    for (y = 0; y < yDim; ++y) {
        for (x = 0; x < xDim; ++x) {
            switch (gameField[y][x]) {
            case head:
                if (y <= (yDim / 2) - 1 && x <= (xDim / 2) - 1) {
                    if ((rand() % 2) == 0) {
                        dirX = 1;
                        dirY = 0;
                        return;
                    }
                    else {
                        dirX = 0;
                        dirY = 1;
                        return;
                    }
                }
                else if (y <= (yDim / 2) - 1 && x > (xDim / 2) - 1) {
                    if ((rand() % 2) == 0) {
                        dirX = -1;
                        dirY = 0;
                        return;
                    }
                    else {
                        dirX = 0;
                        dirY = 1;
                        return;
                    }
                }
                else if (y > (yDim / 2) - 1 && x <= (xDim / 2) - 1) {
                    if ((rand() % 2) == 0) {
                        dirX = 1;
                        dirY = 0;
                        return;
                    }
                    else {
                        dirX = 0;
                        dirY = -1;
                        return;
                    }
                }
                else if (y > (yDim / 2) - 1 && x > (xDim / 2) - 1) {
                    if ((rand() % 2) == 0) {
                        dirX = -1;
                        dirY = 0;
                        return;
                    }
                    else {
                        dirX = 0;
                        dirY = -1;
                        return;
                    }
                }
            }
        }
    }
}
void keyInput() {
    /* Processes the in-game key input, which controlls the movmement through dirX and dirY. It also controlls the game speed options and reset*/
    int keyPressed = 1;
    char key = 0;

    if (_kbhit()) {
        char key = _getch();

        if (key == 'w' && keyPressed == 1) {
            dirX = 0;
            dirY = -1;
            keyPressed = 0;
        }
        else if (key == 's' && keyPressed == 1) {
            dirX = 0;
            dirY = 1;
            keyPressed = 0;
        }
        else if (key == 'a' && keyPressed == 1) {
            dirX = -1;
            dirY = 0;
            keyPressed = 0;
        }
        else if (key == 'd' && keyPressed == 1) {
            dirX = 1;
            dirY = 0;
            keyPressed = 0;
        }
        if (key == '1' && keyPressed == 1) {
            // Reverts any changes and sets speed to original speed.
            speed = startSpeed;
            keyPressed = 0;
        }

        else if (key == '2' && keyPressed == 1) {
            // Increases the game speed  
            if (speed <= minSpeed) {
                // Checks minSpeed to prevent a too low game speed.
                speed = speed + 30;
            }
            keyPressed = 0;
        }
        else if (key == '3' && keyPressed == 1) {
            // Halving the speed, up until minSpeed is reached.
            if (speed >= maxSpeed) {
                // In case minSpeed is set too low, +1 to prevent a zero speed.
                speed = speed - 15;
            }
            keyPressed = 0;
        }
        else if (key == '4' && keyPressed == 1) {
            main();
            keyPressed = 0;
        }
    }
}

void genPlayer() {
    /* Creates the player in a random position. It also ensures that the player is placed on a legal position, otherwise it is a 
    illegalSpawn and the bug count will be incremented by one.*/
    srand(time(NULL));
    // rand % maxValue => random number smaller than max value
    // The boarder pads the values from beeing too close to the edges of the map.
    int x = rand() % (xDim - boarder) + boarder;
    int y = rand() % (yDim - boarder) + boarder;
    illegalSpawn = 0;
    // Checking if legal spawn location.
    if (gameField[y][x] != voidSpace) {
        illegalSpawn = 1;
        bugsPrevented++;
    }
    else {
        for (int i = y - playerSpawnPaddingSides; i <= y + playerSpawnPaddingSides; ++i) {
            for (int j = x - playerSpawnPaddingFront; j <= x + playerSpawnPaddingFront; ++j) {
                if (gameField[i][j] != voidSpace) {
                    illegalSpawn = 1;
                    bugsPrevented++;
                    printf(" bad spawn found @ %d %d: \r \n", i, j);                    
                }
            }
        }
    }
    if (illegalSpawn == 0) {
        gameField[y][x] = head;
        illegalSpawn = 0;
    }
}
void genEnemy() {
    /* Generates enemies (food) on a pseudorandom location. If the location is not free, then it is a illegalSpawn and 
       the bug count is increased by one*/
    srand(time(NULL));
    while (1) {
        // rand % maxValue => random number smaller than max value.
        // The boarder pads the values from beeing too close to the edges of the map.
        int x = rand() % (xDim - boarder) + boarder;
        int y = rand() % (yDim - boarder) + boarder;
        // Checking if legal spawn location.
        if(gameField[y][x] == voidSpace) {
            gameField[y][x] = food;
            illegalSpawn = 0;
            return;
        }
        else {
            illegalSpawn = 1;
            bugsPrevented++;
        }
    }

}
void draw() {
    /* Draw function to monitor */
    for (y = 0; y < xDim; ++y) {
        goToXY(yDim, y);
        for (x = 0; x < xDim; ++x) {
            printf("%c", gameField[y][x]);
        }
        printf("\n");
    }
    goToXY(0, 0);
    if (score != oldScore) {
        printf(" Score : %d \r \n", score);
        oldScore = score;
    }
    if (bugsPrevented != oldBugsPrevented)
    {
        printf(" BugsPrevent %d", bugsPrevented);
        oldBugsPrevented = bugsPrevented;
    }
}
void move() {
    /* The move logic determines wether the snakes can move successfully or if it has lost (alive = 0)
       It can move if there's a free space, it reaches or it reaches food. It has lost otherwise (i.e. crashed)
     */
    for (y = 0; y < yDim; ++y) {
        for (x = 0; x < xDim; ++x) {
            switch (gameField[y][x]) {
            case head:
                if (gameField[y + dirY][x + dirX] == voidSpace) {
                    freeMove();
                    return;
                }
                else if (gameField[y + dirY][x + dirX] == food) {
                    gameField[y + dirY][x + dirX] = voidSpace;
                    score += 10;
                    freeMove();
                    grow();
                    genEnemy();
                    return;
                }
                else if (gameField[y + dirY][x + dirX] != voidSpace && gameField[y + dirY][x + dirX] != food) {
                    alive = 0;
                }
            }
        }
    }
}
void freeMove() {
    /* Movement logic of the snake which is based on dirY and dirX to determine where the snake is headed.
       This is the action which takes place when the snake is allowed to move.*/
    char headY = y;
    char headX = x;
    char temp = gameField[y][x];
    gameField[y][x] = gameField[y + dirY][x + dirX];
    gameField[y + dirY][x + dirX] = temp;
    body[0][0] = y + dirY;
    body[0][1] = x + dirX;
    for (int i = 1; i <= numOfBody; ++i) {
        if (i == 1) {
            oldBodyCor[i][0] = body[i][0];
            oldBodyCor[i][1] = body[i][1];
            char temp = gameField[body[i][0]][body[i][1]];
            gameField[body[i][0]][body[i][1]] = gameField[headY][headX];
            gameField[headY][headX] = temp;
            body[i][0] = headY;
            body[i][1] = headX;
            continue;
        }
        oldBodyCor[i][0] = body[i][0];
        oldBodyCor[i][1] = body[i][1];
        char temp = gameField[body[i][0]][body[i][1]];
        gameField[body[i][0]][body[i][1]] = gameField[oldBodyCor[i - 1][0]][oldBodyCor[i - 1][1]];
        gameField[oldBodyCor[i - 1][0]][oldBodyCor[i - 1][1]] = temp;
        body[i][0] = oldBodyCor[i - 1][0];
        body[i][1] = oldBodyCor[i - 1][1];
    }
    return;
}
void reset() {
    /* Resets the game state. */
    score = 0;
    bugsPrevented = -1;
    oldScore = 'x';
    oldBugsPrevented = 'x';
    speed = startSpeed;
    for (int i = 0; i <= numOfBody; ++i) {
        body[i][0] = 0;
        body[i][0] = 0;
    }
    numOfBody = 0;
}

void gameOverPrint() {
    /* Prints to terminal that the player has lost and explains options to the player
       Purely a print function */
    printf("\r \n");
    printf(" My friend ... you are dead!\r \n");
    printf(" You faught valiantly and reached: %d points! \r \n", score);
    printf("\r \n");
    printf(" Perhaps try again?\r \n");
    printf(" Press 1 to try agian, or 2 to idle, 3 to power down.");
    printf("\r \n");
    printf("\r \n");
    printf("\r \n");
    printSnake3();
}
void choseGameOverPath() {
    /* Game over logic
       Press 1: to play more
       Press 2 to put gecko to sleep
       Press 3 to shut down gecko */
    int keyPressed = 0;
    char key = 0;

    if (_kbhit()) {
        key = _getch();
        keyPressed = 1;
    }
    if (key == '1' && keyPressed == 1) {
        alive = 1;
        keyPressed = 0;
        reset();
    }
    else if (key == '2' && keyPressed == 1) {
        keyPressed = 0;
        // MAKE GECKO SLEEP, and when button wake up...
        printf(" going into sleep mode down \r \m");
    }
    else if (key == '3' && keyPressed == 1) {
        keyPressed = 0;
        // Make GECKO POWER DOWN
        printf(" going into power down \r \n");
    }
}

void menuSystem() {
    /* Usess the menuSsytemLogic to map the menu system to the keyws on the keyboard
    _kbhit reunts true if a key has been pressed*/
        int WhereInMenu = 1;
        char KeyTouched = 0;
        int keyPressed= 0;

        menuSystemLogic(1);

        for (;;)
        {


            if (_kbhit()) {
                KeyTouched = _getch();
                keyPressed = 1;
            }
            if (KeyTouched == 's' && keyPressed == 1)
            {
                WhereInMenu++;
                keyPressed = 0;
                if (WhereInMenu > 4)
                {
                    WhereInMenu = 1;
                }
                menuSystemLogic(WhereInMenu);
                
            }

            else if (KeyTouched == 'w' && keyPressed == 1)
            {
                WhereInMenu--;
                keyPressed = 0;
                if (WhereInMenu < 1)
                {
                    WhereInMenu = 4;
                }
                menuSystemLogic(WhereInMenu);
            }
            else if (KeyTouched == 'd' && keyPressed == 1)
            {
                system("cls");
                printf("Press Enter To Exit...");
                //Do something
                return 0;
            }
        }
}


int menuSystemLogic(int MenuChoice)
/* A simple switch case based menu system
   Could be replaced by a linked list in the future */
{
    switch (MenuChoice)
    {
    case 1:
        system("cls");
        printf("\n* Option Number 1");
        printf("\n  Option Number 2");
        printf("\n  Option Number 3");
        printf("\n  Option Number 4");
        break;

    case 2:
        system("cls");
        printf("\n  Option Number 1");
        printf("\n* Option Number 2");
        printf("\n  Option Number 3");
        printf("\n  Option Number 4");
        break;

    case 3:
        system("cls");
        printf("\n  Option Number 1");
        printf("\n  Option Number 2");
        printf("\n* Option Number 3");
        printf("\n  Option Number 4");
        break;

    case 4:
        system("cls");
        printf("\n  Option Number 1");
        printf("\n  Option Number 2");
        printf("\n  Option Number 3");
        printf("\n* Option Number 4");
        break;

    default:
        system("cls");
        printf("\nError");
    }
    return 0;
}


void hideCursor() {
    /* Hides the cursor*/
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
void goToXY(int column, int line) {
    // Create a COORD structure and fill in its members.
    // This specifies the new position of the cursor that we will set.
    COORD coord;
    coord.X = column;
    coord.Y = line;

    // Obtain a handle to the console screen buffer.
    // (You're just using the standard console, so you can use STD_OUTPUT_HANDLE
    // in conjunction with the GetStdHandle() to retrieve the handle.)
    // Note that because it is a standard handle, we don't need to close it.
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Finally, call the SetConsoleCursorPosition function.
    if (!SetConsoleCursorPosition(hConsole, coord)) {
        // Uh-oh! The function call failed, so you need to handle the error.
        // You can call GetLastError() to get a more specific error code.
        // ...
    }
}

// Various snakes available for end game screen.
// https://ascii.co.uk/art/snake
// https://www.asciiart.eu/animals/reptiles/snakes

void printSnake() {
    printf("    _________         _________\r \n");
    printf("   /         \       /         \   Normand\r \n");
    printf("  /  /~~~~~\  \     /  /~~~~~\  \  Veilleux\r \n");
    printf("  |  |     |  |     |  |     |  |\r \n");
    printf("  |  |     |  |     |  |     |  |\r \n");
    printf("  |  |     |  |     |  |     |  |         /\r \n");
    printf("  |  |     |  |     |  |     |  |       //\r \n");
    printf(" (o  o)    \  \_____/  /     \  \_____/ /\r \n");
    printf("  \__/      \         /       \        /\r \n");
    printf("   |         ~~~~~~~~~         ~~~~~~~~\r \n");
}


void printSnake2() {
    // Art by CJR
    printf("            ____\r \n");
    printf("      _,.-'`_ o `;__,                \r \n");
    printf("       _.-'` '---'  '\r \n");
}


void printSnake3() {
    // Art by CJR
    printf("                    ____\r \n");
    printf("                 .'`_ o `;__,\r \n");
    printf("       .       .'.'` '---'  '            \r \n");
    printf("       .`-...-'.'\r \n");
    printf("        `-...-'\r \n");
}
