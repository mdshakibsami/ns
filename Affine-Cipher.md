# Affine Cipher

## Theory & Formulas

The **Affine Cipher** is a monoalphabetic substitution cipher that combines a **multiplicative** step followed by an **additive** step. It is a generalisation of both the additive and multiplicative ciphers.

### Encryption

```
E(c) = (a × c + b) mod 26
```

Where:
- `c` = letter position (A=0, B=1, …, Z=25)
- `a` = multiplicative key (must be coprime with 26)
- `b` = additive key (any integer 0–25)

### Decryption

```
D(c) = a⁻¹ × (c - b + 26) mod 26
```

Where `a⁻¹` is the **modular inverse** of `a` modulo 26.

### Valid `a` Values

`a` must share no factor with 26:

```
1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25
```

### Key Space

| Key  | Possible values | Count |
|------|----------------|-------|
| `a`  | Coprime with 26| 12    |
| `b`  | 0–25           | 26    |
| **Total** |              | **12 × 26 = 312** |

312 keys makes the affine cipher slightly harder to brute-force than the additive (26) or multiplicative (12) ciphers alone.

---

## Worked Example

**Plaintext:** `HI`  
**Keys:** `a = 5`, `b = 8`

**Encryption step-by-step:**

| Letter | c | (5c + 8) mod 26 | Cipher |
|--------|---|-----------------|--------|
| H      | 7 | 35+8=43→17      | R      |
| I      | 8 | 40+8=48→22      | W      |

**Ciphertext:** `RW`

**Decryption step-by-step:**

First find `a⁻¹` mod 26. Since `5 × 21 = 105 ≡ 1 (mod 26)`, `a⁻¹ = 21`.

| Letter | c | 21 × (c - 8 + 26) mod 26 | Plain |
|--------|---|--------------------------|-------|
| R      | 17| 21 × (17-8+26)=21×35=735→7| H    |
| W      | 22| 21 × (22-8+26)=21×40=840→8| I    |

**Recovered plaintext:** `HI`

---

## Code Walkthrough

### `getKey(inputKey)`

Parses a digit string (mod 26) or a single letter into an integer 0–25. Same utility function used in the additive and multiplicative cipher implementations.

### `modInverse(key)`

Brute-force searches `i` from 1 to 25 for `(key × i) % 26 == 1`. Returns the modular inverse or exits with an error.

### `encrypt(input, a, b)`

For each character:
```
result += (((c - base) * a + b) % 26) + base
```
Non-alphabetic characters pass through unchanged.

### `decrypt(input, a, b)`

1. Computes `inverse = modInverse(a)`.  
2. For each character:
```
result += (inverse * ((c - base - b + 26) % 26)) % 26 + base
```
The `+26` inside ensures the subtraction `c - b` stays non-negative.

### `main()`

1. Reads plaintext and two keys (`a` for multiplication, `b` for addition).  
2. Validates `gcd(a, 26) == 1`.  
3. Encrypts, decrypts, and outputs results.

---

## Edge Cases

| Scenario                    | Input        | a  | b  | Expected Behavior          |
|-----------------------------|--------------|----|----|----------------------------|
| a=1, b=0 (identity)        | `"message"`  | 1  | 0  | Cipher = Plain             |
| a=1, b≠0 (additive only)   | `"abc"`      | 1  | 3  | Cipher = `"def"`           |
| a not coprime (e.g. 2)     | `"hello"`    | 2  | 5  | Error: must be coprime     |
| Wrap-around                 | `"z"`        | 5  | 7  | (25×5+7)%26=2 → cipher `"c"` |
| Non-alphabetic characters   | `"a b"`      | 3  | 4  | Space preserved            |
| Empty input                 | `""`         | 5  | 8  | Returns empty string       |

---

## FAQ

**Q: What makes the affine cipher different from the additive cipher?**  
A: The affine cipher uses **two** operations: multiplication (`a`) then addition (`b`). The additive cipher uses only addition. The multiplicative step introduces more diffusion but also restricts valid `a` keys.

**Q: Why must `a` be coprime with 26?**  
A: If `gcd(a, 26) ≠ 1`, multiplication is not one-to-one on ℤ₂₆. Multiple plaintext letters map to the same ciphertext letter, making unique decryption impossible.

**Q: Is the affine cipher secure?**  
A: No — with only 312 possible key pairs, it is trivially broken by brute force or frequency analysis.

**Q: What is `a⁻¹ × (c - b + 26)` in decryption?**  
A: Decryption first removes the additive shift (`-b + 26`), then multiplies by the modular inverse of `a` (which undoes the multiplication). The `+26` ensures the intermediate value is non-negative.

**Q: How many possible `a` keys are there?**  
A: 12 values in 0–25 are coprime with 26: 1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25.
