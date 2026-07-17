// File: 1-additive.cpp -- Additive Cipher -- Shifts each letter by a fixed key (Caesar cipher generalization)
#include<bits/stdc++.h>
using namespace std;

// Parses key from a digit string (0-25 after mod 26) or a single letter (A=0,...,Z=25)
int getKey(string inputKey)
{
    int key;
    // If numeric, convert to int and reduce mod 26
    if(isdigit(inputKey[0])) key = stoi(inputKey)%26;
    // If single alphabetic character, map A/a -> 0, B/b -> 1, ...
    else if(inputKey.length()==1 && isalpha(inputKey[0])) key = toupper(inputKey[0])-'A';
    else{
        cout<<"Invalid Key"<<endl;
        exit(1);
    }
    return key;
}

// Encrypt: E(c) = (c + key) mod 26, preserving case; non-letters pass through
string encrypt(string text,int key)
{
    string result = "";
    for(auto c : text)
    {
        if(isalpha(c))
        {
            char base = isupper(c)? 'A':'a';
            result+=((c - base)+key)%26+base;
        }
        else result+=c;
    }
    return result;
}

// Decrypt: D(c) = (c - key + 26) mod 26; +26 ensures positive modulo
string decrypt(string text, int key)
{
    string result = "";
    for(auto c:text)
    {
        if(isalpha(c))
        {
            char base = isupper(c)?'A':'a';
            result+=((c-base)-key+26)%26+base;
        }
        else result+=c;
    }
    return result;
}

int main()
{
    string inputText,inputKey;
    cout<<"Enter Plain Text: ";
    getline(cin,inputText);
    cout<<"Enter Key (Number or A-Z): ";
    cin>>inputKey;

    int key = getKey(inputKey);
    cout<<key;

    string cipher = encrypt(inputText,key);
    string plaintext = decrypt(cipher,key);

    cout<<"Cipher Text: "<<cipher<<endl;
    cout<<"Plain Text: "<<plaintext<<endl;

}
/*

=== TEST CASES ===

// --- Valid ---
// 1) Basic round-trip (lowercase)
// Input:  "hello"  key=3
// Expect: cipher="khoor" plain="hello"

// 2) Round-trip sentence with spaces and mixed case
// Input:  "Hello World"  key=5
// Expect: cipher="Mjqqt Btwqi" plain="Hello World"

// 3) Numeric key (large)
// Input:  "test"  key=55   (55 % 26 = 3)
// Expect: cipher="whvw" plain="test"

// 4) Letter key (e.g. key "D" -> 3)
// Input:  "abc"  key=D
// Expect: cipher="def" plain="abc"

// --- Invalid ---
// 5) Multi-character non-digit string
// Input:  "hello"  key=XY
// Expect: "Invalid Key" and exit

// 6) Empty key
// Input:  "hello"  key=""
// Expect: "Invalid Key" and exit

// --- Edge ---
// 7) Wrap-around (z + 1 = a)
// Input:  "z"  key=1
// Expect: cipher="a" plain="z"

// 8) Key = 0 (identity)
// Input:  "message"  key=0
// Expect: cipher="message" plain="message"

// 9) Non-alphabetic characters only
// Input:  "123 !@#"  key=5
// Expect: cipher="123 !@#" plain="123 !@#"
*/
