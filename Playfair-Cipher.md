# Playfair Cipher: Implementation, Theory & Examples

The **Playfair cipher** is a manual symmetric encryption technique that encrypts pairs of letters (digraphs) instead of single letters, making it significantly harder to crack than simple substitution ciphers. Invented by **Charles Wheatstone** in 1854 and popularized by **Lord Playfair**, it was used by the British military in World War I.

---

## Table of Contents

- [How It Works](#how-it-works)
- [The 5×5 Key Matrix](#the-55-key-matrix)
- [Encryption Rules](#encryption-rules)
- [Worked Example](#worked-example)
- [Code Walkthrough](#code-walkthrough)
- [Edge Cases & Pitfalls](#edge-cases--pitfalls)
- [Frequently Asked Questions](#frequently-asked-questions)

---

## How It Works

Playfair operates on **digraphs** — pairs of letters. Given a 5×5 matrix built from a keyword (in our implementation the matrix is hardcoded), each plaintext digraph is transformed into a ciphertext digraph using three simple geometric rules.

### Key Properties

| Property            | Detail                                      |
| ------------------- | ------------------------------------------- |
| **Key space**       | 25 letters (I and J share one cell)         |
| **Plaintext unit**  | Digraph (2 letters)                         |
| **Cipher type**     | Symmetric, digraph substitution             |
| **Historical use**  | British WWI communications                  |

---

## The 5×5 Key Matrix

Our code uses this fixed matrix:

```
P L A Y F
I R E X M
B C D G H
K N O Q S
T U V W Z
```

Position of each letter is identified by its row (0–4) and column (0–4). `J` is mapped to `I`.

---

## Encryption Rules

For a digraph `(a, b)`:

| Condition          | Action                                                      |
| ------------------ | ----------------------------------------------------------- |
| **Same row**       | Replace each letter with the one immediately to its right (wrap around) |
| **Same column**    | Replace each letter with the one immediately below (wrap around) |
| **Rectangle**      | Swap columns: keep rows, exchange columns                   |

### Decryption

Same logic, but **reverse** the shifts:
- Same row → shift **left** (subtract 1, or add 4 mod 5)
- Same column → shift **up**
- Rectangle → same swap (symmetric)

---

## Worked Example

**Plaintext**: `HELLO`

### Step 1 — Prepare
1. Uppercase → `HELLO`
2. Split into digraphs: `HE LL O` (odd) → pad last with `X` → `HE LX LO`
3. `LL` has repeated letters → insert `X` between → `HE LX LO` → done.

**Prepared text**: `HELXLO`

### Step 2 — Locate positions
- `H` → (2,4)
- `E` → (1,2)
- `L` → (0,1)
- `X` → (1,3)
- `L` → (0,1)
- `O` → (2,2)

### Step 3 — Encrypt each digraph

| Digraph | Rule           | Cipher |
| ------- | -------------- | ------ |
| `HE`    | Rectangle      | `?`    |
| `LX`    | Same row (1)   | `?`    |
| `LO`    | Rectangle      | `?`    |

*(Full calculation left as exercise for the reader.)*

### Step 4 — Decrypt
Feed ciphertext back into `decrypt()` — the output should be `HELXL` (prepared form). The original trailing `X` is not removed because Playfair does not strip padding.

---

## Code Walkthrough

### File: `5-playfair.cpp`

| Function               | Purpose                                                   |
| ---------------------- | --------------------------------------------------------- |
| `findPosition()`       | Searches the 5×5 matrix for a character, returns row/col  |
| `prepareText()`        | Cleans input (alpha only, uppercase, J→I), inserts X fillers |
| `encrypt()`            | Applies Playfair rules to each digraph                    |
| `decrypt()`            | Reverses the encryption rules                             |

#### `prepareText()` details

1. Removes non-alphabetic characters
2. Converts to uppercase; maps `J → I`
3. Iterates: if two consecutive letters are the same, inserts `X` between them
4. If odd length, appends `X` at the end

> **Note**: Step 3 and 4 together can produce unexpected results — read the source comments carefully.

#### `encrypt()` / `decrypt()`

Both loop over the text two characters at a time, call `findPosition()` for each, then apply the appropriate rule. Decryption uses `+4` instead of `+1` (which is equivalent to `-1 mod 5`).

---

## Edge Cases & Pitfalls

| Case                       | Behaviour                                                    |
| -------------------------- | ------------------------------------------------------------ |
| **Empty string**           | `prepareText` returns empty; loops run 0 times               |
| **Non-alphabetic input**   | All non-alpha stripped; may produce empty string             |
| **Single character**       | Always padded with `X` → length 2                            |
| **`J` in plaintext**       | Mapped to `I` (irreversible loss)                            |
| **Double letters**         | Separated by `X` (e.g. `LL` → `LXL`)                        |
| **Double X in input**      | `XX` → `X` inserted → `XXX` (3 chars, last pair `X`+`X` — same rule applies again!) |
| **Odd length after prep**  | Technically impossible because `prepareText` always produces even length |
| **Decrypted padding `X`**  | Not removed — cipher output has same length as prepared input |

### Common Mistake

The `prepareText` function processes repeated letters **before** handling the odd-length case. This means `XX` becomes `XXX` (not `XX`), which changes the original data. The decrypted text therefore contains extra `X` characters that were never in the original plaintext.

---

## Frequently Asked Questions

**Q: Is Playfair still secure today?**
A: No. Playfair can be broken with ~50–100 known plaintext–ciphertext digraph pairs using hill climbing or frequency analysis.

**Q: Why does the code treat J as I?**
A: The 5×5 matrix has only 25 cells for 26 letters. I and J historically share a cell.

**Q: How do I use a custom key matrix?**
A: Replace the hardcoded `matrix[5][5]` with one generated from your keyword. The standard process: write the keyword, remove duplicates, fill the rest with the remaining letters of the alphabet (omitting J).

**Q: Why does decryption return extra X letters?**
A: The code does not strip filler X's inserted during preparation. For a true round-trip, those X's must be removed manually or the prepare/decode logic must track filler positions.

**Q: Can I encrypt numbers and symbols?**
A: No. Non-alphabetic characters are stripped by `prepareText`. Only A–Z (with I/J merged) are supported.

**Q: What is the determinant condition for a Playfair matrix?**
A: Unlike Hill, Playfair has no determinant requirement — any 5×5 Latin square (each letter used once) works.

---

*For the full source code, see [`5-playfair.cpp`](5-playfair.cpp).*
