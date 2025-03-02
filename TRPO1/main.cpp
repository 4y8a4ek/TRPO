#include <windows.h>
#include <commctrl.h>
#include <string>
#include <sstream>
#include <vector>
#include "resource.h"
#include "control.cpp"
#pragma comment(lib, "comctl32.lib")
#define ID_EDIT_INPUT 101
#define ID_EDIT_OUTPUT 102
#define ID_TRACKBAR1 103
#define ID_TRACKBAR2 104
#define ID_EDIT_BASE1 105
#define ID_EDIT_BASE2 106
#define ID_BUTTON_EXEC 200
#define ID_BUTTON_BS 201
#define ID_BUTTON_CE 202
#define ID_BUTTON_DOT 203
#define ID_BUTTONS_START 300
#define ID_MENU_EXIT 400
#define ID_MENU_HISTORY 401
#define ID_MENU_HELP 402
#define IDI_ICON1 101
const char *btnLabels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
HWND hEditInput, hEditOutput, hTrackbar1, hTrackbar2, hEditBase1, hEditBase2;
HWND hHistoryWindow = NULL;
HWND hEditHistory;
Control_ cont;
void UpdateBaseEdit(HWND hwndEdit, HWND hwndTrackbar)
{
    int value = SendMessage(hwndTrackbar, TBM_GETPOS, 0, 0);
    char buffer[10];
    sprintf(buffer, "%d", value);
    SetWindowText(hwndEdit, buffer);
}
void UpdateResult(HWND hwnd)
{
    char buffer[256];
    GetWindowText(hEditInput, buffer, sizeof(buffer));
    int fromBase = SendMessage(hTrackbar1, TBM_GETPOS, 0, 0); 
    int toBase = SendMessage(hTrackbar2, TBM_GETPOS, 0, 0);   
    cont.setInput(buffer);
    cont.setBase(fromBase, toBase);
    string result = cont.convert();;
    SetWindowText(hEditOutput, result.c_str());
    string historyText = cont.getHistory();
    SetWindowText(hEditHistory, historyText.c_str());
}

void ShowHistoryWindow(HWND parent)
{
    if (hHistoryWindow == NULL || !IsWindow(hHistoryWindow))
    {
        hHistoryWindow = CreateWindow("HistoryClass", "История вычислений", WS_OVERLAPPEDWINDOW,
                                      CW_USEDEFAULT, CW_USEDEFAULT, 350, 250,
                                      parent, NULL, GetModuleHandle(NULL), NULL);
        hEditHistory = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                                    10, 10, 315, 190, hHistoryWindow, NULL, GetModuleHandle(NULL), NULL);
        ShowWindow(hHistoryWindow, SW_SHOW);
    }
    else
    {
        SetForegroundWindow(hHistoryWindow);
    }
    string historyText = cont.getHistory(); 
    SetWindowText(hEditHistory, historyText.c_str());
}

void UpdateHexButtonsState(HWND hwnd)
{
    int toBase = SendMessage(hTrackbar1, TBM_GETPOS, 0, 0);
    for (int i = 0; i < 16; ++i)
    {
        HWND btn = GetDlgItem(hwnd, ID_BUTTONS_START + i);
        EnableWindow(btn, toBase > i);
    }
}

