# Hybrid Transposition Cipher

> **File:** `10-Hybrid.cpp` | **Type:** Classical Cipher (Transposition) | **Difficulty:** Medium

---

## Theory

A **Hybrid Transposition Cipher** combines two transposition stages for stronger diffusion:

1. **Keyless Columnar Transposition** — basic row-wise fill / column-wise read with a fixed column count.
2. **Keyed Columnar Transposition** — column permutation determined by a keyword.

Applying two different transpositions in sequence increases the complexity of the resulting permutation, making it harder to reverse without knowing both parameters.

**Encryption pipeline:**
```
Plaintext → [Keyless] → [Keyed] → Ciphertext
```

**Decryption pipeline:**
```
Ciphertext → [Keyed⁻¹] → [Keyless⁻¹] → Plaintext
```

**Key scheduling** follows the same `getKeyOrder` mechanism as the basic keyed transposition cipher: the keyword's characters are sorted alphabetically, and the original indices of the sorted characters define the column-read order.

### Why "Hybrid"?

Standalone keyed columnar transposition permutes columns once. By first applying a keyless transposition, the characters are already redistributed before the keyed step acts on them. This two-layer permutation is equivalent to a single, more complex permutation of the original character positions.

---

## Worked Example

**Plaintext:** `"HELLOWORLD"` (after space removal)  
**Columns:** `4` (overridden by key length)  
**Key:** `"KEY"` → key length = 3

### Stage 1: Keyless Encryption

`encryptKeyless("HELLOWORLD", 3)`

Rows = ceil(10/3) = 4, pad to 12: `"HELLOWORLDXX"`

**Matrix (row-wise fill):**

|       | Col 0 | Col 1 | Col 2 |
|-------|-------|-------|-------|
| Row 0 | H     | E     | L     |
| Row 1 | L     | O     | W     |
| Row 2 | O     | R     | L     |
| Row 3 | D     | X     | X     |

**Read column-wise:** `Col0: H,L,O,D` + `Col1: E,O,R,X` + `Col2: L,W,L,X`  
**Step1 output:** `"HLOD EORX LWLX"` → `"HLODEORXLWLX"`

### Stage 2: Keyed Encryption

`encryptKeyed("HLODEORXLWLX", "KEY")`

Columns = 3, rows = 12/3 = 4

**Matrix (row-wise fill):**

|       | Col 0 | Col 1 | Col 2 |
|-------|-------|-------|-------|
| Row 0 | H     | L     | O     |
| Row 1 | D     | E     | O     |
| Row 2 | R     | X     | L     |
| Row 3 | W     | L     | X     |

**Key order** of `"KEY"` → sorted `E, K, Y` → indices `[1, 0, 2]`

**Read columns in order [1, 0, 2]:**
- Col 1: `L, E, X, L` → `"LEX L"`
- Col 0: `H, D, R, W` → `"HDRW"`
- Col 2: `O, O, L, X` → `"OOLX"`

**Final Ciphertext:** `"LEXLHDRW OOLX"` → `"LEXLHDRWOO LX"` → `"LEXLHDRWOO LX"` → `"LEXLHDRWOO LX"`

(Spaces added for readability: `"LEXL HDRW OOLX"`)

**Decryption** reverses both stages (keyed first, then keyless) → `"HELLOWORLD"`.

---

## Code Walkthrough

### 1. `getKeyOrder(key)` — Key Scheduling
Same as `9-keyedTrans.cpp`: pairs `(uppercase-char, index)`, sorts by character, extracts indices. Defines the column permutation for the keyed stage.

### 2. `encryptKeyless(text, column)` — Stage 1 Encryption
- Computes `rows = ceil(len / columns)` and pads with `'X'`.
- Fills matrix row-wise, reads column-wise left-to-right.
- Returns the transposed string.

### 3. `decryptKeyless(text, column)` — Stage 1 Decryption
- Fills matrix column-wise, reads row-wise.
- Strips trailing `'X'` padding.

### 4. `encryptKeyed(text, key)` — Stage 2 Encryption
- Text must already be a multiple of key length (guaranteed because input comes from keyless output which is a multiple of columns = key length).
- Fills matrix row-wise, reads column-wise by key-determined order.
- No padding needed (text length is guaranteed multiple).

### 5. `decryptKeyed(text, key)` — Stage 2 Decryption
- Fills matrix column-wise by key-determined order, reads row-wise.

### 6. `hybridEncrypt(text, columns, key)` / `hybridDecrypt(text, columns, key)` — Orchestrators
- Override `columns` with `key.length()` (user-provided column count is unused).
- Chain the two stages sequentially.
- Print intermediate results for educational visibility.

**Note:** The `columns` parameter in `main()` is read but never actually used — `hybridEncrypt` immediately overwrites it with `key.length()`.

---

## Edge Cases

| Case | Input | Behavior |
|------|-------|----------|
| Empty plaintext | `text=""` | Both stages receive empty strings, returns `""` |
| Empty key | `key=""` | `getKeyOrder` returns empty vector → keyed stage produces `""`, downstream crashes |
| Single character | `text="A", key="KEY"` | Keyless: 1×3 grid → `"AXX"`; Keyed: 1×3 grid → permuted but single row = same output |
| Key length = 1 | `key="A"` | `getKeyOrder` returns `[0]` → no permutation; hybrid reduces to keyless only |
| Spaces in input | `text="HELLO WORLD"` | `prepare()` strips all spaces before encryption |

---

## FAQ

**Q: Why does `hybridEncrypt` ignore the user-provided column count?**  
A: The keyed stage forces `columns = key.length()` for compatibility. The initial `column` parameter is effectively dead code. Future versions could support different column counts per stage.

**Q: Is this more secure than single-stage transposition?**  
A: Marginally — two transpositions in sequence create a composite permutation that is harder to reverse than a single one, but still easily broken by known-plaintext attacks.

**Q: Can the two stages use different column counts?**  
A: Not in this implementation. Both stages are locked to `key.length()`. Modifying `hybridEncrypt` to accept two separate column parameters would add flexibility.

**Q: Why are intermediate results printed?**  
A: For educational purposes — you can see how each stage transforms the data.

**Q: What if plaintext, after removing spaces, is shorter than the key?**  
A: The keyless stage pads to fill the grid. If shorter than key length, you get a single row. The keyed stage then permutes a single row (no-op, since one row has no column variation).
