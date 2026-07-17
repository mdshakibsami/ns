# Digital Signature Algorithm (DSA)

> **DSA** (Digital Signature Algorithm) is a Federal Information Processing Standard for digital signatures. It was proposed by NIST in 1991 and is based on the difficulty of the discrete logarithm problem.

---

## Table of Contents

- [How DSA Works (Theory)](#how-dsa-works-theory)
- [Worked Example](#worked-example)
- [Code Walkthrough](#code-walkthrough)
- [Edge Cases](#edge-cases)
- [FAQ](#faq)

---

## How DSA Works (Theory)

### Domain Parameters

- **p** — large prime modulus (typically 1024–3072 bits)
- **q** — prime divisor of p−1 (160–256 bits)
- **g** — generator of the subgroup of order q:  
  g = h^( (p−1)/q ) mod p, where 1 < h < p−1

### Key Generation

1. Choose private key **x** randomly: 0 < x < q
2. Compute public key **y** = g^(x) mod p

### Signing (per message)

- Generate random **k**: 0 < k < q
- r = (g^(k) mod p) mod q
- s = k^(−1) · (H(m) + x·r) mod q
- Signature is the pair **(r, s)**

### Verification

- w = s^(−1) mod q
- u1 = H(m) · w mod q
- u2 = r · w mod q
- v = (g^(u1) · y^(u2) mod p) mod q
- Valid if **v == r**

### Why It Works

g^(u1) · y^(u2) = g^(u1) · g^(x·u2) = g^(u1 + x·u2) = g^(H·w + x·r·w) = g^(w·(H + x·r))

Since w = s^(−1) mod q and s = k^(−1)·(H + x·r) mod q:  
w·(H + x·r) = s^(−1)·(H + x·r) = k·(H + x·r)^(−1)·(H + x·r) = k mod q

Therefore v = (g^(k) mod p) mod q = r ✅

---

## Worked Example

```
p = 23, q = 11, g = 4

Private key: x = 3
Public key:  y = 4^3 mod 23 = 64 mod 23 = 18

Hash: H(m) = 5
Random: k = 2

k⁻¹ mod 11 = 6  (since 2 × 6 = 12 ≡ 1 mod 11)

r = (4^2 mod 23) mod 11 = 16 mod 11 = 5
s = 6 × (5 + 3 × 5) mod 11 = 6 × 20 mod 11 = 120 mod 11 = 10

Signature: (r=5, s=10)

Verification:
  w = 10⁻¹ mod 11 = 10  (10 × 10 = 100 ≡ 1 mod 11)
  u1 = 5 × 10 mod 11 = 50 mod 11 = 6
  u2 = 5 × 10 mod 11 = 50 mod 11 = 6
  v = (4^6 × 18^6 mod 23) mod 11
    4^6 mod 23 = 4096 mod 23 = 2
    18^6 mod 23:
      18² = 324 mod 23 = 2
      18⁴ = 2² = 4 mod 23
      18⁶ = 18⁴ × 18² = 4 × 2 = 8 mod 23
    v = (2 × 8 mod 23) mod 11 = 16 mod 11 = 5
  v = 5 == r = 5  ✅ Signature Valid
```

---

## Code Walkthrough

### `modPower(long long base, long long exp, long long mod)`

Repeated-multiplication modular exponentiation. O(exp) time — acceptable for demo values only.

### `modInverse(long long a, long long m)`

Brute-force search for **i** such that `(a × i) % m == 1`.  
Returns `-1` if no inverse exists. In real DSA, this uses the Extended Euclidean Algorithm.

### `sign(p, q, g, x, hash, k)`

1. Computes r = (g^(k) mod p) mod q
2. Computes k⁻¹ mod q
3. Computes s = k⁻¹ · (hash + x·r) mod q
4. Returns (r, s)

### `verify(p, q, g, y, hash, r, s)`

1. w = s⁻¹ mod q
2. u1 = hash · w mod q
3. u2 = r · w mod q
4. v = (g^(u1) · y^(u2) mod p) mod q
5. Returns v == r

### `main()` — Demonstration

Uses hard-coded small parameters (p=23, q=11, g=4, x=3, hash=5, k=2), signs and verifies, then prints the outcome.

---

## Edge Cases

| Scenario | Behaviour | Explanation |
|----------|-----------|-------------|
| **k = 0** | `modInverse(0, q)` returns -1 → s gets -1 → verification fails. | k must be in (0, q). |
| **k = 1** | k⁻¹ = 1; r = g mod q. Works but predictable — never reuse k. | Reusing k leaks the private key. |
| **hash = 0** | u1 = 0; u2 = r·w. Signature still verifies correctly. | Mathematically valid but hashing to 0 is practically impossible. |
| **x = 1** | Private key = 1 (weak). Works mathematically. | Insecure; anyone can guess x = 1. |
| **r = 0** | If g^(k) mod p == 0 (impossible since p is prime and g > 0). | r is never 0 for valid parameters. |
| **s = 0** | `modInverse(0, q)` returns -1 → verification fails. | If s = 0, choose a new k. |
| **Wrong public key y** | Verification returns "Signature Invalid". | Signer is not who they claim to be. |
| **Tampered signature (r or s changed)** | v ≠ r → "Signature Invalid". | Tampering is detected. |

---

## FAQ

### Q1: Why must k be unique per message?

If two different messages use the same **k**, the private key **x** can be recovered:
- s1 = k⁻¹·(H(m1) + x·r) mod q
- s2 = k⁻¹·(H(m2) + x·r) mod q  
Subtracting: s1 − s2 = k⁻¹·(H(m1) − H(m2)) mod q → solve for k, then solve for x.

This was famously used to extract the PlayStation 3 private key (2010).

### Q2: Why is brute-force `modInverse` bad in practice?

Looping up to q (160–256 bits) is infeasible. The Extended Euclidean Algorithm computes inverses in O(log q) time.

### Q3: Why is `modPower` implemented with a loop?

The demo uses `exp--` (linear time). Real implementations use **square-and-multiply** for O(log exp) time.

### Q4: What are the recommended key sizes?

- p: 2048 bits or 3072 bits
- q: 256 bits (SHA-256)
- These provide security equivalent to RSA-2048 or RSA-3072.

### Q5: Can DSA be used for encryption?

No — DSA is a signature-only scheme. For encryption, use RSA, ElGamal, or ECIES.

### Q6: What is the difference between DSA and RSA signatures?

- **RSA**: sign with private key, verify with public key. Deterministic (same message → same signature).
- **DSA**: uses random k. Probabilistic (same message → different signatures each time).

### Q7: Is DSA quantum-safe?

No — Shor's algorithm breaks the discrete logarithm problem. Post-quantum signature schemes (CRYSTALS-Dilithium, FALCON) are being standardised as alternatives.

### Q8: What happens if the same k is used with different x values?

Nothing — k is per-signer. But if two signers accidentally share the same (k, x) pair, both private keys are compromised.