LRESULT CALLBACK HistoryWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        break;
    case WM_CLOSE:
        hHistoryWindow = NULL;
        DestroyWindow(hwnd);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        HMENU hMenu = CreateMenu();
        HMENU hSubMenu = CreatePopupMenu();
        AppendMenu(hSubMenu, MF_STRING, ID_MENU_HISTORY, "История вычислений");
        AppendMenu(hSubMenu, MF_STRING, ID_MENU_HELP, "Справка");
        AppendMenu(hSubMenu, MF_SEPARATOR, ID_DEFAULTINST, "");
        AppendMenu(hSubMenu, MF_STRING, ID_MENU_EXIT, "Выход");
        AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Настройки");
        SetMenu(hwnd, hMenu);
        hEditInput = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
                                  20, 30, 240, 25, hwnd, (HMENU)ID_EDIT_INPUT, NULL, NULL);
        hEditOutput = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_READONLY,
                                   20, 110, 240, 25, hwnd, (HMENU)ID_EDIT_OUTPUT, NULL, NULL);
        hTrackbar1 = CreateWindow(TRACKBAR_CLASS, "", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
                                  20, 65, 190, 30, hwnd, (HMENU)ID_TRACKBAR1, NULL, NULL);
        SendMessage(hTrackbar1, TBM_SETRANGE, TRUE, MAKELPARAM(2, 16));
        SendMessage(hTrackbar1, TBM_SETPOS, TRUE, 10);
        hEditBase1 = CreateWindow("EDIT", "10", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
                                  220, 65, 40, 25, hwnd, (HMENU)ID_EDIT_BASE1, NULL, NULL);
        hTrackbar2 = CreateWindow(TRACKBAR_CLASS, "", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
                                  20, 145, 190, 30, hwnd, (HMENU)ID_TRACKBAR2, NULL, NULL);
        SendMessage(hTrackbar2, TBM_SETRANGE, TRUE, MAKELPARAM(2, 16));
        SendMessage(hTrackbar2, TBM_SETPOS, TRUE, 16);
        hEditBase2 = CreateWindow("EDIT", "16", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
                                  220, 145, 40, 25, hwnd, (HMENU)ID_EDIT_BASE2, NULL, NULL);
        for (int i = 0; i < 16; i++)
        {
            CreateWindow("BUTTON", btnLabels[i], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                         20 + (i % 4) * 50, 200 + (i / 4) * 50, 40, 40,
                         hwnd, (HMENU)(ID_BUTTONS_START + i), NULL, NULL);
        }
        CreateWindow("BUTTON", ".", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            220, 200, 40, 40, hwnd, (HMENU)ID_BUTTON_DOT, NULL, NULL);
        CreateWindow("BUTTON", "BS", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                     220, 250, 40, 40, hwnd, (HMENU)ID_BUTTON_BS, NULL, NULL);
        CreateWindow("BUTTON", "CE", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                     220, 300, 40, 40, hwnd, (HMENU)ID_BUTTON_CE, NULL, NULL);
        CreateWindow("BUTTON", "EXEC", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                     220, 350, 40, 40, hwnd, (HMENU)ID_BUTTON_EXEC, NULL, NULL);
        UpdateHexButtonsState(hwnd);
        break;
    }

    case WM_HSCROLL:
    {
        if ((HWND)lParam == hTrackbar2)
        {
            UpdateBaseEdit(hEditBase2, hTrackbar2);
            UpdateResult(hwnd);
        }
    }
        if ((HWND)lParam == hTrackbar1)
        {
            UpdateBaseEdit(hEditBase1, hTrackbar1);
            UpdateHexButtonsState(hwnd);
            SetWindowText(hEditOutput, "");
            SetWindowText(hEditInput, "");
        }
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        if (wmId == ID_EDIT_INPUT && HIWORD(wParam) == EN_CHANGE)
        {
            SetWindowText(hEditOutput, "");
        }

        if (wmId >= ID_BUTTONS_START && wmId < ID_BUTTONS_START + 16)
        {
            const char *btnLabel = btnLabels[wmId - ID_BUTTONS_START];
            char buffer[256];
            GetWindowText(hEditInput, buffer, sizeof(buffer));
            strcat(buffer, btnLabel);
            SetWindowText(hEditInput, buffer);
        }

        if (wmId == ID_EDIT_BASE1 || wmId == ID_EDIT_BASE2)
        {
            if (HIWORD(wParam) == EN_CHANGE)
            {
                char buffer[10];
                GetWindowText((HWND)lParam, buffer, sizeof(buffer));
                int value = atoi(buffer);
                if (value >= 2 && value <= 16)
                {
                    if (wmId == ID_EDIT_BASE1)
                    {
                        SendMessage(hTrackbar1, TBM_SETPOS, TRUE, value);
                    }
                    else
                    {
                        SendMessage(hTrackbar2, TBM_SETPOS, TRUE, value);
                    }
                }
            }
        }
        if (wmId == ID_MENU_HISTORY)
        {
            ShowHistoryWindow(hwnd);
        }
        if (wmId == ID_BUTTON_EXEC)
        {
            UpdateResult(hwnd);
        }
        if( wmId == ID_BUTTON_CE)
        {
            cont.ce();
            SetWindowText(hEditOutput, cont.getInput().c_str());
            SetWindowText(hEditInput, cont.getInput().c_str());
        }
        if (wmId == ID_BUTTON_BS)
        {
            cont.bs();
            SetWindowText(hEditInput, cont.getInput().c_str());
        }
        if(wmId == ID_BUTTON_DOT)
        {
            char buffer[256];
            GetWindowText(hEditInput, buffer, sizeof(buffer));
            strcat(buffer, ".");
            SetWindowText(hEditInput, buffer);
        }

        if (wmId == ID_MENU_EXIT)
        {
            PostMessage(hwnd, WM_CLOSE, 0, 0);
        }
        if (wmId == ID_MENU_HELP)
        {
            MessageBox(hwnd, "Руководство по использованию приложения: \n \x20\x20 1. Введите число \n \x20\x20 2. Выберите системы счисления \n \x20\x20 3. Нажмите на кнопку \"EXEC\"  \n \n Назначение кнопок: \n \x20\x20 \"BS\" - удаление последнего символа \n \x20\x20 \"CE\" - очистить поле ввода \n \x20\x20 \"EXEC\" - вычислить", "Справка", MB_OK | MB_ICONINFORMATION);
        }
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Главная функция
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
    InitCommonControls(); // Инициализация библиотеки управления элементами

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "CalculatorWindow";
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClass(&wc);
    WNDCLASS wcHistory = {0};
    wcHistory.lpfnWndProc = HistoryWndProc;
    wcHistory.hInstance = hInstance;
    wcHistory.lpszClassName = "HistoryClass";
    RegisterClass(&wcHistory);

    HWND hwnd = CreateWindowEx(0, "CalculatorWindow", "Конвертер систем счисления",
                               WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 500,
                               nullptr, nullptr, hInstance, nullptr);
    if (hwnd == nullptr)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
