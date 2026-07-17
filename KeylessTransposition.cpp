/*
 * 8-keylessTransposition.cpp — Keyless (Columnar) Transposition Cipher
 *
 * Spaces are removed, then plaintext is written row-wise into a
 * fixed-width matrix (column count supplied by user).  The ciphertext
 * is read column-wise.  Decryption fills column-wise and reads row-wise.
 * Trailing X padding is stripped during decryption.
 *
 * Compile: g++ -std=c++11 -o transposition 8-keylessTransposition.cpp
 * Run:     ./transposition
 */

#include<bits/stdc++.h>
using namespace std;

// Remove all spaces from input
string prepare(string text)
{
    string result = "";
    for(auto ch: text)
        if( ch !=' ') result+=ch;
    return result;
}

// Encrypt: write row-wise, read column-wise
string encrypt(string text, int column)
{
    // Number of rows needed (ceil division)
    int row = ceil((text.length()*1.0)/column);

    // Pad with X to fill the grid completely
    while (text.length()<column*row) text+='X';

    char matrix[100][100];

    // Fill matrix row-wise
    int k = 0;
    for(int i =0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            matrix[i][j]=text[k++];
        }
    }

    // Read column-wise to produce ciphertext
    string result = "";
    for(int j=0;j<column;j++)
    {
        for(int i=0;i<row;i++)
        {
            result+=matrix[i][j];
        }
    }

    return result;
}

// Decrypt: fill column-wise, read row-wise
string decrypt(string text, int column)
{
    int row = ceil((text.length()*1.0)/column);

    char matrix[100][100];

    // Fill matrix column-wise
    int k = 0;
    for(int j =0;j<column;j++)
    {
        for(int i=0;i<row;i++)
        {
            matrix[i][j]=text[k++];
        }
    }

    // Read row-wise to recover plaintext
    string result = "";
    for(int i =0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            result+=matrix[i][j];
        }
    }

    // Strip trailing X padding
    while (!result.empty() && result.back()=='X') result.pop_back();

    return result ;
}

int main()
{
    string inputText;
    int column;

    cout << "Enter Plain Text: ";
    getline(cin, inputText);

    cout << "Enter Number of Columns: ";
    cin >> column;

    inputText = prepare(inputText);
    cout << "\nPrepared Text: " << inputText << endl;

    string cipher = encrypt(inputText, column);
    string plain = decrypt(cipher, column);

    cout << "\nCipher Text: " << cipher << endl;
    cout << "Plain Text : " << plain << endl;

}

/*
 * ========================================================
 *  TEST CASES (commented out — uncomment to run manually)
 * ========================================================
 *
 *  --- Valid cases ---
 *  Input:  "HELLO"
 *  Cols:   3
 *        → Prepared: HELLO (5), row=2, grid 2×3
 *        → Grid:  H E L
 *                 L O X
 *        → Cipher (col-wise): H L E O L X
 *        → Decrypt → "HELLO"
 *
 *  Input:  "TRANSPOSITION"
 *  Cols:   4
 *        → Prepared: TRANSPOSITION (14), row=4, grid 4×4
 *        → Grid:  T R A N
 *                 S P O S
 *                 I T I O
 *                 N X X X
 *        → Cipher (col-wise): TSIN RPTX AOIX NOSX
 *        → Decrypt → "TRANSPOSITION"
 *
 *  Input (sentence): "This is a secret message"
 *  Cols:             5
 *        → Prepared: THISISASECRETMESSAGE (20), row=4, grid 4×5
 *        → Read col-wise → cipher
 *        → Decrypt → "THISISASECRETMESSAGE"
 *
 *  --- Invalid cases ---
 *  Input:  "HELLO"
 *  Cols:   0
 *        → row = ceil(5/0) → division by zero → crash
 *        → (should be guarded)
 *
 *  Input:  "HELLO"
 *  Cols:   -1
 *        → row = ceil(5/-1) = -5 → matrix loops break → UB
 *        → (should be guarded)
 *
 *  --- Edge cases ---
 *  Input:  "HELLO"
 *  Cols:   1
 *        → Prepared: HELLO, row=5, grid 5×1
 *        → Row-wise: H/E/L/L/O
 *        → Col-wise: H E L L O (same! cipher == plain)
 *
 *  Input:  "HELLO"
 *  Cols:   10
 *        → Prepared: HELLO (5), row=1, grid 1×10
 *        → Padded:   HELLOXXXXX
 *        → Col-wise: HELLOXXXXX (same! since only 1 row)
 *        → Decrypt strip X → "HELLO"
 *
 *  Input:  ""
 *  Cols:   4
 *        → Prepared: "", row=0 → encrypt returns "" → decrypt returns ""
 *
 *  Input:  "     "
 *  Cols:   3
 *        → Prepared: "" (all spaces removed)
 *        → Same as empty case
 */
