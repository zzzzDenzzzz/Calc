#include<iostream>
#include<Windows.h>
#include<string>
#include<fstream>
#include<vector>

using namespace std;

const string PATH = "exp.txt";
const char SPACE = ' ';

double number(ifstream &fin, int &divider, bool &flag);
double brackets(ifstream &fin, int &divider, bool &flag);
double factor(ifstream &fin, int &divider, bool &flag);
double expression(ifstream &fin, int &divider, bool &flag);

void deleteSpace(string &str);
bool ruleChecking(string &str);
bool rule_1(string &str);
bool rule_2(string &str);
bool rule_3(string &str);
bool rule_4(string &str);
bool rule_5(string &str);
void error();

int main()
{
    SetConsoleOutputCP(1251);

    string str_expression;
    bool error_input = true;
    do
    {
        cout << "Введите выражение: \n";
        getline(cin, str_expression);
        deleteSpace(str_expression);
        if (ruleChecking(str_expression))
        {
            error_input = false;
        }
        else
        {
            error();
        }
    } while (error_input);
    
    ofstream fout;
    fout.open(PATH, ios::out);
    fout.clear();
    fout << str_expression;
    fout.close();

    ifstream fin;
    fin.open(PATH, ios::in);
    int divider = 10;
    bool flag = true;
    double result = expression(fin, divider, flag);
    fin.close();
    cout << "Результат: " << result << endl;
    
    return 0;
}

void deleteSpace(string &str)
{
    str.erase(remove(str.begin(), str.end(), SPACE), str.end());
}

bool ruleChecking(string &str)
{
    if (rule_1(str) && rule_2(str) && rule_3(str) && rule_4(str) && rule_5(str))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool rule_1(string &str)
{
    if (str[0] == '(' || str[0] == '-' || str[0] == '+' || str[0] >= '0' && str[0] <= '9')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool rule_2(string &str)
{
    vector<string>sub
    {"/*", "*/", "**", "//", "()",
     "/)", "*)", ")(", "(*", "(/",
     "..", ".*", "*.", "./", "/.",
     "(.", ".(", ").", "(.", ".+",
     "+.", ".-", "-."
    };
    for (size_t i = 0; i < sub.size(); i++)
    {
        size_t pos = str.find(sub[i]);
        if (pos != string::npos)
        {
            return false;
        }
    }
    return true;
}

bool rule_3(string &str)
{
    int count_breckets = 0;
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == '(')
        {
            count_breckets++;
        }
        else if (str[i] == ')')
        {
            count_breckets--;
        }
    }
    if (count_breckets == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool rule_4(string &str)
{
    bool rule;
    size_t i = 0;
    vector<size_t>pos_lb{0};
    for (i = str.find("(", i++); i != string::npos; i = str.find("(", i + 1))
    {
        pos_lb.push_back(i);
    }
    i = 0;
    vector<size_t>pos_rb{0};
    for (i = str.find(")", i++); i != string::npos; i = str.find(")", i + 1))
    {
        pos_rb.push_back(i);
    }

    for (int j = 0; j < pos_lb.size(); j++)
    {
        if (pos_lb[j] < pos_rb[j])
        {
            rule = true;
        }
        else if (pos_lb[j] > pos_rb[j])
        {
            rule = false;
        }
        else
        {
            rule = true;
        }
    }
    return rule;
}

bool rule_5(string &str)
{
    bool match = false;
    vector<char>sym
    {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(', ')', '+', '-', '*', '/', '.'};
    for (size_t i = 0; i < str.length(); i++)
    {
        match = false;
        for (size_t j = 0; j < sym.size(); j++)
        {
            if (str[i] == sym[j])
            {
                match = true;
                break;
            }
        }
        if (match == false)
        {
            return false;
        }
    }
    return match;
}

void error()
{
    cout << "Ошибка ввода! Попробуйте еще раз!\n";
}

double number(ifstream &fin, int &divider, bool &flag)
{
    int factor = 10;
    double res = 0;
    for (;;)
    {
        char c = fin.get();
        if (c >= '0' && c <= '9' && flag == true)
        {
            res = res * factor + c - '0';
        }
        else if (c == '.' || c >= '0' && c <= '9' && flag == false)
        {
            if (flag)
            {
                flag = false;
                continue;
            }
            else
            {
                double res_1 = 0;
                res_1 = (res_1 * factor + c - '0') / divider;
                res = res + res_1;
                divider *= factor;
            }
        }
        else
        {
            flag = true;
            divider = factor;
            fin.putback(c);
            return res;
        }
    }
}

double brackets(ifstream &fin, int &divider, bool &flag)
{
    char c = fin.get();
    if (c == '(')
    {
        double x = expression(fin, divider, flag);
        fin.get();
        return x;
    }
    else
    {
        fin.putback(c);
        return number(fin, divider, flag);
    }
}

double factor(ifstream &fin, int &divider, bool &flag)
{
    double x = brackets(fin, divider, flag);
    for (;;)
    {
        char c = fin.get();
        switch (c)
        {
        case '*':
            x *= brackets(fin, divider, flag);
            break;
        case '/':
            if (brackets(fin, divider, flag) == 0)
            {
                cout<< "Деление на ноль!\n";
                _exit(0);
            }
            x /= brackets(fin, divider, flag);
            break;
        default:
            fin.putback(c);
            return x;
        }
    }
}

double expression(ifstream &fin, int &divider, bool &flag)
{
    double x = factor(fin, divider, flag);
    for (;;)
    {
        char c = fin.get();
        switch (c)
        {
        case '+':
            x += factor(fin, divider, flag);
            break;
        case '-':
            x -= factor(fin, divider, flag);
            break;
        default:
            fin.putback(c);
            return x;
        }
    }
}