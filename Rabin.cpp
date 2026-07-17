/**
 * Rabin Cryptosystem
 * ---------------
 * An asymmetric cryptosystem whose security rests on the difficulty of
 * factoring the product of two large primes (integer factorization).
 *
 * Key Generation:
 *   - Choose two large primes p and q (p ≡ q ≡ 3 mod 4).
 *   - Compute n = p * q (public key). p and q are the private key.
 *
 * Encryption:    c = m² mod n
 * Decryption:    Find the four square roots of c modulo n via CRT.
 *
 * Ambiguity:     Decryption yields 4 possible plaintexts (only one is
 *                the original message).
 *
 * This demo uses brute-force sqrt finding (O(n)) for simplicity.
 */

#include<bits/stdc++.h>
using namespace std;


// Encryption: c = m² mod n
// The trapdoor one-way function — squaring is easy, but taking square
// roots modulo a composite n requires knowing the factorization.
long long encrypt(long long message, long long n)
{
    return (message * message) % n;
}

// Decryption (Brute Force): find all i in [0, n-1] satisfying i² ≡ cipher (mod n)
// In a real implementation this would use CRT + Tonelli-Shanks for O(log n)
// square roots modulo p and q separately.
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

/*============================================================================
 * TEST CASES (commented — paste into a test runner or uncomment to run)
 *============================================================================
 *
 * VALID CASES (3-4)
 * -----------------
 * 1) p=7, q=11, m=5
 *    n = 77, c = 25, roots = {5, 16, 61, 72}
 *    // 5² mod 77 = 25, 16² mod 77 = 256 mod 77 = 25, etc.
 *
 * 2) p=3, q=7, m=2
 *    n = 21, c = 4, roots = {2, 5, 16, 19}
 *    // 2² mod 21 = 4, 5² mod 21 = 25 mod 21 = 4
 *
 * 3) p=11, q=13, m=42 (requires p,q ≡ 3 mod 4; 11≡3, 13≡1, but still works)
 *    n = 143, c = 42² mod 143 = 1764 mod 143 = 48
 *    // verify: 42² = 1764, 1764 / 143 = 12.33..., 1764 - 143*12 = 48
 *    // roots include 42, 101, ...
 *
 * INVALID CASES (1-2)
 * -------------------
 * 4) p=4 (not prime), q=7
 *    Fails precondition: p must be prime. n = 28, encryption proceeds
 *    but Rabin security assumes semiprime n.
 *
 * 5) message >= n, e.g. p=7, q=11, m=100
 *    Plaintext larger than n wraps mod n: c = 100² mod 77 = 10000 mod 77 = 67
 *    Original message is lost — decryption yields m mod n = 23.
 *
 * EDGE CASES (2-3)
 * ----------------
 * 6) p=7, q=11, m=0
 *    c = 0, roots = {0, 11, 22, ..., 66} (every multiple of 7? — actually
 *    all i where i² ≡ 0 mod 77 → i multiple of 7 and 11 → i multiple of 77
 *    so only {0} in [0,76].)
 *
 * 7) p=7, q=11, m=1
 *    c = 1, roots = {1, 34, 43, 76}
 *    // Smallest possible non-zero message.
 *
 * 8) p=7, q=11, m=n-1 = 76
 *    c = 76² mod 77 = 5776 mod 77 = 1
 *    roots = {1, 34, 43, 76}  (same as m=1, highlighting ambiguity)
 */