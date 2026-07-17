# Vigenère Cipher: Implementation, Theory & Examples

The **Vigenère cipher** is a method of encrypting alphabetic text by using a series of interwoven Caesar ciphers based on the letters of a keyword. Invented by **Giovan Battista Bellaso** in 1553 and later misattributed to **Blaise de Vigenère**, it was considered *le chiffre indéchiffrable* (the indecipherable cipher) for three centuries.

---

## Table of Contents

- [How It Works](#how-it-works)
- [Encryption & Decryption](#encryption--decryption)
- [Worked Example](#worked-example)
- [Code Walkthrough](#code-walkthrough)
- [Edge Cases & Pitfalls](#edge-cases--pitfalls)
- [Frequently Asked Questions](#frequently-asked-questions)

---

## How It Works

Vigenère is a **polyalphabetic substitution cipher**: each plaintext letter is shifted by a different amount determined by the keyword. The keyword is repeated cyclically to match the length of the plaintext.

### Key Properties

| Property            | Detail                                      |
| ------------------- | ------------------------------------------- |
| **Key space**       | `26ⁿ` for an n-letter keyword               |
| **Plaintext unit**  | Single letter                               |
| **Cipher type**     | Symmetric, polyalphabetic substitution      |
| **Security**        | Resists simple frequency analysis            |

---

## Encryption & Decryption

### Formula

```
Cᵢ = (Pᵢ + Kⱼ) mod 26
Pᵢ = (Cᵢ - Kⱼ + 26) mod 26
```

Where:
- `Pᵢ` is the plaintext letter (0–25)
- `Kⱼ` is the key letter (0–25), `j = i mod len(key)`
- `Cᵢ` is the ciphertext letter (0–25)

### Keystream Generation

The keyword `KEY` with plaintext `HELLO` produces keystream:

```
Plain:  H  E  L  L  O
Key:    K  E  Y  K  E
```

---

## Worked Example

**Plaintext**: `ATTACKATDAWN`
**Keyword**:  `KEY`

### Step 1 — Generate Keystream

```
Plain:   A  T  T  A  C  K  A  T  D  A  W  N
Key:     K  E  Y  K  E  Y  K  E  Y  K  E  Y
```

### Step 2 — Convert letters to numbers

```
A=0,  T=19, T=19,  A=0,  C=2,  K=10, A=0,  T=19, D=3,  A=0,  W=22, N=13
K=10, E=4,  Y=24, K=10, E=4,  Y=24, K=10, E=4,  Y=24, K=10, E=4,  Y=24
```

### Step 3 — Add (mod 26)

```
P+K:  10  23  43  10   6  34  10  23  27  10  26  37
Mod:  10  23  17  10   6   8  10  23   1  10   0  11
```

### Step 4 — Convert back to letters

```
K  X  R  K  G  I  K  X  B  K  A  L
```

**Ciphertext**: `KXRKGIKXBKAL`

### Step 5 — Decrypt

`decrypt("KXRKGIKXBKAL", "KEY")` → `ATTACKATDAWN` ✓

---

## Code Walkthrough

### File: `6-Vigenere.cpp`

| Function         | Purpose                                                       |
| ---------------- | ------------------------------------------------------------- |
| `generateKey()`  | Repeats the keyword to match input length, preserving non-alpha positions |
| `encrypt()`      | Shifts each alphabetic character by key value (mod 26)        |
| `decrypt()`      | Reverses the shift                                            |

#### `generateKey()` details

- Loops over every character of `input`
- For alphabetic characters, appends the next key character (cycling)
- For non-alphabetic characters, appends the original character itself (not a key char)

> **Important**: The keystream length equals the input length. Non-alpha positions in the keystream hold the original character, which `encrypt()` passes through unchanged.

#### `encrypt()` / `decrypt()`

- Detects case (`isupper`) and applies shift relative to `'A'` or `'a'`
- Non-alpha characters are appended unchanged
- Uses `toupper(key[i])` so key case does not matter

### Key Validation

```
for(char c : inputKey)
    if(!isalpha(c)) { cout << "Invalid Key!"; return 0; }
```

Only alphabetic keys are accepted; numbers and symbols in the key cause the program to exit.

---

## Edge Cases & Pitfalls

| Case                          | Behaviour                                                    |
| ----------------------------- | ------------------------------------------------------------ |
| **Empty key**                 | `generateKey` uses `key[j]` with `j=0` on an empty string → **out-of-bounds access** (undefined behaviour) |
| **Key with numbers**          | Validation rejects with "Invalid Key!"                       |
| **Empty plaintext**           | `generateKey` returns `""`; encrypt returns `""`             |
| **Mixed case**                | Case preserved; key applied via `toupper` internally         |
| **Non-alphabetic characters** | Pass through unchanged (both in plaintext and keystream positions) |
| **Key longer than plaintext** | Only first `len(plaintext)` key characters are used; no error |
| **Single character**          | Works correctly — one key letter shifts it                   |

### Critical Bug: Empty Key

If the user enters an empty key, `generateKey` still tries to read `key[0]`, which is undefined behaviour (likely crash or garbage). A check `if(key.empty())` should be added.

---

## Frequently Asked Questions

**Q: Is Vigenère still secure?**
A: No. Kasiski examination (1863) and Friedman's index of coincidence break it easily once the keyword length is deduced.

**Q: How do I find the key length (cryptanalysis)?**
A: Use Kasiski's method: find repeated ciphertext sequences and compute the GCD of their distances. Or use the index of coincidence.

**Q: Why does the code preserve non-alphabetic characters?**
A: For practical use — messages often contain spaces, punctuation, or numbers. The cipher only encrypts letters.

**Q: Does the key have to be uppercase?**
A: No. `toupper()` is applied internally to compute the shift, so any case works.

**Q: What is the effective key space?**
A: `26ⁿ` for an n-letter keyword. For a 6-letter keyword, that is ~309 million possibilities.

**Q: Can the same key be used for multiple messages?**
A: Yes, but it leaks information. Modern practice uses a one-time pad (non-repeating key as long as the message).

---

*For the full source code, see [`6-Vigenere.cpp`](6-Vigenere.cpp).*
