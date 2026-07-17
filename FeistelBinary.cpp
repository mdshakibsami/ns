/*
 * FeistelBinary.cpp — Feistel Cipher with Binary Inputs & Binary Keys
 *
 * A Feistel network that operates entirely on binary strings ('0'/'1').
 * The user supplies the binary plaintext, a number of rounds, and a binary
 * key for each round.  Each round prints intermediate state for learning.
 *
 * Round function:  F(right, key) = right XOR key  (key cycled if shorter)
 *
 * Encryption (per round):
 *   L' = R
 *   R' = L XOR F(R, key[i])
 *
 * Decryption (keys applied in reverse):
 *   R' = L
 *   L' = R XOR F(L, key[i])
 *
 * Compile: g++ -std=c++11 -o feistelbin FeistelBinary.cpp
 * Run:     ./feistelbin
 */

#include<bits/stdc++.h>
using namespace std;

string XOR(string a, string b)
{
    string result = "";
    for(int i = 0; i < a.length(); i++)
        result += (a[i] == b[i % b.length()]) ? '0' : '1';
    return result;
}

string F(string right, string key)
{
    return XOR(right, key);
}

string encrypt(string text, vector<string> keys)
{
    if(text.length() % 2 != 0)
        text = "0" + text;

    int mid = text.length() / 2;
    string left = text.substr(0, mid);
    string right = text.substr(mid);

    for(int i = 0; i < keys.size(); i++)
    {
        string temp = right;
        right = XOR(left, F(right, keys[i]));
        left = temp;

        cout << "\nRound " << i+1 << endl;
        cout << "Key   : " << keys[i] << endl;
        cout << "Left  : " << left << endl;
        cout << "Right : " << right << endl;
    }

    return left + right;
}

string decrypt(string text, vector<string> keys)
{
    int mid = text.length() / 2;
    string left = text.substr(0, mid);
    string right = text.substr(mid);

    for(int i = keys.size() - 1; i >= 0; i--)
    {
        string temp = left;
        left = XOR(right, F(left, keys[i]));
        right = temp;

        cout << "\nReverse Round " << keys.size() - i << endl;
        cout << "Key   : " << keys[i] << endl;
        cout << "Left  : " << left << endl;
        cout << "Right : " << right << endl;
    }

    string result = left + right;
    while(result.length() > 1 && result[0] == '0')
        result = result.substr(1);
    return result;
}

int main()
{
    string text;
    int rounds;

    cout << "Enter Binary Plaintext (e.g. 10101101): ";
    getline(cin, text);
    text.erase(remove(text.begin(), text.end(), ' '), text.end());

    cout << "Enter Number of Rounds: ";
    cin >> rounds;

    vector<string> keys(rounds);
    cout << "Enter Binary Round Keys:\n";
    for(int i = 0; i < rounds; i++)
    {
        cout << "K" << i+1 << ": ";
        cin >> keys[i];
    }

    string cipher = encrypt(text, keys);
    cout << "\n====================";
    cout << "\nCipher Text : " << cipher << endl;

    string plain = decrypt(cipher, keys);
    cout << "\nPlain Text  : " << plain << endl;

    return 0;
}

/*
 * ============================================================
 * TEST CASES
 * ============================================================
 *
 * --- Valid Cases ---
 * VC1: 3 rounds with example from prompt
 *   Input: text="1010 1101" rounds=3 keys=[1101,1011,0110]
 *   Round 1: L=1101 R=1010
 *   Round 2: L=1010 R=1100
 *   Round 3: L=1100 R=0000
 *   Cipher: 11000000
 *   Decrypt: 10101101
 *
 * VC2: Single round
 *   Input: text="1100" rounds=1 keys=[1010]
 *   Round-trip: 1100
 *
 * --- Invalid Cases ---
 * IC1: Empty string
 *   Input: text="" rounds=1 keys=[1]
 *   Returns: "" (no error)
 *
 * IC2: All spaces
 *   Input: text="   " rounds=1 keys=[1]
 *   Stripped to "" -> returns ""
 *
 * --- Edge Cases ---
 * EC1: Odd-length binary string
 *   Input: text="101" rounds=1 keys=[1]
 *   Padded to "0101" -> decrypt strips leading zero -> "101"
 *
 * EC2: Single bit
 *   Input: text="1" rounds=1 keys=[0]
 *   Padded to "01" -> round-trip -> "1"
 *
 * EC3: Key shorter than right half
 *   Input: text="1010" rounds=1 keys=[1]
 *   Key "1" cycled to "11" to match right half length
 */
