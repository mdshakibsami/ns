#include<bits/stdc++.h>
using namespace std;

int getKey(string inputKey)
{
    int key;
    if(isdigit(inputKey[0])) key = stoi(inputKey)%26;
    else if(inputKey.length()==1 && isalpha(inputKey[0])) key = toupper(inputKey[0])-'A';
    else{
        cout<<"Invalid Key"<<endl;
        exit(1);
    }
    return key;
}
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