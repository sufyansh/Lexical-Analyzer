#include <iostream>
#include <bits/stdc++.h>
#include <stdio.h>


using namespace std;

enum Tokens
{
    NUM,
    OP,
    ID,
    SemiColon_TOk,
    un_Known
};

map<Tokens, string> NAMES_OF_TOKENS = {
    {NUM, "Number"},
    {OP, "Operator"},
    {ID, "Identifier"},
    {SemiColon_TOk, "Eof_Token"},
    {un_Known, "Unknown"}};

map<string, Tokens> token_map = {
    {"+", OP},{"-", OP},{"*", OP},{"/", OP},{"++", OP},
    {"--", OP},{"=", OP},{"!", OP},{"==", OP},{"!=", OP},
    {"<", OP},{">", OP},{"<=", OP},{">=", OP},{"{", OP},
    {"}", OP},{"[", OP},{"]", OP},{"(", OP},{")", OP},
    {"()", OP},{">>", OP},{"<<", OP},{"||", OP},{"&&", OP},
    {".", OP},{",", OP},{":", OP},{";", SemiColon_TOk},
    {"Unknown", un_Known}};

class Tokenization
{
public:
    string val;
    Tokens type;
    int lineno;
    Tokenization(Tokens Ktype, string Kvalue, int KlineNo)
    {
        val = Kvalue;
        type = Ktype;
        lineno = KlineNo;
    }
};

void printV(vector<string> Arr, bool NextLine = false)
{
    for (int i = 0; i < Arr.size(); i++)
    {
        NextLine ? cout << Arr[i] + "\n"
                        : cout << Arr[i] + "\t";
    }
}

void printV(vector<Tokenization> tokens)
{
    for (Tokenization token : tokens)
    {

        cout << "\n" << NAMES_OF_TOKENS[token.type] << " -> " << token.val << " | \t Line No: " << token.lineno << " ";
    }
}

vector<string> splited(string code, char charecter)
{
    vector<string> splited;
    for (int i = 0; i < code.size(); i++)
    {
        int startIndex = i;
        while (code[i] != charecter && i < code.size())
        {
            if (code[i] == '"')
            {
                int stringStartingIndex = i;
                do
                {
                    i++;
                } while (code[i] != '"' && i < code.size());
            }
            else
            {
                i++;
            }
        }
        splited.push_back(code.substr(startIndex, i - startIndex));
    }
    return splited;
}

string read(string pathToFile)
{
    string result;
    char c;
    // reading file
    ifstream outPutFile;
    outPutFile.open(pathToFile);
    if (outPutFile.is_open())
    {

        while (outPutFile.get(c))
        {
            result += c;
        }
        outPutFile.close();
    }
    else
    {
        cout << "Unable to open file";
    }
    return result;
}

void TokensToFile(string pathToFile, vector<Tokenization> tokens)
{
    ofstream OutPutFile;
    OutPutFile.open(pathToFile);
    if (OutPutFile.is_open())
    {

        for (Tokenization token : tokens)
        {
            OutPutFile << "[" << NAMES_OF_TOKENS[token.type] << " => " << token.val << " | " << token.lineno << "]  ";
        }

        // Close the file
        OutPutFile.close();
    }
    else
    {
        cout << "Unable to open file";
    }
}

string remComments(string code)
{
    string res = "";
    int i = 0;
    while (i < code.size())
    {
        if (code[i] == '/' && code[i + 1] == '/')
        {

            while (code[i] != '\n')
            {
                i++;
            }
        }
        else if (code[i] == '/' && code[i + 1] == '*')
        {
            i = i + 2;
            while (code[i] != '*' && code[i + 1] != '/')
            {
                i++;
                // check for Un-terminated comments
                if (i >= code.size())
                {
                    throw runtime_error("Un-terminated comments are present in your code");
                }
            }
            i = i + 2;
        }
        else
        {
            res += code[i];
            i++;
        }
    }
    return res;
}

vector<string> breakByLines(string code)
{
    vector<string> splited;
    string temp = "";
    for (int i = 0; i <= code.size(); i++)
    {
        if (code[i] == '\n')
        {
            splited.push_back(temp);
            temp = "";
        }
        else
        {
            temp += code[i];
        }
    }
    return splited;
}

