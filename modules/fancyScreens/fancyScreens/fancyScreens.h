#pragma once
#include <Windows.h>
#include <string>
#include <algorithm>
#include <iostream>

class FancyScreens
{
public:
    FancyScreens() = default;
    ~FancyScreens() = default;

    static void SetDisplay(bool swapSecondaryDisplays);

private:
    static void SwapDisplays(DEVMODE* display1, DEVMODE* display2);
};

