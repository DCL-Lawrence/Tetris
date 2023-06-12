#ifndef WINCURSOR_H
#define WINCURSOR_H

#include <curses.h>
#undef MOUSE_MOVED
/*
https://www.reddit.com/r/cpp_questions/comments/583gc2/preprocessor_macro_redefinition_by_cursesh_and/
 */
#include <string>
#include <Windows.h>
#include <Winbase.h>
using std::string;

class Screen {
public:
    Screen();
    ~Screen();
    int key();
    int redraw(void);
    void pause(unsigned int seconds);
    void setTimeOut(int delay);
    string hVersion() { return "1.2"; }
    string version();   // obj version
private:
    WINDOW* win;
};

class String {
public:
    String(string s = "", size_t row=0, size_t col=0, short color=0);
    String(char ch, size_t row = 0, size_t col = 0, short color = 0);
    void show() const ;
    void String::show(int row, int col, int color = 0);
    void hide() const ;
    unsigned short getY();
    unsigned short getX();
    void setY(unsigned short row);
    void setX(unsigned short col);
    string String::getText();
    void String::setText(string s);
    void String::setText(char ch);
    void String::format(char* fmt, ...);
    bool outOfBoundary() const;
private:
    string text;
    short y;
    short x;
    short color;
};

class Timestamp
{
public:
    Timestamp();
    Timestamp(WORD tv_sec, WORD tv_usec);
    WORD getSec();
    WORD getUSec();
    double operator-(const Timestamp& t) ;
private:
    WORD sec;
    WORD usec;
};

#endif