int main()
{

   string pathIFile = "./code.txt";
   string pathOFile = "./code.tokens";

    try
    {
        // Reading file
    string Text = read(pathIFile);
    cout<<"Text =>"<<Text<<endl;


        Text = remComments(Text);


        string Program = Text;

        vector<string> splited_Lines;

        splited_Lines = breakByLines(Program);


        vector<vector<string>> splitBySpaceAndLine;
        for (string line : splited_Lines)
        {
            splitBySpaceAndLine.push_back(splited(line, ' '));
        }

        vector<Tokenization> tokens;
        for (int index = 0; index < splitBySpaceAndLine.size(); index++)
        {
            for (string word : splitBySpaceAndLine[index])
            {
                for (int charIndex = 0; charIndex < word.size(); charIndex++)
                {
                    if (word[charIndex] == '\'')
                    {
                        if (word[charIndex + 2] == '\'')
                        {
                            tokens.push_back(Tokenization(ID, word.substr(charIndex, 3), index + 1));
                            charIndex = charIndex + 2;
                        }
                        else
                        {
                            throw runtime_error("Char constant too long.");
                        }
                    }
                    else if (word[charIndex] == '"')
                    {
                        int stringStartIndex = charIndex;
                        // check for valid String
                        do
                        {
                            charIndex++;
                            if (charIndex >= word.size())
                            {
                                throw runtime_error("String constants exceed line OR not ended properly.");
                            }

                            if (word[charIndex] == '\n')
                            {
                                throw runtime_error("String constants exceed line.");
                            }

                        } while (word[charIndex] != '"');

                        // push string to tokens vector
                        tokens.push_back(Tokenization(ID, word.substr(stringStartIndex, charIndex - stringStartIndex + 1), index + 1));
                    }
                    else if (word[charIndex] >= 48 && word[charIndex] <= 57)
                    {
                        int numberStartIndex = charIndex;
                        // check for valid number constant
                        do
                        {
                            charIndex++;
                            if (charIndex >= word.size())
                            {
                                throw runtime_error("Number constants exceed line OR not ended properly.");
                            }
                        } while (word[charIndex] >= 48 && word[charIndex] <= 57);
                        charIndex--; // to get the last index of number
                        // push number string to tokens vector
                        tokens.push_back(Tokenization(NUM, word.substr(numberStartIndex, charIndex - numberStartIndex + 1), index + 1));
                    }
                    else if (token_map[word.substr(charIndex, 1)] == OP)
                    {
                        // check if next chaecter is also operator
                        if (token_map[word.substr(charIndex + 1, 1)] == OP)
                        {
                            // check of both cherecters combine to form a valid operator
                            if (token_map[word.substr(charIndex, 2)] == OP)
                            {
                                tokens.push_back(Tokenization(OP, word.substr(charIndex, 2), index + 1));
                                charIndex++;
                            }
                            else
                            {
                                throw runtime_error("Not a valid operator.");
                            }
                        }
                        else
                        {
                            tokens.push_back(Tokenization(OP, word.substr(charIndex, 1), index + 1));
                        }
                    }
                    else if (
                        (word[charIndex] >= 65 && word[charIndex] <= 90) ||
                        (word[charIndex] >= 97 && word[charIndex] <= 122))
                    {
                        int identifierStartingIndex = charIndex;
                        do
                        {
                            charIndex++;

                        } while ((word[charIndex] >= 65 && word[charIndex] <= 90) ||
                                 (word[charIndex] >= 97 && word[charIndex] <= 122) ||
                                 (word[charIndex] >= 48 && word[charIndex] <= 57) ||
                                 word[charIndex] == '_');
                        charIndex--; // to get the last index of identifier
                        // push the identifier
                        tokens.push_back(Tokenization(ID, word.substr(identifierStartingIndex, charIndex - identifierStartingIndex + 1), index + 1));
                    }
                    else if (word[charIndex] == ';')
                    {
                        tokens.push_back(Tokenization(SemiColon_TOk, word.substr(charIndex, 1), index + 1));
                    }
                    else
                    {
                        // cout << (int)word[charIndex] << " : " << word[charIndex] << endl;
                        if (word[charIndex] != 0 && word[charIndex] != 32)
                        {
                            throw runtime_error("Undefined symbol" + word.substr(charIndex, 1) + "at line No : ");
                        }
                    }
                }
            }
        }

        // output tokens
        cout << "Tokens : \n";

        printV(tokens);

        // write to file
       TokensToFile(pathOFile, tokens);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
