// File: 2-multi.cpp -- Multiplicative Cipher -- Encrypts by multiplying letter positions by a key (must be coprime with 26)
#include<bits/stdc++.h>
using namespace std;

// Parses key from digit string (mod 26) or single letter (A=0,...,Z=25)
int getKey(string inputKey)
{
    int key;
    if(isdigit(inputKey[0])) key = stoi(inputKey)%26;
    else if(inputKey.length()==1 && isalpha(inputKey[0])) key = toupper(inputKey[0])-'A';
    else 
    {
        cout<<"Invalid Key"<<endl;
        exit(1);
    }
    return key;
}

// Finds modular inverse of key mod 26 by brute-force trial: (key * i) % 26 == 1
int modInverse(int key)
{
    for(int i=1;i<=26;i++) if((key*i)%26 == 1) return i;
    cout<<"Modular inverse does not exist of the key";
    exit(1);
}

// Encrypt: E(c) = (c * key) mod 26; non-letters pass through
string encrypt(string input,int key)
{
    string result = "";
    for(auto c:input)
    {
        if(isalpha(c))
        {
            char base = isupper(c)?'A':'a';
            result+=((c-base)*key)%26+base;
        }
        else result+=c;
    }
    return result;
}

// Decrypt: D(c) = (c * inverse) mod 26, where inverse is the modular inverse of key mod 26
string decrypt(string input, int key)
{
    string result="";
    int inverse = modInverse(key);
    cout<<"Inverse: "<<inverse<<endl;
    for(auto c:input)
    {
        if(isalpha(c))
        {
            char base = isupper(c)?'A':'a';
            result+=((c-base)*inverse)%26+base;
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
    cout<<key<<endl;
    
    // Key must be coprime with 26 (gcd=1) for a modular inverse to exist
    if(gcd(key,26)!=1)
    {
        cout<<"Key must be co-prime with 26!";
        exit(0);
    } 

    string cipherText = encrypt(inputText,key);
    string plainText = decrypt(cipherText,key);

    cout<<"Cipher Text: "<<cipherText<<endl;
    cout<<"Plain Text: "<<plainText<<endl;
}
/*

=== TEST CASES ===

// --- Valid ---
// 1) Basic round-trip
// Input:  "hello"  key=3
// Expect: cipher="vmggi" plain="hello"

// 2) Round-trip sentence with spaces and mixed case
// Input:  "Hi There"  key=5   (gcd(5,26)=1)
// Expect: cipher="Fm Yjwzj" plain="Hi There"

// 3) Letter key (key "B" -> 1, identity)
// Input:  "test"  key=B
// Expect: cipher="test" plain="test"

// 4) Numeric key 9 (coprime with 26)
// Input:  "abc"  key=9
// Expect: cipher="ajq" plain="abc"

// --- Invalid ---
// 5) Key not coprime with 26 (e.g. key=2, gcd(2,26)=2)
// Input:  "hello"  key=2
// Expect: "Key must be co-prime with 26!" and exit

// 6) Multi-character non-digit string
// Input:  "hello"  key=AB
// Expect: "Invalid Key" and exit

// --- Edge ---
// 7) Key = 1 (identity)
// Input:  "message"  key=1
// Expect: cipher="message" plain="message"

// 8) Key = 25 (coprime with 26)
// Input:  "a"  key=25
// Expect: cipher="z" (since 0*25=0 -> a; but 1*25=25 -> z)
// Actually let's trace: 'a'=0, (0*25)%26=0 -> 'a'. Use 'b'=1: (1*25)%26=25 -> 'z'
// Input:  "ab"  key=25
// Expect: cipher="az" plain="ab"

// 9) Non-alphabetic characters
// Input:  "x y"  key=3
// Expect: cipher="q r" plain="x y"
*/
