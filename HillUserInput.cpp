/*
 * HillUserInput.cpp — Hill Cipher with User-Input Key Matrix
 *
 * Unlike the fixed-key Hill22/Hill33 variants, this version lets the user
 * choose the matrix size (2×2 or 3×3) and enter both the key matrix and its
 * modular inverse at runtime.
 *
 * Encryption:  C = K · P  (mod 26)
 * Decryption:  P = K⁻¹ · C (mod 26)
 *
 * Compile: g++ -std=c++11 -o hilluser HillUserInput.cpp
 * Run:     ./hilluser
 */

#include<bits/stdc++.h>
using namespace std;

int n;
vector<vector<int>> key, invKey;

string prepare(string text)
{
    string result = "";
    for(char ch : text)
    {
        if(isalpha(ch))
            result += toupper(ch);
    }
    while(result.length() % n != 0)
        result += 'X';
    return result;
}

string encrypt(string text)
{
    string cipher = "";
    for(int i = 0; i < text.length(); i += n)
    {
        vector<int> p(n);
        for(int j = 0; j < n; j++)
            p[j] = text[i+j] - 'A';

        for(int r = 0; r < n; r++)
        {
            int val = 0;
            for(int c = 0; c < n; c++)
                val = (val + key[r][c] * p[c]) % 26;
            cipher += char(val + 'A');
        }
    }
    return cipher;
}

string decrypt(string text)
{
    string plain = "";
    for(int i = 0; i < text.length(); i += n)
    {
        vector<int> c(n);
        for(int j = 0; j < n; j++)
            c[j] = text[i+j] - 'A';

        for(int r = 0; r < n; r++)
        {
            int val = 0;
            for(int col = 0; col < n; col++)
                val = (val + invKey[r][col] * c[col]) % 26;
            plain += char(val + 'A');
        }
    }
    while(!plain.empty() && plain.back() == 'X')
        plain.pop_back();
    return plain;
}

int main()
{
    cout << "Enter matrix size (2 or 3): ";
    cin >> n;
    cin.ignore();

    key.assign(n, vector<int>(n));
    invKey.assign(n, vector<int>(n));

    cout << "\nEnter Key Matrix (" << n << "x" << n << ") values:\n";
    for(int i = 0; i < n; i++)
    {
        cout << "Row " << i+1 << ": ";
        for(int j = 0; j < n; j++)
            cin >> key[i][j];
    }

    cout << "\nEnter Inverse Key Matrix (" << n << "x" << n << ") values:\n";
    for(int i = 0; i < n; i++)
    {
        cout << "Row " << i+1 << ": ";
        for(int j = 0; j < n; j++)
            cin >> invKey[i][j];
    }

    cin.ignore();

    string text;
    cout << "\nEnter Plain Text: ";
    getline(cin, text);

    text = prepare(text);
    cout << "\nPrepared Text : " << text << endl;

    string cipher = encrypt(text);
    cout << "Cipher Text   : " << cipher << endl;

    string plain = decrypt(cipher);
    cout << "Plain Text    : " << plain << endl;

    return 0;
}

/*
 * ============================================================
 * TEST CASES
 * ============================================================
 *
 * --- Valid Cases ---
 * VC1: 2x2 with standard key
 *   Input: n=2, key=[[3,3],[2,5]], inv=[[15,17],[20,9]]
 *          text="Hello World"
 *   Prepared: HELLOWORLD
 *   Cipher:   HIOZEIPJQL
 *   Decrypt:  HELLOWORLD
 *
 * VC2: 3x3 with standard key
 *   Input: n=3, key=[[6,24,1],[13,16,10],[20,17,15]]
 *          inv=[[8,5,10],[21,8,21],[21,12,8]]
 *          text="HELLO"
 *   Prepared: HELLOX
 *   Round-trip: HELLO
 *
 * --- Invalid Cases ---
 * IC1: Empty input
 *   Input: text=""
 *   Prepared: "" -> cipher="" -> plain=""
 *
 * IC2: Non-alphabetic input
 *   Input: text="123!@#"
 *   Prepared: "" -> cipher="" -> plain=""
 *
 * --- Edge Cases ---
 * EC1: Single letter
 *   Input: text="A" (2x2)
 *   Prepared: "AX"
 *   Round-trip: "A"
 *
 * EC2: All A's (zero vector)
 *   Input: text="AAAA" (2x2)
 *   Cipher: "AAAA" (any key: 0*K = 0)
 *   Round-trip: "AAAA"
 */
