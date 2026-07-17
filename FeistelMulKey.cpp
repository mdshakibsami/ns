/*
 * FILE        : 11-FeistelMulKey.cpp
 * ALGORITHM   : Feistel Cipher (Multiple Round Keys)
 * 
 * Extends the basic Feistel cipher by using a different integer key for each
 * round (supplied as a vector). This adds per-round variation vs. the single-key
 * version (11-Feistel.cpp).
 *
 * ENCRYPTION (per round i):
 *   L' = R
 *   R' = L XOR F(R, keys[i])
 *
 * DECRYPTION (per round i, keys applied in reverse):
 *   R' = L
 *   L' = R XOR F(L, keys[i])
 *
 * Each round prints intermediate state (left & right halves) for educational
 * purposes.
 */

#include<bits/stdc++.h>
using namespace std;


// Round Function: XOR each character of `right` with the integer `key`
string F(string right, int key)
{
    string result = "";

    for(char ch : right)
        result += char(ch ^ key);

    return result;
}


// XOR Function: XOR two strings of equal length character-by-character
string XOR(string a, string b)
{
    string result = "";

    for(int i = 0; i < a.length(); i++)
        result += char(a[i] ^ b[i]);

    return result;
}


// Encryption: apply Feistel rounds 0..keys.size()-1 using keys[i]
string encrypt(string text, vector<int> keys)
{
    // Pad if odd length to ensure equal halves
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


// Decryption: apply Feistel rounds in reverse order keys.size()-1..0
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

/*
 * ============================================================
 * TEST CASES  (all commented out - uncomment to run manually)
 * ============================================================
 *
 * // ----- Valid Cases (4) -----
 * // VC1: Single round, single key
 * //   text="ABCDEF" rounds=1 keys=[5]
 * //   mid=3, L="ABC", R="DEF"
 * //   Round 1: L'="DEF", R'="ABC" XOR F("DEF",5)
 * //   decrypt -> "ABCDEF"
 *
 * // VC2: Multiple rounds, different keys
 * //   text="HELLO" rounds=3 keys=[3,7,15]
 * //   odd length -> pad to "HELLOX"
 * //   3 rounds with different round keys
 * //   decrypt reverses with keys [15,7,3] -> "HELLO"
 *
 * // VC3: All keys = 0 (F returns right half unchanged)
 * //   text="CIPHER" rounds=2 keys=[0,0]
 * //   F(R,0) = R; XOR(L,R) still scrambles via Feistel structure
 * //   round-trip returns "CIPHER"
 *
 * // VC4: Large keys (>255, truncated by char cast)
 * //   text="TEST" rounds=1 keys=[300]
 * //   300 mod 256 = 44 -> effectively key=44
 * //   decrypt works due to XOR symmetry
 *
 * // ----- Invalid Cases (2) -----
 * // IC1: Empty plaintext
 * //   text="" rounds=2 keys=[1,2]
 * //   length 0 -> mid=0 -> empty halves -> returns ""
 *
 * // IC2: Zero rounds (empty keys vector)
 * //   text="TEST" rounds=0 keys=[]
 * //   for loop doesn't execute -> returns input unchanged
 *
 * // ----- Edge Cases (3) -----
 * // EC1: Odd-length plaintext (padding added, stripped on decrypt)
 * //   text="ODD" rounds=1 keys=[5]
 * //   length 3 -> pad to "ODDX"
 * //   round-trip returns "ODD"
 *
 * // EC2: Single character, single round
 * //   text="A" rounds=1 keys=[10]
 * //   pad to "AX", mid=1, L="A", R="X"
 * //   round-trip strips padding -> "A"
 *
 * // EC3: All identical keys (same as single-key Feistel)
 * //   text="BANANA" rounds=3 keys=[7,7,7]
 * //   should produce same result as single-key Feistel with key=7, rounds=3
 * //   decrypt -> "BANANA"
 */
