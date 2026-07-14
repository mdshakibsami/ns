#include<bits/stdc++.h>
using namespace std;

string prepare(string text)
{
    string result = "";
    for(auto ch: text) if(ch!=' ') result+=ch;
    return result;
}

vector<int> getKeyOrder(string key)
{
    vector<pair<char,int>>temp;

    for(int i=0;i<key.length();i++) temp.push_back({key[i],i});

    sort(temp.begin(),temp.end());

    vector<int> order;
    for(auto x:temp) order.push_back(x.second);

    return order;
}

string encrypt(string text,string key)
{
    int column = key.length();
    int row = ceil((text.length()*1.0)/column);

    while (text.length()<column*row) text+='X';
   
    char matrix[100][100];
    int k = 0;

    // Fill (Row wise)
    for(int i =0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            matrix[i][j] = text[k++];
        }
    }

    vector<int> order = getKeyOrder(key);
    string result ="";

    for(auto j:order)
    {
        for(int i=0;i<row;i++) result+= matrix[i][j];
    }

    return result;
}
string decrypt(string text,string key)
{
    int column = key.length();
    int row = text.length()/column;

    char matrix[100][100];
    int k = 0;
    vector<int>order = getKeyOrder(key);

    // Fill (Column wise)
    for(auto j:order)
    {
        for(int i=0;i<row;i++)
        {
            matrix[i][j]=text[k++];
        }
    }

    string result = "";
    // Read (Row wise)
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            result+=matrix[i][j];
        }
    }
    while(!result.empty() && result.back() == 'X') result.pop_back();

    return result;
    
}


int main ()
{
    string text,key;
    
    cout << "Enter Plain Text: ";
    getline(cin,text);

    cout << "Enter Key: ";
    cin >> key;

    text = prepare(text);

    string cipher = encrypt(text,key);
    string plain = decrypt(cipher,key);

    cout << "\nCipher Text: " << cipher << endl;
    cout << "Plain Text : " << plain << endl;
}