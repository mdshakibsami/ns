/*
 * 7-Hill22.cpp — Hill Cipher 2x2 (linear algebra on Z₂₆)
 *
 * Plaintext is cleaned (non-alpha removed, uppercased), padded with X
 * if odd-length, then encrypted 2 letters at a time using a 2x2 key matrix.
 * Decryption uses the pre-computed modular inverse of the key matrix.
 *
 * Key matrix:           Inverse matrix (mod 26):
 *   [3  3]                 [15 17]
 *   [2  5]                 [20  9]
 *
 * Compile: g++ -std=c++11 -o hill22 7-Hill22.cpp
 * Run:     ./hill22
 */

#include<bits/stdc++.h>
using namespace std;

// 2x2 encryption key (must be invertible mod 26)
int key[2][2] =
{
    {3,3},
    {2,5}
};

// Pre-computed modular inverse of key (det=9, inv(9,26)=3)
int inverseKey[2][2] =
{
    {15,17},
    {20,9}
};

// Clean input: keep only letters, uppercase, pad odd length with 'X'
string prepare(string text)
{
    string result="";

    for(char ch:text)
    {
        if(isalpha(ch))
            result+=toupper(ch);
    }

    // Pad with X if single letter remains
    if(result.length()%2!=0)
        result+='X';

    return result;
}


// Encrypt a digraph: C = K * P  (mod 26)
string encrypt(string text)
{
    string cipher="";

    for(int i=0;i<text.length();i+=2)
    {
        int p1=text[i]-'A';
        int p2=text[i+1]-'A';

        int c1=(key[0][0]*p1 + key[0][1]*p2)%26;
        int c2=(key[1][0]*p1 + key[1][1]*p2)%26;

        cipher+=char(c1+'A');
        cipher+=char(c2+'A');
    }

    return cipher;
}


// Decrypt a digraph: P = K⁻¹ * C  (mod 26)
string decrypt(string text)
{
    string plain="";

    for(int i=0;i<text.length();i+=2)
    {
        int c1=text[i]-'A';
        int c2=text[i+1]-'A';

        int p1=(inverseKey[0][0]*c1 + inverseKey[0][1]*c2)%26;
        int p2=(inverseKey[1][0]*c1 + inverseKey[1][1]*c2)%26;

        plain+=char(p1+'A');
        plain+=char(p2+'A');
    }

    // Strip trailing X padding from decryption
    while(!plain.empty() && plain.back()=='X')
        plain.pop_back();

    return plain;
}

int main()
{
    string text;

    cout<<"Enter Plain Text: ";
    getline(cin,text);

    text=prepare(text);

    cout<<"\nPrepared Text : "<<text<<endl;

    string cipher=encrypt(text);

    cout<<"Cipher Text   : "<<cipher<<endl;

    string plain=decrypt(cipher);

    cout<<"Plain Text    : "<<plain<<endl;

    return 0;
}

/*
 * ========================================================
 *  TEST CASES (commented out — uncomment to run manually)
 * ========================================================
 *
 *  --- Valid cases ---
 *  Input: "HELP"
 *        → Prepared: HELP
 *        → Cipher:   (HE → ?, LP → ?)
 *        → Round-trip: HELP
 *
 *  Input: "ATTACK"
 *        → Prepared: ATTACK
 *        → Cipher:   (AT → ?, TA → ?, CK → ?)
 *        → Round-trip: ATTACK
 *
 *  Input (sentence): "Meet me at the park"
 *        → Prepared: MEETMEATTHEPARK  (even length)
 *        → Cipher + decrypt → "MEETMEATTHEPARK"
 *
 *  Input: "GOLD"
 *        → Prepared: GOLD
 *        → Cipher:   (GO → ?, LD → ?)
 *        → Round-trip: GOLD
 *
 *  --- Invalid cases ---
 *  Input: ""
 *        → Prepared: "" (empty)
 *        → Cipher & decrypt return ""
 *
 *  Input: "1234!@#$"
 *        → Prepared: "" (all stripped)
 *        → Cipher & decrypt return ""
 *
 *  --- Edge cases ---
 *  Input: "A"
 *        → Prepared: "AX" (padded)
 *        → Encrypt "AX" → decrypt → "A" (X stripped)
 *
 *  Input: "AAAAAA"
 *        → Prepared: AAAAAA
 *        → (A,A) → K * [0,0]^T = [0,0] → "AA" for each pair
 *        → Cipher: "AAAAAA"
 *        → Round-trip: "AAAAAA"
 *
 *  Input: "HELLO"
 *        → Prepared: HELLOXO (wait: HE LL OX O — 5 chars → pad → HELLOX? no)
 *        → Let's trace: "HELLO" → HE LL OX → but O is last → 5 chars → HE LL OX
 *        → Actually: result length=5 → odd → pad X → "HELLOX" (6 chars → HE LL OX)
 *        Wait: prepare: keep alpha, uppercase → "HELLO" (5), odd → + "X" → "HELLOX" (6)
 *        → Encrypt digraphs: HE, LL, OX
 *        → Decrypt → "HELLO" (trailing X stripped)
 *
 *  Input (with numbers): "Test123Data"
 *        → Prepared: "TESTDATA" (even length)
 *        → Round-trip: "TESTDATA"
 */
