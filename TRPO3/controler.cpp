#include "editor.cpp"

class controler
{
private:
    Editor ed;
    Processor proc;
    Memory<PNumber> mem;
    History<PNumber> hist;
    PNumber number;

public:
    bool add_digit(const char &nb, const int &base)
    {
        if (ed.add(nb, base))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void add_history(const string &op, PNumber &res)
    {
        hist.add(op, res);
    }

    void add_memory()
    {
        mem.add(number);
    }

    void clear_history()
    {
        hist.clear();
    }

    void clear_number()
    {
        ed.clear();
    }

    void clear_memory()
    {
        mem.clear();
    }

    vector<string> get_history()
    {
        return hist.get();
    }

    string get_number()
    {
        return ed.get();
    }

    PNumber get_memory()
    {
        return mem.get();
    }

    void set_memory(const PNumber &nb)
    {
        mem.set(nb);
    }

    PNumber execute(const int &bs){
        number = proc.evaluateExpression(ed.get(), bs);
        return proc.evaluateExpression(ed.get(), bs);
    }

    bool check_correct_ex()
    {
        if (ed.check_correct_ss())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    string get_operation(){
        return ed.get();
    }
    void set(const string &nm, const int &bs)
    {
        number.set(nm, bs);
    }

    void remove_digit(){
        ed.remove_digit();
    }
};