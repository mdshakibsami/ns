# Additive Cipher (Caesar Cipher)

## Theory & Formulas

An **Additive Cipher** (also called a **shift cipher** or **Caesar cipher**) is a monoalphabetic substitution cipher where each letter in the plaintext is shifted forward (or backward) by a fixed integer `key`, wrapping around the alphabet.

### Encryption

```
E(c) = (c + key) mod 26
```

Where `c` is the numeric position of the letter (A=0, B=1, ..., Z=25) and `key` is an integer between 0 and 25.

### Decryption

```
D(c) = (c - key + 26) mod 26
```

The `+26` ensures the result stays non-negative before the modulo operation.

### Key Space

| Key type     | Values            | Notes                        |
|-------------|-------------------|------------------------------|
| Numeric     | 0–25 (any mod 26) | 26 possible keys             |
| Alphabetic  | A–Z (→ 0–25)      | Same 26-key space            |

The total keyspace is only **26**, making brute-force trivial.

---

## Worked Example

**Plaintext:** `HELLO`  
**Key:** `3`

**Encryption step-by-step:**

| Letter | c (A=0) | (c + 3) mod 26 | Cipher |
|--------|----------|----------------|--------|
| H      | 7        | 10             | K      |
| E      | 4        | 7              | H      |
| L      | 11       | 14             | O      |
| L      | 11       | 14             | O      |
| O      | 14       | 17             | R      |

**Ciphertext:** `KHOOR`

**Decryption step-by-step:**

| Letter | c (A=0) | (c - 3 + 26) mod 26 | Plain |
|--------|----------|----------------------|-------|
| K      | 10       | 7                    | H     |
| H      | 7        | 4                    | E     |
| O      | 14       | 11                   | L     |
| O      | 14       | 11                   | L     |
| R      | 17       | 14                   | O     |

---

## Code Walkthrough

### `getKey(inputKey)`

```cpp
int getKey(string inputKey)
```

Converts user-provided key to an integer 0–25:
- **Digit string** → `stoi(inputKey) % 26`
- **Single letter** → `toupper(inputKey[0]) - 'A'` (A→0, B→1, …)
- **Otherwise** → prints `"Invalid Key"` and exits.

### `encrypt(text, key)`

```cpp
string encrypt(string text, int key)
```

Iterates each character `c`:
- **If alphabetic**: determine base (`'A'` for upper, `'a'` for lower), compute `((c - base) + key) % 26 + base`.
- **If non-alphabetic**: append unchanged.

Returns the ciphertext string.

### `decrypt(text, key)`

```cpp
string decrypt(string text, int key)
```

Same iteration as `encrypt` but uses `((c - base) - key + 26) % 26 + base`. The `+26` guarantees a non-negative dividend.

### `main()`

1. Reads plaintext and key from stdin.  
2. Calls `getKey` → `encrypt` → `decrypt`.  
3. Prints ciphertext and recovered plaintext.

---

## Edge Cases

| Scenario               | Input          | Key | Expected Behavior          |
|------------------------|----------------|-----|----------------------------|
| Wrap-around            | `"z"`          | 1   | Cipher = `"a"`             |
| Key = 0 (identity)     | `"message"`    | 0   | Cipher = Plain = `"message"` |
| Key ≥ 26               | `"hello"`      | 55  | 55 mod 26 = 3, works normally |
| Non-alphabetic chars   | `"123 !@#"`    | 5   | Passed through unchanged   |
| Mixed case             | `"Hello World"`| 3   | Case preserved             |
| Empty input            | `""`           | 3   | Returns empty string       |
| Invalid key format     | `"hello"`      | `AB`| Exits with error message   |

---

## FAQ

**Q: Why only 26 keys?**  
A: The additive cipher encrypts by shifting positions in a 26-letter alphabet, so only 26 unique shifts exist. This is trivially broken by brute force.

**Q: How is this related to the Caesar cipher?**  
A: The Caesar cipher is an additive cipher with a specific key of 3. This implementation generalises to any key 0–25.

**Q: Why the `+26` in decryption?**  
A: In C++, `%` can yield a negative remainder. Adding 26 ensures the dividend is non-negative, giving a correct positive modulo result.
