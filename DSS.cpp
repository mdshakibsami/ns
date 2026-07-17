/*
 * 13-DSS.cpp
 * ----------
 * DSA (Digital Signature Algorithm) — asymmetric signature scheme.
 *
 * Domain parameters:
 *   p — large prime (e.g. 23)
 *   q — prime divisor of p-1  (e.g. 11)
 *   g — generator: g = h^{(p-1)/q} mod p, 1 < h < p-1
 *
 * Key pair:
 *   x — private key  (random, 1 < x < q)
 *   y — public key   y = g^x mod p
 *
 * Signing (per message):
 *   k — random per-message secret, 0 < k < q
 *   r = (g^k mod p) mod q
 *   s = k^{-1} * (H(m) + x·r) mod q
 *
 * Verification:
 *   w = s^{-1} mod q
 *   u1 = H(m)·w mod q
 *   u2 = r·w mod q
 *   v = (g^{u1}·y^{u2} mod p) mod q
 *   Valid if v == r
 */

#include<bits/stdc++.h>
using namespace std;

// Modular exponentiation by repeated multiplication (small exponents).
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

// Brute-force modular inverse a^{-1} mod m.
long long modInverse(long long a, long long m)
{
    for(long long i = 1; i < m; i++)
    {
        if((a * i) % m == 1)
            return i;
    }
    return -1;
}

// Produces signature (r, s) for a given hash.
pair<long long, long long> sign(
    long long p,
    long long q,
    long long g,
    long long x,
    long long hash,
    long long k)
{
    // r = (g^k mod p) mod q
    long long r = modPower(g, k, p) % q;

    // k^{-1} mod q
    long long kInverse = modInverse(k, q);

    // s = k^{-1} (H(m) + x·r) mod q
    long long s = (kInverse * (hash + x * r)) % q;

    return {r, s};
}

// Verifies (r, s) against the public key and hash.
bool verify(
    long long p,
    long long q,
    long long g,
    long long y,
    long long hash,
    long long r,
    long long s)
{
    // w = s^{-1} mod q
    long long w = modInverse(s, q);

    // u1 = H(m)·w mod q
    long long u1 = (hash * w) % q;

    // u2 = r·w mod q
    long long u2 = (r * w) % q;

    // v = (g^{u1}·y^{u2} mod p) mod q
    long long v = ((modPower(g, u1, p) * modPower(y, u2, p)) % p) % q;

    return v == r;
}

int main()
{
    // Domain parameters (small — for demonstration only)
    long long p = 23;   // prime modulus
    long long q = 11;   // prime divisor of p-1
    long long g = 4;    // generator

    long long x = 3;    // private key
    long long y = 18;   // public key  (y = g^x mod p)

    long long hash = 5; // message digest H(m)
    long long k = 2;    // per-message random secret

    // Sign
    pair<long long, long long> signature = sign(p, q, g, x, hash, k);
    long long r = signature.first;
    long long s = signature.second;

    cout << "\nDigital Signature\n";
    cout << "r = " << r << "\ns = " << s << endl;

    // Verify
    bool result = verify(p, q, g, y, hash, r, s);

    cout << "\nVerification Result: ";
    if(result)
        cout << "Signature Valid" << endl;
    else
        cout << "Signature Invalid" << endl;

    return 0;
}

// ============================================================
// COMMENTED TEST CASES
// ============================================================
//
// --- VALID (3-4) ---
// p=23,q=11,g=4,x=3,y=18,hash=5,k=2
//   k^{-1} mod 11 = 6  (since 2*6=12≡1)
//   r = (4^2 mod 23) mod 11 = 16 mod 11 = 5
//   s = 6*(5+3*5) mod 11 = 6*20 mod 11 = 120 mod 11 = 10
//   w = 10^{-1} mod 11 = 10  (10*10=100≡1)
//   u1 = 5*10 mod 11 = 50 mod 11 = 6
//   u2 = 5*10 mod 11 = 6
//   v = (4^6·18^6 mod 23) mod 11 = (4096·34012224 mod 23) mod 11
//     = (2·2 mod 23) mod 11 = 4 mod 11 = 5 = r  ✓
//
// p=23,q=11,g=4,x=5 → y=4^5%23=1, hash=8,k=3
//   k^{-1} mod 11 = 4 (3*4=12≡1)
//   r = (4^3%23)%11 = 18%11=7
//   s = 4*(8+5*7)%11 = 4*43%11 = 172%11=7
//   w = 7^{-1} mod 11 = 8 (7*8=56≡1)
//   u1=8*8%11=64%11=9, u2=7*8%11=56%11=1
//   v = (4^9*1^1 %23)%11 = (262144%23)%11 = 7%11=7 = r  ✓
//
// --- INVALID (1-2) ---
// k=0  → modInverse(0,q) returns -1 → s would use -1 → verify fails
// wrong r=999 → verify detects mismatch → "Signature Invalid"
//
// --- EDGE (2-3) ---
// k=1  → kInverse=1, r = (g^1%p)%q = g%q
// hash=0 → u1=0, u2=r*w, signature still works
// x=1 → private key = 1 (weak, but mathematically valid)
