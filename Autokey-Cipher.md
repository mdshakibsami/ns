# Autokey Cipher

## Theory & Formulas

The **Autokey Cipher** (also known as the **Vigenère autokey cipher**) extends the Vigenère cipher by using the **plaintext itself** as the running key after an initial seed key. This makes it a **polyalphabetic cipher** that resists simple frequency analysis better than monoalphabetic ciphers.

### Encryption

An initial key `k₀` is chosen. For each plaintext letter `pᵢ`:

```
cᵢ = (pᵢ + kᵢ) mod 26
kᵢ₊₁ = pᵢ
```

The key for position `i` is:
- `k₀` = initial key (provided by user) for the first letter
- Each subsequent key is the **previous plaintext letter**

### Decryption

```
pᵢ = (cᵢ - kᵢ + 26) mod 26
kᵢ₊₁ = pᵢ
```

The decrypted letter becomes the key for the next position, perfectly mirroring encryption.

### Autokey Feedback Mechanism

```
Step 1: c₀ = (p₀ + k₀) mod 26    (uses initial key)
Step 2: k₁ = p₀                   (plaintext feeds back as key)
Step 3: c₁ = (p₁ + k₁) mod 26     (uses previous plaintext)
Step 4: k₂ = p₁                   (and so on...)
```

This "self-keying" property means the effective key length equals the plaintext length.

---

## Worked Example

**Plaintext:** `HELLO`  
**Initial key:** `7`

**Encryption step-by-step:**

| i | Plain pᵢ | Key kᵢ | (pᵢ + kᵢ) mod 26 | Cipher cᵢ | Next key = pᵢ |
|---|----------|--------|-------------------|-----------|---------------|
| 0 | H (7)    | 7      | 14                | O         | H (7)         |
| 1 | E (4)    | 7      | 11                | L         | E (4)         |
| 2 | L (11)   | 4      | 15                | P         | L (11)        |
| 3 | L (11)   | 11     | 22                | W         | L (11)        |
| 4 | O (14)   | 11     | 25                | Z         | —             |

**Ciphertext:** `OLPWZ`

**Decryption step-by-step:**

| i | Cipher cᵢ | Key kᵢ | (cᵢ - kᵢ + 26) mod 26 | Plain pᵢ | Next key = pᵢ |
|---|-----------|--------|------------------------|----------|---------------|
| 0 | O (14)    | 7      | 7                      | H (7)    | H (7)         |
| 1 | L (11)    | 7      | 4                      | E (4)    | E (4)         |
| 2 | P (15)    | 4      | 11                     | L (11)   | L (11)        |
| 3 | W (22)    | 11     | 11                     | L (11)   | L (11)        |
| 4 | Z (25)    | 11     | 14                     | O (14)   | —             |

**Recovered plaintext:** `HELLO`

---

## Code Walkthrough

### `encrypt(input, key)`

```cpp
string encrypt(string input, int key)
```

Initialises `currentKey = key`. For each character `c`:
1. **If alphabetic**: compute `(c - base + currentKey) % 26 + base`, append to result, then set `currentKey = toupper(c) - 'A'` (the **plaintext letter** becomes the next key).
2. **If non-alphabetic**: append unchanged, key is **not** updated.

Prints the running key sequence to stdout as a diagnostic.

### `decrypt(input, key)`

```cpp
string decrypt(string input, int key)
```

Same structure as `encrypt` but uses subtraction: `(c - base - currentKey + 26) % 26 + base`. After decryption, `currentKey = toupper(nextChar) - 'A'` feeds the decrypted letter back as the next key.

### `main()`

1. Reads plaintext and initial key.  
2. Reduces key modulo 26.  
3. Calls `encrypt` then `decrypt` and prints results.

---

## Edge Cases

| Scenario                    | Input       | Initial Key | Expected Behavior          |
|-----------------------------|-------------|-------------|----------------------------|
| Key = 0                     | `"abc"`     | 0           | Each letter acts as its own key |
| Single character            | `"a"`       | 5           | Only uses initial key      |
| All non-alphabetic          | `"123 !"`   | 5           | Passes through unchanged   |
| Wrap-around (encryption)    | `"zz"`      | 1           | First: a, then uses z as key |
| Key ≥ 26                    | `"hello"`   | 30          | 30 mod 26 = 4, works normally |
| Empty input                 | `""`        | 5           | Returns empty string       |
| Negative key                | `"hello"`   | -1          | Negative modulo → undefined |

---

## FAQ

**Q: How does the autokey cipher differ from the Vigenère cipher?**  
A: Vigenère repeats a fixed keyword. Autokey uses the plaintext itself as the key after the initial seed. This eliminates periodic key patterns.

**Q: What happens if the initial key is 0?**  
A: The first letter maps to itself; subsequent letters are shifted by the previous plaintext letter. The ciphertext will differ from plaintext from position 2 onward.

**Q: Why does `encrypt` not update the key on non-alphabetic characters?**  
A: Non-alphabetic characters (spaces, digits, punctuation) are not part of the plaintext alphabet and are passed through. The key stays the same so that the next alphabetic character uses the correct running key. This is a design choice; some implementations skip non-letters entirely.

**Q: Is the autokey cipher secure?**  
A: More secure than Vigenère since the key length equals the message length (no repetition). However, it is still vulnerable if an attacker knows or guesses part of the plaintext (since that reveals subsequent keys).
