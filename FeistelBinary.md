# Feistel Cipher with Binary Inputs & Binary Keys

> **File:** `FeistelBinary.cpp` | **Type:** Feistel Network (Binary) | **Difficulty:** Medium

---

## Theory

A **Feistel cipher** splits the input into two halves and applies multiple mixing rounds. This variant operates exclusively on **binary strings** (`'0'` and `'1'` characters) and uses **binary round keys**.

### Feistel Round (Binary)

```
Round function:  F(R, K) = R ⊕ K
New right:       R' = L ⊕ F(R, K)
New left:        L' = R
```

XOR on bits: `0⊕0=0`, `0⊕1=1`, `1⊕0=1`, `1⊕1=0`

### Decryption

Keys are applied in **reverse order**; the same Feistel structure recovers the plaintext due to XOR symmetry.

---

## Usage

```
g++ -std=c++11 -o feistelbin FeistelBinary.cpp
./feistelbin
```

### Input Format

1. **Binary plaintext** — string of `0`/`1` (spaces stripped automatically)
2. **Number of rounds** — how many Feistel rounds
3. **Round keys** — one binary string per round

---

## Worked Example

```
Enter Binary Plaintext (e.g. 10101101): 1010 1101
Enter Number of Rounds: 3
Enter Binary Round Keys:
K1: 1101
K2: 1011
K3: 0110

Round 1
Key   : 1101
Left  : 1101
Right : 1010

Round 2
Key   : 1011
Left  : 1010
Right : 1100

Round 3
Key   : 0110
Left  : 1100
Right : 0000

====================
Cipher Text : 11000000

Reverse Round 1
Key   : 0110
Left  : 1010
Right : 1100

Reverse Round 2
Key   : 1011
Left  : 1101
Right : 1010

Reverse Round 3
Key   : 1101
Left  : 1010
Right : 1101

Plain Text  : 10101101
```

### Manual Trace (Round 1)

Initial: `L=1010`, `R=1101`, `K=1101`

```
F(R,K) = 1101 ⊕ 1101 = 0000
R'     = L ⊕ F(R,K) = 1010 ⊕ 0000 = 1010
L'     = R = 1101
```

---

## Edge Cases

| Input | Behaviour |
|-------|-----------|
| Odd length | Padded with leading `'0'` |
| Empty string | Returns empty |
| Single bit | Padded to 2 bits, leading zero stripped on decrypt |
| Key shorter than half | Key is cycled (repeated) to match |
| All-zero key | `F(R,0) = R` — mixing still occurs via Feistel structure |
