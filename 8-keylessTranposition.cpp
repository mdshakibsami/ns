#include<bits/stdc++.h>
using namespace std;

string prepare(string text)
{
    string result = "";
    for(auto ch: text)
        if( ch !=' ') result+=ch;
    return result;
}

string encrypt(string text, int column)
{
    int row = ceil((text.length()*1.0)/column);

    while (text.length()<column*row) text+='X'; 

    char matrix[100][100];

    int k = 0;
    // Fill (Row wise)
    for(int i =0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            matrix[i][j]=text[k++];
        }
    }

    // Read (Column wise)
    string result = "";
    for(int j=0;j<column;j++)
    {
        for(int i=0;i<row;i++)
        {
            result+=matrix[i][j];
        }
    }
    
    return result;
}

string decrypt(string text, int column)
{
    int row = ceil((text.length()*1.0)/column);

    char matrix[100][100];

    int k = 0;

    // Fill (Column wise)
    for(int j =0;j<column;j++)
    {
        for(int i=0;i<row;i++)
        {
            matrix[i][j]=text[k++];
        }
    }

    string result = "";
    // Read (Row wise)
    for(int i =0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            result+=matrix[i][j];
        }
    }

    while (!result.empty() && result.back()=='X') result.pop_back();
   
    
    return result ;
}

int main()
{
    string inputText;
    int column;

    cout << "Enter Plain Text: ";
    getline(cin, inputText);

    cout << "Enter Number of Columns: ";
    cin >> column;

    inputText = prepare(inputText);
    cout << "\nPrepared Text: " << inputText << endl;

    string cipher = encrypt(inputText, column);
    string plain = decrypt(cipher, column);

    cout << "\nCipher Text: " << cipher << endl;
    cout << "Plain Text : " << plain << endl;

}