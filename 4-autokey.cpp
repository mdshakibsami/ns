#include<bits/stdc++.h>
using namespace std;

string encrypt(string input,int key)
{
    string result;
    int currentKey = key;
    for(auto c:input)
    {
        if(isalpha(c))
        {
            cout<<currentKey<<" ";
            char base = isupper(c)?'A':'a';
            result+=(c-base+currentKey)%26+base;
            currentKey=toupper(c)-'A';
        }else result+=c;
    }
    return result;
}

string decrypt(string input,int key)
{
    string result;
    int currentKey = key;
    for(auto c:input)
    {
        if(isalpha(c))
        {
            cout<<currentKey<<" ";
            char base = isupper(c)?'A':'a';
            char nextChar =(c-base-currentKey+26)%26+base;
            result+=nextChar;
            currentKey=toupper(nextChar)-'A';
        }else result+=c;
    }
    return result;
}

int main()
{
    string inputText;
    int key;

    cout << "Enter Plain Text: ";
    getline(cin, inputText);

    cout << "Enter Initial Key (0-25): ";
    cin >> key;

    key%=26;
    string cipher = encrypt(inputText, key);
    string plaintext = decrypt(cipher, key);

    cout << "\nCipher Text : " << cipher << endl;
    cout << "Plain Text  : " << plaintext << endl;

}