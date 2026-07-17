# Hill Cipher (3×3)

> **Difficulty:** Easy | **Type:** Symmetric (Classical) | **Security Basis:** Linear Algebra

## Overview

The **Hill cipher** is a polygraphic substitution cipher based on linear algebra. Invented by Lester S. Hill in 1929, it was the first cipher to operate on **blocks of letters** (rather than individual characters).

Instead of substituting one letter at a time (as in Caesar or Vigenère), the Hill cipher encrypts *n* letters at once by multiplying the plaintext vector by an *n × n* key matrix modulo 26.

### Why 3×3?

This implementation uses a 3×3 key matrix, processing plaintext in blocks of three letters. Larger matrices provide better diffusion but require more computation. The 3×3 case is the smallest practical polygraphic cipher offering meaningful security against frequency analysis.

---

## Theory

### Linear Algebra in Mod 26

Each letter A–Z is mapped to numbers 0–25. A block of *n* plaintext letters forms a column vector *P*. The key is an invertible *n × n* matrix *K* modulo 26.

### Encryption

```
C = K · P (mod 26)
```

For a 3×3 matrix and a 3-letter block:

```
┌ c₁ ┐   ┌ k₁₁ k₁₂ k₁₃ ┐ ┌ p₁ ┐
│ c₂ │ = │ k₂₁ k₂₂ k₂₃ │ │ p₂ │   (mod 26)
└ c₃ ┘   └ k₃₁ k₃₂ k₃₃ ┘ └ p₃ ┘

c₁ = (k₁₁·p₁ + k₁₂·p₂ + k₁₃·p₃) mod 26
c₂ = (k₂₁·p₁ + k₂₂·p₂ + k₂₃·p₃) mod 26
c₃ = (k₃₁·p₁ + k₃₂·p₂ + k₃₃·p₃) mod 26
```

### Decryption

```
P = K⁻¹ · C (mod 26)
```

The key matrix **must** have an inverse modulo 26, which exists iff **gcd(det(K), 26) = 1**.

### Computing the Inverse

1. Compute determinant *d = det(K)*.
2. Find *d⁻¹ mod 26* (modular inverse — exists if gcd(*d*, 26) = 1).
3. Compute the adjugate (transpose of cofactor matrix).
4. *K⁻¹ = d⁻¹ × adj(K) mod 26*

### The Key Matrix in This Demo

```
     ┌  6 24  1 ┐                     ┌  8  5 10 ┐
K =  │ 13 16 10 │       K⁻¹ mod 26 =  │ 21  8 21 │
     └ 20 17 15 ┘                     └ 21 12  8 ┘
```

**Determinant check:** det(K) = 441 → 441 mod 26 = 25.  
gcd(25, 26) = 1 ✓, and 25⁻¹ mod 26 = 25 (since 25 × 25 = 625 ≡ 1 mod 26).

---

## Worked Example

Encrypt the plaintext **"HELLO"** using the 3×3 key matrix above.

### Step 1: Prepare

- Strip non-alpha: "HELLO"
- Uppercase: "HELLO"
- Pad to multiple of 3: "HELLOX"

### Step 2: Encode as numbers

| Letter | H | E | L | L | O | X |
|--------|---|---|---|---|---|---|
| Value  | 7 | 4 | 11 | 11 | 14 | 23 |

### Step 3: Encrypt block 1 ("HEL")

P₁ = [7, 4, 11]ᵀ

```
c₁ = (6×7 + 24×4 + 1×11)  =  42 + 96 + 11  = 149 → 149 mod 26 = 19 → T
c₂ = (13×7 + 16×4 + 10×11) = 91 + 64 + 110 = 265 → 265 mod 26 = 5  → F
c₃ = (20×7 + 17×4 + 15×11) = 140 + 68 + 165 = 373 → 373 mod 26 = 9  → J
```

Block 1 ciphertext: **"TFJ"**

### Step 4: Encrypt block 2 ("LOX")

P₂ = [11, 14, 23]ᵀ

```
c₁ = (6×11 + 24×14 + 1×23)  = 66 + 336 + 23  = 425 → 425 mod 26 = 9  → J
c₂ = (13×11 + 16×14 + 10×23) = 143 + 224 + 230 = 597 → 597 mod 26 = 25 → Z
c₃ = (20×11 + 17×14 + 15×23) = 220 + 238 + 345 = 803 → 803 mod 26 = 23 → X
```

Block 2 ciphertext: **"JZX"**

### Result

Ciphertext: **"TFJJZX"**

Decryption using *K⁻¹* recovers "HELLOX", then trailing 'X' is stripped to give "HELLO".

---

## Code Walkthrough

```cpp
// Hill-Cipher-3x3.cpp — complete annotated source
```

### `key[3][3]` and `inverseKey[3][3]`

Hard-coded 3×3 matrices. The inverse matrix is precomputed — computing it at runtime would require determinant checking and modular inversion.

### `prepare(text)`

- Filters to alphabetic characters only.
- Converts to uppercase.
- Pads with 'X' until length is a multiple of 3.

### `encrypt(text)`

Processes text in 3-letter blocks. For each block:
1. Convert letters to numbers (A→0, ..., Z→25).
2. Compute linear combinations with the key matrix.
3. Apply mod 26 and convert back to letters.

### `decrypt(text)`

Same structure as `encrypt`, but uses `inverseKey`. After reconstruction, strips trailing 'X' characters added during padding.

### `main()`

Reads a line of text, prepares it, encrypts, decrypts, and displays all three stages.

---

## Edge Cases

| Scenario | Behaviour | Notes |
|----------|-----------|-------|
| Empty string | prepare() returns ""; encrypt returns "" | Trivial |
| Single letter | Padded to 3 with "XX" | Round-trips correctly |
| Two letters | Padded to 3 with "X" | Round-trips correctly |
| All digits | Stripped to empty string | prepare() returns "" |
| Mixed case | Converted to uppercase | prepare() normalises |
| Trailing X in original text | Stripped along with padding X | Ambiguity — real messages must avoid trailing X |
| Non-printable characters | Stripped by isalpha() | Silent discard |
| Key matrix has gcd(det,26)≠1 | decryption produces garbage | Must verify invertibility |

---

## FAQ

**Q: Why are padding X characters stripped?**  
A: The receiver has no way to distinguish real X's from padding X's. In practice, use a length field or a different padding scheme (e.g., PKCS#7).

**Q: Can the Hill cipher be broken?**  
A: Yes. It is linear — known-plaintext attacks with *n²* plaintext-ciphertext pairs recover the key. Modern ciphers (AES, ChaCha20) use non-linear S-boxes for a reason.

**Q: Why mod 26?**  
A: The English alphabet has 26 letters. Mod 26 arithmetic keeps all results within A–Z.

**Q: Can I use a different key matrix?**  
A: Yes, as long as gcd(det(K), 26) = 1. A singular matrix (det ≡ 0 mod 26) would lose information.

**Q: What's the advantage over simple substitution?**  
A: Polygraphic substitution hides letter frequencies better because each ciphertext letter depends on multiple plaintext letters (diffusion).

**Q: Does the key matrix need to be 3×3?**  
A: No. Hill ciphers work with any *n × n* matrix. Larger *n* increases diffusion and key size.

---

## References

- L. S. Hill, "Cryptography in an Algebraic Alphabet," *The American Mathematical Monthly*, 1929.
- Wikipedia: [Hill cipher](https://en.wikipedia.org/wiki/Hill_cipher)
