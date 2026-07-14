#include<bits/stdc++.h>
using namespace std;

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
        }else result+=c;
    }
    return result;
}

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
        }else result+=input[i];
    }
    return result;
}

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
        }else result+=input[i];
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