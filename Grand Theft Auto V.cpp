// ====================================================================
// this program is GTA Online PVP macro
// I do not know that this works properly
// support BST, buy all ammo, and quad lens helmet thermal
// ====================================================================

#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

HHOOK keyboardHook;
bool macroRunning = true;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

        if (wParam == WM_KEYDOWN)
        {
            cout << "key pressed: " << p->vkCode << endl;
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void StartHook()
{
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);

    if (keyboardHook == NULL)
    {
        cout << "keyboard hooking failed." << endl;

        return;
    }

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void PressKey(WORD vKey)
{
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = MapVirtualKey(vKey, MAPVK_VK_TO_VSC);
    input.ki.wVk = 0;
    input.ki.dwFlags = KEYEVENTF_SCANCODE;

    if (vKey == VK_UP || vKey == VK_DOWN || vKey == VK_LEFT || vKey == VK_RIGHT || vKey == VK_RETURN)
    {
        input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
    }

    SendInput(1, &input, sizeof(INPUT));
}

void ReleaseKey(WORD vKey)
{
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = MapVirtualKey(vKey, MAPVK_VK_TO_VSC);
    input.ki.wVk = 0;
    input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

    if (vKey == VK_UP || vKey == VK_DOWN || vKey == VK_LEFT || vKey == VK_RIGHT || vKey == VK_RETURN)
    {
        input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
    }

    SendInput(1, &input, sizeof(INPUT));
}

void SendKey(WORD key, int delay = 10)
{
    PressKey(key);
    this_thread::sleep_for(chrono::milliseconds(delay));
    ReleaseKey(key);
    // GTA 입력 씹힘 방지: 키를 뗀 후 다음 키가 눌리기 전까지의 딜레이
    this_thread::sleep_for(chrono::milliseconds(3));
}

void BST()
{
    SendKey(0x4D); // 'M' 키
    SendKey(VK_RETURN);
    SendKey(VK_UP); SendKey(VK_UP); SendKey(VK_UP);
    SendKey(VK_RETURN);
    SendKey(VK_DOWN);
    SendKey(VK_RETURN);
}

void ThermalVision()
{
    SendKey(0x4D);
    SendKey(VK_DOWN); SendKey(VK_DOWN); SendKey(VK_DOWN); SendKey(VK_DOWN); SendKey(VK_DOWN);
    SendKey(VK_RETURN);
    SendKey(VK_DOWN);
    this_thread::sleep_for(chrono::milliseconds(40));
    SendKey(VK_RETURN);
    SendKey(VK_UP); SendKey(VK_UP); SendKey(VK_UP); SendKey(VK_UP); SendKey(VK_UP); SendKey(VK_UP); SendKey(VK_UP); SendKey(VK_UP);
    SendKey(VK_SPACE);
    SendKey(0x4D);
}

void AMMO()
{
    SendKey(0x4D);
    SendKey(VK_DOWN); SendKey(VK_DOWN); SendKey(VK_DOWN); SendKey(VK_DOWN);
    SendKey(VK_RETURN);
    SendKey(VK_RETURN);
    for (int i{}; i < 9; i++)
    {
        SendKey(VK_RIGHT);
        SendKey(VK_DOWN);
        SendKey(VK_RETURN);
        SendKey(VK_UP);
    }
    SendKey(0x4D);
}

void Forcequit()
{
    macroRunning = false;
}

int main()
{
    cout << "Key input hooking activated. (must run as administrator)" << endl;

    while (macroRunning)
    {
        if ((GetAsyncKeyState(VK_RSHIFT) & 0x8000) && (GetAsyncKeyState(VK_F1) & 0x8000)) // 오른쪽 Shift + F1, 강제 종료
        {
            cout << "shutting down." << endl;
            Forcequit();
        }

        if (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) // 마우스 뒤로 가기, 불샤크 
        {
            cout << "BST" << endl;
            BST();
            while (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) Sleep(10);
        }

        if (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) // 마우스 앞으로 가기, 쿼드렌즈 헬멧 열감지
        {
            cout << "AMMO" << endl;
            AMMO();
            while (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) Sleep(10);
        }

        if (GetAsyncKeyState(VK_END) & 0x8000) // End, 모든 탄약 구매
        {
            cout << "ThermalVision ON/OFF" << endl;
            ThermalVision();
            while (GetAsyncKeyState(VK_END) & 0x8000) Sleep(10);
        }

        Sleep(10);
    }

    return 0;
}