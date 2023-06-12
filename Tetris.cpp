#include "wincursor.h"
#include <cstdlib>
#define nSize	4		// How many squares are there in a tetromino
#define nType   7               // How many tetromino types
#define nLeftBorder	9
#define nRightBorder	23
#define nBottomBorder   21
#define nTopBorder      1

enum Movement { UP, ROTATE = 0, DOWN, LEFT, RIGHT };

Screen myScreen;

#include "tetris.h"

void init_game();
void game_over();
void display(int type_id);

int main()
{   
    srand(time(NULL));
    int i, x0, next_i = rand() % nType;
    int c;
    init_game();

    while (true)
    {
        x0 = (rand() % (nRightBorder - nLeftBorder - 1 - 3 - 2)) + 2;
        i = next_i;
        next_i = rand() % nType;
        display(next_i);
        CTetromino a(i, i + 2);
        if (a.detect_confliction())     // a newly constructed tetromino
            break;                      // conflicts, then game over
        do {
            myScreen.redraw();
            c = myScreen.key();
            switch (c)
            {
            case 'q':
                game_over();
                return 0;
                break;
            case KEY_LEFT:
            case 'h':
                a.Move(LEFT);
                break;
            case KEY_RIGHT:
            case 'l':
                a.Move(RIGHT);
                break;
            case KEY_UP:
            case 'k':
                a.Move(ROTATE);
                break;
            case ' ':
            case KEY_DOWN:
            case 'j':
                while (a.Move(DOWN))
                {
                    myScreen.redraw();
                    Sleep(60);
                }
                break;
            }
        } while (a.Move(DOWN));
    }

    myScreen.redraw();
    game_over();
    return 0;
}

void init_game()
{
    int y0, x0;
    myScreen.setTimeOut(300);
    String star("**");

    for (y0 = nTopBorder + 1; y0 < nBottomBorder; y0++)
    {
        star.show(y0, nLeftBorder * 2);
        star.show(y0, nRightBorder * 2);
    }

    for (x0 = (nLeftBorder + 1) * 2; x0 < nRightBorder * 2; x0 += 2)
    {
        star.show(nBottomBorder, x0);
    }
    myScreen.redraw();
}

void game_over()
{
    String msg("Game Over!", nTopBorder + 1, nRightBorder * 2 + 2);
    msg.show();
    myScreen.redraw();
    myScreen.pause(2);
}

void display(int type_id) {
    static int p_id = 0;
    Tetromino t[7] = { 0,0, -2,0, 2,0, 4,0 ,
                    -2,1, -2,0, 0,1, 2,1 ,
                    -2,1, 0,1, 2,1, 2,0 ,
                    -2,1, -2,0, 0,1 ,0,0,
                    -2,1, 0,1, 0,0, 2,0 ,
                    -2,1, 0,1, 0,0, 2,1 ,
                    0,1, -2,0, 0,0, 2,1
    };

    String Str;
    Str.setText("  ");
    for (int i = 0; i < 4; i++) {
        Str.show(t[p_id].s[i].dy + 4, t[p_id].s[i].dx + 52, 0);
    }
    for (int i = 0; i < 4; i++) {
        Str.show(t[type_id].s[i].dy + 4, t[type_id].s[i].dx + 52, type_id + 1);
    }
    myScreen.redraw();
    p_id = type_id;
}