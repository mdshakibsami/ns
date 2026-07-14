#include<bits/stdc++.h>
using namespace std;


// Encryption
long long encrypt(long long message, long long n)
{
    return (message * message) % n;
}

// Decryption (Brute Force)
vector<long long> decrypt(long long cipher, long long n)
{
    vector<long long> possible;

    for(long long i = 0; i < n; i++)
    {
        if((i*i)%n == cipher)
        {
            possible.push_back(i);
        }
    }

    return possible;
}

int main()
{
    long long p,q;

    cout << "Enter prime p: ";
    cin >> p;

    cout << "Enter prime q: ";
    cin >> q;

    // Public Key
    long long n = p*q;
    cout << "\nPublic Key (n) = " << n << endl;

    long long message;
    cout << "\nEnter Plain Text Number: ";
    cin >> message;

    // Encryption
    long long cipher = encrypt(message,n);
    cout << "\nCipher Text = " << cipher << endl;

    // Decryption
    vector<long long> roots = decrypt(cipher,n);
    cout << "\nPossible Plain Texts:\n";
    for(long long x : roots)
    {
        cout << x << endl;
    }
}