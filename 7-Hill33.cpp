#include <bits/stdc++.h>
using namespace std;

// 3x3 Key Matrix
int key[3][3] =
{
    {6, 24, 1},
    {13, 16, 10},
    {20, 17, 15}
};

// Inverse Matrix (mod 26)
int inverseKey[3][3] =
{
    {8, 5, 10},
    {21, 8, 21},
    {21, 12, 8}
};

// Remove spaces, convert to uppercase and pad with X
string prepare(string text)
{
    string result = "";

    for(char ch : text)
    {
        if(isalpha(ch))
            result += toupper(ch);
    }

    while(result.length() % 3 != 0)
        result += 'X';

    return result;
}

// Encryption
string encrypt(string text)
{
    string result = "";

    for(int i = 0; i < text.length(); i += 3)
    {
        int p1 = text[i] - 'A';
        int p2 = text[i + 1] - 'A';
        int p3 = text[i + 2] - 'A';

        int c1 = (key[0][0] * p1 + key[0][1] * p2 + key[0][2] * p3) % 26;
        int c2 = (key[1][0] * p1 + key[1][1] * p2 + key[1][2] * p3) % 26;
        int c3 = (key[2][0] * p1 + key[2][1] * p2 + key[2][2] * p3) % 26;

        result += char(c1 + 'A');
        result += char(c2 + 'A');
        result += char(c3 + 'A');
    }

    return result;
}

// Decryption
string decrypt(string text)
{
    string result = "";

    for(int i = 0; i < text.length(); i += 3)
    {
        int c1 = text[i] - 'A';
        int c2 = text[i + 1] - 'A';
        int c3 = text[i + 2] - 'A';

        int p1 = (inverseKey[0][0] * c1 + inverseKey[0][1] * c2 + inverseKey[0][2] * c3) % 26;
        int p2 = (inverseKey[1][0] * c1 + inverseKey[1][1] * c2 + inverseKey[1][2] * c3) % 26;
        int p3 = (inverseKey[2][0] * c1 + inverseKey[2][1] * c2 + inverseKey[2][2] * c3) % 26;

        result += char(p1 + 'A');
        result += char(p2 + 'A');
        result += char(p3 + 'A');
    }

    // Remove padding X
    while(!result.empty() && result.back() == 'X')
        result.pop_back();

    return result;
}

int main()
{
    string text;

    cout << "Enter Plain Text: ";
    getline(cin, text);

    text = prepare(text);

    cout << "\nPrepared Text : " << text << endl;

    string cipher = encrypt(text);

    cout << "Cipher Text   : " << cipher << endl;

    string plain = decrypt(cipher);

    cout << "Plain Text    : " << plain << endl;

    return 0;
}