# Hill Cipher 2×2: Implementation, Theory & Examples

The **Hill cipher** is a polygraphic substitution cipher based on linear algebra. Invented by **Lester S. Hill** in 1929, it encrypts blocks of letters using matrix multiplication modulo 26. This implementation uses a **2×2 key matrix** — the smallest useful block size.

---

## Table of Contents

- [How It Works](#how-it-works)
- [The Key Matrix & Its Inverse](#the-key-matrix--its-inverse)
- [Worked Example](#worked-example)
- [Code Walkthrough](#code-walkthrough)
- [Edge Cases & Pitfalls](#edge-cases--pitfalls)
- [Frequently Asked Questions](#frequently-asked-questions)

---

## How It Works

Plaintext is split into **digraphs** (2-letter blocks). Each digraph is treated as a column vector and multiplied by the 2×2 key matrix modulo 26.

### Encryption Formula

```
C = K × P  (mod 26)

[ c1 ]   [ k11  k12 ] [ p1 ]
[ c2 ] = [ k21  k22 ] [ p2 ]
```

### Decryption Formula

```
P = K⁻¹ × C  (mod 26)
```

Where `K⁻¹` is the modular inverse of the key matrix (exists only if `det(K)` is invertible modulo 26).

### Key Properties

| Property            | Detail                                         |
| ------------------- | ---------------------------------------------- |
| **Key matrix**      | 2×2 invertible matrix over Z₂₆                |
| **Plaintext unit**  | Digraph (2 letters)                            |
| **Cipher type**     | Symmetric, polygraphic substitution            |
| **Security factor** | Hides single-letter frequency                  |

---

## The Key Matrix & Its Inverse

Our implementation uses:

**Key matrix (K)**:
```
[ 3  3 ]
[ 2  5 ]
```

**Determinant**: `(3×5 − 3×2) = 15 − 6 = 9`

**Modular inverse of 9 mod 26**: `9 × 3 = 27 ≡ 1 (mod 26)` → inverse of 9 is **3**.

**Inverse matrix (K⁻¹)**:
```
det⁻¹ × [ k22  -k12 ]  =  3 × [ 5  -3 ]
         [ -k21  k11 ]         [ -2  3 ]

= [ 15  -9 ]  mod 26 → [ 15  17 ]
  [ -6   9 ]           [ 20   9 ]
```

This inverse is hardcoded as:
```
int inverseKey[2][2] = { {15,17}, {20,9} };
```

---

## Worked Example

**Plaintext**: `HELP`

### Step 1 — Prepare

No cleaning needed: `HELP` (4 letters, uppercase, even length).

### Step 2 — Convert letters to numbers

```
H=7,  E=4,  L=11,  P=15
```

### Step 3 — Encrypt digraph `HE`

```
c1 = (3×7 + 3×4) mod 26 = (21 + 12) mod 26 = 33 mod 26 = 7  → H
c2 = (2×7 + 5×4) mod 26 = (14 + 20) mod 26 = 34 mod 26 = 8  → I
```

→ `HE` → `HI`

### Step 4 — Encrypt digraph `LP`

```
c1 = (3×11 + 3×15) mod 26 = (33 + 45) mod 26 = 78 mod 26 = 0  → A
c2 = (2×11 + 5×15) mod 26 = (22 + 75) mod 26 = 97 mod 26 = 19 → T
```

→ `LP` → `AT`

**Ciphertext**: `HIAT`

### Step 5 — Decrypt

Decrypt digraph `HI`:

```
p1 = (15×7 + 17×8) mod 26 = (105 + 136) mod 26 = 241 mod 26 = 7  → H
p2 = (20×7 +  9×8) mod 26 = (140 +  72) mod 26 = 212 mod 26 = 4  → E
```

→ `HI` → `HE` ✓

---

## Code Walkthrough

### File: `7-Hill22.cpp`

| Function     | Purpose                                                     |
| ------------ | ----------------------------------------------------------- |
| `prepare()`  | Strip non-alpha, uppercase, pad odd length with `X`         |
| `encrypt()`  | Matrix multiply key × plaintext digraph (mod 26)            |
| `decrypt()`  | Matrix multiply inverse × ciphertext digraph (mod 26), strip trailing `X` |

#### `prepare()` details

```
for(char ch:text)
    if(isalpha(ch))
        result+=toupper(ch);
if(result.length()%2!=0)
    result+='X';
```

- Removes all non-alphabetic characters
- Converts to uppercase
- Appends `X` if the cleaned text has odd length

#### `encrypt()` details

```
int c1=(key[0][0]*p1 + key[0][1]*p2)%26;
int c2=(key[1][0]*p1 + key[1][1]*p2)%26;
```

Perform the matrix multiplication for each digraph.

#### `decrypt()` details

```
while(!plain.empty() && plain.back()=='X')
    plain.pop_back();
```

Strips **trailing** `X` characters after decryption. This removes the padding `X` and any `X` that was genuinely at the end of the original text (ambiguity).

---

## Edge Cases & Pitfalls

| Case                         | Behaviour                                                    |
| ---------------------------- | ------------------------------------------------------------ |
| **Empty string**             | `prepare` returns `""`; encrypt/decrypt loops 0 times        |
| **Non-alphabetic only**      | `prepare` strips all → `""`                                  |
| **Odd-length plaintext**     | Padded with `X` (e.g. `"A"` → `"AX"`)                       |
| **Trailing X in original**   | Stripped by decryption — **data loss** if the original ended with `X` |
| **Non-invertible key**       | Not checked — if det is even or 13, the key has no inverse, and decryption will produce garbage |
| **All identical letters**    | e.g. `AAAA`: each digraph `(0,0)` → `(0,0)` → cipher equals plain |
| **Non-alphabetic in input**  | Stripped; `"Test123Data"` → `"TESTDATA"`                     |

### The Trailing X Ambiguity

The decryption function strips ALL trailing `X` characters. If plaintext `HELL` is encrypted (no padding), the result decrypts to `HELL`. But if plaintext `HELLX` is encrypted, it also decrypts to `HELL` — the original `X` is lost. This is a design trade-off; a more robust implementation would track padding positions.

---

## Frequently Asked Questions

**Q: How do I find the 2×2 key matrix?**
A: Any 2×2 matrix with determinant coprime to 26 (odd and not 13) works. Common values: det = 1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25.

**Q: How do I compute the modular inverse of a 2×2 matrix?**
A: Compute det = (a×d − b×c) mod 26. Find `det⁻¹` such that `det × det⁻¹ ≡ 1 (mod 26)` using extended Euclidean algorithm. Then:

```
K⁻¹ = det⁻¹ × [ d  -b ]
               [ -c  a ]
```

**Q: Is Hill cipher secure with a 2×2 matrix?**
A: No. A 2×2 Hill cipher is trivial to break with known-plaintext attacks. Only two digraphs (4 letters) reveal the key.

**Q: What about larger block sizes (3×3, 4×4)?**
A: Larger matrices increase security but require more ciphertext to break. Hill's original paper described any block size; 3×3 and 4×4 were common.

**Q: Can I encrypt non-alphabetic characters?**
A: No. The implementation strips everything except A–Z.

**Q: Why does decryption strip trailing X?**
A: To remove padding added by `prepare()`. But it also removes genuine trailing X — see [edge cases](#the-trailing-x-ambiguity).

**Q: What happens if the key is not invertible?**
A: Decryption will produce wrong plaintext because `K⁻¹ × K ≠ I`. The code does not validate invertibility.

---

*For the full source code, see [`7-Hill22.cpp`](7-Hill22.cpp).*
