#include<bits/stdc++.h>
using namespace std;

char matrix[5][5]={
    {'P','L','A','Y','F'},
    {'I','R','E','X','M'},
    {'B','C','D','G','H'},
    {'K','N','O','Q','S'},
    {'T','U','V','W','Z'}
};

void findPosition(char ch,int &row,int &col)
{
    if(ch=='J') ch='I';

    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
        {
            if(matrix[i][j]==ch)
            {
                row = i;
                col = j;
                return;
            }
        }
    }
}

string prepareText(string text)
{
    string temp = "";
    for(auto c:text)
    {
        if(isalpha(c))
        {
            c = toupper(c);
            if(c=='J') c ='I';
            temp+=c;
        }
    }

    string result="";
    for(int i=0;i<temp.length();)
    {
        // Last character
        if(i==temp.length()-1)
        {
            result+=temp[i];
            result+='X';
            break;
        }
        if(temp[i]==temp[i+1])
        {
            result+=temp[i];
            result+='X';
            i++;
        }
        else 
        {
            result+=temp[i];
            result+=temp[i+1];
            i+=2;
        }
    }
    return result;
}

string encrypt(string text)
{
    string result="";

    for(int i=0;i<text.length();i+=2)
    {
        char a = text[i];
        char b = text[i+1];

        int r1,r2,c1,c2;

        findPosition(a,r1,c1);
        findPosition(b,r2,c2);

        // same row
        if(r1==r2)
        {
            result+=matrix[r1][(c1+1)%5];
            result+=matrix[r2][(c2+1)%5];
        }
        //same column
        else if(c1==c2)
        {
            result+=matrix[(r1+1)%5][c1];
            result+=matrix[(r2+1)%5][c2];
        }
        else
        {
            result += matrix[r1][c2];
            result += matrix[r2][c1];
        }
    }

    return result;
}

string decrypt(string text)
{
    string result="";

    for(int i=0;i<text.length();i+=2)
    {
        char a = text[i];
        char b = text[i+1];

        int r1,r2,c1,c2;

        findPosition(a,r1,c1);
        findPosition(b,r2,c2);

        // same row
        if(r1==r2)
        {
            result+=matrix[r1][(c1+4)%5];
            result+=matrix[r2][(c2+4)%5];
        }
        //same column
        else if(c1==c2)
        {
            result+=matrix[(r1+4)%5][c1];
            result+=matrix[(r2+4)%5][c2];
        }
        else
        {
            result += matrix[r1][c2];
            result += matrix[r2][c1];
        }
    }

    return result;
}

int main()
{
    cout << "Playfair Matrix:\n\n";

    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
            cout << matrix[i][j] << " ";

        cout << endl;
    }

    string inputText;
    cout << "\nEnter Plain Text: ";
    getline(cin, inputText);

    string prepared = prepareText(inputText);
    cout<<"Preprocessed: "<<prepared<<endl;

    string cipher = encrypt(prepared);
    cout << "Cipher Text   : " << cipher << endl;

    string decrypted = decrypt(cipher);

    cout << "Decrypted Text: " << decrypted << endl;


}