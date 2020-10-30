#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

struct termios old, current;

/* Initialize new terminal i/o settings */
void initTermios() 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  current.c_lflag &= ~ECHO; /* set no echo mode */
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch() 
{
  char ch;
  initTermios();
  ch = getchar();
  resetTermios();
  return ch;
}

char Map[9][9] = {
    {"vvwwwwwvv"},
    {"vw     wv"},
    {"w   w   w"},
    {"w   w   w"},
    {"w   w   w"},
    {"w   w   w"},
    {"w   w   w"},
    {"vw     wv"},
    {"vvwwwwwvv"},
};

typedef struct Player {
    uint8_t x;
    uint8_t y;
    uint8_t size;
} Player_t;

Player_t player = {3,3,1};

void drawMap() {
    for (uint8_t y = 0; y < 9; y++)
    {
        for (uint8_t x = 0; x < 9; x++)
        {
            if (x == player.x && y == player.y) {
                printf("@");
            } else {
             printf("%c", Map[y][x]);
            }
            /* code */
        }
        printf("\n");
        /* code */
    }
    
}


char readInput() {
    char input;
    input = getch();
    return input;
}

void movePlayer(char input) {
    switch (input)
    {
    case 'w':
        if (Map[player.y-1][player.x] != 'w'){
            player.y--;
        }
        break;
    case 'a':
        if (Map[player.y][player.x-1] != 'w'){
            player.x--;
        }
        break;
    case 's':
        if (Map[player.y+1][player.x] != 'w'){
            player.y++;
        }
        break;
    case 'd':
        if (Map[player.y][player.x+1] != 'w'){
            player.x++;
        }
        break;
    default:
        break;
    }
}

int main() {
    while (1) {   
        drawMap();
        char input = readInput();
        if (input == 'q') {
            break;
        }
        movePlayer(input);
        system("clear"); 
    };
    return 0;
}
