#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <codecvt>
#include <locale>
#include "telbook.cpp"
using namespace std;


class controler
{
private:
    TelBook tb;
    Abonent ab;

public:
    const int count()
    {
        return tb.count();
    }

    map<string, string> get()
    {
        return tb.get();
    }
    const vector<string> get_abon(const int &nm)
    {
        auto it = tb.get().begin();
        for (size_t i = 0; i < nm - 1; i++)
        {
            it++;
        }
        ab.edit(it->first, it->second);
        return ab.get();
    }

    const string get_number(const int &nm)
    {
        return get_abon(nm)[0];
    }

    const string get_name(const int &nm)
    {
        return get_abon(nm)[1];
    }

    int add(const string &nb, const string &nm)
    {
        if (tb.findByNumber(nb, tb.get()).empty())
        {
            ab.edit(nb, nm);
            tb.add(ab);
            return 0;
        }
        else
        {
            return 1;
        }
    }

    const void del_by_number(const int &nm)
    {
        get_abon(nm);
        tb.del(ab);
    }

    const void del(const string &nb)
    {
        ab.edit(nb, "");
        tb.del(ab);
    }

    const void clear()
    {
        tb.clear();
    }

    map<string, string> findByName(const string &str, const map<string, string> tlb)
    {
        return tb.findByName(str, tlb);
    }
    map<string, string> findByNumber(const string &str, const map<string, string> tlb)
    {
        return tb.findByNumber(str, tlb);
    }

    const void edit(const string &olnb, const string &olnm, const string &nb, const string &nm)
    {
        ab.edit(olnb, olnm);
        tb.del(ab);
        ab.edit(nb, nm);
        tb.add(ab);
    }
};