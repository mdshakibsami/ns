#include<bits/stdc++.h>
using namespace std;


// Modular Exponentiation
long long modPower(long long base, long long exp, long long mod)
{
    long long result = 1;

    while(exp > 0)
    {
        result = (result * base) % mod;
        exp--;
    }

    return result;
}


// Modular Inverse
long long modInverse(long long a, long long m)
{
    for(long long i = 1; i < m; i++)
    {
        if((a * i) % m == 1)
            return i;
    }

    return -1;
}



// Generate Signature
pair<long long,long long> sign(
        long long p,
        long long q,
        long long g,
        long long x,
        long long hash,
        long long k)
{

    // r = (g^k mod p) mod q
    long long r = modPower(g,k,p) % q;


    // k inverse mod q
    long long kInverse = modInverse(k,q);


    // s = k^-1 (H(m)+xr) mod q
    long long s =
        (kInverse * (hash + x*r)) % q;

    return {r,s};
}



// Verify Signature
bool verify(
        long long p,
        long long q,
        long long g,
        long long y,
        long long hash,
        long long r,
        long long s)
{

    // w = s^-1 mod q
    long long w = modInverse(s,q);

    // u1 = H(m)*w mod q
    long long u1 = (hash*w)%q;

    // u2 = r*w mod q
    long long u2 = (r*w)%q;

    long long v =
    ((modPower(g,u1,p) *
      modPower(y,u2,p)) % p) % q;

    return v == r;
}



int main()
{

    // Given DSS Parameters
    long long p = 23;
    long long q = 11;
    long long g = 4;
    // Private key
    long long x = 3;
    // Public key
    long long y = 18;
    // Message hash
    long long hash = 5;
    // Random number
    long long k = 2;

    // Generate Signature

    pair<long long,long long> signature =
        sign(p,q,g,x,hash,k);

    long long r = signature.first;
    long long s = signature.second;

    cout<<"\nDigital Signature\n";

    cout<<"r = "<<r<<endl;
    cout<<"s = "<<s<<endl;

    // Verification

    bool result =
        verify(p,q,g,y,hash,r,s);



    cout<<"\nVerification Result: ";

    if(result)
        cout<<"Signature Valid"<<endl;

    else
        cout<<"Signature Invalid"<<endl;


    return 0;
}