#include <windows.h>
#include <commctrl.h>
#include <sstream>
#include <iostream>
#include "controler.cpp"

#define ID_BUTTONS_START 300

#define ID_BUT_MemClear 100
#define ID_BUT_MemCopy 101
#define ID_BUT_MemSave 102
#define ID_BUT_MemAdd 103
#define ID_BUT_MemCond 104

#define ID_BUT_BackSpace 110
#define ID_BUT_Clear 111
#define ID_BUT_Erase 112

#define ID_BUT_OPERATION_START 200

#define ID_TRACKBAR 400

#define ID_MENU_HISTORY 500
#define ID_MENU_EDIT 501
#define ID_MENU_HELP 502
#define ID_MENU_EXIT 503
#define ID_MENU_COPY 504
#define ID_MENU_PASTE 505

#define ID_EDITPOLE 600
#define ID_BASEPOLE 601

bool ButtonChange = false;
bool ProgrammChange = false;

int wmId;
using namespace std;

controler cont;

void UpdateBaseEdit(HWND hwndEdit, HWND hwndTrackbar)
{
    int value = SendMessage(hwndTrackbar, TBM_GETPOS, 0, 0);
    char buffer[10];
    sprintf(buffer, "%d", value);
    SetWindowText(hwndEdit, buffer);
}

HWND hTrackbar, hEdit, hBase, hHistoryWindow, hEditHistory;

const char *OperLabels[] = {"+", "-", "*", "/", "^", "Sqr", "1/x", "="};
const char *NumberLabels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
const char g_szClassName[] = "CalculatorWinAPI";

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

    string historyText = "";
    vector<string> res = cont.get_history();
    for (size_t i = 0; i < res.size(); i++)
    {
        historyText = historyText + res[i] + "\r\n";
        cout << historyText;
    }

    SetWindowText(hEditHistory, historyText.c_str());
}

void CopyToClipboard(const string &text)
{
    if (OpenClipboard(NULL))
    {
        EmptyClipboard();
        HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
        if (hGlob)
        {
            char *pData = (char *)GlobalLock(hGlob);
            if (pData)
            {
                memcpy(pData, text.c_str(), text.size() + 1);
                GlobalUnlock(hGlob);

                SetClipboardData(CF_TEXT, hGlob);
            }
        }
        CloseClipboard();
    }
}

string PasteFromClipboard()
{
    string text;
    if (!OpenClipboard(NULL))
        return text;
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData)
    {
        char *pData = (char *)GlobalLock(hData);
        if (pData)
        {
            text = pData;
            GlobalUnlock(hData);
        }
    }
    CloseClipboard();
    return text;
}

