// Name:Rithvik Konidena
// Roll Number : 2302CS09
// Declaration of Authorship
// this file myemu.cpp is a part of CS2102 course miniproject at the department of computer science egineering in iit patna

#include <bits/stdc++.h>
using namespace std;
#define fr(i, a, b) for (int i = (a); i < (int)(b); ++i)
#define all(a) a.begin(), a.end()
int memory[1 << 24];
int maxlimit = (1 << 23);
int p = 0;  // programcounter
int sp = 0; // stackpointer
int A = 0;  // regA  
int B = 0;  // regB
map<string, string> optab;
int i = 0;
int maxsp = 0; // adress till which it it will dump
void labelmnemonics()
{
    optab["ldc"] = "00";
    optab["adc"] = "01";
    optab["ldl"] = "02";
    optab["stl"] = "03";
    optab["ldnl"] = "04";
    optab["stnl"] = "05";
    optab["add"] = "06";
    optab["sub"] = "07";
    optab["shl"] = "08";
    optab["shr"] = "09";
    optab["adj"] = "0A";
    optab["a2sp"] = "0B";
    optab["sp2a"] = "0C";
    optab["call"] = "0D";
    optab["return"] = "0E";
    optab["brz"] = "0F";
    optab["brlz"] = "10";
    optab["br"] = "11";
    optab["HALT"] = "12";
}
void askquery();
vector<string> code;
string gethex(unsigned int num)
{
    if (num < 0)
    {
        int shift = 32;
        num += (1 << shift);
    }
    string s;
    while (num != 0)
    {
        int x = num % 16;
        if (x >= 0 && x <= 9)
        {
            s.push_back(x + '0');
        }
        else
        {
            s.push_back('A' + x - 10);
        }
        num /= 16;
    }
    while ((int)s.size() < 8)
    {
        s.push_back('0');
    }
    reverse(all(s));
    return s;
}
int power(int x, int n)
{
    int result = 1;
    while (n > 0)
    {
        if (n & 1 == 1)
        {
            result = (result * x);
        }
        x = (x * x);
        n = n >> 1;
    }
    return result;
}
int getdec(string k)
{
    if (k == "")
    {
        return 0;
    }
    int ans = 0;
    for (int j = (int)k.size() - 1; j >= 0; j--)
    {
        int val = 0;
        if (k[j] >= 'a' and k[j] <= 'f')
        {
            val = (k[j] - 'a' + 10);
        }
        else if (k[j] >= 'A' and k[j] <= 'F')
        {
            val = (k[j] - 'A' + 10);
        }
        else
        {
            val = k[j] - '0';
        }
        ans += (val * (int)power(16, (int)k.size() - j - 1));
    }
    if (ans >= (1 << 23)) //-ve number in 2's complement form
    {
        ans -= (1 << 24);
    }
    return ans;
}
void runopcode(string opcode, int operand, int iter)
{
    // cout << opcode << " " << operand << " " << A << " " << B << endl;
    if (opcode == "00")
    {
        B = A;
        A = operand;
    }
    else if (opcode == "01")
    {
        A += operand;
    }
    else if (opcode == "02")
    {
        B = A;
        if (sp + operand > maxlimit)
        {
            cout << "Error : Segmentation fault has occured1" << endl;
            askquery();
            return;
        }
        A = memory[sp + operand];
    }
    else if (opcode == "03")
    {
        if (sp + operand > maxlimit)
        {
            cout << "Error : Segmentation fault has occured2" << endl;
            askquery();
            return;
        }
        // cout << sp + operand << " " << A << endl;
        memory[sp + operand] = A;
        // cout << memory[sp + operand] << endl;
        maxsp = max(sp + operand, maxsp);
        A = B;
    }
    else if (opcode == "04")
    {
        if (sp + A > maxlimit)
        {
            cout << "Error : Segmentation fault has occured3" << endl;
            askquery();
            return;
        }
        A = memory[A + operand];
        maxsp = max(A + operand, maxsp);
    }
    else if (opcode == "05")
    {
        if (operand + A > maxlimit)
        {
            cout << "Error : Segmentation fault has occured4" << endl;
            askquery();
            return;
        }
        memory[A + operand] = B;
    }
    else if (opcode == "06")
    {
        A = A + B;
    }
    else if (opcode == "07")
    {
        A = B - A;
    }
    else if (opcode == "08")
    {
        A = B << A;
    }
    else if (opcode == "09")
    {
        A = B >> A;
    }
    else if (opcode == "0A")
    {
        if (sp + operand > maxlimit)
        {
            cout << "Error : Segmentation fault has occured5" << endl;
            askquery();
            return;
        }
        sp += operand;
        maxsp = max(sp, maxsp);
    }
    else if (opcode == "0B")
    {
        sp = A;
        A = B;
        if (sp > maxlimit)
        {
            cout << "Error : Segmentation fault has occured6" << endl;
            askquery();
            return;
        }
    }
    else if (opcode == "0C")
    {
        B = A;
        A = sp;
    }
    else if (opcode == "0D")
    {
        B = A;
        A = p;
        p += operand;
    }
    else if (opcode == "0E")
    {
        p = A;
        A = B;
    }
    else if (opcode == "0F")
    {
        if (A == 0)
        {
            p += operand;
        }
    }
    else if (opcode == "10")
    {
        if (A < 0)
        {
            p += operand;
        }
    }
    else if (opcode == "11")
    {
        p += operand;
    }
}
void print()
{
    string pc = gethex(p);
    string stackpointer = gethex(sp);
    string a = gethex(A);
    string b = gethex(B);
    cout << "Program Counter: " << pc << endl;
    cout << "Stack Pointer: " << stackpointer << endl;
    cout << "Val stored in RegA: " << a << endl;
    cout << "Val stored in RegB: " << b << endl;
    cout << endl;
    return;
}
void dump()
{
    int k = 0;
    int len = maxsp;
    while (k < code.size())
    {
        cout << gethex(k) << " ";
        int j = k;
        while (j < min(k + 4, len))
        {
            cout << gethex(memory[j]) << " ";
            j++;
        }
        k = k + 4;
        cout << endl;
    }
    return;
}
void runcode(string s)
{
    int iter = 0;
    while (p < i)
    {
        iter++;
        if (iter > 1e7)
        {
            cout << "Error : Segmentation fault has occured7(infinite loop)" << endl;
            return;
        }
        string k = code[p];
        // cout << k << endl;
        string op1 = k.substr(0, (int)k.size() - 2);
        string opcode = k.substr((int)k.size() - 2);
        if (opcode == "12")
        {
            cout << "Halt was encountered" << endl;
            cout << "No. of statements executed: " << iter << endl;
            break;
        }
        runopcode(opcode, getdec(op1), iter);
        p++;
        // cout << p << endl;
        if (p < 0 || p > i)
        {
            cout << "Error : Segmentation fault has occured8" << endl;
            return;
        }
        if (s == "-t")
        {
            cout << "code ran successfully" << endl;
            print();
            askquery();
        }
    }
    cout << "code ran successfully" << endl << endl;
    askquery();
}
void askquery()
{
    cout << "the following operations can be performed :" << endl;
    cout << "trace using instruction: -t" << endl;
    cout << "memory dump using instruction: -dump" << endl;
    cout << "emulate full code using instruction: -e" << endl;
    cout << "terminate emu using insturction: -0" << endl;
    cout << endl;
    cout << "input instruction: " << endl;
    string s;
    cin >> s;
    if (s != "-0" && s != "-t" && s != "-dump" && s != "-e")
    {
        cout << "unrecognized instruction" << endl;
        askquery();
    }
    if (s == "-0")
    {
        cout << "terminated" << endl;
        exit(0);
    }
    if (s == "-dump")
    {
        dump();
        askquery();
    }
    else
    {
        runcode(s);
    }
}
int main()
{
    string ofile;
    cout << "Enter filename (please make sure format to be of .o file)" << endl;
    cin >> ofile;
    if (ofile.size() < 2)
    {
        cout << "Error occured : incorrect format of file. Please try again." << endl;
        return 0;
    }
    else if (ofile.substr(ofile.size() - 2) != ".o")
    {
        cout << "Error occured : incorrect format of file. Please try again." << endl;
        return 0;
    }
    ifstream machinecode(ofile, ios::in | ios::binary);
    unsigned int y;
    while (machinecode.read((char *)&y, sizeof(int)))
    {
        // cout << y << " " << gethex(y) << endl;
        memory[i] = y;
        code.push_back(gethex(y));
        i++;
        sp++;
    }
    maxsp = max(sp, maxsp);
    askquery();
}