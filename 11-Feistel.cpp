#include<bits/stdc++.h>

using namespace std;
string F(int key, string right )
{
    string result = "";
    for(auto ch:right) result+= char(ch^key);
    return result;
}

string XOR(string a,string b)
{
    string result="";
    for(int i=0;i<a.length();i++) result+= char(a[i]^b[i]);
    return result;
}
string encrypt(string text,int key, int rounds)
{
    if(text.length()%2!=0) text+='X';

    int mid = text.length()/2;

    string left = text.substr(0,mid);
    string right = text.substr(mid);

    for(int i =0 ;i<rounds;i++)
    {
        string temp = right;
        right = XOR(left,F(key,right));
        left = temp;

    }
    return left+right;
}

string decrypt(string text,int key, int rounds)
{
    int mid = text.length()/2;

    string left = text.substr(0,mid);
    string right = text.substr(mid);

    for(int i =0 ;i<rounds;i++)
    {
        string temp = left;
        left = XOR(right,F(key,left));
        right = temp;

    }
    string result = left+right;

    while (!result.empty() && result.back()=='X') result.pop_back();
   
    return result;
    
}

int main()
{
    string text;
    int key, rounds;

    cout << "Enter Plain Text: ";
    getline(cin, text);

    cout << "Enter Key (Number): ";
    cin >> key;

    cout << "Enter Number of Rounds: ";
    cin >> rounds;

    string cipher = encrypt(text, key, rounds);

    cout << "\nCipher Text : " << cipher << endl;

    string plain = decrypt(cipher, key, rounds);

    cout << "Plain Text  : " << plain << endl;

    return 0;
}