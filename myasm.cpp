// Name:Rithvik Konidena
// Roll Number : 2302CS09

#include <bits/stdc++.h>
#define fr(i, a, b) for (int i = (a); i < (int)(b); ++i)
using namespace std;
#define all(a) a.begin(), a.end()

bool basehex(string s);
bool baseoct(string s);

set<string> opzero;
set<string> opone;
set<string> optwo;
vector<string> oldlines;
vector<string> newlines;   // after cleaning
vector<string> finalcode;  // removing spaces in between;
map<string, string> optab; // string opcodes of the respective mnemonics
map<string, int> labelvals;
vector<pair<int, string>> allerrors;
vector<string> allwarnings;
vector<string> listfile;
vector<string> ofile;
set<string> labelsused;

bool useful(string &s)
{
    for (char it : s)
    {
        if (it >= 'a' and it <= 'z')
            return true;
        if (it >= 'A' and it <= 'Z')
            return true;
    }
    return false;
}
void optable_initializing()
{
    // opzero for no operands
    // opone for value operand
    // optwo for offset operand
    opzero.insert("add");
    opzero.insert("sub");
    opzero.insert("shl");
    opzero.insert("sp2a");
    opzero.insert("shr");
    opzero.insert("a2sp");
    opzero.insert("return");
    opzero.insert("HALT");
    optab["add"] = "06";
    optab["sub"] = "07";
    optab["shl"] = "08";
    optab["shr"] = "09";
    optab["a2sp"] = "0B";
    optab["sp2a"] = "0C";
    optab["return"] = "0E";
    optab["HALT"] = "12";
    opone.insert("data");
    opone.insert("ldc");
    opone.insert("adc");
    opone.insert("adj");
    opone.insert("SET");
    optab["data"] = "";
    optab["ldc"] = "00";
    optab["adc"] = "01";
    optab["adj"] = "0A";
    optab["SET"] = "";
    optwo.insert("ldl");
    optwo.insert("stl");
    optwo.insert("ldnl");
    optwo.insert("stnl");
    optwo.insert("call");
    optwo.insert("brz");
    optwo.insert("brlz");
    optwo.insert("br");
    optab["ldl"] = "02";
    optab["stl"] = "03";
    optab["ldnl"] = "04";
    optab["stnl"] = "05";
    optab["call"] = "0D";
    optab["brz"] = "0F";
    optab["brlz"] = "10";
    optab["br"] = "11";
}
void removespaces()
{
    int n = oldlines.size();
    fr(i, 0, n)
    {
        string temp;
        bool flag = true;
        for (char it : oldlines[i])
        {
            if (it == ';')
                break;
            if (it != ' ' && it != '\t')
            {
                flag = false;
            }
            else if (flag)
            {
                continue;
            }
            temp.push_back(it);
        }
        if (useful(temp))
        {
            newlines.push_back(temp);
        }
        else
        {
            string empty = "";
            newlines.push_back(empty);
        }
    }
}
bool labelcheck(string cur)
{
    // Checks whether the label  name is valid. Ex: 1Loop is invalid, Loop1 is valid
    for (auto to : cur)
    {
        if ((to >= 'a' && to <= 'z') || (to >= 'A' && to <= 'Z') || (to >= '0' && to <= '9') || (to == '_'))
            continue;
        return false;
    }
    if ((cur[0] >= 'a' && cur[0] <= 'z') || (cur[0] >= 'A' && cur[0] <= 'Z') || (cur[0] == '_'))
        return true;
    return false;
}
void SETfunction(string &operand, string &label, int i)
{
    string s = operand;
    int n = s.length();
    if (n == 0)
    {
        labelvals[label] = 0;
        return;
    }
    if (s[0] == '-')
    {
        if (n == 1)
        {
            string mistake = "operand unrecognized while using SET at line ";
            allerrors.push_back({i + 1, mistake});
            return;
        }
        reverse(all(s));
        s.pop_back();
        reverse(all(s));
    }
    if (s[0] == '+')
    {
        if (n == 1)
        {
            string mistake = "operand unrecognized while using SET at line ";
            allerrors.push_back({i + 1, mistake});
            return;
        }
        reverse(all(s));
        s.pop_back();
        reverse(all(s));
    }
    bool f = true;
    n = s.length();
    fr(i, 0, n)
    {
        if (isdigit(s[i]) == 0)
        {
            f = false;
        }
    }
    char *t = (char *)malloc(n * sizeof(char));
    fr(i, 0, n)
    {
        t[i] = s[i];
    }
    for (int i = n; i < strlen(t); i++)
    {
        t[i] = '\0';
    }
    char *end;
    if (f && (s[0] != '0' || s.size() == 1))
    {
        int num = stoi(s);
        labelvals[label] = num;
        return;
    }
    else if (basehex(s) == true)
    {
        int num = strtol(t, &end, 16);
        labelvals[label] = num;
        return;
    }
    else if (baseoct(s) == true)
    {
        int num = strtol(t, &end, 8);
        labelvals[label] = num;
        return;
    }
    string mistake = "operand unrecognized while using SET at line ";
    allerrors.push_back({i + 1, mistake});
}
void opread(pair<int, bool> &checker, string &op1, string &label)
{
    int n = op1.length();
    if (n == 0)
    {
        checker.first = 0;
        checker.second = true;
        return;
    }
    bool neg = false;
    if (op1[0] == '-')
    {
        neg = true;
        if (n == 1)
        {
            checker.first = -1;
            checker.second = false;
            return;
        }
        reverse(all(op1));
        op1.pop_back();
        reverse(all(op1));
    }
    if (op1[0] == '+')
    {
        if (n == 1)
        {
            checker.first = -1;
            checker.second = false;
            return;
        }
        reverse(all(op1));
        op1.pop_back();
        reverse(all(op1));
    }
    n = op1.length();
    bool f1 = true;
    fr(i, 0, n)
    {
        if (isdigit(op1[i]) == 0)
        {
            f1 = false;
        }
    }
    char *t = (char *)malloc(n * sizeof(char));
    fr(i, 0, n)
    {
        t[i] = op1[i];
    }
    for (int i = n; i < strlen(t); i++)
    {
        t[i] = '\0';
    }
    char *end;
    if (f1 && (op1[0] != '0' || op1.size() == 1))
    {
        int num = stoi(op1);
        if (neg)
        {
            int shift = 24;
            num = -num;
            if (label != "data")
            {
                num += (1 << shift);
            }
        }
        checker.first = num;
        checker.second = true;
        return;
    }
    else if (basehex(op1) == true)
    {
        int num = strtol(t, &end, 16);
        checker.first = num;
        checker.second = true;
        return;
    }
    else if (baseoct(op1) == true)
    {
        int num = strtol(t, &end, 8);
        checker.first = num;
        checker.second = true;
        return;
    }
    checker.first = -1;
    checker.second = false;
}
void getlabels()
{
    int p = 0;
    fr(i, 0, finalcode.size())
    {
        if (finalcode[i] != "")
        {
            int countlabel = 0;
            for (char it : finalcode[i])
            {
                countlabel += (it == ':');
            }
            if (countlabel)
            {
                if (countlabel > 1)
                {
                    string error = "Incorrect label format at line ";
                    allerrors.push_back({i + 1, error});
                    continue;
                }
                else
                {
                    finalcode[i].push_back(' ');
                    finalcode[i].push_back(' ');
                    fr(j, 0, finalcode[i].size())
                    {
                        if (finalcode[i][j] == ':')
                        {
                            for (int k = finalcode[i].size() - 1; k > j + 1; k--)
                            {
                                finalcode[i][k] = finalcode[i][k - 1];
                            }
                            finalcode[i][j + 1] = ' ';
                        }
                    }
                    stringstream ss(finalcode[i]);
                    string label;
                    ss >> label;
                    if (label.back() != ':')
                    {
                        string error = "Incorrect format label did not appear at the start of the line ";
                        allerrors.push_back({i + 1, error});
                        continue;
                    }
                    else
                    {
                        label.pop_back();
                        bool flag = labelcheck(label);
                        if (flag == false) // Label is not a valid name
                        {
                            string e = "Invalid label name at line ";
                            allerrors.push_back({i + 1, e});
                            continue;
                        }
                        if (optab[label] == "" && label != "data" && label != "SET")
                        {
                            if (labelvals.find(label) != labelvals.end())
                            {
                                string error = "Repeated label at line ";
                                allerrors.push_back({i + 1, error});
                                continue;
                            }
                            else
                            {
                                labelvals[label] = p;
                                string opcode;
                                ss >> opcode;
                                if (opcode != "")
                                {
                                    p++;
                                }
                                if (opcode == "SET")
                                {
                                    string operand;
                                    ss >> operand;
                                    SETfunction(operand, label, i);
                                }
                            }
                        }
                        else
                        {
                            string error = "Label name \"" + label + "\" is a mnemonic at line ";
                            allerrors.push_back({i + 1, error});
                            continue;
                        }
                    }
                }
            }
            else
            {
                p++;
                continue;
            }
        }
    }
}
void addzeros(string &s, int sz)
{
    int rem = sz - (int)s.length();
    reverse(all(s));
    while ((int)s.length() < sz)
    {
        s.push_back('0');
    }
    reverse(all(s));
}
string gethexstring(int i, int sz, string &label)
{
    string result;
    unsigned int num;
    if (label == "data")
    {
        num = i;
    }
    else
    {
        if (i < 0)
        {
            i += (1 << 24);
        }
        num = i;
    }
    while (num != 0)
    {
        int x = num % 16;
        if (x >= 0 && x <= 9)
        {
            result.push_back(x + '0');
        }
        else
        {
            result.push_back('A' + x - 10);
        }
        num /= 16;
    }
    reverse(result.begin(), result.end());
    addzeros(result, sz);
    return result;
}
void compute(string &label, string &op1, string &op2, string result, int i, int j, string prelabel)
{
    if (optab[label] == "" && label != "data" && label != "SET")
    {
        string mistake = "bogus mnemonic at line ";
        allerrors.push_back({i + 1, mistake});
        return;
    }
    if (op2 != "")
    {
        string mistake = "too many operands at line ";
        allerrors.push_back({i + 1, mistake});
        return;
    }
    if (opzero.find(label) != opzero.end())
    {
        if (op1 != "")
        {
            string mistake = "too many operands at line ";
            allerrors.push_back({i + 1, mistake});
            return;
        }
        else
        {
            string ans = result + " " + "000000" + optab[label];
            listfile.push_back(ans + " " + finalcode[i]);
            ofile.push_back(ans);
            return;
        }
    }
    else if (opone.find(label) != opone.end())
    {
        if (op1 == "")
        {
            string mistake = "operand missing at line ";
            allerrors.push_back({i + 1, mistake});
            return;
        }
        else
        {
            if (labelvals.find(op1) != labelvals.end())
            {
                labelsused.insert(op1);
                int sz = 6;
                if (label == "data" || label == "SET")
                {
                    sz = 8;
                }
                string ans = result + " " + gethexstring(labelvals[op1], sz, label) + optab[label];
                listfile.push_back(ans + " " + finalcode[i]);
                ofile.push_back(ans);
                return;
            }
            else
            {
                pair<int, bool> checker;
                opread(checker, op1, label);
                if (checker.second)
                {
                    int sz = 6;
                    if (label == "data" || label == "SET")
                    {
                        sz = 8;
                        if (label == "data")
                        {
                            if (checker.first > INT_MAX || checker.first < INT_MIN)
                            {
                                string wrong = "Integer over flow deteced at line " + to_string(i + 1);
                                allwarnings.push_back(wrong);
                            }
                        }
                    }
                    string ans = result + " " + gethexstring(checker.first, sz, label) + optab[label];
                    listfile.push_back(ans + " " + finalcode[i]);
                    ofile.push_back(ans);
                    return;
                }
                else
                {
                    string mistake = "invalid operand ";
                    allerrors.push_back({i + 1, mistake});
                    return;
                }
            }
        }
    }
    else // offset
    {
        if (labelvals.find(op1) != labelvals.end())
        {
            labelsused.insert(op1);
            int sz = 6;
            string ans = result + " " + gethexstring(labelvals[op1] - j - 1, sz, label) + optab[label];
            // if (label == "call")
            // {
            //     ans = result + " " + gethexstring(labelvals[op1], sz) + optab[label];
            // }
            listfile.push_back(ans + " " + finalcode[i]);
            ofile.push_back(ans);
            return;
        }
        else
        {
            pair<int, bool> checker;
            opread(checker, op1, label);
            if (checker.second)
            {
                int sz = 6;
                string ans = result + " " + gethexstring(checker.first, sz, label) + optab[label];
                listfile.push_back(ans + " " + finalcode[i]);
                ofile.push_back(ans);
                return;
            }
            else
            {
                string mistake = "invalid operand ";
                allerrors.push_back({i + 1, mistake});
                return;
            }
        }
    }
}
void secondpass()
{
    int n = finalcode.size();
    int j = -1;
    int cnt = 0;
    fr(i, 0, n)
    {
        if (finalcode[i] != "")
        {
            string prelabel;
            j++;
            string x = "";
            string result = gethexstring(cnt, (int)8, x);
            stringstream div(finalcode[i]);
            string label;
            div >> label;
            string op1;
            string op2;
            string op3;
            div >> op1;
            div >> op2;
            div >> op3;
            if (label.back() != ':')
            {
                // cout << label << " " << op1 << " " << op2 << " " << i+1 << endl;
                compute(label, op1, op2, result, i, j, prelabel);
            }
            else
            {
                prelabel = label;
                label = op1;
                op1 = op2;
                op2 = op3;
                // cout << label.size() << " " << op1 << " " << op2 << " " << i+1 << endl;
                if (label != "")
                {
                    compute(label, op1, op2, result, i, j, prelabel);
                }
                else
                {
                    j--;
                    string x = "";
                    listfile.push_back(gethexstring(cnt, (int)8, x) + "          " + newlines[i]);
                }
            }
            cnt++;
        }
    }
}
int main()
{
    optable_initializing();

    ifstream inputfile;
    cout << "Enter file name to assemble (.asm format)" << endl;

    string filename;
    cin >> filename;

    int n = filename.length(); // length of file name
    bool f = false;
    if (n >= 4 && filename.substr(n - 4) == ".asm")
    {
        f = true;
    }
    if (!f)
    {
        cout << "The file format is incompatible, please provide file with .asm format" << endl;
        return 0;
    }
    inputfile.open(filename);
    if (inputfile.fail())
    {
        cout << "Inpur file does not exist in the current folder" << endl;
        return 0;
    }
    string t;
    for (; getline(inputfile, t);)
    {
        oldlines.push_back(t);
    }
    removespaces();
    fr(i, 0, newlines.size())
    {
        if (newlines[i] != "")
        {
            string s;
            string temp;
            stringstream ss(newlines[i]);
            while (ss >> s)
            {
                for (char it : s)
                {
                    temp.push_back(it);
                }
                temp.push_back(' ');
            }
            finalcode.push_back(temp);
        }
        else
        {
            finalcode.push_back("");
        }
    }
    getlabels();
    // 1st pass complete
    secondpass();
    string file = filename.substr(0, (int)filename.size() - 4);
    string temp = file;
    file += ".log";
    ofstream log_file;
    for (auto it : labelvals)
    {
        if (labelsused.find(it.first) == labelsused.end())
        {
            string warn = "unused label " + it.first;
            allwarnings.push_back(warn);
        }
    }
    if ((int)allerrors.size() > 0)
    {
        // system("Color E4");
        sort(all(allerrors));
        cout << "Compilation error check " << file << endl;
        log_file.open(file);
        log_file << "Errors have been detected :" << endl;
        for (auto itr : allerrors)
        {
            log_file << "Error : " << itr.second << itr.first << endl;
        }
        for (string i : allwarnings)
        {
            log_file << "Warning : " << i << endl;
        }
        log_file.close();
        return 0;
    }
    else
    {
        log_file.open(file);
        log_file << "No errors have been detected :" << endl;
        for (string i : allwarnings)
        {
            log_file << "Warning : " << i << endl;
        }
        log_file << "See " << temp + ".l " << "and " << temp + ".o " << "for listing and machine codes respectively" << endl;
        log_file.close();
    }
    file = filename.substr(0, (int)filename.size() - 4);
    file = file + ".l";
    cout << "Successfully compiled, see " << temp + ".l " << "and " << temp + ".o " << endl;
    ofstream list;
    list.open(file);
    for (auto it : listfile) // listing code
    {
        list << it << endl;
    }
    list.close();
    ofstream machinecode;
    temp += ".o";
    machinecode.open(temp, ios::binary | ios::out); // machinecode
    for (string it : ofile)
    {
        stringstream ss(it);
        string chk;
        string chk1;
        ss >> chk;
        ss >> chk1;
        stringstream stream;
        stream << hex << chk1;
        unsigned int x;
        stream >> x;
        int y = (int)x;
        machinecode.write((const char *)&y, sizeof(unsigned int));
    }
    machinecode.close();
}
bool basehex(string s)
{
    // checks if numbers star with 0x;
    int n = s.length();
    if (n <= 2) // length 3 is minimum;
    {
        return false;
    }
    if (s[0] != '0' || !(s[1] == 'x' || s[1] == 'X'))
    {
        return false; // dont have 0x/0X at the start;
    }
    bool f = true;
    fr(i, 2, n)
    {
        if ((s[i] >= 'a' && s[i] <= 'f') || (s[i] >= 'A' && s[i] <= 'F') || isdigit(s[i]) == 1)
        {
            continue;
        }
        else
        {
            f = false;
        }
    }
    return f;
}
bool baseoct(string s)
{
    int n = s.length();
    if (n <= 1 || s[0] != '0')
    {
        return false; // always starts with 0 so length atleast 2
    }
    bool f = true;
    fr(i, 1, n)
    {
        if (s[i] < '0' || s[i] > '7')
        {
            f = false;
        }
    }
    return f;
}