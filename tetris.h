#pragma once
#include <iostream>
#include <cstdlib>
#include <curses.h>
#include "wincursor.h"

#define nHeight 19
#define nWidth 26
#define length 4

Screen S;

typedef struct SQ {
    int dx;
    int dy;
}Square;

typedef struct TET {
    Square s[length];
}Tetromino;

Tetromino t1[7] = { 0,0, -2,0, 2,0, 4,0 ,
                    -2,1, -2,0, 0,1, 2,1 ,
                    -2,1, 0,1, 2,1, 2,0 ,
                    -2,1, -2,0, 0,1 ,0,0,
                    -2,1, 0,1, 0,0, 2,0 ,
                    -2,1, 0,1, 0,0, 2,1 ,
                    0,1, -2,0, 0,0, 2,1 
};

Tetromino t2[7] = { 0,0, 0,1, 0,2, 0,3,
                    -2,1, -2,0, -2,-1, 0,-1,
                    -2,1, -2,0, -2,-1, 0,1,
                    -2,1, -2,0, 0,1, 0,0,
                    -2,1, -2,0, 0,2, 0,1,
                    -2,1, -2,0, -2,-1, 0,0,
                    0,1, 0,0, 2,0, 2,-1
};

Tetromino t3[7] = { 0,0, -2,0, 2,0, 4,0 ,
                    0,1, 2,1, 4,2, 4,1,
                    0,1, 0,0, 2,0, 4,0,
                    -2,1, -2,0, 0,1, 0,0,
                    0,1, 2,1, 2,0, 4,0 ,
                    0,1, 2,2, 2,1, 4,1,
                     0,1, -2,0, 0,0, 2,1
};

Tetromino t4[7] = { 0,0, 0,1, 0,2, 0,3,
                    4,1, 6,1, 6,0, 6,-1,
                    4,1, 6,3, 6,2, 6,1,
                    -2,1, -2,0, 0,1, 0,0,
                    2,1, 2,0, 4,2, 4,1,
                    4,1, 6,2, 6,1, 6,0,
                    0,1, 0,0, 2,0, 2,-1
};

bool occupied[nHeight][nWidth] = { false };

class CTetromino : public String {
    unsigned short type_id;
    int pos_x[length];
    int pos_y[length];
    string block = "  ";

    int rotate_time = 0;
    int again = 0;
public:
    CTetromino(unsigned short color = 0, int what = 0) {
        type_id = color;
        setText(block);
        for (int i = 0; i < length; i++) {
            pos_y[i] = t1[type_id].s[i].dy + 2;
            pos_x[i] = t1[type_id].s[i].dx + 30;
        }
    }

    void Draw(void) {
        for (int i = 0; i < length; i++) {
            show(pos_y[i], pos_x[i], type_id + 1);
        }
    }

    void Erase(void) {
        for (int i = 0; i < length; i++) {
            show(pos_y[i], pos_x[i], 0);
        }
    }

    void update_occupy(void) {
        Tetromino arr;
        int get = 0;
        for (int i = 0; i < length; i++) {     // send positions to an array
            arr.s[i].dy = pos_y[i] - 2;
            arr.s[i].dx = pos_x[i] - 20;
        }

        for (int i = 0; i < length; i++) {     // update the occupied positions
            occupied[arr.s[i].dy][arr.s[i].dx] = 1;
            occupied[arr.s[i].dy][arr.s[i].dx + 1] = 1;
        }

        for (int i = 0; i < nHeight; i++) {   // check every lines
            get+=check_horizontal(i);
        }
        
        Point(get);
    }

