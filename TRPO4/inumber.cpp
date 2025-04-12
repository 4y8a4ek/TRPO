#include <iostream>
#include <string>
#include <map>
#include <string>
#include <math.h>

using namespace std;

class INumber
{
private:
    string number;
    int base;
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
    INumber operator+(const INumber &second) const{
        INumber result(to_string(stof(number) + stof(second.number)), base);
        return result;
    }
    INumber operator-(const INumber &second) const{
        INumber result(to_string(stof(number) - stof(second.number)), base);
        return result;
    }
    INumber operator/(const INumber &second) const{
        INumber result(to_string(stof(number) / stof(second.number)), base);
        return result;
    }
    INumber operator^(const int &power) const{
        INumber result(to_string(pow(stof(number), power)), base);
        return result;
    }
    INumber operator*(const INumber &second) const{
        INumber result(to_string(stof(number) * stof(second.number)), base);
        return result;
    }
    string get_number(){
        return cut_zeros(number);;
    }
    int get_base(){
        return base;
    }

    INumber(const string &str, const int &bs){
        number = str;
        base =bs;
    }
    void set(const string &nm, const int &bs)
    {
        number = nm;
        base = bs;
    }
    INumber() = default;
};
