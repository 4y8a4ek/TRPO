#include "control.cpp"
#include <codecvt>
#include <locale>
#include <fstream>
class Files
{
private:
public:
    void save(const wstring &path, const map<string, string> teb)
    {
        // Преобразуем wstring в string (с UTF-8)
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::string utf8_path = converter.to_bytes(path);

        std::ofstream file(utf8_path, std::ios::out | std::ios::trunc);
        std::cerr << "Ошибка при открытии файла: " << utf8_path << std::endl;

        // Записываем данные в фа
        for (auto i = teb.begin(); i != teb.end(); i++)
        {
            file << i->first << "|" << i->second << "\n";
        }
        file.close();
    }

    void load(const wstring &path, controler &cont)
    {
        Abonent ab;

        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::string utf8_path = converter.to_bytes(path);
        fstream file(utf8_path);

        if (!file.is_open())
        {
            std::cerr << "Ошибка при открытии файла: " << utf8_path << std::endl;
            return;
        }

        string line, number, name;
        while (getline(file, line))
        {
            size_t del = line.find("|");
            if (del != string::npos)
            {
                number = line.substr(0, del);
                name = line.substr(del + 1);
                ab.edit(number, name);
                cont.add(number, name);
            }
            else
            {
                std::cerr << "Неверный формат строки: " << line << std::endl;
            }
        }
    }
};
