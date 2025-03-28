#include <map>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Abonent
{
private:
    string name;
    string number;

public:
    const vector<string> get()
    {
        vector<string> res;
        res.push_back(number);
        res.push_back(name);
        return res;
    }
    const void edit(string nnb, string nnm){
        name = nnm;
        number = nnb;
    }

    Abonent(const string &nm, const string &nb){
        name = nm;
        number = nb;
    }
    Abonent() = default;
    
};
