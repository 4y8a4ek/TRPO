#include <string>
#include <algorithm>
#include <iostream>

class Converter_p
{
private:
    static char itc(int n)
    {
        std::string digits = "0123456789ABCDEF";
        return digits[n];
    };

    static std::string itp(long int intPart, int p)
    {
        std::string res;
        do
        {
            res += itc(intPart % p);
            intPart /= p;
        } while (intPart > 0);
        std::reverse(res.begin(), res.end());
        return (res);
    };

    static std::string ftp( long double fracPart, int p, int c)
    {
        std::string res = ".";
        for (int i = 0; i < c; ++i)
        {
            fracPart *= p;
            res += itc(static_cast<int>(fracPart));
            fracPart -= static_cast<int>(fracPart);
        }
        return res;
    };

public:
    static std::string Do(long double n, int p, int c)
    {
        long int intPart = static_cast<int>(n);
        long double fracPart = n - intPart;
        if (c > 0 && fracPart != 0.0)
        {
            return itp(intPart, p) + ftp(fracPart, p, c);
        }
        else
        {
            return itp(intPart, p);
        }
    }
};

