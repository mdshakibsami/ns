/*
 * FILE        : 11-Feistel.cpp
 * ALGORITHM   : Feistel Cipher (Single-Key)
 * 
 * A simplified Feistel network using XOR with a round function that applies
 * bitwise XOR between the right half and the integer key.
 *
 * ENCRYPTION (per round):
 *   L' = R
 *   R' = L XOR F(key, R)
 *
 * DECRYPTION (per round):
 *   R' = L
 *   L' = R XOR F(key, L)
 *
 * The same key and round function are reused for every round.
 */

#include<bits/stdc++.h>

using namespace std;

// Round function: XOR each character of `right` with the integer `key`
string F(int key, string right )
{
    string result = "";
    for(auto ch:right) result+= char(ch^key);
    return result;
}

// XOR two strings of equal length character-by-character
string XOR(string a,string b)
{
    string result="";
    for(int i=0;i<a.length();i++) result+= char(a[i]^b[i]);
    return result;
}

// Feistel encryption: split text into two halves, apply `rounds` iterations
string encrypt(string text,int key, int rounds)
{
    // Pad if odd length to ensure equal halves
    if(text.length()%2!=0) text+='X';

    int mid = text.length()/2;

    string left = text.substr(0,mid);
    string right = text.substr(mid);

    for(int i =0 ;i<rounds;i++)
    {
        string temp = right;
        right = XOR(left,F(key,right));
        left = temp;

    }
    return left+right;
}

// Feistel decryption: apply rounds in reverse using the same structure
string decrypt(string text,int key, int rounds)
{
    int mid = text.length()/2;

    string left = text.substr(0,mid);
    string right = text.substr(mid);

    for(int i =0 ;i<rounds;i++)
    {
        string temp = left;
        left = XOR(right,F(key,left));
        right = temp;

    }
    string result = left+right;

    // Strip padding added during encryption
    while (!result.empty() && result.back()=='X') result.pop_back();
   
    return result;
    
}

int main()
{
    string text;
    int key, rounds;

    cout << "Enter Plain Text: ";
    getline(cin, text);

    cout << "Enter Key (Number): ";
    cin >> key;

    cout << "Enter Number of Rounds: ";
    cin >> rounds;

    string cipher = encrypt(text, key, rounds);

    cout << "\nCipher Text : " << cipher << endl;

    string plain = decrypt(cipher, key, rounds);

    cout << "Plain Text  : " << plain << endl;

    return 0;
}

/*
 * ============================================================
 * TEST CASES  (all commented out - uncomment to run manually)
 * ============================================================
 *
 * // ----- Valid Cases (4) -----
 * // VC1: Single round, even-length text
 * //   text="ABCDEF" key=5 rounds=1
 * //   mid=3, L="ABC", R="DEF"
 * //   R' = "ABC" XOR F(5,"DEF") = "ABC" XOR (each D^5,E^5,F^5)
 * //   L' = "DEF"
 * //   decrypt -> "ABCDEF"
 *
 * // VC2: Multiple rounds (3 rounds, same key)
 * //   text="HELLO" key=7 rounds=3
 * //   odd length -> pad to "HELLOX"
 * //   round-trip returns "HELLO" (padding stripped)
 *
 * // VC3: Zero rounds (identity)
 * //   text="TEST" key=42 rounds=0
 * //   encrypt = "TEST" (no change)
 * //   decrypt("TEST",42,0) = "TEST"
 *
 * // VC4: Key = 0 (no-op round function: F(0, R) = R)
 * //   text="CIPHER" key=0 rounds=2
 * //   F(0,R) = R XOR 0 = R
 * //   R' = L XOR R; L' = R (standard Feistel even without actual mixing)
 * //   decrypt returns "CIPHER"
 *
 * // ----- Invalid Cases (2) -----
 * // IC1: Empty plaintext
 * //   text="" key=5 rounds=2
 * //   text length 0 -> mid=0 -> empty halves -> returns ""
 *
 * // IC2: Zero-length after space removal? (no spaces in input handled by prepare,
 * //       but if user passes all spaces, prepare would have removed them if it existed)
 * //   Since no prepare() here, "   " (3 spaces) -> length 3 -> pad to 4
 *
 * // ----- Edge Cases (3) -----
 * // EC1: Odd-length text (padding added then stripped)
 * //   text="ODD" key=3 rounds=1
 * //   length 3 -> pad to 4: "ODDX"
 * //   mid=2, L="OD", R="DX"
 * //   round-trip returns "ODD"
 *
 * // EC2: Very long single round (entire text as half)
 * //   text="A" key=1 rounds=1
 * //   pad to 2: "AX", mid=1, L="A", R="X"
 * //   round-trip -> "A"
 *
 * // EC3: Key larger than 255 (XOR truncates via char cast)
 * //   text="TEST" key=300 rounds=1
 * //   (300 % 256) = 44, so F uses key=44 effectively
 * //   round-trip still works due to XOR symmetry
 */
