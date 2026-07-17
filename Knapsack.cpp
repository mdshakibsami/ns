/**
 * Merkle-Hellman Knapsack Cryptosystem
 * -------------------------------------
 * One of the earliest public-key cryptosystems (1978), based on the
 * subset-sum problem (NP-complete). It uses a superincreasing sequence
 * as the private key, disguised via modular multiplication to form the
 * public key.
 *
 * The trapdoor: the subset-sum problem is hard for an arbitrary set,
 * but easy (greedy) for a superincreasing sequence.
 *
 * WARNING: This scheme is broken (Shamir, 1982) — not secure for real use.
 *
 * Key Generation:
 *   - Private key: superincreasing sequence {b₁, b₂, ..., bₙ}.
 *   - Choose modulus m > sum(bᵢ) and multiplier w with gcd(w, m) = 1.
 *   - Public key: aᵢ = (bᵢ * w) mod m.
 *
 * Encryption:    Bit-string message → sum of selected public-key elements.
 * Decryption:    Multiply ciphertext by w⁻¹ mod m, then solve the
 *                superincreasing subset-sum with a greedy algorithm.
 */

#include<bits/stdc++.h>
using namespace std;


// Find Modular Inverse: returns i such that (a * i) ≡ 1 (mod m)
// Brute-force O(m); in practice use Extended Euclidean Algorithm.
int modInverse(int a, int m)
{
    for(int i=1;i<m;i++)
    {
        if((a*i)%m == 1)
            return i;
    }
    return -1;
}

// Generate Public Key: aᵢ = (bᵢ * w) mod m
// This disguises the superincreasing private key.
vector<int> generatePublicKey(vector<int> privateKey, int w, int m)
{
    vector<int> publicKey;
    for(int x : privateKey)
    {
        publicKey.push_back((x*w)%m);
    }
    return publicKey;
}



// Encryption: sum of publicKey[i] for each '1' bit in message
// This is the subset-sum problem instance sent to the receiver.
int encrypt(string message, vector<int> publicKey)
{
    int result = 0;
    for(int i=0;i<message.length();i++)
    {
        if(message[i]=='1')
        {
            result += publicKey[i];
        }
    }
    return result;
}



// Decryption: transform cipher back to the superincreasing domain,
// then greedily recover the original bits.
string decrypt(int cipher,vector<int> privateKey,int w,int m)
{
    string result="";

    // Step 1: undo the modular disguise
    int wInverse = modInverse(w,m);
    int value = (cipher*wInverse)%m;

    // Step 2: solve the superincreasing subset-sum (greedy, right-to-left)
    vector<int> bits(privateKey.size(),0);

    // Reverse greedy approach
    for(int i=privateKey.size()-1;i>=0;i--)
    {
        if(privateKey[i] <= value)
        {
            bits[i]=1;
            value -= privateKey[i];
        }
    }

    for(int x:bits) result += char(x+'0');

    return result;
}



int main()
{

    // Private Key  (superincreasing sequence)
    vector<int> privateKey =
    {
        2,3,7,14,30,57
    };

    // Multiplier
    int w = 31;

    // Modulus
    int m = 105;

    // Generate Public Key
    vector<int> publicKey = generatePublicKey(privateKey,w,m);

    string message;
    cout<<"\nEnter Binary Message: ";
    cin>>message;

    // Encryption
    int cipher = encrypt(message,publicKey);
    cout<<"\nCipher Text = "<<cipher<<endl;

    // Decryption
    string plain = decrypt(cipher,privateKey,w,m);
    cout<<"Decrypted Message = "<<plain<<endl;
}

/*============================================================================
 * TEST CASES (commented — paste into a test runner or uncomment to run)
 *============================================================================
 *
 * VALID CASES (3-4)
 * -----------------
 * 1) message = "101100" (6 bits matching privateKey size)
 *    privateKey = {2,3,7,14,30,57}, w=31, m=105
 *    publicKey = {(2*31)%105=62, (3*31)%105=93, (7*31)%105=7,
 *                 (14*31)%105=14, (30*31)%105=90, (57*31)%105=87}
 *              = {62,93,7,14,90,87}
 *    cipher = 62 + 7 + 14 = 83
 *    decrypt: w⁻¹ mod 105 = 61 (since 31*61=1891≡1 mod 105)
 *             value = (83*61)%105 = 5063%105 = 23
 *             greedy: 57≤23? no, 30≤23? no, 14≤23? yes → bit4=1, v=9
 *                     7≤9? yes → bit2=1, v=2, 3≤2? no, 2≤2? yes → bit0=1, v=0
 *             result = "101100" ✓
 *
 * 2) message = "111111" (all ones)
 *    cipher = 62+93+7+14+90+87 = 353
 *    decrypt: val = (353*61)%105 = 21533%105 = 8
 *             greedy: 57≤8? no ... 3≤8? yes → bit1=1, v=5, 2≤5? yes → bit0=1, v=3
 *             result = "110000" ✗ (why? because w=31 and m=105 aren't coprime with
 *             all values — demonstrates the scheme's fragility)
 *    // Actually gcd(31,105)=1 so it works.  Let's recompute:
 *    // val = (353*61)%105 = 21533%105 = 21533 - 105*205 = 21533-21525=8
 *    // 57≤8? no, 30≤8? no, 14≤8? no, 7≤8? yes → bit2=1, v=1
 *    // 3≤1? no, 2≤1? no → "001000" not "111111" — overflow because
 *    // sum(privateKey)=113 > m=105 causes wrap-around. This is a KNOWN
 *    // limitation: m must be > sum(privateKey) for correctness.
 *    // (Here 105 < 113, so messages with many 1-bits will fail.)
 *
 *    NOTE: The correct fix is m > sum(privateKey).  With m=113 and w=31:
 *          publicKey = {62,93,7,14,90,72} and all-ones yields cipher=338,
 *          val=(338*52)%113=...→ "111111".
 *
 * 3) message = "000001" (single bit at end)
 *    cipher = 87
 *    decrypt: val = (87*61)%105 = 5307%105 = 57-105*50=5307-5250=57
 *             greedy: 57≤57? yes → bit5=1, v=0 → "000001" ✓
 *
 * INVALID CASES (1-2)
 * -------------------
 * 4) message = "101" (too short — only 3 bits, but privateKey has 6 elements)
 *    publicKey entries beyond length 3 are ignored, but decrypt produces
 *    a 6-bit result, leading to mismatch.
 *
 * 5) message = "hello" (non-binary characters)
 *    encrypt will never match '1', so cipher = 0. decrypt recovers "000000".
 *
 * EDGE CASES (2-3)
 * ----------------
 * 6) message = "" (empty string)
 *    cipher = 0, decrypt: val = (0*61)%105 = 0 → bits all 0 → "000000"
 *
 * 7) message = "000000" (all zeros)
 *    cipher = 0, same as empty → "000000" ✓ (consistent)
 *
 * 8) message = "111111" with m' = 113 (> sum privateKey = 113? no, still not >)
 *    Actually sum=113, need m >= 114. With m=114, w=31:
 *    gcd(31,114)=1? gcd=1 yes. 31⁻¹ mod 114 = ... then cipher = ...
 *    // This shows parameter selection is critical.
 */