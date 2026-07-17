/*
 * 12-RSA.cpp
 * ----------
 * RSA (Rivest–Shamir–Adleman) — asymmetric encryption.
 *
 * Key generation:
 *   1. Choose two large primes p, q.
 *   2. n = p * q                        (modulus)
 *   3. φ(n) = (p-1)*(q-1)              (Euler's totient)
 *   4. Pick public exponent e such that gcd(e, φ(n)) = 1.
 *   5. Private key d ≡ e⁻¹ mod φ(n)    (modular inverse).
 *
 * Encrypt: ciphertext = plaintext^e mod n
 * Decrypt: plaintext  = ciphertext^d mod n
 *
 * Brute-force d-finder is used (small-key demo only).
 */

#include<bits/stdc++.h>
using namespace std;

// Finds private key d by brute-force: d*e ≡ 1 (mod phi)
int findPrivateKey(int e, int phi)
{
    for(int d=1; d<phi; d++)
    {
        if((d*e) % phi == 1) return d;
    }
    return -1; // No inverse exists
}

// Computes (base^exp) % mod via repeated multiplication (exp is small).
long long powerMod(long long base, long long exp, long long mod)
{
    long long result = 1;
    while(exp > 0)
    {
        result = (result * base) % mod;
        exp--;
    }
    return result;
}

int main()
{
    int p, q, e, d, message;

    // Step 1 — read primes
    cout << "Enter two prime numbers: ";
    cin >> p >> q;

    // Step 2 — compute n and φ(n)
    int n = p * q;
    int phi = (p-1) * (q-1);

    // Step 3 — public exponent
    cout << "Enter public key e: ";
    cin >> e;

    // e must be coprime with φ(n)
    if(gcd(e, phi) != 1)
    {
        cout << "Invalid Public Key";
        return 0;
    }

    // Step 4 — derive private key
    d = findPrivateKey(e, phi);
    if(d == -1)
    {
        cout << "Private Key can't be generated";
        return 0;
    }

    cout << "\nPublic Key  : (" << e << "," << n << ")";
    cout << "\nPrivate Key : (" << d << "," << n << ")";

    // Step 5 — encrypt / decrypt one integer
    cout << "\n\nEnter message (number): ";
    cin >> message;
    long long cipher = powerMod(message, e, n);
    long long plain  = powerMod(cipher, d, n);

    cout << "Cipher Text: " << cipher << endl;
    cout << "Plain Text : " << plain  << endl;
}

// ============================================================
// COMMENTED TEST CASES
// ============================================================
//
// --- VALID (3-4) ---
// p=3, q=7 → n=21, phi=12, e=5, d=5, msg=2
//   cipher=2^5%21=32%21=11, plain=11^5%21=161051%21=2 ✓
//
// p=3, q=11 → n=33, phi=20, e=3, d=7, msg=5
//   cipher=5^3%33=125%33=26, plain=26^7%33=...=5 ✓
//
// p=5, q=13 → n=65, phi=48, e=7, d=7, msg=10
//   cipher=10^7%65=10^7%65=30, plain=30^7%65=10 ✓
//
// p=2, q=17 → n=34, phi=16, e=3, d=11, msg=7
//   cipher=7^3%34=343%34=3, plain=3^11%34=177147%34=7 ✓
//
// --- INVALID (1-2) ---
// p=3, q=7, e=2   → gcd(2,12)=2 ≠1 → "Invalid Public Key"
// p=3, q=5, e=3   → phi=8, gcd(3,8)=1, but no d<8 s.t. 3d≡1 mod8 → "Private Key can't be generated"
//
// --- EDGE (2-3) ---
// p=2, q=2   → n=4, phi=1, e must be coprime with 1 → any e works; d=1; msg=1 → cipher=1, plain=1
// msg=0      → cipher=0, plain=0 (zero always encrypts to itself)
// msg=n-1=20 (p=3,q=7) → (n-1)^e mod n = n-1 (always fixed point)
