# Feistel Cipher (Multiple Round Keys)

> **File:** `11-FeistelMulKey.cpp` | **Type:** Classical Cipher (Feistel Network) | **Difficulty:** Medium

---

## Theory

The **Feistel Cipher with Multiple Round Keys** extends the single-key Feistel cipher (`11-Feistel.cpp`) by using a **different integer key for each round**. This adds per-round variation, making the cipher slightly stronger and more representative of real-world Feistel designs (e.g., DES uses 16 different 48-bit round keys derived from a 56-bit master key).

### Encryption (per round i)

```
Lᵢ₊₁ = Rᵢ
Rᵢ₊₁ = Lᵢ ⊕ F(Rᵢ, keys[i])
```

### Decryption (keys applied in reverse)

```
Rᵢ = Lᵢ₊₁
Lᵢ = Rᵢ₊₁ ⊕ F(Lᵢ₊₁, keys[i])
```

Keys are traversed **forward** (0 to N-1) during encryption and **backward** (N-1 to 0) during decryption.

### Round Function

`F(right, key) = right XOR key` — each character of the right half is XORed with the integer key (lower 8 bits only).

### This Implementation

- Accepts a `vector<int>` of round keys.
- Prints intermediate state (left/right halves) after each round for educational visibility.
- Padding: odd-length plaintext gets an `'X'` appended.

---

## Worked Example

**Plaintext:** `"HELLO"`  
**Rounds:** `3`  
**Keys:** `[3, 7, 15]`

**Step 1:** Length = 5 (odd) → pad to `"HELLOX"`.  
`mid = 3`, `L = "HEL"`, `R = "LOX"`

**Round 1 (key=3):**
- `F("LOX", 3)`:
  - `'L' ^ 3 = 76 ^ 3 = 79` → `'O'`
  - `'O' ^ 3 = 79 ^ 3 = 76` → `'L'`
  - `'X' ^ 3 = 88 ^ 3 = 91` → `'['`
- `new_R = "HEL" XOR "OL["` = `(H^O) (E^L) (L^[)` = non-printable characters
- `new_L = "LOX"`
- **Output:** `L="LOX"`, `R=<non-printable>`

**Round 2 (key=7):**
- Applies same structure with key=7 on new halves.

**Round 3 (key=15):**
- Final round.

**Final ciphertext** = `left + right` (non-printable bytes).

**Decryption** with keys `[15, 7, 3]` (reversed) recovers `"HELLO"`.

---

## Code Walkthrough

### 1. `F(right, key)` — Round Function
Character-by-character XOR of `right` with `key`. The `int` key is truncated to 8 bits when cast to `char`.

### 2. `XOR(a, b)` — String XOR
Standard character-by-character XOR of two equal-length strings.

### 3. `encrypt(text, keys)` — Encryption
- Pads odd-length input with `'X'`.
- Splits into left/right halves.
- Iterates `keys` in order: `new_R = L XOR F(R, keys[i])`, `new_L = R` (swap).
- Prints each round's state for debugging/education.

### 4. `decrypt(text, keys)` — Decryption
- Splits ciphertext.
- Iterates `keys` in **reverse** order: `new_L = R XOR F(L, keys[i])`, `new_R = L` (swap).
- Prints reverse-round state.
- Strips trailing `'X'` padding.

### 5. `main()` — Driver
- Prompts for plaintext, round count, and each round key.
- Calls `encrypt`, then `decrypt`, displaying intermediate and final results.

---

## Differences from Single-Key Feistel (`11-Feistel.cpp`)

| Feature | Single-Key (11) | Multi-Key (12) |
|---------|----------------|----------------|
| Key parameter | Single `int` | `vector<int>` (per-round) |
| Key schedule | Same key every round | User provides each round key |
| Round iteration | `for(int i=0; i<rounds; i++)` | `for(int i=0; i<keys.size(); i++)` |
| Decryption loop | `for(int i=0; i<rounds; i++)` | `for(int i=keys.size()-1; i>=0; i--)` |
| Round state output | No | Yes (each round prints L/R) |
| Round function signature | `F(int key, string right)` | `F(string right, int key)` |

---

## Edge Cases

| Case | Input | Behavior |
|------|-------|----------|
| Empty plaintext | `text=""` | `mid=0`, empty halves, returns `""` |
| Zero rounds | `keys=[]` | Loop doesn't execute, returns input unchanged |
| Single character | `text="A"` | Padded to `"AX"`, works normally |
| Odd length | `text="ODD"` | Padded to `"ODDX"`, decryption strips padding → `"ODD"` |
| All keys = 0 | `keys=[0,0,0]` | `F(R,0)=R`; Feistel structure still provides mixing via `XOR(L,R)` |
| All keys identical | `keys=[7,7,7]` | Equivalent to single-key Feistel with key=7, rounds=3 |
| Key > 255 | `keys=[300]` | Truncated to `300 % 256 = 44` via char cast |
| Key array mismatch | User enters fewer keys than intended | Vector size determines round count; fewer keys = fewer rounds |

---

## FAQ

**Q: Does using multiple round keys improve security?**  
A: Slightly — different round keys prevent slide attacks and add variability. However, since the round function is still `XOR(key, right)`, the cipher is trivially breakable.

**Q: Why iterate `keys.size()` instead of a separate `rounds` variable?**  
A: The vector size IS the round count. This avoids a mismatch between `rounds` and `keys.size()`.

**Q: Why does decryption iterate keys in reverse?**  
A: Feistel decryption applies round keys in the opposite order of encryption. The last encryption round must be undone first.

**Q: What are the non-printable characters in the output?**  
A: XOR operations frequently produce byte values outside the printable ASCII range (32–126). The underlying data is preserved correctly; `cout` may render them as garbage, but decryption still works.

**Q: Can I encrypt binary data?**  
A: In principle yes, but the code uses `string` which may contain null bytes. `getline` will stop at nulls. For binary data, use `vector<byte>` or `char[]`.
