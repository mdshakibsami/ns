#include<bits/stdc++.h>
using namespace std;

//Key Matrix
int key[2][2] =
{
    {3,3},
    {2,5}
};

//Inverse Matrix
int inverseKey[2][2] =
{
    {15,17},
    {20,9}
};

string prepare(string text)
{
    string result="";

    for(char ch:text)
    {
        if(isalpha(ch))
            result+=toupper(ch);
    }

    if(result.length()%2!=0)
        result+='X';

    return result;
}


string encrypt(string text)
{
    string cipher="";

    for(int i=0;i<text.length();i+=2)
    {
        int p1=text[i]-'A';
        int p2=text[i+1]-'A';

        int c1=(key[0][0]*p1 + key[0][1]*p2)%26;
        int c2=(key[1][0]*p1 + key[1][1]*p2)%26;

        cipher+=char(c1+'A');
        cipher+=char(c2+'A');
    }

    return cipher;
}


string decrypt(string text)
{
    string plain="";

    for(int i=0;i<text.length();i+=2)
    {
        int c1=text[i]-'A';
        int c2=text[i+1]-'A';

        int p1=(inverseKey[0][0]*c1 + inverseKey[0][1]*c2)%26;
        int p2=(inverseKey[1][0]*c1 + inverseKey[1][1]*c2)%26;

        plain+=char(p1+'A');
        plain+=char(p2+'A');
    }

    while(!plain.empty() && plain.back()=='X')
        plain.pop_back();

    return plain;
}

int main()
{
    string text;

    cout<<"Enter Plain Text: ";
    getline(cin,text);

    text=prepare(text);

    cout<<"\nPrepared Text : "<<text<<endl;

    string cipher=encrypt(text);

    cout<<"Cipher Text   : "<<cipher<<endl;

    string plain=decrypt(cipher);

    cout<<"Plain Text    : "<<plain<<endl;

    return 0;
}