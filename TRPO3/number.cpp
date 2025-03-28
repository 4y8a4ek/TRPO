#include <iostream>
#include <string>
#include <map>
#include "converter_t.cpp"
#include "converter_p.cpp"

using namespace std;

class PNumber
{
private:
    map<string, int> alphabet = {{"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9}, {"A", 10}, {"B", 11}, {"C", 12}, {"D", 13}, {"E", 14}, {"F", 15}};
    int base ;
    string number;
    Converter_t convtt;
    Converter_p convtp;

    string cut_zeros(string st) {
        // Если точки нет, возвращаем строку без изменений
        if (st.find('.') == string::npos) {
            return st;
        }
        // Удаляем конечные нули, но оставляем саму точку, если она в конце
        while (!st.empty() && st.back() == '0') {
            st.pop_back();
        }
        // Если после удаления осталась точка в конце, тоже удаляем её
        if (!st.empty() && st.back() == '.') {
            st.pop_back();
        }
        return st;
    }

public:
    PNumber operator+(const PNumber &second) const
    {
        PNumber result(convtp.Do(convtt.Do(number, base, 8) + convtt.Do(second.number, second.base, 8), base, 8), base);
        return result;
    }

    PNumber operator-(const PNumber &second) const
    {
        PNumber result(convtp.Do(convtt.Do(number, base, 8) - convtt.Do(second.number, second.base, 8), base, 8), base);
        return result;
    }

    PNumber operator/(const PNumber &second) const
    {
        PNumber result(convtp.Do(convtt.Do(number, base, 8) / convtt.Do(second.number, second.base, 8), base, 8), base);
        return result;
    }

    PNumber operator^(const int &power) const
    {
        int powe = power;
        PNumber result(convtp.Do(pow(convtt.Do(number, base, 8), powe), base, 8), base);
        return result;
    }

    PNumber operator*(const PNumber &second) const
    {
        PNumber result(convtp.Do(convtt.Do(number, base, 8) * convtt.Do(second.number, second.base, 8), base, 8), base);
        return result;
    }

    PNumber conected() const
    {
        PNumber result(convtp.Do(1.0 / convtt.Do(number, base, 8), base, 8), base);
        return result;
    }

    string get_number()
    {
        return cut_zeros(number);
    }
    
    void set(const string &nm, const int &bs)
    {
        number = nm;
        base = bs;
    }

    int get_base(){
        return base;
    }
    PNumber(const string &nm, const int &bs)
    {
        number = nm;
        base = bs;
    }
    PNumber() = default;
};
