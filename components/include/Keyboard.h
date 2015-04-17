#pragma once

#include "DataType.h"

#include <string>

#define PRESS_A

class Keyboard
{
private:
    enum press
    {
        c = 'c',
        t = 't',
        x = 'x',
        X = 'X',
        y = 'y',
        Y = 'Y',
        z = 'z',
        Z = 'Z',
        r = 'r',
        o = 'o',
        O = 'O',
        p = 'p',
        P = 'P',
        l = 'l',
        L = 'L',
        s = 's',
        S = 'S',
        b = 'b',
        h = 'h',
        k1 = '1',
        k2 = '2',
        k3 = '3'
    };
public:
    Keyboard();
    ~Keyboard();

    static void keyPressCb(unsigned char key, INT32 x, INT32 y);
    static void specialFuncCb(int key, int x, int y);
    static std::string key2str(UINT8);
};

