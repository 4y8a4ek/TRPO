#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <algorithm>

using namespace std;
    class CNumber
    {
    private:
        double real;
        double imag;
        int base;

    public:
        CNumber operator+(const CNumber &second) const
        {
            return CNumber(real + second.real, imag + second.imag, base);
        }
        CNumber operator-(const CNumber &second) const
        {
            return CNumber(real - second.real, imag - second.imag, base);
        }
        CNumber operator/(const CNumber &second) const
        {
            double denominator = second.real * second.real + second.imag * second.imag;
            return CNumber(
                (real * second.real + imag * second.imag) / denominator,
                (imag * second.real - real * second.imag) / denominator,
                base);
        }
        CNumber operator^(const int &power) const
        {
            cout << "HERE" ;
            double r = sqrt(real * real + imag * imag);
            double theta = atan2(imag, real);
            double r_pow = pow(r, power);
            return CNumber(r_pow * cos(power * theta), r_pow * sin(power * theta), base);
        }
        CNumber operator*(const CNumber &second) const
        {
            return CNumber(
                real * second.real - imag * second.imag,
                real * second.imag + imag * second.real,
                base);
        }

        double get_real() const { return real; }
        double get_imag() const { return imag; }
        int get_base() const { return base; }
        string get_number() const
        {
            string res;
            res = to_string(get_real());
            res.erase(res.find_last_not_of('0') + 1, std::string::npos);
            if (res.back() == '.')
            {
                res.pop_back();
            }
            if (get_imag() >= 0){
                res += " + ";
            }
            else(res += " - ");
            res += to_string(get_imag());
            res.erase(res.find_last_not_of('0') + 1, std::string::npos);
            if (res.back() == '.')
            {
                res.pop_back();
            }
            res += "i";
            cout << res;
            return res;
        }

        // Конструктор, который преобразует строку в комплексное число
        CNumber(const string &str, const int &bs) : base(bs){
        real = 0.0;
        imag = 0.0;
    
        // Если число содержит 'i', это мнимая часть
        size_t pos = str.find('i');
        if (pos != string::npos)
        {
            string imag_part = str.substr(0, pos);
            if (imag_part.empty() || imag_part == "+")
                imag = 1.0;
            else if (imag_part == "-")
                imag = -1.0;
            else
                imag = stod(imag_part);  // Тут уже безопасно
        }
        else
        {
            real = stod(str);  // Если просто число, сохраняем в real
        }
    }
        

        CNumber(const double &r, const double &i, const int &bs) : real(r), imag(i), base(bs) {}

        void set(const std::string &number, const int &bs)
        {
            base = bs;
            real = 0.0;
            imag = 0.0;

            // Разделим строку на действительную и мнимую части
            size_t plusPos = number.find('+');
            size_t minusPos = number.find('-');

            if (plusPos != std::string::npos || minusPos != std::string::npos)
            {
                // Число содержит мнимую часть
                size_t splitPos = (plusPos != std::string::npos) ? plusPos : minusPos;
                std::string realPart = number.substr(0, splitPos);
                std::string imagPart = number.substr(splitPos + 1, number.length() - splitPos - 1);

                // Преобразуем части строки в числа
                real = stoi(realPart);
                imag = stoi(imagPart);
            }
            else
            {
                // Это только действительное число
                real = stoi(number);
                imag = 0.0;
            }
        }

        CNumber() = default;
    };