# Feistel Cipher (Single-Key)

> **File:** `11-Feistel.cpp` | **Type:** Classical Cipher (Feistel Network) | **Difficulty:** Medium

---

## Theory

A **Feistel cipher** is a symmetric structure used in many block ciphers (DES, Blowfish, etc.). It splits the plaintext into two halves — **left (L)** and **right (R)** — and applies a number of **rounds** that mix the halves using a **round function F** and a **key**.

The defining property of a Feistel network is that **encryption and decryption use the same structure**, making hardware/software implementation simpler.

### Feistel Round

For each round *i*:

```
Lᵢ₊₁ = Rᵢ
Rᵢ₊₁ = Lᵢ ⊕ F(key, Rᵢ)
```

### Decryption (same structure, reverse order)

```
Rᵢ = Lᵢ₊₁
Lᵢ = Rᵢ₊₁ ⊕ F(key, Lᵢ₊₁)
```

This works because XOR is its own inverse: if `R' = L ⊕ F(key, R)`, then `L = R' ⊕ F(key, R)`.

### This Implementation

- **Round function F:** XOR each character of the right half with the integer key: `F(key, R) = R ⊕ key` (byte-wise).
- **XOR:** Character-by-character XOR of two strings.
- **Same key** is reused for every round (simplified).
- **Padding:** If the plaintext has odd length, `'X'` is appended to make it even.

---

## Worked Example

**Plaintext:** `"ABCDEF"`  
**Key:** `5`  
**Rounds:** `1`

**Step 1:** Length = 6 (even), no padding. `mid = 3`.  
`L = "ABC"`, `R = "DEF"`

**Step 2:** Round 1
- `F(5, "DEF")`:
  - `'D' ^ 5 = 68 ^ 5 = 65` → `'A'`
  - `'E' ^ 5 = 69 ^ 5 = 64` → `'@'`
  - `'F' ^ 5 = 70 ^ 5 = 67` → `'C'`
  - Result: `"A@C"`
- `right' = L XOR F(5, R)` = `"ABC" XOR "A@C"`
  - `'A' ^ 'A' = 0` → `'\0'`
  - `'B' ^ '@' = 66 ^ 64 = 2` → (non-printable)
  - `'C' ^ 'C' = 0` → `'\0'`
- `left' = R = "DEF"`

**Output:** `"DEF" + right'` = `"DEF" + non-printable-chars`

**Decryption** with the same key and rounds reverses this to recover `"ABCDEF"`.

---

## Code Walkthrough

### 1. `F(key, right)` — Round Function
Takes the integer key and right-half string, XORs each character of `right` with `key`. Since `char` is typically 8 bits and `int` is larger, only the lower 8 bits of `key` affect the result (`char` overflow truncation).

### 2. `XOR(a, b)` — String XOR
XORs two equal-length strings character-by-character. Used to combine the round function output with one half.

### 3. `encrypt(text, key, rounds)` — Encryption
- Pads odd-length text with `'X'`.
- Splits into left and right halves.
- For each round: computes `new_right = left XOR F(key, right)`, `new_left = right` (swap).
- Returns concatenation `left + right`.

### 4. `decrypt(text, key, rounds)` — Decryption
- Splits ciphertext into two halves.
- Applies the **same loop structure** but with left/right roles swapped in the assignment.
- Strips trailing `'X'` padding.

### 5. `main()` — Driver
Prompts for plaintext, key (integer), and round count; runs encrypt and decrypt; prints results.

---

## Edge Cases

| Case | Input | Behavior |
|------|-------|----------|
| Empty plaintext | `text=""` | `mid=0`, empty halves, returns `""` |
| Odd length | `text="ODD"` | Padded to `"ODDX"`, decryption strips `'X'` → `"ODD"` |
| Zero rounds | `text="TEST", rounds=0` | Encryption/decryption loop doesn't execute — identity operation |
| Key = 0 | `key=0` | `F(0, R) = R` (XOR with 0 is identity). Still a valid Feistel round (XOR mixing occurs) |
| Key > 255 | `key=300` | Truncated to 8 bits: `300 % 256 = 44`. Functionally equivalent to `key=44` |
| Single character | `text="A"` | Padded to `"AX"`, single round works, decryption returns `"A"` |
| Trailing `'X'` in original | `text="BOX"` (3 chars) | Padded to `"BOXX"`, decryption strips last `'X'`, but original `"BOX"` has no trailing X → correct. If text were `"BO"` (already even), no issue. Problematic if text ends with `'X'` and is odd-length: that `'X'` is indistinguishable from padding |

---

## FAQ

**Q: Is this a secure Feistel cipher?**  
A: No. The round function `F(key, right) = right XOR key` is trivially invertible. Real Feistel ciphers use complex non-linear round functions (S-boxes, permutations). This is purely educational.

**Q: Why use the same key for every round?**  
A: Simplicity. Real ciphers derive distinct round keys from a master key via a key schedule.

**Q: Why does decryption use the same code structure as encryption?**  
A: That's the core property of Feistel networks. Swapping the left/right assignments and applying rounds in reverse is the only change.

**Q: What happens if left and right have different lengths?**  
A: They never do, because odd-length inputs are padded. Midpoint is always `length/2`, and both halves are equal.

**Q: Does XOR with a non-printable character produce visible output?**  
A: No — `cout` will display non-printable characters as garbage or nothing, but the underlying bytes are preserved, so decryption still works correctly.
