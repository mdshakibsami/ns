#include<bits/stdc++.h>
using namespace std;


// Find Modular Inverse
int modInverse(int a, int m)
{
    for(int i=1;i<m;i++)
    {
        if((a*i)%m == 1)
            return i;
    }
    return -1;
}

// Generate Public Key
vector<int> generatePublicKey(vector<int> privateKey, int w, int m)
{
    vector<int> publicKey;
    for(int x : privateKey)
    {
        publicKey.push_back((x*w)%m);
    }
    return publicKey;
}



// Encryption
int encrypt(string message, vector<int> publicKey)
{
    int result = 0;
    for(int i=0;i<message.length();i++)
    {
        if(message[i]=='1')
        {
            result += publicKey[i];
        }
    }
    return result;
}



// Decryption
string decrypt(int cipher,vector<int> privateKey,int w,int m)
{
    string result="";

    int wInverse = modInverse(w,m);
    int value = (cipher*wInverse)%m;
    vector<int> bits(privateKey.size(),0);

    // Reverse greedy approach
    for(int i=privateKey.size()-1;i>=0;i--)
    {
        if(privateKey[i] <= value)
        {
            bits[i]=1;
            value -= privateKey[i];
        }
    }

    for(int x:bits) result += char(x+'0');

    return result;
}



int main()
{

    // Private Key 
    vector<int> privateKey =
    {
        2,3,7,14,30,57
    };

    // Multiplier
    int w = 31;

    // Modulus
    int m = 105;

    // Generate Public Key
    vector<int> publicKey = generatePublicKey(privateKey,w,m);

    string message;
    cout<<"\nEnter Binary Message: ";
    cin>>message;

    // Encryption
    int cipher = encrypt(message,publicKey);
    cout<<"\nCipher Text = "<<cipher<<endl;

    // Decryption
    string plain = decrypt(cipher,privateKey,w,m);
    cout<<"Decrypted Message = "<<plain<<endl;
}