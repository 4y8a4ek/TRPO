#include "proc.cpp"
#include <vector>
#include "memory.cpp"
#include "history.cpp"
using namespace std;

class Editor
{
private:
    vector<char> cor_char = {')', '(', ' ', '.'};
    vector<char> cor_oper = {'+', '-', '*', '/', '^'};
    vector<char> cor_numb = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    string cur_str;

    bool check_correct_char(const char &nb, const int &base)
    {
        for (size_t i = 0; i < base; i++)
        {
            if (nb == cor_numb[i])
            {
                return true;
            }
        }
        if (find(cor_char.begin(), cor_char.end(), nb) != cor_char.end())
        {
            return true;
        }
        if (find(cor_oper.begin(), cor_oper.end(), nb) != cor_oper.end())
        {
            return true;
        }
        return false;
    }

    bool check_correct_zero() {
        if (cur_str.empty()) {
            return true;
        }
        char last_char = cur_str.back();
        if (last_char == ' ' || last_char == '+' || last_char == '-' || last_char == '*' || last_char == '/' || last_char == '(') {
            return true;
        }
        int i = cur_str.size() - 1;
        while (i > 0 && isdigit(cur_str[i])) {
            i--;
        }
        if (!isdigit(cur_str[i])) {
            i++;
        }
        if (cur_str[i] == '0' && (i == cur_str.size() - 1 || !isdigit(cur_str[i + 1]))) {
            return false;
        }
        return true;
    }

    bool check_correct_oper(const char &nb)
    {
        if(nb == '-' && cur_str.back() == '^'){
            return true;
        }
        if(cur_str.empty()){
            return false;
        }
        if (find(cor_oper.begin(), cor_oper.end(), cur_str[cur_str.size() - 1]) != cor_oper.end())
        {
            return false;
        }
        if (cur_str[cur_str.size() - 1] == '(')
        {
            return false;
        }
        return true;
    }

    bool check_correct_dot()
    {
        if (cur_str[cur_str.size() - 1] == '.')
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    bool check_correct_scobe()
    {
        if (find(cor_oper.begin(), cor_oper.end(), cur_str[cur_str.size() - 1]) != cor_oper.end() || cur_str[cur_str.size() - 1] == '(')
        {
            return true;
        }
        else
        {
            return false;
        }
    }

public:
    bool check_correct_ss()
    {
        int count_open = 0, count_close = 0;
        for (size_t i = 0; i < cur_str.size() - 1; i++)
        {
            if (cur_str[i] == '(')
            {
                count_open++;
            }
            if (cur_str[i] == ')')
            {
                count_close++;
            }
        }
        if (count_close != count_open)
        {
            return false;
        }
        return true;
    }

    bool add(char nb, const int &base)
    {
        if (check_correct_char(nb, base))
        {
            if (nb == '0')
            {
                if (check_correct_zero())
                {
                    cur_str += nb;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            if (find(cor_oper.begin(), cor_oper.end(), nb) != cor_oper.end())
            {
                if (check_correct_oper(nb))
                {
                    cur_str += nb;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            if (nb == '.')
            {
                if (check_correct_dot())
                {
                    cur_str += nb;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            if (nb == '(')
            {
                if (check_correct_scobe())
                {
                    cur_str += nb;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            cur_str += nb;
            return true;
        }
        return false;
    }

    string get()
    {
        return cur_str;
    }

    void clear()
    {
        cur_str = "";
    }

    void remove_digit(){
        cur_str.pop_back();
    }
};

// int main()
// {
//     Editor ed;
//     Processor proc;
//     Memory<PNumber> mem;
//     History<PNumber> histor;

//     cout << "add 0   " << ed.add('0', 16) << endl;
//     cout << "add 0   " << ed.add('0', 16) << endl;
//     cout << "add .   " << ed.add('.', 16) << endl;
//     cout << "add .   " << ed.add('.', 16) << endl;
//     cout << "add F   " << ed.add('F', 16) << endl;
//     cout << "add (   " << ed.add('(', 16) << endl;
//     cout << "add +   " << ed.add('+', 16) << endl;
//     cout << "add (   " << ed.add('(', 16) << endl;
//     cout << "add (   " << ed.add('(', 16) << endl;
//     cout << "add F   " << ed.add('F', 16) << endl;
//     cout << "add +   " << ed.add('+', 16) << endl;
//     cout << "add F   " << ed.add('F', 16) << endl;
//     cout << "add )   " << ed.add(')', 16) << endl;
//     cout << "add )   " << ed.add(')', 16) << endl;
//     cout << "add ^   " << ed.add('^', 16) << endl;
//     cout << "add 2   " << ed.add('2', 16) << endl;

//     cout << ed.get() << endl;
//     if (ed.check_correct_ss())
//     {
//         PNumber res = proc.evaluateExpression(ed.get());
//         histor.add(ed.get(), res);
//         mem.add(res);
//         cout << "Result: |" << res.get_number() << "|" << endl;
//     }

//     cout << "Получено из памяти: " << mem.get().get_number() << endl;
//     if (ed.check_correct_ss())
//     {
//         PNumber res = proc.evaluateExpression("F.0^3");
//         histor.add("A.0^2", res);
//         mem.add(res);
//         cout << "Result: |" << res.get_number() << "|" << endl;
//     }
//     cout << "Получено из памяти: " << mem.get().get_number() << endl;
//     mem.clear();
//     cout << "Получено из памяти: " << mem.get().get_number() << endl;

//     cout << "Записи в истории: " << endl;
//     for (size_t i = 0; i < histor.get().size(); i++)
//     {
//         cout << "Запись с номером " << i + 1 << ": " << histor.get()[i] << endl;
//     }
// }