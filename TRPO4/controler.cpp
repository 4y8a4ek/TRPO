#include "editor.cpp"

class BaseControler
{
public:
    virtual bool add_digit(const char &nb, const int &base) = 0;
    virtual void add_history(const string &op, const string &res) = 0;
    virtual void add_memory() = 0;
    virtual void clear_history() = 0;
    virtual void clear_number() = 0;
    virtual void clear_memory() = 0;
    virtual int get_mem_base() = 0;
    virtual vector<string> get_history() = 0;
    virtual string get_number() = 0;
    virtual string get_memory() = 0;
    virtual void set_memory(const string &nb) = 0;
    virtual string execute(const int &bs) = 0;
    virtual bool check_correct_ex() = 0;
    virtual string get_operation() = 0;
    virtual void set(const string &nm, const int &bs) = 0;
    virtual void remove_digit() = 0;
    virtual ~BaseControler() = default;
};

template <typename Number>
class controler : public BaseControler
{
private:
    Editor ed;
    Processor<Number> proc;
    Memory<Number> mem;
    History<Number> hist;
    Number number;

public:
    bool add_digit(const char &nb, const int &base) override
    {
        return ed.add(nb, base);
    }

    void add_history(const string &op, const string &res) override
    {
        hist.add(op, res);
    }

    void add_memory() override
    {
        mem.add(number);
    }

    void clear_history() override
    {
        hist.clear();
    }

    void clear_number() override
    {
        ed.clear();
    }

    void clear_memory() override
    {
        mem.clear();
    }

    vector<string> get_history() override
    {
        return hist.get();
    }

    string get_number() override
    {
        return ed.get();
    }

    string get_memory() override
    {
        return mem.get().get_number();
    }
    int get_mem_base() override
    {
        return mem.get().get_base();
    }

    void set_memory(const string &nb) override
    {
        Number temp;
        temp.set(nb, number.get_base());
        mem.set(temp);
    }

    string execute(const int &bs) override
    {
        number = proc.evaluateExpression(ed.get(), bs);
        return number.get_number();
    }

    bool check_correct_ex() override
    {
        return ed.check_correct_ss();
    }

    string get_operation() override
    {
        return ed.get();
    }

    void set(const string &nm, const int &bs) override
    {
        number.set(nm, bs);
    }

    void remove_digit() override
    {
        ed.remove_digit();
    }
};