    int check_horizontal(int i) {
        int j;
        for (j = 0; j < nWidth; j++) {
            if (occupied[i][j] == 0) {
                return 0;
            }
        }

        if (j == 26) {
            for (int k = 0; k < 13; k++) {   // while light
                show(i + 2, 20 + (k * 2), 3);
            }
            S.redraw();
            Sleep(100);
            for (int k = 0; k < 13; k++) {   // black light
                show(i + 2, 20 + (k * 2), 0);
            }

            for (int k = i; k > 0; k--) {
                for (int j = 0; j < nWidth; j++) {
                    occupied[k][j] = 0;
                    occupied[k][j] = occupied[k - 1][j];
                }
            }
            for (int k = 0; j < nWidth; j++) {
                occupied[0][k] = 0;
            }

            

            for (int k = 0; k < nHeight; k++) {
                for (int j = 0; j < nWidth; j+=2) {
                    if (occupied[k][j] == 1) {
                        show(k + 2, j + 20, type_id+1);
                    }
                    else {
                        show(k + 2, j + 20, 0);
                    }
                }
            }
        }
        return 1;
    }

    bool detect_confliction(int mod = 1) {
        Tetromino arr;
        for (int i = 0; i < length; i++) {
            arr.s[i].dy = pos_y[i] - 2;
            arr.s[i].dx = pos_x[i] - 20;
        }

        if (mod == 2) {
            for (int i = 0; i < length; i++) {
                if (occupied[arr.s[i].dy][arr.s[i].dx - 2] == true) {
                    return true;
                }

            }
        }
        else if (mod == 3) {
            for (int i = 0; i < length; i++) {
                if (occupied[arr.s[i].dy][arr.s[i].dx + 2] == true) {
                    return true;
                }
            }
        }
        else if (mod == 1) {
            for (int i = 0; i < length; i++) {
                if (occupied[arr.s[i].dy + 1][arr.s[i].dx] == true) {
                    return true;
                }
            }
        }
        else {
            if (rotate_time % 2) {                           // 1 or 3
                if (!type_id) {
                    for (int i = 0; i < 4; i++) {
                        if (pos_x[i] > 41 || pos_x[i] < 21) {
                            return true;
                        }
                        if (occupied[arr.s[i].dy][arr.s[i].dx - 2] == 1 || occupied[arr.s[i].dy][arr.s[i].dx + 3] == 1) {
                            return true;
                        }
                    }
                }
                else {
                    for (int i = 0; i < 4; i++) {
                        if (rotate_time==1) {
                           if (pos_x[i] > 42) {
                                return true;
                            }
                           if (occupied[arr.s[i].dy][arr.s[i].dx + 2]==1) {
                               return true;
                           }
                           if (pos_y[i] + 1 > 20 || occupied[arr.s[i].dy + 1][arr.s[i].dx] == 1) {
                               return true;
                           }
                        }
                        else {
                            if (pos_x[i] < 22) {
                                return true;
                            }
                            if (occupied[arr.s[i].dy][arr.s[i].dx - 2] == 1) {
                                return true;
                            }
                            if (pos_y[i] + 2 > 20 || occupied[arr.s[i].dy + 2][arr.s[i].dx] == 1) {
                                return true;
                            }
                        }
                    }
                }
                   
            }
            else {                                          // 0 or 2
                if (!type_id) {
                    for (int i = 0; i < 4; i++) {
                        if (occupied[arr.s[i].dy + 3][arr.s[i].dx] == 1) {
                            return true;
                        }
                        if (pos_y[i] + 2 > 20) {
                            return true;
                        }
                    }
                }
                else {
                    for (int i = 0; i < 4; i++) {
                        if (pos_y[i] + 2 > 20) {
                            return true;
                        }
                        if (occupied[arr.s[i].dy + 2][arr.s[i].dx] == 1) {
                            return true;
                        }
                        if (pos_x[i] - 1 < 20 || occupied[arr.s[i].dy][arr.s[i].dx - 1] == 1) {
                            return true;
                        }
                        if (pos_x[i] + 1 > 43 || occupied[arr.s[i].dy][arr.s[i].dx + 1] == 1) {
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        return false;
    }
   
    bool Move(int v) {
        Erase();

        if (v == 1) {
            if (!again) {
                int stop = 0;
                for (int j = 0; j < length; j++) {
                    if (pos_y[j] == 20) {
                        stop++;
                    }
                }

                if (stop > 0) {                            // no tetromino in the bottom
                    Draw();
                    update_occupy();
                    again = 1;
                    Point(1000);
                    return false;
                }
                else if (detect_confliction() == 0) {     // fall
                    for (int i = 0; i < length; i++) {
                        pos_y[i] += 1;
                    }
                    Draw();
                    Point(1000);
                    return true;
                }
                else {                                 // stack
                    Draw();
                    update_occupy();
                    again = 1;
                    Point(1000);
                    return false;
                }
            }
            else {
                for (int k = 0; k < nHeight; k++) {
                    for (int j = 0; j < nWidth; j += 2) {
                        if (occupied[k][j] == 1) {
                            show(k + 2, j + 20, type_id + 1);
                        }
                        else {
                            show(k + 2, j + 20, 0);
                        }
                    }
                }
                return false;
            }
            
        }
        else if (v == 2) { 
            for (int i = 0; i < length; i++) {        // attach to the left board
                if (pos_x[i] == 20) {
                    return false;  
                }
            }


            if (detect_confliction(v) == 0) {      // horezential movement
                for (int i = 0; i < length; i++) {
                    pos_x[i] -= 2;
                }
                Draw();
            }

            return false;
        }
        else if (v == 3) {
            for (int i = 0; i < length; i++) {       //  attach to the right board
                if (pos_x[i] == 44) {
                    return false;
                }
            }

            if (detect_confliction(v) == 0) {     // horezential movement
                for (int i = 0; i < length; i++) {
                    pos_x[i] += 2;
                }
                Draw();
            }
            return false;
        }
        else if (v == 0) {
            if (detect_confliction(v) == 0) {   // can or can not agree to rotate
                rotate_time++;
                if (rotate_time % 4 == 0) {
                    int origin_y = pos_y[0];
                    int origin_x = pos_x[0];

                    for (int i = 0; i < 4; i++) {
                        pos_y[i] -= origin_y;
                        pos_x[i] -= origin_x;

                        pos_y[i] = t1[type_id].s[i].dy + origin_y;
                        pos_x[i] = t1[type_id].s[i].dx + origin_x;
                    }
                }
                else if (rotate_time % 4 == 1) {
                    int origin_y = pos_y[0];
                    int origin_x = pos_x[0];

                    for (int i = 0; i < 4; i++) {
                        pos_y[i] -= origin_y;
                        pos_x[i] -= origin_x;

                        pos_y[i] = t2[type_id].s[i].dy + origin_y;
                        pos_x[i] = t2[type_id].s[i].dx + origin_x;
                    }
                }
                else if (rotate_time % 4 == 2) {
                    int origin_y = pos_y[0];
                    int origin_x = pos_x[0];

                    for (int i = 0; i < 4; i++) {
                        pos_y[i] -= origin_y;
                        pos_x[i] -= origin_x;

                        pos_y[i] = t3[type_id].s[i].dy + origin_y;
                        pos_x[i] = t3[type_id].s[i].dx + origin_x;
                    }
                }
                else {
                    int origin_y = pos_y[0];
                    int origin_x = pos_x[0];

                    for (int i = 0; i < 4; i++) {
                        pos_y[i] -= origin_y;
                        pos_x[i] -= origin_x;

                        pos_y[i] = t4[type_id].s[i].dy + origin_y;
                        pos_x[i] = t4[type_id].s[i].dx + origin_x;
                    }
                }
            }
            Draw();
            return false;
        }
        else {
            return false;
        }
    }

    void Point(int num) {
        static int point = 0;
        if (num > 2 && num<1000) {
            point += 90;
        }
        else if (num == 2) {
            point += 40;
        }
        else if(num==1){
            point += 10;
        }
        else if (num == 1000) {
            point += 1;
        }
        else {
            point += 0;
        }

        char Cpoint[10];
        itoa(point, Cpoint, 10);
        string Cstr(Cpoint);
        string text("Score:") ;
        text=text+Cstr;
        setText(text);
        show(2, 5, 0);
        setText(block);
    }
};





