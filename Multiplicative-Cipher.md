# Multiplicative Cipher

## Theory & Formulas

A **Multiplicative Cipher** replaces each letter by multiplying its numeric position by a fixed key, then taking the result modulo 26. The key must be **coprime with 26** (i.e., gcd(key, 26) = 1) for the cipher to be reversible.

### Encryption

```
E(c) = (c × key) mod 26
```

Where `c` is the letter position (A=0, B=1, …, Z=25) and `key` is an integer coprime with 26.

### Decryption

```
D(c) = (c × key⁻¹) mod 26
```

`key⁻¹` is the **modular inverse** of `key` modulo 26, defined as the integer `i` such that:

```
(key × i) ≡ 1 (mod 26)
```

### Valid Keys

Only 12 numbers between 0–25 are coprime with 26:

```
1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25
```

### Why gcd(key, 26) = 1 is Required

If `key` shares a factor with 26 (e.g., 2 or 13), multiplication maps multiple plaintext letters to the same ciphertext letter, making decryption ambiguous.

---

## Worked Example

**Plaintext:** `HI`  
**Key:** `5`

**Encryption step-by-step (gcd(5,26) = 1):**

| Letter | c (A=0) | (c × 5) mod 26 | Cipher |
|--------|----------|----------------|--------|
| H      | 7        | 35 mod 26 = 9  | J      |
| I      | 8        | 40 mod 26 = 14 | O      |

**Ciphertext:** `JO`

**Decryption step-by-step:**

First, find the modular inverse of 5 mod 26. Since `5 × 21 = 105 ≡ 1 (mod 26)`, `key⁻¹ = 21`.

| Letter | c (A=0) | (c × 21) mod 26 | Plain |
|--------|----------|-----------------|-------|
| J      | 9        | 189 mod 26 = 7  | H     |
| O      | 14       | 294 mod 26 = 8  | I     |

**Recovered plaintext:** `HI`

---

## Code Walkthrough

### `getKey(inputKey)`

Identical to the additive cipher version — parses a digit string (mod 26) or a single alphabetic character into an integer 0–25.

### `modInverse(key)`

```cpp
int modInverse(int key)
```

Brute-force searches `i` from 1 to 26 for the condition `(key × i) % 26 == 1`. Returns `i` if found; otherwise prints an error and exits. This is the **Extended Euclidean algorithm** replacement for small 26.

### `encrypt(input, key)`

```cpp
string encrypt(string input, int key)
```

For each character `c`:
- **If alphabetic**: `result += ((c - base) * key) % 26 + base`
- **If non-alphabetic**: appended unchanged.

### `decrypt(input, key)`

```cpp
string decrypt(string input, int key)
```

1. Computes `inverse = modInverse(key)`.  
2. For each character `c`: `result += ((c - base) * inverse) % 26 + base`.

### `main()`

1. Reads plaintext and key.  
2. Calls `getKey` and checks `gcd(key, 26) == 1`.  
3. Encrypts, decrypts, and prints both results.

---

## Edge Cases

| Scenario                   | Input       | Key | Expected Behavior             |
|----------------------------|-------------|-----|-------------------------------|
| Key = 1 (identity)         | `"hello"`   | 1   | Cipher = Plain                |
| Key not coprime (e.g. 2)   | `"hello"`   | 2   | Error: must be coprime        |
| Key = 25 (coprime)         | `"ab"`      | 25  | Cipher = `"az"`               |
| Non-alphabetic characters  | `"a b"`     | 3   | Space preserved               |
| Key from letter (C→2)      | `"test"`    | C   | gcd(2,26)=2 → error           |
| Empty input                | `""`        | 5   | Returns empty string          |

---

## FAQ

**Q: Why only 12 keys?**  
A: Only 12 numbers in 0–25 share no common factor with 26 = 2 × 13. Any other key makes decryption impossible.

**Q: How does the multiplicative cipher differ from the additive cipher?**  
A: The additive cipher uses addition (shift); the multiplicative cipher uses multiplication. The multiplicative cipher has a much smaller keyspace (12 vs 26) but is still trivially brute-forced.

**Q: What is a modular inverse?**  
A: The modular inverse of `k` modulo 26 is a number `i` such that `(k × i) mod 26 = 1`. It "undoes" multiplication in modular arithmetic.

**Q: How would `getKey` handle a key like `"3"` vs `"C"`?**  
A: `"3"` → `stoi("3") % 26 = 3`. `"C"` → `'C' - 'A' = 2`. Both return an integer 0–25.
