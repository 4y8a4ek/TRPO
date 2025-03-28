#include <windows.h>
#include <commctrl.h>
#include "files.cpp"

const char g_szClassName[] = "PhonebookWindow";
char buffer[256];                          // Буфер для текста
const char g_szEditClass[] = "EditWindow"; // Новый класс для окна редактирования

string currentName, currentNumber;
HWND hwndEditWindow;
HWND hListView;
controler cont;
Files file;
map<string, string> contacts;
char *cur_path = "book.txt";

#define ID_TABLE 901

#define ID_BUT_CLEAR 101
#define ID_BUT_SAVE 102
#define ID_BUT_DEL 103
#define ID_BUT_EDIT 104
#define ID_BUT_CREATE 105
#define ID_BUT_ADD 106
#define ID_BUT_FIND 107
#define ID_BUT_LOAD 108
#define ID_BUT_OK 109
#define ID_BUT_CANCEL 110
#define ID_BUT_HELP 111

#define ID_NAME 201
#define ID_NUMBER 202
#define ID_NAME_EDIT 201
#define ID_NUMBER_EDIT 202
void ShowHelp(HWND hwnd)
{
    MessageBox(hwnd,
               "Руководство по использованию приложения:\n"
               "  1. Введите ФИО и номер телефона\n"
               "  2. Нажмите 'Добавить' для сохранения контакта\n"
               "  3. Выберите контакт в списке для редактирования или удаления\n"
               "  4. Нажмите 'Сохранить' для сохранения списка в файл\n"
               "  5. Нажмите 'Загрузить' для восстановления контактов из файла\n"
               "\nНазначение кнопок:\n"
               "  'Очистить' - удалить все контакты\n"
               "  'Сохранить' - записать контакты в файл\n"
               "  'Удалить' - удалить выбранный контакт\n"
               "  'Изменить' - редактировать выбранный контакт\n"
               "  'Загрузить' - загрузить контакты из файла\n"
               "  'Найти' - поиск контакта по ФИО или номеру",
               "Справка",
               MB_OK | MB_ICONINFORMATION);
}
std::wstring convertToWString(const char *str)
{
    int len = MultiByteToWideChar(CP_ACP, 0, str, -1, nullptr, 0); // Получаем размер буфера
    wchar_t *wstr = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, str, -1, wstr, len); // Конвертируем строку
    std::wstring result(wstr);
    delete[] wstr;
    return result;
}

void SaveFile(HWND hwnd)
{
    OPENFILENAME ofn;      // Диалоговое окно для выбора файла
    char szFile[260] = ""; // Буфер для пути к файлу

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Все файлы\0*.*\0Текстовые файлы\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = "Выберите место для сохранения файла";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

    // Открытие диалога сохранения файла
    if (GetSaveFileName(&ofn) == TRUE)
    {
        // Здесь ofn.lpstrFile будет содержать путь к выбранному файлу
        MessageBox(hwnd, ofn.lpstrFile, "Путь к файлу", MB_OK);
        file.save((convertToWString(ofn.lpstrFile)), cont.get()); // Передаем путь в функцию сохранения
        cur_path = ofn.lpstrFile;
    }
}

void Load(HWND hwnd)
{
    ListView_DeleteAllItems(hListView);

    int count = cont.count();
    for (int i = 1; i <= count; i++)
    {
        const string number = cont.get_number(i);
        const string name = cont.get_name(i);
        cout << i;

        LVITEM lvItem = {0};
        lvItem.mask = LVIF_TEXT;
        lvItem.iItem = ListView_GetItemCount(hListView); // Добавляем в конец списка
        lvItem.iSubItem = 0;                             // Первая колонка — имя
        lvItem.pszText = const_cast<char *>(name.c_str());
        ListView_InsertItem(hListView, &lvItem);

        // Вторая колонка — номер телефона
        ListView_SetItemText(hListView, lvItem.iItem, 1, const_cast<char *>(number.c_str()));
    }
}

