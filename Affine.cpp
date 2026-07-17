// File: 3-affine.cpp -- Affine Cipher -- Combines multiplicative then additive encryption: E(c) = (a*c + b) mod 26
#include<bits/stdc++.h>
using namespace std;

// Parses key from digit string (mod 26) or single letter (A=0,...,Z=25)
int getKey(string inputKey)
{
    int key;

    if(isdigit(inputKey[0]))
        key = stoi(inputKey) % 26;
    else if(inputKey.length() == 1 && isalpha(inputKey[0]))
        key = toupper(inputKey[0]) - 'A';
    else
    {
        cout << "Invalid Key" << endl;
        exit(1);
    }

    return key;
}

// Brute-force search for modular inverse of key mod 26: finds i s.t. (key * i) % 26 == 1
int modInverse(int key)
{
    for(int i = 1; i < 26; i++)
    {
        if((key * i) % 26 == 1)
            return i;
    }

    cout << "Modular Inverse does not exist!" << endl;
    exit(1);
}

// Encrypt: E(c) = (a * c + b) mod 26; 'a' must be coprime with 26 for decryption to work
string encrypt(string input, int a, int b)
{
    string result = "";

    for(auto c : input)
    {
        if(isalpha(c))
        {
            char base = isupper(c) ? 'A' : 'a';
            result += (((c - base) * a + b) % 26) + base;
        }
        else
            result += c;
    }

    return result;
}

// Decrypt: D(c) = a⁻¹ * (c - b + 26) mod 26; a⁻¹ is the modular inverse of a mod 26
string decrypt(string input, int a, int b)
{
    string result = "";

    int inverse = modInverse(a);

    for(auto c : input)
    {
        if(isalpha(c))
        {
            char base = isupper(c) ? 'A' : 'a';

            result += (inverse * ((c - base - b + 26) % 26)) % 26 + base;
        }
        else
            result += c;
    }

    return result;
}

int main()
{
    string inputText;
    string inputKey1, inputKey2;

    cout << "Enter Plain Text: ";
    getline(cin, inputText);

    cout << "Enter Multiplication Key (Number or A-Z): ";
    cin >> inputKey1;

    cout << "Enter Addition Key (Number or A-Z): ";
    cin >> inputKey2;

    int a = getKey(inputKey1);
    int b = getKey(inputKey2);

    // The multiplicative key 'a' must be coprime with 26 for a modular inverse to exist
    if(gcd(a, 26) != 1)
    {
        cout << "Multiplication Key must be co-prime with 26!";
        exit(0);
    }

    string cipherText = encrypt(inputText, a, b);
    string plainText = decrypt(cipherText, a, b);

    cout << "\nCipher Text: " << cipherText << endl;
    cout << "Plain Text : " << plainText << endl;

    return 0;
}
/*

=== TEST CASES ===

// --- Valid ---
// 1) Basic round-trip (a=5, b=8)
// Input:  "hello"  a=5  b=8
// Expect: cipher="rcllo" plain="hello"

// 2) Round-trip sentence with spaces and mixed case
// Input:  "Hi World"  a=7  b=3
// Expect: cipher="Rq Utpmx" plain="Hi World"

// 3) a=1 (multiplicative identity), b=3 (additive shift only = additive cipher)
// Input:  "abc"  a=1  b=3
// Expect: cipher="def" plain="abc"

// 4) Letter keys (a=D->3, b=G->6)
// Input:  "test"  a=D  b=G
// Expect: cipher="juxj" plain="test"

// --- Invalid ---
// 5) 'a' not coprime with 26 (e.g. a=2, b=5)
// Input:  "hello"  a=2  b=5
// Expect: "Multiplication Key must be co-prime with 26!" and exit

// 6) Invalid key format
// Input:  "hello"  a=XY  b=5
// Expect: "Invalid Key" and exit

// --- Edge ---
// 7) a=1, b=0 (identity)
// Input:  "message"  a=1  b=0
// Expect: cipher="message" plain="message"

// 8) Wrap-around for both multiply and add
// Input:  "z"  a=5  b=7  (z=25: (25*5+7)%26 = (125+7)%26 = 132%26 = 2 -> 'c')
// Expect: cipher="c" plain="z"

// 9) Non-alphabetic characters
// Input:  "a b"  a=3  b=4
// Expect: cipher="e g" plain="a b"
*/
