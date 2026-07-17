# Merkle-Hellman Knapsack Cryptosystem

> **Difficulty:** Medium | **Type:** Asymmetric (Public-Key) | **Security Basis:** Subset-Sum (NP-Complete) | **Status:** Broken (1982)

## Overview

The **Merkle-Hellman knapsack cryptosystem** was one of the earliest public-key cryptosystems, published by Ralph Merkle and Martin Hellman in 1978. It is based on the **subset-sum problem** (also known as the knapsack problem), which is NP-complete.

### The Trapdoor

The subset-sum problem is hard for arbitrary sets of numbers, but **easy** for **superincreasing sequences** — sequences where each element is greater than the sum of all previous elements.

The private key is a superincreasing sequence. It is disguised via modular multiplication to produce a seemingly "random" public key. The intended recipient can undo the disguise and solve the easy superincreasing subset-sum, while an attacker sees only the hard general subset-sum problem.

### Broken Status

In 1982, Adi Shamir broke the Merkle-Hellman scheme by showing that the public key's structure could be recovered without knowing the original private key. The scheme is **not secure** for modern use.

---

## Theory

### Superincreasing Sequences

A sequence *b₁, b₂, ..., bₙ* is **superincreasing** if:

```
bₖ > Σᵢ₌₁ᵏ⁻¹ bᵢ    for all k = 2, ..., n
```

The subset-sum problem on a superincreasing sequence is trivially solvable by a **greedy algorithm** (scan from largest to smallest, subtract when possible).

### Key Generation

1. Choose a superincreasing private key *b₁, b₂, ..., bₙ*.
2. Choose modulus *m* such that *m > Σ bᵢ*.
3. Choose multiplier *w* such that *gcd(w, m) = 1*.
4. Compute public key: *aᵢ = (bᵢ × w) mod m*.

**Private key:** (*b*, *w*, *m*)  
**Public key:** *a*

### Encryption

Represent the message as an *n*-bit binary string *s₁ s₂ ... sₙ*. Compute:

```
c = Σ sᵢ × aᵢ
```

That is, sum the public-key elements where the message bit is 1.

### Decryption

1. Compute *w⁻¹ mod m* (modular inverse).
2. Transform ciphertext: *c' = (c × w⁻¹) mod m*.
3. Solve the superincreasing subset-sum on *b* for target *c'* using the greedy algorithm.

The result is the original *n*-bit string.

### Why *m > Σ bᵢ*?

If *m* were smaller than the sum, modular reduction during decryption would wrap around, destroying information. Messages with many 1-bits would decrypt incorrectly.

---

## Worked Example

**Private key:** *b* = {2, 3, 7, 14, 30, 57}  
**Modulus:** *m = 105*  
**Multiplier:** *w = 31*  
(gcd(31, 105) = 1 ✓ — but note *m* = 105 < sum(*b*) = 113, a known flaw!)

**Public key:**

```
a₁ = (2 × 31) mod 105  = 62
a₂ = (3 × 31) mod 105  = 93
a₃ = (7 × 31) mod 105  = 7
a₄ = (14 × 31) mod 105 = 14
a₅ = (30 × 31) mod 105 = 90
a₆ = (57 × 31) mod 105 = 87
```

**Encrypt message "101100":**

```
c = a₁ + a₃ + a₄ = 62 + 7 + 14 = 83
```

**Decrypt:**

1. *w⁻¹ mod 105*: 31 × 61 = 1891 ≡ 1 (mod 105) → *w⁻¹ = 61*
2. *c' = (83 × 61) mod 105 = 5063 mod 105 = 23*
3. Greedy decomposition:

| Step | Value | b₆=57 | b₅=30 | b₄=14 | b₃=7 | b₂=3 | b₁=2 |
|------|-------|-------|-------|-------|------|------|------|
| 1 | 23 | ≤23? No | ≤23? No | **≤23? Yes** → bit₄=1, v=9 | | | |
| 2 | 9 | | | | **≤9? Yes** → bit₃=1, v=2 | | |
| 3 | 2 | | | | | ≤2? No | **≤2? Yes** → bit₁=1, v=0 |

Result: **"101100"** ✓

---

## Code Walkthrough

```cpp
// Merkle-Hellman-Knapsack.cpp — complete annotated source
```

### `modInverse(a, m)`

Brute-force search for *i* such that *a × i ≡ 1 (mod m)*. Linear scan up to *m*. In practice, use Extended Euclidean Algorithm.

### `generatePublicKey(privateKey, w, m)`

Element-wise: *pub[i] = (priv[i] × w) mod m*. This is the modular disguise that hides the superincreasing structure.

### `encrypt(message, publicKey)`

Sums public-key elements corresponding to '1' bits in the binary message string. Length of message must match the key size.

### `decrypt(cipher, privateKey, w, m)`

1. Find modular inverse of *w* modulo *m*.
2. Transform cipher: *value = (cipher × w⁻¹) mod m*.
3. Walk the private key from largest to smallest, setting bit = 1 if element ≤ current value and subtracting.

### `main()`

Defines a fixed private key {2, 3, 7, 14, 30, 57}, *w = 31*, *m = 105*. Reads a binary string, encrypts and decrypts.

---

## Edge Cases

| Scenario | Behaviour | Notes |
|----------|-----------|-------|
| All-zero message | Ciphertext = 0 | Valid, decrypted as "000000" |
| All-one message | Sum may exceed m | Failure if m < sum(privKey) |
| Message shorter than key | encrypt only sums existing bits; decrypt produces full-length key | Size mismatch |
| Non-binary characters | Never match '1', treated as 0 | Silent data corruption |
| w, m not coprime | modInverse returns -1; decrypt fails | Invalid parameter |
| m ≤ sum(privateKey) | Overflow in decryption | Messages with many 1-bits decode incorrectly |

---

## FAQ

**Q: Why is Merkle-Hellman called a "knapsack" cryptosystem?**  
A: Encryption creates an instance of the subset-sum (knapsack) problem: given the public-key set, find which subset sums to the ciphertext — which is NP-complete.

**Q: How was it broken?**  
A: Shamir showed that the modular multiplication disguise does not adequately hide the superincreasing structure. The public key's lattice structure can be exploited to recover the private key using integer programming / lattice reduction.

**Q: Is there a modern version that's secure?**  
A: Later knapsack variants (Chor-Rivest, etc.) were also broken. Lattice-based cryptography has largely superseded knapsack-based schemes.

**Q: Why is a superincreasing sequence easy to solve?**  
A: Because each element is larger than all smaller ones combined, the greedy choice (take if it fits) is always optimal — no backtracking needed.

**Q: What happens if m is too small?**  
A: Modular reduction during decryption wraps the ciphertext, making the superincreasing structure unrecoverable. This caused the demo's all-ones test to fail.

---

## References

- R. Merkle and M. Hellman, "Hiding Information and Signatures in Trapdoor Knapsacks," *IEEE Trans. Info. Theory*, 1978.
- A. Shamir, "A Polynomial Time Algorithm for Breaking the Basic Merkle-Hellman Cryptosystem," *CRYPTO 1982*.
- Wikipedia: [Merkle–Hellman knapsack cryptosystem](https://en.wikipedia.org/wiki/Merkle%E2%80%93Hellman_knapsack_cryptosystem)