void load_from_file(HWND hwnd)
{
    OPENFILENAME ofn;      // Диалоговое окно для выбора файла
    char szFile[260] = ""; // Буфер для пути к файлу

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Все файлы\0*.*\0Текстовые файлы\0*.TXT\0"; // Фильтр для файлов
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = "Выберите файл для загрузки"; // Описание для загрузки файла
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

    // Открытие диалога загрузки файла
    if (GetOpenFileName(&ofn) == TRUE)
    {
        // Здесь ofn.lpstrFile будет содержать путь к выбранному файлу
        MessageBox(hwnd, ofn.lpstrFile, "Путь к файлу", MB_OK);
        file.load(convertToWString(ofn.lpstrFile), cont); // Передаем путь в функцию загрузки
        cur_path = ofn.lpstrFile;
    }
    else
    {
        MessageBox(hwnd, "Ошибка при выборе файла!", "Ошибка", MB_ICONERROR | MB_OK); // Обработка ошибки
    }
    Load(hwnd);
}
void update(HWND hwnd)
{
    Load(hwnd);
}
void load_contacts_from_map()
{
    if (!hListView)
        return; // Проверяем, инициализирован ли ListView

    ListView_DeleteAllItems(hListView); // Очищаем список перед загрузкой новых данных

    for (auto &contact : contacts)
    {
        const std::string &name = contact.second;
        const std::string &number = contact.first;

        LVITEM lvItem = {0};
        lvItem.mask = LVIF_TEXT;
        lvItem.iItem = ListView_GetItemCount(hListView); // Добавляем в конец списка
        lvItem.iSubItem = 0;

        // Используем буфер для корректной передачи строки
        char nameBuffer[256];
        strncpy(nameBuffer, name.c_str(), sizeof(nameBuffer));
        nameBuffer[sizeof(nameBuffer) - 1] = '\0'; // Гарантируем null-терминированную строку
        lvItem.pszText = nameBuffer;

        // Добавляем строку и получаем индекс
        int index = ListView_InsertItem(hListView, &lvItem);
        if (index == -1)
        {
            std::cerr << "Ошибка вставки в ListView!" << std::endl;
            continue;
        }
        std::cout << "Добавление: " << name << " - " << number << std::endl;

        // Добавляем номер в 2-ю колонку
        ListView_SetItemText(hListView, index, 1, const_cast<char *>(number.c_str()));
    }
}

