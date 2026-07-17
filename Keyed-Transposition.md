# Keyed Transposition Cipher

> **File:** `9-keyedTrans.cpp` | **Type:** Classical Cipher (Transposition) | **Difficulty:** Easy

---

## Theory

A **Keyed Transposition Cipher** (also called *columnar transposition with a keyword*) rearranges plaintext characters by writing them row-wise into a grid and then reading them column-wise in an order determined by the alphabetical ranking of a keyword.

Unlike simple columnar transposition (which always reads left-to-right), the keyed variant permutes the column-read order, making the cipher harder to break without knowing the key.

**Encryption steps:**
1. Remove spaces from plaintext.
2. Determine grid dimensions: `columns = len(key)`, `rows = ceil(len(text) / columns)`.
3. Pad plaintext with `'X'` to fill the grid.
4. Fill the matrix **row-wise**.
5. Determine column-read order by sorting the keyword characters alphabetically.
6. Read columns in that key-determined order to produce ciphertext.

**Decryption steps:**
1. Determine grid dimensions: `rows = len(ciphertext) / columns`.
2. Fill the matrix **column-wise** in the same key-determined order.
3. Read **row-wise** to recover the padded plaintext.
4. Strip trailing `'X'` padding.

### Key Order Example

```
Key:  "KEY"
Upper: K  E  Y
Idx:   0  1  2
Sorted: E(1)  K(0)  Y(2)
Order: [1, 0, 2]
```

---

## Worked Example

**Plaintext:** `"HELLO WORLD"` → after removing spaces: `"HELLOWORLD"`  
**Key:** `"KEY"` → columns = 3, rows = ceil(10/3) = 4, padding = 2 → `"HELLOWORLDXX"`

**Matrix (row-wise fill):**

|       | Col 0 | Col 1 | Col 2 |
|-------|-------|-------|-------|
| Row 0 | H     | E     | L     |
| Row 1 | L     | O     | W     |
| Row 2 | O     | R     | L     |
| Row 3 | D     | X     | X     |

**Key order:** `"KEY"` → sorted `E, K, Y` → indices `[1, 0, 2]`

**Read columns in order [1, 0, 2]:**
- Col 1: `E, O, R, X` → `"EORX"`
- Col 0: `H, L, O, D` → `"HLOD"`
- Col 2: `L, W, L, X` → `"LWLX"`

**Ciphertext:** `"EORXHLODLWLX"`

**Decryption** reverses the process → `"HELLOWORLD"`.

---

## Code Walkthrough

### 1. `prepare(text)` — Pre-processing
Strips all whitespace from the input. The cipher operates on a continuous character stream.

### 2. `getKeyOrder(key)` — Key Scheduling
Creates `(char, index)` pairs for each key character (uppercased for case-insensitivity), sorts them by character (stable sort preserves insertion order for duplicates), and returns the indices array that defines column-read order.

### 3. `encrypt(text, key)` — Encryption
- Computes grid dimensions and pads with `'X'`.
- Fills a 2D `char` matrix row-wise.
- Calls `getKeyOrder` to get the column permutation.
- Appends each column's characters (top-to-bottom) in that permuted order.

**Fixed-size concern:** Matrix is hardcoded at `100×100` — plaintext longer than 10,000 chars will overflow.

### 4. `decrypt(text, key)` — Decryption
- Computes `rows = len(ciphertext) / columns`.
- Fills the matrix **column-wise** in the key-determined order (the exact reverse of encryption's read order).
- Reads **row-wise** to reconstruct the padded plaintext.
- Strips trailing `'X'` characters.

### 5. `main()` — Driver
Prompts for plaintext and key, calls `prepare`, `encrypt`, `decrypt`, and prints results.

---

## Edge Cases

| Case | Input | Behavior |
|------|-------|----------|
| Empty plaintext | `text=""` | `row=0`, no matrix fill, returns `""` |
| Empty key | `key=""` | `column=0` → division by zero in `row = ceil(len/0)` (crash) |
| Single char | `text="A", key="LONGKEY"` | 1 row, padded to 7 chars, single row means any key order gives same output |
| Exact multiple | `text="ABCDEF", key="KEY"` | No padding needed; column permutation still scrambles |
| Duplicate key letters | `key="KEEK"` | Stable sort breaks ties by original index: `E[1], E[2], K[0], K[3]` → order `[1,2,0,3]` |
| Spaces in input | `text="HELLO WORLD"` | Spaces are stripped by `prepare()` |

---

## FAQ

**Q: Why is the matrix fixed at 100×100?**  
A: This is a teaching implementation. A production version would use dynamic allocation or `std::vector`.

**Q: Does the key need to be uppercase?**  
A: No — `getKeyOrder` calls `toupper()` on each character, making it case-insensitive.

**Q: What happens if the key has duplicate letters?**  
A: `std::sort` is stable, so duplicates retain their original relative order. This is consistent between encryption and decryption.

**Q: Is this cipher secure?**  
A: No — transposition ciphers are trivially broken with known-plaintext attacks or frequency analysis. This is an educational implementation.

**Q: Why does decryption strip only trailing `'X'` characters?**  
A: Encryption always pads at the end, so padding appears only on the last row's trailing cells. Stripping only trailing `'X'` is safe unless the original plaintext ends with `'X'` (information loss — see note below).

> **Note on trailing-X loss:** If the original plaintext ends with `'X'`, those characters are indistinguishable from padding and will be stripped during decryption. This is a known limitation of this simple padding scheme.
