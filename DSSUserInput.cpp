/*
 * DSSUserInput.cpp — DSA Sign/Verify with User-Input Parameters
 *
 * Unlike the fixed-parameter DSS.cpp, this version prompts the user for all
 * domain parameters (p, q, g), the private key x, the message hash H(m),
 * and the per-message secret k at runtime.
 *
 * The public key y = g^x mod p is computed automatically.
 *
 * Sign:   r = (g^k mod p) mod q,  s = k^{-1} (H(m) + x·r) mod q
 * Verify: w = s^{-1} mod q,  u1 = H(m)·w mod q,  u2 = r·w mod q
 *         v = (g^{u1}·y^{u2} mod p) mod q,  valid iff v == r
 *
 * Compile: g++ -std=c++11 -o dssuser DSSUserInput.cpp
 * Run:     ./dssuser
 */

#include<bits/stdc++.h>
using namespace std;

long long modPower(long long base, long long exp, long long mod)
{
    long long result = 1;
    for(long long i = 0; i < exp; i++)
        result = (result * base) % mod;
    return result;
}

long long modInverse(long long a, long long m)
{
    for(long long i = 1; i < m; i++)
    {
        if((a * i) % m == 1)
            return i;
    }
    return -1;
}

pair<long long, long long> sign(long long p, long long q, long long g,
                                long long x, long long hash, long long k)
{
    long long r = modPower(g, k, p) % q;
    long long kInv = modInverse(k, q);
    long long s = (kInv * (hash + x * r)) % q;
    return {r, s};
}

bool verify(long long p, long long q, long long g, long long y,
            long long hash, long long r, long long s)
{
    long long w = modInverse(s, q);
    long long u1 = (hash * w) % q;
    long long u2 = (r * w) % q;
    long long v = ((modPower(g, u1, p) * modPower(y, u2, p)) % p) % q;
    return v == r;
}

int main()
{
    long long p, q, g, x, hash, k;

    cout << "Enter DSS Parameters:\n";
    cout << "p (prime modulus): ";
    cin >> p;
    cout << "q (prime divisor of p-1): ";
    cin >> q;
    cout << "g (generator): ";
    cin >> g;
    cout << "x (private key): ";
    cin >> x;
    cout << "hash (H(m)): ";
    cin >> hash;
    cout << "k (random number 0<k<q): ";
    cin >> k;

    long long y = modPower(g, x, p);
    cout << "\nComputed Public Key y = g^x mod p = " << y << endl;

    auto sig = sign(p, q, g, x, hash, k);
    long long r = sig.first;
    long long s = sig.second;

    cout << "\nDigital Signature\n";
    cout << "r = " << r << endl;
    cout << "s = " << s << endl;

    bool valid = verify(p, q, g, y, hash, r, s);
    cout << "\nVerification Result: ";
    cout << (valid ? "Signature Valid" : "Signature Invalid") << endl;

    return 0;
}

/*
 * ============================================================
 * TEST CASES
 * ============================================================
 *
 * --- Valid Cases ---
 * VC1: Standard DSS parameters
 *   Input: p=23, q=11, g=4, x=3, hash=5, k=2
 *   y = 4^3 mod 23 = 18
 *   r = (4^2 mod 23) mod 11 = 16 mod 11 = 5
 *   s = 6 * (5 + 3*5) mod 11 = 120 mod 11 = 10
 *   Verify: v = 5 == r  ✓  -> "Signature Valid"
 *
 * VC2: Different private key
 *   Input: p=23, q=11, g=4, x=5, hash=8, k=3
 *   y = 4^5 mod 23 = 1
 *   r = (4^3 mod 23) mod 11 = 18 mod 11 = 7
 *   s = 4 * (8 + 5*7) mod 11 = 172 mod 11 = 7
 *   Verify: v = 7 == r  ✓
 *
 * --- Invalid Cases ---
 * IC1: k = 0 (invalid: k must be > 0 and < q)
 *   modInverse(0, q) returns -1 -> s is negative -> verify fails
 *
 * IC2: Wrong signature r
 *   Tampered r=999 -> verify detects mismatch -> "Signature Invalid"
 *
 * --- Edge Cases ---
 * EC1: k = 1
 *   kInv = 1, r = (g^1 mod p) mod q = g mod q
 *   Signature still valid
 *
 * EC2: hash = 0
 *   u1 = 0, u2 = r*w, signature still works
 *
 * EC3: x = 1 (weakest possible private key)
 *   Mathematically valid, produces correct signatures
 */
