/*
 * FILE        : 9-keyedTrans.cpp
 * ALGORITHM   : Keyed Transposition Cipher (Columnar Transposition)
 * 
 * ENCRYPTION:
 *   1. Strip spaces from plaintext.
 *   2. Compute rows = ceil(len / key_len); pad with 'X' if needed.
 *   3. Write plaintext row-wise into a matrix.
 *   4. Derive column-read order from the sorted keyword characters.
 *   5. Read columns in that key-determined order to form ciphertext.
 *
 * DECRYPTION:
 *   1. Compute rows = len / key_len.
 *   2. Fill matrix column-wise in the key-determined order.
 *   3. Read row-wise to recover plaintext.
 *   4. Strip trailing 'X' padding characters.
 */

#include<bits/stdc++.h>
using namespace std;

// Removes all spaces from the input string (pre-processing step)
string prepare(string text)
{
    string result = "";
    for(auto ch: text) if(ch!=' ') result+=ch;
    return result;
}

// Returns column indices sorted by the alphabetical order of key characters
// Example: key="KEY" -> uppercase K,E,Y -> sorted E,K,Y -> original indices [1,0,2]
vector<int> getKeyOrder(string key)
{
    vector<pair<char,int>>temp;
    // Build (character, original-index) pairs
    for(int i=0;i<key.length();i++) temp.push_back({toupper(key[i]),i});
    sort(temp.begin(),temp.end()); // Sort by char; tie-break by original index (stable)
    vector<int> order;
    for(auto x:temp) order.push_back(x.second);
    return order;
}

// Encrypts plaintext using keyed columnar transposition
string encrypt(string text,string key)
{
    int column = key.length();
    int row = ceil((text.length()*1.0)/column);

    while (text.length()<column*row) text+='X'; // Pad to fill the grid
   
    char matrix[100][100];
    int k = 0;

    // Fill matrix row-wise
    for(int i =0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            matrix[i][j] = text[k++];
        }
    }

    vector<int> order = getKeyOrder(key);
    string result ="";

    // Read columns in key-determined order
    for(auto j:order)
    {
        for(int i=0;i<row;i++) result+= matrix[i][j];
    }

    return result;
}

// Decrypts ciphertext back to original plaintext
string decrypt(string text,string key)
{
    int column = key.length();
    int row = text.length()/column;

    char matrix[100][100];
    int k = 0;
    vector<int>order = getKeyOrder(key);

    // Fill matrix column-wise in key-determined order (reverse of read order)
    for(auto j:order)
    {
        for(int i=0;i<row;i++)
        {
            matrix[i][j]=text[k++];
        }
    }

    string result = "";
    // Read row-wise to recover plaintext
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            result+=matrix[i][j];
        }
    }
    // Strip padding that was added during encryption
    while(!result.empty() && result.back() == 'X') result.pop_back();

    return result;
    
}


int main ()
{
    string text,key;
    
    cout << "Enter Plain Text: ";
    getline(cin,text);

    cout << "Enter Key: ";
    cin >> key;

    text = prepare(text); // Remove spaces

    string cipher = encrypt(text,key);
    string plain = decrypt(cipher,key);

    cout << "\nCipher Text: " << cipher << endl;
    cout << "Plain Text : " << plain << endl;
}

/*
 * ============================================================
 * TEST CASES  (all commented out - uncomment to run manually)
 * ============================================================
 *
 * // ----- Valid Cases (4) -----
 * // VC1: Basic round-trip
 * //   text="HELLOWORLD" key="KEY"
 * //   encrypt -> cipher; decrypt(cipher) -> "HELLOWORLD"
 *
 * // VC2: Key in alphabetical order -> same as keyless columnar
 * //   text="CIPHER" key="ABC"  -> encrypt -> "CHIEPR"
 * //   decrypt("CHIEPR","ABC")  -> "CIPHER"
 *
 * // VC3: Multi-row with repeated key chars
 * //   text="TRANSPOSITION" key="BANANA"
 * //   round-trip must return original
 *
 * // VC4: Plaintext with spaces -> spaces removed by prepare()
 * //   text="HELLO WORLD" key="KEY"
 * //   prepare("HELLO WORLD") -> "HELLOWORLD"
 *
 * // ----- Invalid Cases (2) -----
 * // IC1: Empty plaintext
 * //   text="" key="KEY" -> prepare="" -> row=0 -> returns ""
 *
 * // IC2: Empty key (zero-length -> column=0 -> division by zero)
 * //   text="TEST" key="" -> crash at row=ceil(len/0)
 *
 * // ----- Edge Cases (3) -----
 * // EC1: Single char, long key
 * //   text="A" key="LONGKEY" -> col=7 row=1 pad=6 -> "AXXXXXX"
 * //   decrypt returns "A"
 *
 * // EC2: Exact multiple of key length (no padding needed)
 * //   text="ABCDEF" key="KEY" -> col=3 row=2 -> matrix: ABC/DEF
 * //   key order [1,0,2] -> read col1=BE, col0=AD, col2=CF => "BEADCF"
 * //   decrypt("BEADCF","KEY") -> "ABCDEF"
 *
 * // EC3: Duplicate letters in key (stable sort resolves ties by position)
 * //   text="HELLO" key="KEEK" -> key sorted: E[1],E[2],K[0],K[3] -> order [1,2,0,3]
 * //   round-trip must return "HELLO"
 */
