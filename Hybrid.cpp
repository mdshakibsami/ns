/*
 * FILE        : 10-Hybrid.cpp
 * ALGORITHM   : Hybrid Transposition Cipher (Keyless + Keyed)
 * 
 * Combines two transposition stages for stronger diffusion:
 *   Stage 1 (Keyless): Basic columnar transposition with fixed column count.
 *   Stage 2 (Keyed):   Keyed columnar transposition (re-orders columns by keyword).
 *
 * ENCRYPTION:
 *   1. Strip spaces from plaintext.
 *   2. Keyless encrypt: fill row-wise, read column-wise using fixed column count.
 *   3. Keyed encrypt:   fill row-wise, read column-wise by key-determined column order.
 *
 * DECRYPTION:
 *   1. Keyed decrypt:   fill column-wise by key-determined order, read row-wise.
 *   2. Keyless decrypt: fill column-wise, read row-wise using fixed column count.
 *   3. Strip trailing 'X' padding.
 *
 * NOTE: The user-supplied column count is overridden by key.length() in
 *       hybridEncrypt/hybridDecrypt, so the column input is effectively unused.
 */

#include <bits/stdc++.h>
using namespace std;


// Removes all spaces from the input string (pre-processing step)
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



// Keyless columnar transposition encryption
// Fills matrix row-wise, reads column-wise
string encryptKeyless(string text, int column)
{
    int row = ceil(text.length() * 1.0 / column);

    // Padding
    while(text.length() < row * column)
        text += 'X';

    char matrix[100][100];

    int k = 0;

    // Fill Row-wise
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
        {
            matrix[i][j] = text[k++];
        }
    }

    string result = "";

    // Read Column-wise
    for(int j = 0; j < column; j++)
    {
        for(int i = 0; i < row; i++)
        {
            result += matrix[i][j];
        }
    }

    return result;
}



// Keyless columnar transposition decryption
// Fills matrix column-wise, reads row-wise
string decryptKeyless(string text, int column)
{
    int row = text.length() / column;

    char matrix[100][100];

    int k = 0;

    // Fill Column-wise
    for(int j = 0; j < column; j++)
    {
        for(int i = 0; i < row; i++)
        {
            matrix[i][j] = text[k++];
        }
    }

    string result = "";

    // Read Row-wise
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
        {
            result += matrix[i][j];
        }
    }

    // Remove Padding
    while(!result.empty() && result.back() == 'X')
        result.pop_back();

    return result;
}


// Returns column indices sorted by the alphabetical order of key characters
// Example: key="KEY" -> uppercase K,E,Y -> sorted E,K,Y -> original indices [1,0,2]
vector<int> getKeyOrder(string key)
{
    vector<pair<char,int>> temp;

    for(int i = 0; i < key.length(); i++)
        temp.push_back({toupper(key[i]), i});

    sort(temp.begin(), temp.end());

    vector<int> order;

    for(auto x : temp)
        order.push_back(x.second);

    return order;
}



// Keyed columnar transposition encryption
// Fills matrix row-wise, reads column-wise by key-determined order
string encryptKeyed(string text, string key)
{
    int column = key.length();
    int row = text.length() / column;

    char matrix[100][100];

    int k = 0;

    // Fill Row-wise
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
        {
            matrix[i][j] = text[k++];
        }
    }

    vector<int> order = getKeyOrder(key);

    string result = "";

    // Read by Key Order
    for(int j : order)
    {
        for(int i = 0; i < row; i++)
        {
            result += matrix[i][j];
        }
    }

    return result;
}



// Keyed columnar transposition decryption
// Fills matrix column-wise by key-determined order, reads row-wise
string decryptKeyed(string text, string key)
{
    int column = key.length();
    int row = text.length() / column;

    char matrix[100][100];

    vector<int> order = getKeyOrder(key);

    int k = 0;

    // Fill by Key Order
    for(int j : order)
    {
        for(int i = 0; i < row; i++)
        {
            matrix[i][j] = text[k++];
        }
    }

    string result = "";

    // Read Row-wise
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
        {
            result += matrix[i][j];
        }
    }

    return result;
}



// Hybrid encryption: keyless first, then keyed
string hybridEncrypt(string text, int column, string key)
{
    column = key.length();

    string step1 = encryptKeyless(text, column);

    cout << "\nAfter Keyless Encryption : " << step1 << endl;

    string step2 = encryptKeyed(step1, key);

    cout << "After Keyed Encryption   : " << step2 << endl;

    return step2;
}



// Hybrid decryption: keyed first, then keyless
string hybridDecrypt(string text, int column, string key)
{
    column = key.length();

    string step1 = decryptKeyed(text, key);

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

/*
 * ============================================================
 * TEST CASES  (all commented out - uncomment to run manually)
 * ============================================================
 *
 * // ----- Valid Cases (4) -----
 * // VC1: Basic hybrid round-trip
 * //   text="HELLOWORLD" col=4 key="KEY"
 * //   keyless(4): fill 3x4, read cols -> "HLOLELWRDOX"
 * //   keyed("KEY"): order [1,0,2] -> cipher
 * //   decrypt -> "HELLOWORLD"
 *
 * // VC2: Single column (key length = 1) -> no keyed reordering
 * //   text="CIPHER" col=any key="A"
 * //   col=1 -> encryptKeyless reads from 1-col -> "CIPHER"
 * //   keyed("A") order [0] -> no change
 *
 * // VC3: Square grid (text length = row * col)
 * //   text="ABCDEFGHIJKL" col=4 key="KEY"
 * //   12 chars, col=4 -> row=3, no padding needed in keyless step
 * //   round-trip returns original
 *
 * // VC4: Spaces in plaintext
 * //   text="HELLO WORLD" col=3 key="KEY"
 * //   prepare -> "HELLOWORLD"
 *
 * // ----- Invalid Cases (2) -----
 * // IC1: Empty plaintext
 * //   text="" col=4 key="KEY" -> prepare="" -> encryptKeyless returns ""
 *
 * // IC2: Empty key
 * //   text="TEST" col=4 key="" -> getKeyOrder("") -> empty vector
 * //   loop over empty order returns ""; crash in decryptKeyed / column calc
 *
 * // ----- Edge Cases (3) -----
 * // EC1: Single character plaintext
 * //   text="A" col=5 key="KEY"
 * //   keyless: 1x3 -> "AXX", keyed: order [1,0,2] -> "XXA"
 * //   decrypt -> "A"
 *
 * // EC2: Plaintext length < key length (single row)
 * //   text="AB" col=4 key="LONG"
 * //   keyless: col=4 -> row=1 -> "ABXX"
 * //   keyed: single row -> any order gives same result
 *
 * // EC3: Key with duplicate letters
 * //   text="TRANSPOSE" col=3 key="KEEK"
 * //   key sorted: E[1],E[2],K[0],K[3] -> order [1,2,0,3]
 * //   round-trip must return original
 */
