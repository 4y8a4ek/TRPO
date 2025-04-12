#include <string>
#include <iostream>
#include <cmath>
#include <stdexcept>
class Converter_t
{
private:
    const static double rtp(const double &value, const int &prec)
    {
        double factor = std::pow(10, prec);
        return std::round(value * factor) / factor;
    };
    const static double ctd(const char &ch, const int &base)
    {
        const std::string DIGITS = "0123456789ABCDEF";
        for (int j = 0; j < base; ++j)
        {
            if (DIGITS[j] == ch)
            {
                return j;
            }
        }
        throw std::invalid_argument("Invalid character '" + std::string(1, ch) + "' for base " + std::to_string(base));
    };

    const static long double itt(const std::string &s, const int &base)
    {
        int power = 0;
        long double result = 0.0;

        for (int i = s.size() - 1; i >= 0; --i)
        {
            int value = ctd(s[i], base);
            result += value * std::pow(base, power++);
        }

        return result;
    }

    const static long double ftp(const std::string &s, const int &base, const int &prec)
    {
        long double fraction = 0.0;
        long double divisor = base;

        for (size_t i = 0; i < s.size() && i < prec; ++i)
        {
            int value = ctd(s[i], base);
            fraction += value / divisor;
            divisor *= base;
        }

        return rtp(fraction, prec);
    }

public:
    static long double Do(const std::string &s, const int &base, const int &prec)
    {
        size_t pointPos = s.find('.');

        long double integerPartValue = 0.0;
        long double fractionPartValue = 0.0;

        if (pointPos != 0)
        {
            integerPartValue = itt(s.substr(0, pointPos), base);
        }
        if (pointPos != std::string::npos && pointPos + 1 < s.size())
        {
            fractionPartValue = ftp(s.substr(pointPos + 1), base, prec);
        }
        return integerPartValue + fractionPartValue;
    }
};