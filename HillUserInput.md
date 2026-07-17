# Hill Cipher with User-Input Key Matrix

> **File:** `HillUserInput.cpp` | **Type:** Classical Cipher (Polygraphic) | **Difficulty:** Medium

---

## Theory

The **Hill cipher** is a polygraphic substitution cipher based on linear algebra. Unlike the fixed-key variants (`Hill22.cpp`, `Hill33.cpp`), this version allows the user to supply both the key matrix and its inverse at runtime, supporting **2×2** and **3×3** matrices.

### Encryption Formula

```
C = K × P  (mod 26)
```

Each block of `n` letters is treated as a column vector and multiplied by the `n×n` key matrix.

### Decryption Formula

```
P = K⁻¹ × C  (mod 26)
```

The inverse matrix `K⁻¹` must satisfy `K · K⁻¹ = I (mod 26)`. It exists only if `det(K)` is coprime with 26.

---

## Usage

```
g++ -std=c++11 -o hilluser HillUserInput.cpp
./hilluser
```

### Input Format

1. **Matrix size** — `2` or `3`
2. **Key matrix** — space-separated integers, one row per line
3. **Inverse key matrix** — space-separated integers, one row per line
4. **Plaintext** — alphabetic text (digits/symbols stripped automatically)

---

## Worked Example (2×2)

Using the same key as `Hill22.cpp`:

```
Enter matrix size (2 or 3): 2

Enter Key Matrix (2x2) values:
Row 1: 3 3
Row 2: 2 5

Enter Inverse Key Matrix (2x2) values:
Row 1: 15 17
Row 2: 20 9

Enter Plain Text: Hello World

Prepared Text : HELLOWORLD
Cipher Text   : HIOZEIPJQL
Plain Text    : HELLOWORLD
```

### Manual Trace

Block `HE` (H=7, E=4):
```
c1 = (3·7 + 3·4) mod 26 = 33 mod 26 = 7 → H
c2 = (2·7 + 5·4) mod 26 = 34 mod 26 = 8 → I
```
Result: `HI` — matches first digraph of ciphertext.

---

## Edge Cases

| Input | Behaviour |
|-------|-----------|
| Empty string | Returns empty |
| Non-alphabetic (`"123"`) | All stripped → empty |
| Single letter | Padded with `X` to length `n`, stripped on decrypt |
| Odd length (2×2) | Padded with `X` to even length |
| Not multiple of 3 (3×3) | Padded with `X` to multiple of 3 |
| All `A`'s | Zero vector → ciphertext is all `A`'s |