void ShowEditWindow(HWND parent)
{

    hwndEditWindow = CreateWindowEx(WS_EX_CLIENTEDGE, "EditClass", "Редактировать контакт",
                                    WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 450, 200,
                                    parent, NULL, GetModuleHandle(NULL), NULL);
    CreateWindowEx(0, "EDIT", currentName.c_str(),
                   WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL,
                   60, 50, 150, 20, hwndEditWindow, (HMENU)ID_NAME_EDIT, NULL, NULL);

    CreateWindowEx(0, "EDIT", currentNumber.c_str(),
                   WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL,
                   270, 50, 150, 20, hwndEditWindow, (HMENU)ID_NUMBER_EDIT, NULL, NULL);

    // Создаем кнопки
    CreateWindow("BUTTON", "OK", WS_VISIBLE | WS_CHILD, 150, 80, 100, 25, hwndEditWindow, (HMENU)ID_BUT_OK, NULL, NULL);
    CreateWindow("BUTTON", "Cancel", WS_VISIBLE | WS_CHILD, 260, 80, 100, 25, hwndEditWindow, (HMENU)ID_BUT_CANCEL, NULL, NULL);
    SetForegroundWindow(hwndEditWindow);
    if (hwndEditWindow == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    ShowWindow(hwndEditWindow, SW_SHOW);
    UpdateWindow(hwndEditWindow);
}
// Функция для создания окна редактирования
LRESULT CALLBACK EditWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BUT_OK)
        {
            // Обработать сохранение данных
            GetWindowText(GetDlgItem(hwnd, ID_NAME_EDIT), buffer, sizeof(buffer));
            string newName = buffer;
            GetWindowText(GetDlgItem(hwnd, ID_NUMBER_EDIT), buffer, sizeof(buffer));
            string newNumber = buffer;

            if (newName.empty() || newNumber.empty())
            {
                MessageBox(hwnd, "Введите имя и номер!", "Ошибка", MB_ICONWARNING | MB_OK);
                return 0;
            }
            if (newNumber.size() != 11)
            {
                MessageBox(hwnd, "Номер должен иметь длину в 11 цифр!", "Ошибка", MB_ICONWARNING | MB_OK);
                return 0;
            }

            // Обновляем контакт
            cont.edit(currentNumber, currentName, newNumber, newName);
            update(hwnd);
            DestroyWindow(hwnd); // Закрываем только окно редактирования
        }

        if (LOWORD(wParam) == ID_BUT_CANCEL)
        {
            hwndEditWindow = NULL;
            DestroyWindow(hwnd);
        }
        break;

    case WM_CLOSE:
        hwndEditWindow = NULL;
        DestroyWindow(hwnd); // Закрытие окна редактирования
        break;

    case WM_DESTROY:
        hwndEditWindow = NULL;
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
        // Создаем ListView
        HMENU hMenu = CreateMenu();
        hListView = CreateWindowEx(0, WC_LISTVIEW, NULL,
                                   WS_CHILD | WS_VISIBLE | LVS_REPORT,
                                   10, 10, 450, 100,
                                   hwnd, (HMENU)ID_TABLE, GetModuleHandle(NULL), NULL);

        LVCOLUMN lvCol;
        lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

        lvCol.pszText = "Name";
        lvCol.cx = 100;
        ListView_InsertColumn(hListView, 0, &lvCol);

        lvCol.pszText = "Number";
        lvCol.cx = 100;
        ListView_InsertColumn(hListView, 1, &lvCol);
        AppendMenu(hMenu, MF_STRING, ID_BUT_HELP, "Справка");
        SetMenu(hwnd, hMenu);
        // Создаем кнопки
        CreateWindow("BUTTON", "Очистить", WS_VISIBLE | WS_CHILD, 470, 10, 100, 25, hwnd, (HMENU)ID_BUT_CLEAR, NULL, NULL);
        CreateWindow("BUTTON", "Сохранить", WS_VISIBLE | WS_CHILD, 470, 40, 100, 25, hwnd, (HMENU)ID_BUT_SAVE, NULL, NULL);
        CreateWindow("BUTTON", "Удалить", WS_VISIBLE | WS_CHILD, 470, 70, 100, 25, hwnd, (HMENU)ID_BUT_DEL, NULL, NULL);
        CreateWindow("BUTTON", "Изменить", WS_VISIBLE | WS_CHILD, 470, 100, 100, 25, hwnd, (HMENU)ID_BUT_EDIT, NULL, NULL);
        CreateWindow("BUTTON", "Загрузить", WS_VISIBLE | WS_CHILD, 470, 130, 100, 25, hwnd, (HMENU)ID_BUT_LOAD, NULL, NULL);
        // Поля ввода и подписи
        CreateWindow("STATIC", "ФИО", WS_VISIBLE | WS_CHILD, 10, 160, 50, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 60, 160, 150, 20, hwnd, (HMENU)ID_NAME, NULL, NULL);

        CreateWindow("STATIC", "Номер", WS_VISIBLE | WS_CHILD, 220, 160, 50, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 270, 160, 150, 20, hwnd, (HMENU)ID_NUMBER, NULL, NULL);

        CreateWindow("BUTTON", "Добавить", WS_VISIBLE | WS_CHILD, 430, 160, 100, 25, hwnd, (HMENU)ID_BUT_ADD, NULL, NULL);
        CreateWindow("BUTTON", "Найти", WS_VISIBLE | WS_CHILD, 540, 160, 100, 25, hwnd, (HMENU)ID_BUT_FIND, NULL, NULL);
    }
    break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        if (wmId == ID_BUT_ADD)
        {
            GetWindowText(GetDlgItem(hwnd, ID_NAME), buffer, sizeof(buffer));
            string name = buffer;
            GetWindowText(GetDlgItem(hwnd, ID_NUMBER), buffer, sizeof(buffer));
            string number = buffer;
            int erorr = 0;
            for (size_t i = 0; i < number.length(); i++)
            {
                if (number[i] > '9' || number[i] < '0')
                {
                    MessageBox(hwnd, "Номер должен содержать только цифры!", "Ошибка", MB_ICONWARNING | MB_OK);
                    return 0;
                }
            }
            if (name.empty() || number.empty()) // Проверяем пустые поля
            {
                MessageBox(hwnd, "Введите имя и номер!", "Ошибка", MB_ICONWARNING | MB_OK);
                return 0; // Выход, чтобы не вызывать add() с пустыми строками
            }
            if (number.size() != 11)
            {
                MessageBox(hwnd, "Номер должен иметь длину в 11 цифр!", "Ошибка", MB_ICONWARNING | MB_OK);
                return 0;
            }
            erorr = cont.add(number, name);
            if (erorr == 1)
            {
                MessageBox(hwnd, "Контакт с таким номером уже существует!", "Ошибка", MB_ICONWARNING | MB_OK);
            }
            update(hwnd);
            SetWindowText(GetDlgItem(hwnd, ID_NAME), "");
            SetWindowText(GetDlgItem(hwnd, ID_NUMBER), "");
        }

        if (wmId == ID_BUT_CLEAR)
        {
            cont.clear();
            update(hwnd);
        }

        if (wmId == ID_BUT_DEL)
        {
            int index = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
            if (index == -1)
            {
                MessageBox(hwnd, "Выберите контакт для удаления!", "Ошибка", MB_ICONWARNING | MB_OK);
            }
            else
            {
                cont.del_by_number(index + 1);
                update(hwnd);
            }
        }

        if (wmId == ID_BUT_EDIT)
        {
            int index = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
            if (index != -1)
            {
                currentName = cont.get_name(index + 1);
                currentNumber = cont.get_number(index + 1);
                ShowEditWindow(hwnd);
            }
            else
            {
                MessageBox(hwnd, "Выберите контакт для редактирования!", "Ошибка", MB_ICONWARNING | MB_OK);
            }
        }

        if (wmId == ID_BUT_FIND)
        {
            GetWindowText(GetDlgItem(hwnd, ID_NAME), buffer, sizeof(buffer));
            string name = string(buffer);
            cout << name.length() << endl;
            GetWindowText(GetDlgItem(hwnd, ID_NUMBER), buffer, sizeof(buffer));
            string number = string(buffer);
            cout << number.length() << endl;
            if (name.length() == 0 && number.length() == 0) // Проверяем пустые поля
            {
                update(hwnd);
                return 0;
            }
            cout << contacts.size() << endl;
            if (name.length() == 0)
            {
                contacts.clear();
                map<string, string> res(cont.findByNumber(number, cont.get()));
                contacts.insert(res.begin(), res.end());
            }
            else if (number.length() == 0)
            {
                contacts.clear();
                map<string, string> res(cont.findByName(name, cont.get()));
                contacts.insert(res.begin(), res.end());
                cout << contacts.size();
            }
            if (number.length() != 0 && name.length() != 0)
            {
                contacts.clear();
                map<string, string> resc(cont.findByName(name, cont.get()));
                map<string, string> res(cont.findByNumber(number, resc));
                contacts.insert(res.begin(), res.end());
                cout << contacts.size();
            }
            cout << contacts.size();
            load_contacts_from_map();
        }

        if (wmId == ID_BUT_SAVE)
        {
            SaveFile(hwnd);
        }

        if (wmId == ID_BUT_LOAD)
        {
            load_from_file(hwnd);
        }

        if (wmId == ID_BUT_HELP)
        {
            ShowHelp(hwnd);
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
    MSG Msg = {};
    InitCommonControls();

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "PhoneBook";
    RegisterClass(&wc);

    WNDCLASS wcEdit = {0};
    wcEdit.lpfnWndProc = EditWndProc;
    wcEdit.hInstance = hInstance;
    wcEdit.lpszClassName = "EditClass";
    RegisterClass(&wcEdit);

    HWND hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "PhoneBook", "Телефонная книга",
                               WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 660, 250,
                               NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}