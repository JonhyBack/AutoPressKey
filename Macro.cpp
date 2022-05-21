#include "Macro.h"

using namespace std::chrono;

Macro::Macro(UINT recordToggleKey, UINT playKey, bool loop) {
    this->loop = loop;
    this->playKey = playKey;
    this->recordToggleKey = recordToggleKey;
}

void Macro::pressKey(MacroStep step) {
    INPUT input;

    input.ki.wScan = MapVirtualKey(step.key, 0);
    input.type = INPUT_KEYBOARD;

    auto duration = step.pressFor;
    const short speed = 50;

    Sleep(step.pause);

    do {
        input.ki.dwFlags = KEYEVENTF_SCANCODE;
        SendInput(1, &input, sizeof(input));

        Sleep(10);

        input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(input));

        duration -= speed;
    } while (duration > 0);
}

void Macro::record() {
    unsigned char key;
    steady_clock::time_point start = high_resolution_clock::now();
    steady_clock::time_point finish;
    long long pause;

    steps.empty();

    while (true) {
        Sleep(10);

        if (GetAsyncKeyState(recordToggleKey)) {
            Helper::waitForTargerKeyUp(recordToggleKey);
            break;
        }

        for (key = 8; key <= 190; key++)
        {
            SHORT keyInfo = GetAsyncKeyState(key);

            if (keyInfo < 0) {
                finish = high_resolution_clock::now();
                pause = duration_cast<std::chrono::milliseconds>(finish - start).count();

                std::cout << key << " is down and pause for: " << pause << std::endl;

                auto duration = watchForKeyRealease(key);

                std::cout << key << " is up for: " << duration << std::endl;

                auto step = MacroStep();
                step.key = key;
                step.pause = pause;
                step.pressFor = duration;

                steps.push_back(step);

                start = high_resolution_clock::now();
            }
        }
    }
}

long long Macro::watchForKeyRealease(unsigned char key) {
    steady_clock::time_point start;
    steady_clock::time_point finish;

    start = high_resolution_clock::now();
    while (true) {
        Sleep(50);

        SHORT keyState = GetAsyncKeyState(key);

        if (keyState == 0) {
            finish = high_resolution_clock::now();
            auto milliseconds = duration_cast<std::chrono::milliseconds>(finish - start);

            return milliseconds.count();
        }
    }
}

void Macro::run() {
    while (true) {
        for (auto& step : steps)
        {
            if (GetAsyncKeyState(playKey)) {
                Helper::waitForTargerKeyUp(playKey);
                return;
            }

            pressKey(step);
        }
    }
}