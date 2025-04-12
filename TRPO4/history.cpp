#include <vector>
#include <sstream>
#include <string>

using namespace std;

template <class Number>
class History
{
private:
    vector<string> hist;
public:
    void add(const string &oper, const string &res){
        stringstream st;
        st << oper << " = " << res;
        hist.push_back(st.str());
    }

    vector<string> get(){
        return hist;
    }

    void clear(){
        hist.clear();
    }

};
