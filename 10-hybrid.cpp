#include<bits/stdc++.h>
using namespace std;


string prepare(string text)
{
    string result = "";

    for(char ch : text)
    {
        if(ch != ' ')
            result += ch;
    }

    return result;
}

string encryptKeyless(string text, int column)
{
    int row = ceil(text.length()*1.0/column);

    while(text.length() < row*column)
        text += 'X';

    char matrix[100][100];

    int k = 0;

    // Fill Row-wise
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            matrix[i][j] = text[k++];
        }
    }

    string result = "";

    // Read Column-wise
    for(int j=0;j<column;j++)
    {
        for(int i=0;i<row;i++)
        {
            result += matrix[i][j];
        }
    }

    return result;
}

//---------------- Keyless Decryption ----------------//

string decryptKeyless(string text,int column)
{
    int row = text.length()/column;

    char matrix[100][100];

    int k = 0;

    // Fill Column-wise
    for(int j=0;j<column;j++)
    {
        for(int i=0;i<row;i++)
        {
            matrix[i][j] = text[k++];
        }
    }

    string result = "";

    // Read Row-wise
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            result += matrix[i][j];
        }
    }

    while(!result.empty() && result.back()=='X')
        result.pop_back();

    return result;
}


vector<int> getKeyOrder(string key)
{
    vector<pair<char,int>> temp;

    for(int i=0;i<key.length();i++) temp.push_back({toupper(key[i]),i});

    sort(temp.begin(),temp.end());

    vector<int> order;

    for(auto x:temp)
        order.push_back(x.second);

    return order;
}


string encryptKeyed(string text,string key,bool pad = true)
{
    int column = key.length();

    int row = ceil(text.length()*1.0/column);

    if(pad)
    {
        while(text.length()<row*column)
            text+='X';
    }

    char matrix[100][100];

    int k = 0;

    // Fill Row-wise
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            matrix[i][j]=text[k++];
        }
    }

    vector<int> order = getKeyOrder(key);

    string result="";

    // Read by Key Order
    for(int j:order)
    {
        for(int i=0;i<row;i++)
        {
            result+=matrix[i][j];
        }
    }

    return result;
}


string decryptKeyed(string text,string key,bool removePadding = true)
{
    int column = key.length();

    int row = text.length()/column;

    char matrix[100][100];

    vector<int> order = getKeyOrder(key);

    int k = 0;

    // Fill by Key Order
    for(int j:order)
    {
        for(int i=0;i<row;i++)
        {
            matrix[i][j]=text[k++];
        }
    }

    string result="";

    // Read Row-wise
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            result+=matrix[i][j];
        }
    }

    if(removePadding)
    {
        while(!result.empty() && result.back()=='X')
            result.pop_back();
    }

    return result;
}



string hybridEncrypt(string text, int column, string key)
{
    column = key.length();

    string step1 = encryptKeyless(text, column);

    cout << "\nAfter Keyless Encryption : " << step1 << endl;

    string step2 = encryptKeyed(step1, key, false);

    cout << "After Keyed Encryption   : " << step2 << endl;

    return step2;
}


string hybridDecrypt(string text, int column, string key)
{
    column = key.length();

    string step1 = decryptKeyed(text, key, false);

    cout << "\nAfter Keyed Decryption   : " << step1 << endl;

    string step2 = decryptKeyless(step1, column);

    cout << "After Keyless Decryption : " << step2 << endl;

    return step2;
}


int main()
{
    string text, key;
    int column;

    cout << "Enter Plain Text: ";
    getline(cin, text);

    cout << "Enter Number of Columns: ";
    cin >> column;

    cout << "Enter Key: ";
    cin >> key;

    text = prepare(text);

    string cipher = hybridEncrypt(text, column, key);

    string plain = hybridDecrypt(cipher, column, key);

    cout << "\n============================\n";
    cout << "Cipher Text : " << cipher << endl;
    cout << "Plain Text  : " << plain << endl;

    return 0;
}