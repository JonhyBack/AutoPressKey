#pragma once

#include <iostream>
#pragma once

#include <vector>
#include <Windows.h>
#include <chrono>
#include "Helper.h"
#include "MacroStep.h"

class Macro
{
    bool loop;
    UINT recordToggleKey;
    UINT playKey;
    std::vector<MacroStep> steps;

    static void pressKey(MacroStep step);
    long long watchForKeyRealease(unsigned char key);
public:
    Macro(UINT recordToggleKey = VK_NUMPAD1, UINT playKey = VK_NUMPAD2, bool loop = true);

    void record();
    void run();
};