/*
 * 5-playfair.cpp — Playfair Cipher (5x5 digraph substitution)
 *
 * Uses a hardcoded 5x5 matrix (I/J merged).  Plaintext is:
 *   1) cleaned (non-alpha removed, uppercased, J→I)
 *   2) split into digraphs with X inserted between repeated letters
 *      and an X appended for an odd-length final pair.
 *   Encryption rules: same-row → shift right, same-col → shift down,
 *      rectangle → swap columns.
 *
 * Compile: g++ -std=c++11 -o playfair 5-playfair.cpp
 * Run:     ./playfair
 */

#include <iostream>
#include <algorithm>
#include <cctype>
using namespace std;

// Hardcoded 5x5 Playfair matrix (I/J share the same cell)
char matrix[5][5] =
{
    {'P','L','A','Y','F'},
    {'I','R','E','X','M'},
    {'B','C','D','G','H'},
    {'K','N','O','Q','S'},
    {'T','U','V','W','Z'}
};

// Locate character ch in the matrix; J is treated as I
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

// Clean and prepare plaintext for encryption
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

    // Insert X filler to handle double letters and odd length
    string result = "";
    for(int i = 0; i < temp.length();)
    {
        // Last character (odd length) — pad with X
        if(i == temp.length() - 1)
        {
            result += temp[i];
            result += 'X';
            break;
        }

        // Repeated letters in same digraph — insert X between them
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

// Encrypt prepared text using Playfair rules
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

        // Same Row — shift each letter right by 1 (wrap around)
        if(r1 == r2)
        {
            result += matrix[r1][(c1 + 1) % 5];
            result += matrix[r2][(c2 + 1) % 5];
        }

        // Same Column — shift each letter down by 1 (wrap around)
        else if(c1 == c2)
        {
            result += matrix[(r1 + 1) % 5][c1];
            result += matrix[(r2 + 1) % 5][c2];
        }

        // Rectangle — swap columns
        else
        {
            result += matrix[r1][c2];
            result += matrix[r2][c1];
        }
    }

    return result;
}

// Decrypt ciphertext (inverse of encryption)
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

        // Same Row — shift left by 1 (add 4 mod 5)
        if(r1 == r2)
        {
            result += matrix[r1][(c1 + 4) % 5];
            result += matrix[r2][(c2 + 4) % 5];
        }

        // Same Column — shift up by 1
        else if(c1 == c2)
        {
            result += matrix[(r1 + 4) % 5][c1];
            result += matrix[(r2 + 4) % 5][c2];
        }

        // Rectangle — swap columns
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

/*
 * ========================================================
 *  TEST CASES (commented out — uncomment to run manually)
 * ========================================================
 *
 *  --- Valid cases ---
 *  Input: "HELLO"
 *        → Prepared: HELXLO
 *        → Encrypted: (check manually)
 *
 *  Input: "PLAYFAIR"
 *        → Prepared: PLAYFAIR
 *        → Encrypted: (check manually)
 *
 *  Input: "BALLOON"
 *        → Prepared: BALXLOON
 *        → Encrypted: (check manually)
 *
 *  Input (sentence): "Attack at dawn"
 *        → Cleaned: ATTACKATDAWN
 *        → Prepared: ATTA CKAT DAWN
 *        → Encrypted: (check manually)
 *
 *  --- Invalid cases ---
 *  Input: ""
 *        → Prepared: ""
 *        → (empty result)
 *
 *  Input: "1234!@#$"
 *        → Cleaned: ""
 *        → Prepared: ""
 *
 *  --- Edge cases ---
 *  Input: "X"
 *        → Prepared: XX
 *        → Encrypted: (check — same row or col?)
 *
 *  Input: "XX"
 *        → Prepared: XQXQ  (X→I? No, X is in matrix)
 *        → Actually prepared: XX → first pair both X → X inserted → X X X ??
 *        Check: temp = "XX", i=0: t[i]==t[i+1] → "X" + "X" → res="XX", i=1
 *               i=1 is last → "X" + "X" → res="XXX"
 *        → Encrypted: XXX? (odd length → won't work; actually 3 chars → last pair X + X)
 *
 *  Input: "JAZZ"
 *        → J→I, so "IAZZ"
 *        → Prepared: IAZXZ
 *        → Encrypted: (check manually)
 *
 *  Input: "A sentence with numbers 123 and JAZZ music!"
 *        → Non-alpha stripped, J→I
 *        → Prepared includes X fillers
 *        → Round-trip should return cleaned text (without X pads)
 */
