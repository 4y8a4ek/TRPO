#include "inumber.cpp"
#include "cnumber.cpp"
#include "pnumber.cpp"
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <stack>
#include <map>

template <class Number>
class Processor
{
public:
    Converter_t convt;
    Number evaluateExpression(const string &expr, const int &bs)
    {
        string postfix = infixToPostfix(expr);
        return evaluatePostfix(postfix, bs);
    }

private:
    int precedence(const string &op)
    {
        if (op == "+" || op == "-")
            return 1;
        if (op == "*" || op == "/")
            return 2;
        if (op == "^")
            return 3;
        return 0;
    }

    bool isOperator(const string &op)
    {
        return op == "+" || op == "-" || op == "*" || op == "/" || op == "^";
    }

    string infixToPostfix(const string &expr)
    {
        stack<string> ops;
        stringstream output;
        size_t i = 0;

        while (i < expr.length())
        {
            if (isalnum(expr[i]) || expr[i] == '.' || expr[i] == 'i' || (expr[i] >= 'A' && expr[i] <= 'F'))
        {
            while (i < expr.length() && (isalnum(expr[i]) || expr[i] == '.' || expr[i] == 'i' || (expr[i] >= 'A' && expr[i] <= 'F')))
            {
                output << expr[i];
                i++;
            }
            output << ' ';
            i--;
        }
        // Обработка скобок
        else if (expr[i] == '(')
        {
            ops.push("(");
        }
        else if (expr[i] == ')')
        {
            while (!ops.empty() && ops.top() != "(")
            {
                output << ops.top() << ' ';
                ops.pop();
            }
            ops.pop();
        }
        else if (expr[i] == '^')
            {
                if (i + 1 < expr.length() && expr[i + 1] == '-')
                {
                    ops.push("^-");
                    i++; // Пропускаем '-'
                }
            }
        else if (isOperator(string(1, expr[i])))
        {
            string op(1, expr[i]);
            while (!ops.empty() && precedence(ops.top()) >= precedence(op))
            {
                output << ops.top() << ' ';
                ops.pop();
            }
            ops.push(op);
        }
        i++;
        }
        while (!ops.empty())
        {
            output << ops.top() << ' ';
            ops.pop();
        }
        return output.str();
    }

    Number evaluatePostfix(const string &postfix, const int &bs)
    {
        stack<Number> stack;
        stringstream ss(postfix);
        string token;

        while (ss >> token)
        {
            if (isdigit(token[0]) || token.find('.') != string::npos || token.find('i') != string::npos || (token[0] >= 'A' && token[0] <= 'F'))
            {
                cout << "Обрабатываем токен: " << token << endl;
                stack.push(Number(token, bs));
            }
            else
            {
                Number b = stack.top();
                stack.pop();
                Number a = stack.top();
                stack.pop();

                cout << "Вычисляем: " << a.get_number() << " ^ " << b.get_number() << endl;
                if (token == "+")
                    stack.push(a + b);
                else if (token == "-")
                    stack.push(a - b);
                else if (token == "*")
                    stack.push(a * b);
                else if (token == "/")
                    stack.push(a / b);
                else if (token == "^")
                {
                    string s = b.get_number();
                    if (!s.empty() && s.back() == 'i')
                    {
                        s.pop_back(); // Удаляет последний символ
                        s.pop_back();
                    }
                    stack.push(a ^ convt.Do(s, b.get_base(), 8));
                }
                else if (token == "^-")
                {
                    string s = b.get_number();
                    if (!s.empty() && s.back() == 'i')
                    {
                        s.pop_back(); // Удаляет последний символ
                        s.pop_back();
                    }
                    stack.push(a ^ (-convt.Do(s, b.get_base(), 8)));
                }
            }
        }
        return stack.top();
    }
};