LRESULT CALLBACK HistoryWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        break;
    case WM_CLOSE:
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
        HMENU hEditSubMenu = CreatePopupMenu();

        AppendMenu(hEditSubMenu, MF_STRING, ID_MENU_COPY, "Копировать");
        AppendMenu(hEditSubMenu, MF_STRING, ID_MENU_PASTE, "Вставить");

        AppendMenu(hSubMenu, MF_POPUP, (UINT_PTR)hEditSubMenu, "Правка");
        AppendMenu(hSubMenu, MF_STRING, ID_MENU_HISTORY, "История вычислений");
        AppendMenu(hSubMenu, MF_STRING, ID_MENU_HELP, "Справка");
        AppendMenu(hSubMenu, MF_SEPARATOR, ID_DEFAULTINST, "");
        AppendMenu(hSubMenu, MF_STRING, ID_MENU_EXIT, "Выход");
        AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Настройки");
        SetMenu(hwnd, hMenu);
        // Поле ввода
        hEdit = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
                             15, 15, 340, 40, hwnd, (HMENU)ID_EDITPOLE, NULL, NULL);
        CreateWindow("STATIC", "Основание:", WS_CHILD | WS_VISIBLE,
                     15, 345, 80, 20, hwnd, NULL, NULL, NULL);
        hBase = CreateWindow("EDIT", "10", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER,
                             310, 345, 50, 25, hwnd, (HMENU)ID_BASEPOLE, NULL, NULL);
        hTrackbar = CreateWindow(TRACKBAR_CLASS, "", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
                                 100, 345, 200, 25, hwnd, (HMENU)ID_TRACKBAR, NULL, NULL);
        SendMessage(hTrackbar, TBM_SETRANGE, TRUE, MAKELPARAM(2, 16));
        SendMessage(hTrackbar, TBM_SETPOS, TRUE, 10);

        CreateWindow("BUTTON", "", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                     15, 70, 40, 40, hwnd, (HMENU)(ID_BUT_MemCond), NULL, NULL);
        CreateWindow("BUTTON", "MC", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                     15, 125, 40, 40, hwnd, (HMENU)(ID_BUT_MemClear), NULL, NULL);
        CreateWindow("BUTTON", "MR", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                     15, 180, 40, 40, hwnd, (HMENU)(ID_BUT_MemCopy), NULL, NULL);
        CreateWindow("BUTTON", "MS", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                     15, 235, 40, 40, hwnd, (HMENU)(ID_BUT_MemSave), NULL, NULL);
        CreateWindow("BUTTON", "M+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                     15, 290, 40, 40, hwnd, (HMENU)(ID_BUT_MemAdd), NULL, NULL);
        EnableWindow(GetDlgItem(hwnd, ID_BUT_MemCond), false);

        CreateWindow("BUTTON", "BackSpace", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                     70, 70, 95, 40, hwnd, (HMENU)(ID_BUT_BackSpace), NULL, NULL);
        CreateWindow("BUTTON", "CE", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                     180, 70, 95, 40, hwnd, (HMENU)(ID_BUT_Clear), NULL, NULL);
        CreateWindow("BUTTON", "C", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                     290, 70, 95, 40, hwnd, (HMENU)(ID_BUT_Erase), NULL, NULL);

        for (int i = 0; i < 8; i++)
        {
            CreateWindow("BUTTON", OperLabels[i], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                         290 + (i % 2) * 55, 125 + (i / 2) * 55, 40, 40,
                         hwnd, (HMENU)(ID_BUT_OPERATION_START + i), NULL, NULL);
        }
        for (int i = 0; i < 16; i++)
        {
            CreateWindow("BUTTON", NumberLabels[i], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                         70 + (i % 4) * 55, 125 + (i / 4) * 55, 40, 40,
                         hwnd, (HMENU)(ID_BUTTONS_START + i), NULL, NULL);
        }
        break;
    }
    case WM_HSCROLL:
        if ((HWND)lParam == hTrackbar)
        {
            UpdateBaseEdit(hBase, hTrackbar);
            SetWindowText(hEdit, "");
            char buffer[256];
            GetWindowText(hBase, buffer, sizeof(buffer));
            cont.set("", stoi(buffer));
            cont.clear_number();
            cont.clear_memory();
            SetWindowText(GetDlgItem(hwnd, ID_BUT_MemCond), "");
        }
        break;
    case WM_COMMAND:
        wmId = LOWORD(wParam);

        if (wmId >= ID_BUTTONS_START && wmId < ID_BUTTONS_START + 16)
        {
            ButtonChange = true;
            const char *btnLabel = NumberLabels[wmId - ID_BUTTONS_START];
            char buffer[256];
            char label = *btnLabel;
            GetWindowText(hBase, buffer, sizeof(buffer));
            if (!cont.add_digit(label, stoi(buffer)))
            {
                MessageBox(hwnd, "Введена некорректная цифра!", "Ошибка", MB_OK | MB_ICONINFORMATION);
            }
            SetWindowText(hEdit, cont.get_number().c_str());
            ButtonChange = false;
            break;
        }

        if (wmId >= ID_BUT_OPERATION_START && wmId < ID_BUT_OPERATION_START + 5)
        {
            ButtonChange = true;
            const char *operLabel = OperLabels[wmId - ID_BUT_OPERATION_START];
            char buffer[256];
            char lab = *operLabel;
            GetWindowText(hBase, buffer, sizeof(buffer));
            if (!cont.add_digit(lab, stoi(buffer)))
            {
                MessageBox(hwnd, "Введен некорректный оператор!", "Ошибка", MB_OK | MB_ICONINFORMATION);
            }
            SetWindowText(hEdit, cont.get_number().c_str());
            ButtonChange = false;
            break;
        }

        if (wmId == 207)
        {
            ButtonChange = true;
            char buffer[256];
            GetWindowText(hBase, buffer, sizeof(buffer));
            SetWindowText(hEdit, cont.execute(stoi(buffer)).get_number().c_str());
            PNumber res = cont.execute(stoi(buffer));
            cont.add_history(cont.get_operation(), res);
            cont.clear_number();
            ButtonChange = false;
        }

        if (wmId == 206)
        {
            ButtonChange = true;
            cont.add_digit('^', 16);
            cont.add_digit('-', 16);
            cont.add_digit('1', 16);
            SetWindowText(hEdit, cont.get_number().c_str());
            ButtonChange = false;
        }

        if (wmId == 205)
        {
            ButtonChange = true;
            cont.add_digit('^', 16);
            char buffer[256];
            GetWindowText(hBase, buffer, sizeof(buffer));
            if (stoi(buffer) <= 2)
            {
                cont.add_digit('1', stoi(buffer));
                cont.add_digit('0', stoi(buffer));
            }
            else
            {
                cont.add_digit('2', stoi(buffer));
            }
            SetWindowText(hEdit, cont.get_number().c_str());
            ButtonChange = false;
        }

        if (wmId == ID_BUT_MemAdd)
        {
            cont.add_memory();
            SetWindowText(GetDlgItem(hwnd, ID_BUT_MemCond), "Using");
        }

        if (wmId == ID_BUT_MemSave)
        {
            cont.clear_memory();
            cont.add_memory();
            SetWindowText(GetDlgItem(hwnd, ID_BUT_MemCond), "Using");
        }

        if (wmId == ID_BUT_MemClear)
        {
            cont.clear_memory();
            SetWindowText(GetDlgItem(hwnd, ID_BUT_MemCond), "");
        }

        if (wmId == ID_BUT_MemCopy)
        {
            string curnum = cont.get_memory().get_number();
            int curbase = cont.get_memory().get_base();
            for (size_t i = 0; i < curnum.size(); i++)
            {
                cont.add_digit(curnum[i], curbase);
            }

            SetWindowText(hEdit, cont.get_number().c_str());
        }

        if (wmId == ID_BUT_Clear)
        {
            cont.clear_memory();
            cont.clear_number();
            cont.clear_history();
            SetWindowText(hEdit, "");
        }

        if (wmId == ID_BUT_Erase)
        {
            cont.clear_number();
            SetWindowText(hEdit, "");
        }

        if (wmId == ID_BUT_BackSpace)
        {
            cont.remove_digit();
            SetWindowText(hEdit, cont.get_number().c_str());
        }

        if (wmId == ID_MENU_HELP)
        {
            MessageBox(hwnd, "Руководство по использованию приложения: \n\n"
                             "1. Выберите основание системы счисления с помощью ползунка. Вы можете выбрать значения от 2 до 16.\n"
                             "2. Введите выражение в поле ввода.\n"
                             "3. Вводите только цифры, соответствующие выбранной системе счисления (например, для системы с основанием 16 можно использовать цифры от 0 до F).\n"
                             "4. Вводите только операции +, -, *, /, ^, Sqr, 1/x.\n"
                             "5. Нажмите кнопку \"EXEC\" для вычисления результата.\n\n"
                             "Назначение кнопок:\n"
                             "\"BackSpace\" - удаляет последний символ в выражении.\n"
                             "\"CE\" - очищает поле ввода.\n"
                             "\"C\" - очищает память и историю вычислений.\n"
                             "\"MC\" - очищает память.\n"
                             "\"MR\" - копирует число из памяти в поле ввода.\n"
                             "\"MS\" - сохраняет число в память.\n"
                             "\"M+\" - добавляет число в память.\n"
                             "\"EXEC\" - выполняет вычисление.\n\n"
                             "Для работы с буфером обмена используйте \"Правка\" в меню:\n"
                             "\"Копировать\" - копирует содержимое поля ввода в буфер обмена.\n"
                             "\"Вставить\" - вставляет содержимое буфера обмена в поле ввода.\n\n"
                             "Для просмотра истории вычислений выберите \"История вычислений\" в меню.\n",
                       "Справка", MB_OK | MB_ICONINFORMATION);
        }

        if (wmId == ID_MENU_EXIT)
        {
            PostQuitMessage(0);
        }

        if (wmId == ID_MENU_COPY)
        {
            char buffer[256];
            GetWindowText(hEdit, buffer, sizeof(buffer));
            CopyToClipboard(buffer);
        }

        if (wmId == ID_MENU_PASTE)
        {
            char buffer[256];
            GetWindowText(hBase, buffer, sizeof(buffer));
            string text = PasteFromClipboard();
            bool flag = false;
            for (size_t i = 0; i < text.size(); i++)
            {
                if (!cont.add_digit(text[i], stoi(buffer)))
                {
                    flag = true;
                    break;
                }
            }
            if (flag)
            {
                MessageBox(hwnd, "Часть скопированного текста не удалось вставить, так как он содержит некорректные символы!", "Ошибка", MB_OK | MB_ICONINFORMATION);
            }

            SetWindowText(hEdit, cont.get_number().c_str());
        }

        if (wmId == ID_MENU_HISTORY)
        {
            ShowHistoryWindow(hwnd);
        }

        if (wmId == ID_EDITPOLE && HIWORD(wParam) == EN_CHANGE)
        {
            if (!ButtonChange && !ProgrammChange)
            {
                char buffer[256];
                char basebuff[256];
                GetWindowText(hEdit, buffer, sizeof(buffer));
                GetWindowText(hBase, basebuff, sizeof(basebuff));
                string text(buffer);
                string curtext = cont.get_number();
                for (size_t i = curtext.size(); i < text.size(); i++)
                {
                    if (!cont.add_digit(text[i], stoi(basebuff)))
                    {
                        MessageBox(hwnd, "Часть скопированного текста не удалось вставить, так как он содержит некорректные символы!", "Ошибка", MB_OK | MB_ICONINFORMATION);
                        break;
                    }
                }
                ProgrammChange = true;
                SetWindowText(hEdit, cont.get_number().c_str());
                SendMessage(hEdit, EM_SETSEL, (WPARAM)cont.get_number().length(), (LPARAM)cont.get_number().length());
                ProgrammChange = false;
            }
        }
        
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = g_szClassName;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassEx(&wc);

    WNDCLASS wcHistory = {0};
    wcHistory.lpfnWndProc = HistoryWndProc;
    wcHistory.hInstance = hInstance;
    wcHistory.lpszClassName = "HistoryClass";
    RegisterClass(&wcHistory);

    HWND hwnd = CreateWindowEx(0, g_szClassName, "Калькулятор р-ичных чисел", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
                               CW_USEDEFAULT, CW_USEDEFAULT, 400, 430, NULL, NULL, hInstance, NULL);
    if (!hwnd)
        return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
