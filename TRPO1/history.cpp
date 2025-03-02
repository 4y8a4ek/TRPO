#include <iostream>
#include <list>
#include <string>
#include <sstream>
using namespace std;
class History {
private:
    struct Record {
        string inputNumber;
        string resultNumber;
        int fromBase;
        int toBase;

        Record(const string& input, const string& result, int from, int to)
            : inputNumber(input), resultNumber(result), fromBase(from), toBase(to) {}
    };

    list<Record> records;

public:

    void addRecord(const string& input, const string& result, int fromBase, int toBase) {
        records.emplace_back(input, result, fromBase, toBase);
    }

    string getRecord(size_t index) const {
        if (index >= records.size()) {
            return "Error: Invalid record index!";
        }
        auto it = records.begin();
        advance(it, index);
        ostringstream oss;
        oss << it->inputNumber << " (Base " << it->fromBase << ") -> "
            << it->resultNumber << " (Base " << it->toBase << ")";
        return oss.str();
    }

    void clearHistory() {
        records.clear();
    }

    size_t size() const {
        return records.size();
    }
};
