# Rabin Cryptosystem

> **Difficulty:** Medium | **Type:** Asymmetric (Public-Key) | **Security Basis:** Integer Factorization

## Overview

The **Rabin cryptosystem** is an asymmetric encryption technique whose security rests on the difficulty of factoring large composite numbers. It was introduced by Michael O. Rabin in 1979. Unlike RSA (which is also factorization-based), Rabin's scheme is **provably secure** — recovering the plaintext from the ciphertext is computationally equivalent to factoring the modulus *n*.

### Key Idea

Encryption is extremely simple: **square the plaintext modulo *n***. Decryption requires taking square roots modulo *n*, which is hard without knowing the factorization *n = p × q*.

Because squaring is a 4-to-1 function modulo a composite (for appropriate primes), decryption yields **four possible plaintexts**. The real message must be identified via redundancy or pre-agreed conventions.

---

## Theory

### Key Generation

1. Choose two large primes *p* and *q* such that *p ≡ q ≡ 3 (mod 4)*.  
   (This condition ensures square roots are easy to compute via Tonelli-Shanks.)
2. Compute *n = p × q*.  
   **Public key:** *n*  
   **Private key:** (*p*, *q*)

### Encryption

```
c = m² mod n
```

where *m* is the numeric plaintext (0 ≤ *m* < *n*) and *c* is the ciphertext.

Squaring modulo *n* is a **trapdoor one-way function** — easy to compute, but inverting it (finding *m* given *c*) requires the factorization.

### Decryption

To decrypt, the receiver finds the four square roots of *c* modulo *p* and modulo *q* separately (using the Chinese Remainder Theorem), then combines them.

For a prime *p ≡ 3 mod 4*, the square roots of *c* mod *p* are:

```
r = ± c^{(p+1)/4} mod p
```

The four roots modulo *n* are obtained by combining the ± roots mod *p* with the ± roots mod *q* via CRT. Only one of the four is the original message.

> **Note:** This demo implements a brute-force search O(n) for simplicity. A real implementation uses CRT + Tonelli-Shanks for O(log n) performance.

### Why Four Roots?

Since *n = p × q* and gcd(*p*, *q*) = 1, by CRT the congruence *x² ≡ c (mod n)* is equivalent to the system:

```
x² ≡ c (mod p)
x² ≡ c (mod q)
```

Each congruence has 2 solutions (±), giving 2 × 2 = 4 combined solutions modulo *n*.

---

## Worked Example

Let *p = 7*, *q = 11*, so *n = 77*. Encrypt message *m = 5*.

**Encryption:**

```
c = 5² mod 77 = 25
```

**Decryption (brute force):**

Find all *i* in [0, 76] such that *i² mod 77 = 25*:

| i | i² | i² mod 77 | Match? |
|---|---|----------|--------|
| 5 | 25 | 25 | **✓** |
| 16 | 256 | 256 - 3×77 = 25 | **✓** |
| 61 | 3721 | 3721 - 48×77 = 25 | **✓** |
| 72 | 5184 | 5184 - 67×77 = 25 | **✓** |

The four possible plaintexts are {**5**, 16, 61, 72}. Only 5 is the original.

---

## Code Walkthrough

```cpp
// Rabin-Cryptosystem.cpp — complete annotated source
```

### `encrypt(message, n)`

```
c = (message * message) % n;
```

A single line. The trapdoor one-way function at its simplest.

### `decrypt(cipher, n)`

Loops through *i* from 0 to *n-1*, checking if *i² mod n == cipher*. Inefficient but illustrative. A production version would:
1. Compute `rp = pow(cipher, (p+1)/4) mod p`
2. Compute `rq = pow(cipher, (q+1)/4) mod q`
3. Combine via CRT to get four roots.

### `main()`

1. Reads primes *p*, *q* from user.
2. Computes *n = p × q*.
3. Calls `encrypt()` to produce ciphertext.
4. Calls `decrypt()` to enumerate all possible plaintexts.

---

## Edge Cases

| Scenario | Behaviour | Notes |
|----------|-----------|-------|
| Plaintext ≥ n | Wraps modulo n | Original value unrecoverable |
| Plaintext = 0 | Ciphertext = 0; roots = multiples of n (only 0 in [0,n-1]) | Trivial, avoid |
| Plaintext = 1 | Ciphertext = 1 | Smallest non-zero |
| p or q not prime | Security compromised | n is not a semiprime |
| p ≡ 1 mod 4 (not 3) | Square root formula changes | Still works with Tonelli-Shanks |
| n is a perfect square | Factorization trivial | Use distinct primes |

---

## FAQ

**Q: Is Rabin more secure than RSA?**  
A: Rabin is *provably* as hard as factoring (reduction exists). RSA is believed to be as hard but no proof exists. However, Rabin is vulnerable to chosen-ciphertext attacks (an attacker who can ask for decryptions can factor *n*).

**Q: Why does decryption give 4 answers?**  
A: Because squaring is a 4-to-1 map modulo a product of two primes. Redundancy (e.g., structured plaintext) resolves the ambiguity.

**Q: Can I use any primes?**  
A: For efficient decryption, *p ≡ q ≡ 3 (mod 4)* is conventional. Any primes work mathematically, but the simple exponent formula fails.

**Q: What is the ciphertext size?**  
A: Ciphertext is in [0, *n-1*], same as the plaintext space. The scheme is rate-1.

**Q: Is the brute-force decrypt in this demo usable for real keys?**  
A: No. For 1024-bit *n*, the loop would take longer than the age of the universe. Real decryption uses CRT + Tonelli-Shanks — O(log n).

---

## References

- M. O. Rabin, "Digitalized Signatures and Public-Key Functions as Intractable as Factorization," MIT/LCS/TR-212, 1979.
- Wikipedia: [Rabin Cryptosystem](https://en.wikipedia.org/wiki/Rabin_cryptosystem)
