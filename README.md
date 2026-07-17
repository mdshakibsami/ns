# Cryptography Algorithms in C++

[![Language](https://img.shields.io/badge/language-C%2B%2B-blue)](https://isocpp.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A comprehensive collection of **cryptography algorithms** implemented in **C++**, covering both **classical ciphers** and **modern public-key cryptosystems**. Each implementation includes commented source code, theory documentation, worked examples, edge case analysis, and test cases.

---

## Table of Contents

- [Classical Ciphers (Symmetric)](#classical-ciphers-symmetric)
- [Modern Cryptosystems (Asymmetric)](#modern-cryptosystems-asymmetric)
- [Getting Started](#getting-started)
- [Repository Structure](#repository-structure)
- [Educational Value](#educational-value)

---

## Classical Ciphers (Symmetric)

### Substitution Ciphers

| # | Cipher | Files | Type | Key Space |
|---|--------|-------|------|-----------|
| 1 | **[Additive Cipher](Additive.cpp)** (Caesar Cipher) | [`.cpp`](Additive.cpp) · [`.md`](Additive-Cipher.md) | Monoalphabetic | 26 |
| 2 | **[Multiplicative Cipher](Multiplicative.cpp)** | [`.cpp`](Multiplicative.cpp) · [`.md`](Multiplicative-Cipher.md) | Monoalphabetic | 12 |
| 3 | **[Affine Cipher](Affine.cpp)** | [`.cpp`](Affine.cpp) · [`.md`](Affine-Cipher.md) | Monoalphabetic | 312 |
| 4 | **[Autokey Cipher](Autokey.cpp)** | [`.cpp`](Autokey.cpp) · [`.md`](Autokey-Cipher.md) | Polyalphabetic | Variable |
| 5 | **[Playfair Cipher](Playfair.cpp)** | [`.cpp`](Playfair.cpp) · [`.md`](Playfair-Cipher.md) | Digraph (5×5) | 25 letters |
| 6 | **[Vigenère Cipher](Vigenere.cpp)** | [`.cpp`](Vigenere.cpp) · [`.md`](Vigenere-Cipher.md) | Polyalphabetic | 26ⁿ |

### Polygraphic / Linear Algebra Ciphers

| # | Cipher | Files | Block Size |
|---|--------|-------|------------|
| 7 | **[Hill Cipher 2×2](Hill22.cpp)** | [`.cpp`](Hill22.cpp) · [`.md`](Hill-Cipher-2x2.md) | Digraph |
| 8 | **[Hill Cipher 3×3](Hill33.cpp)** | [`.cpp`](Hill33.cpp) · [`.md`](Hill-Cipher-3x3.md) | Trigraph |
| 9 | **[Hill Cipher (User Input)](HillUserInput.cpp)** | [`.cpp`](HillUserInput.cpp) · [`.md`](HillUserInput.md) | 2×2 or 3×3 |

### Transposition Ciphers

| # | Cipher | Files | Key |
|---|--------|-------|-----|
| 10 | **[Keyless Columnar Transposition](KeylessTransposition.cpp)** | [`.cpp`](KeylessTransposition.cpp) · [`.md`](Keyless-Transposition.md) | Column count |
| 11 | **[Keyed Columnar Transposition](KeyedTrans.cpp)** | [`.cpp`](KeyedTrans.cpp) · [`.md`](Keyed-Transposition.md) | Keyword |
| 12 | **[Hybrid Transposition](Hybrid.cpp)** | [`.cpp`](Hybrid.cpp) · [`.md`](Hybrid-Transposition.md) | Keyless + Keyed |

### Feistel Networks

| # | Cipher | Files | Feature |
|---|--------|-------|---------|
| 13 | **[Feistel Cipher (Single-Key)](Feistel.cpp)** | [`.cpp`](Feistel.cpp) · [`.md`](Feistel-Cipher.md) | Same key per round |
| 14 | **[Feistel Cipher (Multi-Key)](FeistelMulKey.cpp)** | [`.cpp`](FeistelMulKey.cpp) · [`.md`](Feistel-Cipher-MultiKey.md) | Per-round keys |
| 15 | **[Feistel Cipher (Binary)](FeistelBinary.cpp)** | [`.cpp`](FeistelBinary.cpp) · [`.md`](FeistelBinary.md) | Binary strings |

---

## Modern Cryptosystems (Asymmetric)

| # | Algorithm | Files | Security Basis |
|---|-----------|-------|----------------|
| 16 | **[RSA](RSA.cpp)** | [`.cpp`](RSA.cpp) · [`.md`](RSA.md) | Integer factorization |
| 17 | **[DSA (Digital Signature Algorithm)](DSS.cpp)** | [`.cpp`](DSS.cpp) · [`User Input`](DSSUserInput.cpp) · [`.md`](Digital-Signature-DSA.md) | Discrete logarithm |
| 18 | **[Rabin Cryptosystem](Rabin.cpp)** | [`.cpp`](Rabin.cpp) · [`.md`](Rabin-Cryptosystem.md) | Integer factorization |
| 19 | **[Merkle-Hellman Knapsack](Knapsack.cpp)** | [`.cpp`](Knapsack.cpp) · [`.md`](Merkle-Hellman-Knapsack.md) | Subset-sum (NP-complete) |

---

## Getting Started

### Compile and Run

All programs are self-contained and compile with any C++11-compliant compiler:

```bash
g++ -std=c++11 -o additive Additive.cpp
./additive
```

```bash
g++ -std=c++11 -o rsa RSA.cpp
./rsa
```

### Requirements

- C++11 or later
- No external dependencies — everything uses the C++ Standard Library

---

## Repository Structure

```
├── *.cpp                 # C++ implementations
├── *.md                  # Documentation (theory, examples, edge cases, FAQ)
├── README.md             # This file
└── paseo.json            # Project configuration
```

Each `.cpp` file is self-documented with:

- **Algorithm theory** in header comments
- **Test cases** in footer comments (valid, invalid, edge)
- **Clear variable names** and inline explanations

Each `.md` file includes:

- **Theory & formulas**
- **Worked examples** with step-by-step calculations
- **Code walkthrough** of each function
- **Edge case analysis** table
- **FAQ** section

---

## Educational Value

This repository serves as a hands-on cryptography learning resource:

- **Understand the math** behind encryption, decryption, and key generation
- **Trace through worked examples** that match the code logic
- **Study edge cases** — empty inputs, padding, wrap-around, invalid keys
- **Compare classical vs. modern** — from Caesar cipher (26 keys) to RSA (2048-bit keys)
- **See cryptanalysis weaknesses** — each markdown file explains why the cipher is (in)secure

### Topics Covered

- Modular arithmetic and number theory (gcd, mod inverse, Euler's totient)
- Linear algebra in Z₂₆ (Hill cipher matrix operations)
- Feistel network structure (foundation of DES, Blowfish)
- Public-key cryptography (RSA, DSA, Rabin)
- Lattice-based cryptanalysis (Merkle-Hellman break)
- Padding schemes and their pitfalls

---

## License

This project is for **educational purposes**. Use it to learn, teach, and experiment with cryptographic algorithms — not for production security.
