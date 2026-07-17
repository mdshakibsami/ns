# Keyless (Columnar) Transposition Cipher: Implementation, Theory & Examples

The **keyless columnar transposition cipher** rearranges the characters of a message by writing them into a grid row-wise and reading them column-wise. Unlike substitution ciphers (which replace letters), transposition ciphers **reorder** them — the letters themselves remain unchanged.

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

1. Remove all spaces from the plaintext.
2. Decide the number of columns (supplied by user).
3. Compute the number of rows: `ceil(length / columns)`.
4. Pad the text with `X` so the grid is completely filled.
5. **Encrypt**: write left-to-right (row-wise), then read top-to-bottom (column-wise).
6. **Decrypt**: write top-to-bottom (column-wise), then read left-to-right (row-wise). Strip trailing `X` padding.

### Key Properties

| Property            | Detail                                           |
| ------------------- | ------------------------------------------------ |
| **Key**             | Number of columns (keyless = no keyword)         |
| **Plaintext unit**  | Entire message                                   |
| **Cipher type**     | Symmetric, transposition                        |
| **Security**        | Low — brute-force columns is trivial             |

---

## Encryption & Decryption

### Encryption (Write Row-wise, Read Column-wise)

```
Plaintext:  HELLO
Columns:    3

Grid (2 rows × 3 cols):
  H  E  L
  L  O  X

Cipher: H L E O L X
         ↓ ↓ ↓ ↓ ↓ ↓
         col0 col1 col2
```

### Decryption (Write Column-wise, Read Row-wise)

```
Cipher:  H L E O L X
Columns: 3

Grid (2 rows × 3 cols):
  H  E  L
  L  O  X

Plaintext after stripping trailing X: HELLO
```

---

## Worked Example

**Plaintext**: `TRANSPOSITION`
**Columns**: `4`

### Step 1 — Prepare

No spaces to remove. Length = 14.

### Step 2 — Compute dimensions

```
rows = ceil(14 / 4) = ceil(3.5) = 4
```

### Step 3 — Pad

Grid needs `4 × 4 = 16` cells. Pad with `14 → 16`: `TRANSPOSITIONXX`

### Step 4 — Fill row-wise

```
T  R  A  N
S  P  O  S
I  T  I  O
N  X  X  X
```

### Step 5 — Read column-wise

```
Col 0: T  S  I  N  → TSIN
Col 1: R  P  T  X  → RPTX
Col 2: A  O  I  X  → AOIX
Col 3: N  S  O  X  → NOSX
```

**Ciphertext**: `TSINRPTXAOIXNOSX`

### Step 6 — Decrypt

Write `TSINRPTXAOIXNOSX` column-wise into a 4×4 grid, read row-wise → `TRANSPOSITIONXX`, strip trailing `X` → `TRANSPOSITION` ✓

---

## Code Walkthrough

### File: `8-keylessTransposition.cpp`

| Function    | Purpose                                              |
| ----------- | ---------------------------------------------------- |
| `prepare()` | Remove all spaces                                    |
| `encrypt()` | Build grid row-wise, read column-wise                |
| `decrypt()` | Build grid column-wise, read row-wise, strip padding |

#### `prepare()` details

Simple: iterate over each character, append if not a space.

```
for(auto ch: text)
    if( ch !=' ') result+=ch;
```

#### `encrypt()` details

```
int row = ceil((text.length()*1.0)/column);
while (text.length()<column*row) text+='X';
```

- Compute row count using `ceil`
- Pad with `X` up to `row × column`

Then a double loop fills `matrix[row][col]` row-wise, and another double loop reads it column-wise.

#### `decrypt()` details

The reverse of encrypt: fill column-wise, read row-wise. Trailing `X` padding is stripped:

```
while (!result.empty() && result.back()=='X') result.pop_back();
```

---

## Edge Cases & Pitfalls

| Case                          | Behaviour                                                    |
| ----------------------------- | ------------------------------------------------------------ |
| **Empty string**              | `prepare` returns `""`; `row=0`; loops produce `""`          |
| **All spaces**                | `prepare` strips everything → `""`                           |
| **Columns = 0**               | `ceil(n/0)` → division by zero → **crash**                    |
| **Columns negative**          | `ceil(n/neg)` → negative row count → loops misbehave → **undefined behaviour** |
| **Columns = 1**               | Row-wise = column-wise → cipher = plain                      |
| **Columns ≥ length**          | Only 1 row → cipher = plain (plus padding if needed)          |
| **Trailing X in original**    | Stripped during decryption — **data loss**                    |

### Missing Input Validation

The code does not check `column <= 0`. A user entering `0` or a negative number causes undefined behaviour (likely crash). Production code should validate:

```
if(column <= 0) { cerr << "Columns must be positive\n"; return 1; }
```

---

## Frequently Asked Questions

**Q: Is transposition secure on its own?**
A: No. Columnar transposition is easily identified by letter frequency analysis (frequencies match plaintext English) and brute-forced over a small number of column values.

**Q: How is this "keyless" if the number of columns is a key?**
A: True columnar transposition uses a keyword-derived column order. This simpler version has no keyword — just the column count — making it even weaker.

**Q: How many columns should I use?**
A: Any value between 2 and half the message length produces noticeable scrambling. Common values: 4, 5, 6.

**Q: Does columnar transposition combine well with other ciphers?**
A: Yes. Historically, transposition was often combined with substitution (e.g., ADFGVX cipher). This is the foundation of modern **product ciphers**.

**Q: Why strip trailing X instead of all padding X's?**
A: Without tracking which positions were padding, stripping only trailing X is a heuristic. If the plaintext ends with X, those characters are lost.

**Q: Can I use this for non-text data?**
A: In principle, any sequence of symbols can be transposed. This implementation works on strings.

**Q: What is the difference between "keyless" and "keyed" transposition?**
A: Keyed transposition assigns each column a permutation order based on a keyword. Keyless reads columns in natural order (0, 1, 2, …). Keyed is significantly harder to cryptanalyse.

---

*For the full source code, see [`8-keylessTransposition.cpp`](8-keylessTransposition.cpp).*
