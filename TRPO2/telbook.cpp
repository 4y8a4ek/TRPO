#include "./abon.cpp"
#include <map>
#include <iostream>
#include <algorithm>

using namespace std;

class TelBook
{
private:
    map<string, string> TelList;

public:
    const void add(Abonent &ab)
    {
        TelList[(ab.get())[0]] = ab.get()[1];
    }

    const void del(Abonent &ab)
    {
        TelList.erase(ab.get()[0]);
    }

    const map<string, string> get()
    {
        return TelList;
    }

    const int count()
    {
        return TelList.size();
    }

    map<string, string> findByNumber(const string &number, const map<string, string> &tlb)
    {
        map<string, string> res;
        if (number.size() >= 3 && all_of(number.begin(), number.end(), ::isdigit)) // Проверяем, что это число
        {
            for (auto &entry : tlb)
            {
                if (entry.first.find(number) != string::npos) // Подстрока в номере
                {
                    res[entry.first] = entry.second;
                }
            }
        }

        return res;
    }

    map<string, string> findByName(const string &name, const map<string, string> &tlb)
    {
        map<string, string> res;
        for (auto &entry : tlb)
        {
            if (entry.second.find(name) == 0) // Имя начинается с введенной строки
            {
                res[entry.first] = entry.second;
            }
        }
        return res;
    }

    const void clear()
    {
        TelList.clear();
        cout << TelList.size();
    }
};

