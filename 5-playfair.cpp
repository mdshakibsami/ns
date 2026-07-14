#include <iostream>
#include <algorithm>
#include <cctype>
using namespace std;

// Playfair Matrix
char matrix[5][5] =
{
    {'P','L','A','Y','F'},
    {'I','R','E','X','M'},
    {'B','C','D','G','H'},
    {'K','N','O','Q','S'},
    {'T','U','V','W','Z'}
};

// Find position of a character
void findPosition(char ch, int &row, int &col)
{
    if(ch == 'J')
        ch = 'I';

    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            if(matrix[i][j] == ch)
            {
                row = i;
                col = j;
                return;
            }
        }
    }
}

// Prepare plaintext
string prepareText(string text)
{
    string temp = "";

    // Remove non-alphabet, Uppercase, J -> I
    for(char c : text)
    {
        if(isalpha(c))
        {
            c = toupper(c);

            if(c == 'J')
                c = 'I';

            temp += c;
        }
    }

    cout << "After Cleaning: " << temp << endl;

    string result = "";

    for(int i = 0; i < temp.length();)
    {
        // Last character
        if(i == temp.length() - 1)
        {
            result += temp[i];
            result += 'X';
            break;
        }

        // Same pair
        if(temp[i] == temp[i + 1])
        {
            result += temp[i];
            result += 'X';
            i++;
        }
        else
        {
            result += temp[i];
            result += temp[i + 1];
            i += 2;
        }
    }

    return result;
}
// Encrypt
string encrypt(string text)
{
    string result = "";

    for(int i = 0; i < text.length(); i += 2)
    {
        char a = text[i];
        char b = text[i + 1];

        int r1, c1, r2, c2;

        findPosition(a, r1, c1);
        findPosition(b, r2, c2);

        // Same Row
        if(r1 == r2)
        {
            result += matrix[r1][(c1 + 1) % 5];
            result += matrix[r2][(c2 + 1) % 5];
        }

        // Same Column
        else if(c1 == c2)
        {
            result += matrix[(r1 + 1) % 5][c1];
            result += matrix[(r2 + 1) % 5][c2];
        }

        // Rectangle
        else
        {
            result += matrix[r1][c2];
            result += matrix[r2][c1];
        }
    }

    return result;
}

// Decrypt
string decrypt(string cipher)
{
    string result = "";

    for(int i = 0; i < cipher.length(); i += 2)
    {
        char a = cipher[i];
        char b = cipher[i + 1];

        int r1, c1, r2, c2;

        findPosition(a, r1, c1);
        findPosition(b, r2, c2);

        // Same Row
        if(r1 == r2)
        {
            result += matrix[r1][(c1 + 4) % 5];
            result += matrix[r2][(c2 + 4) % 5];
        }

        // Same Column
        else if(c1 == c2)
        {
            result += matrix[(r1 + 4) % 5][c1];
            result += matrix[(r2 + 4) % 5][c2];
        }

        // Rectangle
        else
        {
            result += matrix[r1][c2];
            result += matrix[r2][c1];
        }
    }

    return result;
}

int main()
{
    cout << "Playfair Matrix:\n\n";

    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
            cout << matrix[i][j] << " ";

        cout << endl;
    }

    string plain;

    cout << "\nEnter Plain Text: ";
    getline(cin, plain);

    string prepared = prepareText(plain);

    cout << "\nPrepared Text : " << prepared << endl;

    string cipher = encrypt(prepared);

    cout << "Cipher Text   : " << cipher << endl;

    string decrypted = decrypt(cipher);

    cout << "Decrypted Text: " << decrypted << endl;

    return 0;
}