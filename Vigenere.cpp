/*
 * 6-Vigenere.cpp — Vigenère Cipher (polyalphabetic substitution)
 *
 * Repeats the keyword cyclically to match plaintext length, then
 * shifts each letter by the corresponding key letter's position (A=0..Z=25).
 * Non-alphabetic characters pass through unchanged.
 *
 * Compile: g++ -std=c++11 -o vigenere 6-Vigenere.cpp
 * Run:     ./vigenere
 */

#include<bits/stdc++.h>
using namespace std;

// Generate keystream by repeating the keyword to match input length
string generateKey(string input,string key)
{
    string result ="";
    int j=0;
    for(auto c:input)
    {
        if(isalpha(c))
        {
            result+=key[j];
            j++;
            if(j==key.length()) j = 0;
        }
        else result+=c;  // non-alpha → keep original char (not shifted)
    }
    return result;
}

// Encrypt: (plaintext[i] + key[i]) mod 26, preserving case
string encrypt(string input,string key)
{
    string result = "";
    for(int i =0 ; i<input.length();i++)
    {
        if(isalpha(input[i]))
        {
            char base = isupper(input[i])?'A':'a';
            int shift = toupper(key[i])-'A';
            result+=((input[i]-base)+shift)%26+base;
        }
        else result+=input[i];  // pass non-alpha unchanged
    }
    return result;
}

// Decrypt: (ciphertext[i] - key[i]) mod 26, preserving case
string decrypt(string input,string key)
{
    string result = "";
    for(int i =0 ; i<input.length();i++)
    {
        if(isalpha(input[i]))
        {
            char base = isupper(input[i])?'A':'a';
            int shift = toupper(key[i])-'A';
            result+=((input[i]-base)-shift+26)%26+base;
        }
        else result+=input[i];
    }
    return result;
}


int main()
{
    string inputText,inputKey;

    cout << "Enter Plain Text: ";
    getline(cin, inputText);
    cout << "Enter Key (Word): ";
    cin >> inputKey;

    // Validate Key
    for(char c : inputKey)
    {
        if(!isalpha(c))
        {
            cout << "Invalid Key! Key must contain only alphabets." << endl;
            return 0;
        }
    }

    string key = generateKey(inputText,inputKey);
    cout<<inputText<<" "<<key<<endl;

    string cipher = encrypt(inputText,key);
    string plaintext = decrypt(cipher,key);

    cout<<"Cipher Text: "<<cipher<<endl;
    cout<<"Plain Text: "<<plaintext<<endl;

}

/*
 * ========================================================
 *  TEST CASES (commented out — uncomment to run manually)
 * ========================================================
 *
 *  --- Valid cases ---
 *  Input:        "HELLO"
 *  Key:          "KEY"
 *        → Keystream: KEYKE
 *        → Cipher:    (compute manually)
 *        → Round-trip: "HELLO"
 *
 *  Input:        "AttackAtDawn"
 *  Key:          "KEY"
 *        → Keystream: KEYKEYKEYKEY
 *        → Cipher text
 *        → Round-trip: "AttackAtDawn"
 *
 *  Input:        "Hello World"
 *  Key:          "KEY"
 *        → Keystream: KEYKE  KEYKE (space unchanged)
 *        → Cipher preserves case & space
 *        → Round-trip: "Hello World"
 *
 *  Input (sentence): "The quick brown fox jumps over the lazy dog"
 *  Key:              "CIPHER"
 *        → Full sentence with spaces preserved
 *        → Round-trip should match original
 *
 *  --- Invalid cases ---
 *  Input:        "HELLO"
 *  Key:          ""
 *        → generateKey returns "" (key empty → key[j] out of bounds — bug!)
 *        → (current code does not guard against empty key)
 *
 *  Input:        "HELLO"
 *  Key:          "123"
 *        → Validation rejects: "Invalid Key!"
 *
 *  --- Edge cases ---
 *  Input:        "A"
 *  Key:          "Z"
 *        → Keystream: Z
 *        → Shift: A + 25 = Z
 *        → Decrypt: Z - 25 = A
 *
 *  Input:        "ZZZZZ"
 *  Key:          "AAAAA"
 *        → Shift 0 → cipher = plain
 *
 *  Input:        "hello"
 *  Key:          "HELLO"  (mixed case key)
 *        → Keystream: HELLO (all uppercased via toupper)
 *        → Works correctly
 *
 *  Input:        "abc123xyz"
 *  Key:          "KEY"
 *        → Keystream: KEYKEYKEY (only for alpha positions)
 *        → "123" passes through unchanged
 *        → Round-trip: "abc123xyz"
 */
