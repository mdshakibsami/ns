#include<bits/stdc++.h>
using namespace std;


// Round Function
string F(string right, int key)
{
    string result = "";

    for(char ch : right)
        result += char(ch ^ key);

    return result;
}


// XOR Function
string XOR(string a, string b)
{
    string result = "";

    for(int i = 0; i < a.length(); i++)
        result += char(a[i] ^ b[i]);

    return result;
}


// Encryption
string encrypt(string text, vector<int> keys)
{
    if(text.length() % 2 != 0)
        text += 'X';

    int mid = text.length()/2;

    string left = text.substr(0, mid);
    string right = text.substr(mid);


    for(int i = 0; i < keys.size(); i++)
    {
        string temp = right;

        right = XOR(left, F(right, keys[i]));

        left = temp;


        cout << "\nRound " << i+1 << endl;
        cout << "Key   : " << keys[i] << endl;
        cout << "Left  : " << left << endl;
        cout << "Right : " << right << endl;
    }


    return left + right;
}


// Decryption
string decrypt(string text, vector<int> keys)
{
    int mid = text.length()/2;

    string left = text.substr(0, mid);
    string right = text.substr(mid);


    // Reverse key order
    for(int i = keys.size()-1; i >= 0; i--)
    {
        string temp = left;

        left = XOR(right, F(left, keys[i]));

        right = temp;


        cout << "\nReverse Round " << keys.size()-i << endl;
        cout << "Key   : " << keys[i] << endl;
        cout << "Left  : " << left << endl;
        cout << "Right : " << right << endl;
    }


    string result = left + right;


    // Remove padding
    while(!result.empty() && result.back()=='X')
        result.pop_back();


    return result;
}



int main()
{
    string text;
    int rounds;


    cout << "Enter Plain Text: ";
    getline(cin,text);


    cout << "Enter Number of Rounds: ";
    cin >> rounds;


    vector<int> keys(rounds);


    cout << "Enter Round Keys:\n";

    for(int i=0;i<rounds;i++)
    {
        cout << "K" << i+1 << ": ";
        cin >> keys[i];
    }



    string cipher = encrypt(text, keys);


    cout << "\n====================";
    cout << "\nCipher Text : " << cipher << endl;



    string plain = decrypt(cipher, keys);


    cout << "\nPlain Text  : " << plain << endl;


    return 0;
}