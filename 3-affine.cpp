#include<bits/stdc++.h>
using namespace std;

int getKey(string inputKey)
{
    int key;

    if(isdigit(inputKey[0]))
        key = stoi(inputKey) % 26;
    else if(inputKey.length() == 1 && isalpha(inputKey[0]))
        key = toupper(inputKey[0]) - 'A';
    else
    {
        cout << "Invalid Key" << endl;
        exit(1);
    }

    return key;
}

int modInverse(int key)
{
    for(int i = 1; i < 26; i++)
    {
        if((key * i) % 26 == 1)
            return i;
    }

    cout << "Modular Inverse does not exist!" << endl;
    exit(1);
}

string encrypt(string input, int a, int b)
{
    string result = "";

    for(auto c : input)
    {
        if(isalpha(c))
        {
            char base = isupper(c) ? 'A' : 'a';
            result += (((c - base) * a + b) % 26) + base;
        }
        else
            result += c;
    }

    return result;
}

string decrypt(string input, int a, int b)
{
    string result = "";

    int inverse = modInverse(a);

    for(auto c : input)
    {
        if(isalpha(c))
        {
            char base = isupper(c) ? 'A' : 'a';

            result += (inverse * ((c - base - b + 26) % 26)) % 26 + base;
        }
        else
            result += c;
    }

    return result;
}

int main()
{
    string inputText;
    string inputKey1, inputKey2;

    cout << "Enter Plain Text: ";
    getline(cin, inputText);

    cout << "Enter Multiplication Key (Number or A-Z): ";
    cin >> inputKey1;

    cout << "Enter Addition Key (Number or A-Z): ";
    cin >> inputKey2;

    int a = getKey(inputKey1);
    int b = getKey(inputKey2);

    if(gcd(a, 26) != 1)
    {
        cout << "Multiplication Key must be co-prime with 26!";
        exit(0);
    }

    string cipherText = encrypt(inputText, a, b);
    string plainText = decrypt(cipherText, a, b);

    cout << "\nCipher Text: " << cipherText << endl;
    cout << "Plain Text : " << plainText << endl;

    return 0;
}