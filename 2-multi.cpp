#include<bits/stdc++.h>
using namespace std;

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

int modInverse(int key)
{
    for(int i=1;i<=26;i++) if((key*i)%26 == 1) return i;
    cout<<"Modular inverse does not exist of the key";
    exit(1);
}

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