#include "Helper.h"

void Helper::waitForTargerKeyUp(UINT targetKey)
{
    while (true) {
        if (GetAsyncKeyState(targetKey) >= 0) {
            Sleep(50);
            return;
        }
    }
}
