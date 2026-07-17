# RSA Cryptosystem

> **RSA** (Rivest–Shamir–Adleman) is one of the first public-key cryptosystems and is widely used for secure data transmission. Its security relies on the practical difficulty of factoring the product of two large prime numbers.

---

## Table of Contents

- [How RSA Works (Theory)](#how-rsa-works-theory)
- [Worked Example](#worked-example)
- [Code Walkthrough](#code-walkthrough)
- [Edge Cases](#edge-cases)
- [FAQ](#faq)

---

## How RSA Works (Theory)

### Key Generation

1. Choose two distinct prime numbers **p** and **q**.
2. Compute the modulus:  
   **n = p × q**
3. Compute Euler's totient:  
   **φ(n) = (p − 1)(q − 1)**
4. Choose public exponent **e** such that:  
   1 < e < φ(n) and **gcd(e, φ(n)) = 1**
5. Compute private key **d** as the modular multiplicative inverse of e modulo φ(n):  
   **d ≡ e⁻¹ (mod φ(n))**  
   i.e., d × e ≡ 1 (mod φ(n))

### Encryption

c = m^(e) mod n

### Decryption

m = c^(d) mod n

### Why It Works

By Euler's theorem, for any m coprime to n:  
m^(φ(n)) ≡ 1 (mod n)  

Since e·d ≡ 1 (mod φ(n)), we have e·d = 1 + k·φ(n), therefore:  
c^(d) ≡ m^(e·d) ≡ m^(1 + k·φ(n)) ≡ m · (m^(φ(n)))^(k) ≡ m (mod n)

---

## Worked Example

```
p = 3, q = 7
n = 3 × 7 = 21
φ(n) = (3−1)(7−1) = 2 × 6 = 12

Choose e = 5 (gcd(5,12) = 1 ✅)

Find d such that d × 5 ≡ 1 (mod 12):
  1×5 = 5
  2×5 = 10
  3×5 = 15 ≡ 3
  4×5 = 20 ≡ 8
  5×5 = 25 ≡ 1 ✅  →  d = 5

Public key:  (5, 21)
Private key: (5, 21)

Message: m = 2

Encrypt: c = 2^5 mod 21 = 32 mod 21 = 11
Decrypt: m = 11^5 mod 21 = 161051 mod 21 = 2 ✅
```

---

## Code Walkthrough

### `findPrivateKey(int e, int phi)`

Brute-force search for **d** such that `(d × e) % phi == 1`.  
Returns `-1` if no inverse exists (should not happen when gcd(e,phi)=1).

**Limitation**: only feasible for tiny φ(n). Real RSA uses the Extended Euclidean Algorithm.

### `powerMod(long long base, long long exp, long long mod)`

Computes `base^exp % mod` by repeated multiplication.

**Limitation**: O(exp) time. Real RSA uses fast exponentiation (square-and-multiply).

### `main()` — Encryption/Decryption Flow

1. Read primes **p**, **q**.
2. Compute **n = p × q** and **φ(n) = (p−1)(q−1)**.
3. Read public exponent **e**; validate gcd(e, φ(n)) = 1.
4. Derive private key **d**.
5. Encrypt: `cipher = message^e mod n`.
6. Decrypt: `plain = cipher^d mod n`.
7. Display results.

---

## Edge Cases

| Scenario | Behaviour | Explanation |
|----------|-----------|-------------|
| **gcd(e, φ) ≠ 1** | Program prints "Invalid Public Key" and exits. | e and φ are not coprime — no inverse exists. |
| **No d found in range** | Program prints "Private Key can't be generated". | Inverse exists but is ≥ φ (impossible — if gcd=1, inverse is always in [1, φ−1]). |
| **message = 0** | cipher = 0, plain = 0. | 0^anything = 0. |
| **message = 1** | cipher = 1, plain = 1. | 1^anything = 1. |
| **message = n − 1** | cipher = n−1, plain = n−1. | Fixed point: (n−1)^k mod n = n−1 for any k (since n−1 ≡ −1). |
| **message ≥ n** | cipher wraps modulo n; decryption recovers message mod n (information loss). | Message must be < n. |
| **p = q** | n = p²; φ(n) = p(p−1) not (p−1)² — formula incorrect. | p and q must be distinct primes. |
| **p or q = 2** | Works mathematically but insecure (trivial to factor). | Fine for demo; never use in practice. |

---

## FAQ

### Q1: Why is the brute-force `findPrivateKey` bad for real use?

It loops up to φ(n) which is ~n. For 2048-bit RSA, n is ~2²⁰⁴⁸ — the loop would never finish. The Extended Euclidean Algorithm finds d in O(log n) time.

### Q2: Why is `powerMod` implemented with a loop instead of fast exponentiation?

`exp--` makes it O(exp). For RSA with small demo values this is fine, but real implementations use square-and-multiply for O(log exp) time.

### Q3: What is the maximum message size?

The message must be an integer less than n. For text, each block should be converted to an integer < n.

### Q4: Can we encrypt text directly?

No — RSA works on integers. Text must first be encoded (e.g., ASCII → integer), then encrypted block by block.

### Q5: Is RSA still secure today?

Yes, with sufficiently large keys (≥ 2048 bits). However, it is vulnerable to quantum attacks (Shor's algorithm). Post-quantum cryptography is an active research area.

### Q6: Why does `gcd(e, phi)` need to be 1?

The decryption key **d** is the modular inverse of e modulo φ(n). An inverse exists **iff** gcd(e, φ(n)) = 1.

### Q7: What happens if p and q are not prime?

φ(n) = (p−1)(q−1) would be wrong, and decryption would fail to recover the original message.
