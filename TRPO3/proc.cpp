#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include "number.cpp"

using namespace std;

class Processor
{
public:
    Converter_t convt;
    PNumber evaluateExpression(const string &expr, const int &bs)
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
        if (op == "^" || op == "^-") // Учитываем ^-
            return 3;
        return 0;
    }

    bool isOperator(const string &op)
    {
        return op == "+" || op == "-" || op == "*" || op == "/" || op == "^" || op == "^-";
    }

    string infixToPostfix(const string &expr)
    {
        stack<string> ops;
        stringstream output;
        size_t i = 0;

        while (i < expr.length())
        {
            if (isalnum(expr[i]) || expr[i] == '.')
            {
                while (i < expr.length() && (isalnum(expr[i]) || expr[i] == '.'))
                {
                    output << expr[i];
                    i++;
                }
                output << ' ';
                i--;
            }
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
                else
                {
                    ops.push("^");
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

    PNumber evaluatePostfix(const string &postfix, const int &bs)
    {
        stack<PNumber> stack;
        stringstream ss(postfix);
        string token;

        while (ss >> token)
        {
            if (isdigit(token[0]) || isalpha(token[0]) || token.find('.') != string::npos)
            {
                stack.push(PNumber(token, bs));
            }
            else
            {
                PNumber b = stack.top();
                stack.pop();
                PNumber a = stack.top();
                stack.pop();

                if (token == "+")
                    stack.push(a + b);
                else if (token == "-")
                    stack.push(a - b);
                else if (token == "*")
                    stack.push(a * b);
                else if (token == "/")
                    stack.push(a / b);
                else if (token == "^")
                    stack.push(a ^ convt.Do(b.get_number(), b.get_base(), 8));
                else if (token == "^-")
                    stack.push(a ^ ( - convt.Do(b.get_number(), b.get_base(), 8)));
            }
        }
        return stack.top();
    }
};
