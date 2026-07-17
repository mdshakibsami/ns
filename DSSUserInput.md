# Digital Signature Standard (DSS) with User-Input Parameters

> **File:** `DSSUserInput.cpp` | **Type:** Public-Key Cryptography (Digital Signature) | **Difficulty:** Hard

---

## Theory

The **Digital Signature Algorithm (DSA)** is a FIPS standard for digital signatures based on the discrete logarithm problem. Unlike `DSS.cpp` which uses hardcoded parameters, this version allows the user to supply all cryptographic parameters at runtime.

### Domain Parameters

| Parameter | Description | Example |
|-----------|-------------|---------|
| `p` | Large prime modulus | `23` |
| `q` | Prime divisor of `p-1` | `11` |
| `g` | Generator: `g = h^{(p-1)/q} mod p` | `4` |
| `x` | Private key (`0 < x < q`) | `3` |
| `y` | Public key: `y = g^x mod p` (computed) | `18` |

### Signing

- Choose random `k`: `0 < k < q`
- `r = (g^k mod p) mod q`
- `s = k^{-1} · (H(m) + x·r) mod q`
- Signature: `(r, s)`

### Verification

- `w = s^{-1} mod q`
- `u1 = H(m)·w mod q`
- `u2 = r·w mod q`
- `v = (g^{u1} · y^{u2} mod p) mod q`
- Valid if `v == r`

---

## Usage

```
g++ -std=c++11 -o dssuser DSSUserInput.cpp
./dssuser
```

---

## Worked Example

```
Enter DSS Parameters:
p (prime modulus): 23
q (prime divisor of p-1): 11
g (generator): 4
x (private key): 3
hash (H(m)): 5
k (random number 0<k<q): 2

Computed Public Key y = g^x mod p = 18

Digital Signature
r = 5
s = 10

Verification Result: Signature Valid
```

### Manual Verification

```
k^{-1} mod 11 = 6  (since 2·6 = 12 ≡ 1 mod 11)
r = (4^2 mod 23) mod 11 = 16 mod 11 = 5
s = 6·(5 + 3·5) mod 11 = 6·20 mod 11 = 120 mod 11 = 10

w = 10^{-1} mod 11 = 10  (10·10 = 100 ≡ 1 mod 11)
u1 = 5·10 mod 11 = 50 mod 11 = 6
u2 = 5·10 mod 11 = 6
v = (4^6 · 18^6 mod 23) mod 11
  = (4096 · 34012224 mod 23) mod 11
  = (2 · 2 mod 23) mod 11
  = 4 mod 11 = 5 = r  ✓
```

---

## Edge Cases

| Input | Behaviour |
|-------|-----------|
| `k = 0` | `modInverse(0, q)` returns `-1` → signature invalid |
| `k = 1` | Works: `kInv = 1`, `r = g mod q` |
| `hash = 0` | Works: `u1 = 0`, signature still valid |
| `x = 1` | Weakest private key, but mathematically valid |
| Tampered `r` | Verification detects mismatch → "Signature Invalid" |
