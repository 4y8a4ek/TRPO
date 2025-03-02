#include <iostream>
#include <string>
#include "converter_p.cpp"
#include "converter_t.cpp"
#include "history.cpp"
#include "number_handler.cpp"
class Control_ {
private:
    Converter_t convT; 
    Converter_p convP;  
    History history;  
    handler editor;     

    int Pin = 10;       
    int Pout = 16;

    int acc()
    {
        return static_cast<int>(std::round(editor.Acc() * std::log(Pin) / std::log(Pout) + 0.5));
    }
public:

    Control_() {}

    void setInput(const std::string& input) {
        editor.set_en(input.c_str());
    }
    std::string getInput() const {
        return std::string(editor.get_en());
    }
    std::string convert() {
        std::string input = getInput();
        double decimalValue = convT.Do(input, Pin, acc());
        std::string result = convP.Do(decimalValue, Pout,acc());
        history.addRecord(input, result, Pin, Pout);
        return result;
    }
    string getHistory() const {
        size_t historySize = history.size();
        if (historySize == 0) {
            return "No conversion history.";
        }
        string res;
        for (size_t i = 0; i < historySize; ++i) {
            res += history.getRecord(i) + "\r\n";
        }
        return res;
    }
    void clearHistory() {
        history.clearHistory();
        std::cout << "History cleared." << std::endl;
    }
    void setBase(int fromBase, int toBase) {
        Pin = fromBase;
        Pout = toBase;
    }

    void ce(){
        editor.Clear();
    }
    void bs(){
        editor.BackSpace();
    }
};
