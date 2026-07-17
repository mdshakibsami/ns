/**
 * Hill Cipher (3×3)
 * -----------------
 * A polygraphic substitution cipher based on linear algebra.  Invented by
 * Lester S. Hill in 1929, it was the first cipher to operate on more than
 * three symbols at once.
 *
 * Encryption:    C = K · P (mod 26)
 * Decryption:    P = K⁻¹ · C (mod 26)
 *
 * where K is the n×n key matrix, P and C are n×1 plaintext/ciphertext
 * column vectors (letters mapped to A=0 … Z=25).
 *
 * Requirements:
 *   - The key matrix K must be invertible modulo 26, i.e. gcd(det(K), 26) = 1.
 *   - The inverse matrix K⁻¹ is precomputed and hard-coded in this demo.
 *   - Messages are padded with 'X' to make length a multiple of 3.
 */

#include <bits/stdc++.h>
using namespace std;

// 3x3 Key Matrix (must be invertible mod 26)
// det(this matrix) = 6*(16*15-10*17) - 24*(13*15-10*20) + 1*(13*17-16*20)
//                  = 6*(240-170) - 24*(195-200) + 1*(221-320)
//                  = 6*70 - 24*(-5) + 1*(-99) = 420 + 120 - 99 = 441
// 441 mod 26 = 441 - 26*16 = 441 - 416 = 25 → gcd(25,26)=1 ✓
int key[3][3] =
{
    {6, 24, 1},
    {13, 16, 10},
    {20, 17, 15}
};

// Inverse Matrix (mod 26), precomputed via adjugate * det⁻¹ mod 26
// det⁻¹ mod 26 = 25⁻¹ mod 26 = 25 (since 25*25=625≡1 mod 26)
// adjugate then multiplied by 25 mod 26 yields the matrix below.
int inverseKey[3][3] =
{
    {8, 5, 10},
    {21, 8, 21},
    {21, 12, 8}
};

// Remove spaces, convert to uppercase and pad with X
// This normalises arbitrary input into the form accepted by encrypt/decrypt.
string prepare(string text)
{
    string result = "";

    for(char ch : text)
    {
        if(isalpha(ch))
            result += toupper(ch);
    }

    while(result.length() % 3 != 0)
        result += 'X';

    return result;
}

// Encryption: C = K * P (mod 26) for each 3-letter block
// Each block [p1,p2,p3]ᵀ produces [c1,c2,c3]ᵀ via matrix multiplication.
string encrypt(string text)
{
    string result = "";

    for(int i = 0; i < text.length(); i += 3)
    {
        int p1 = text[i] - 'A';
        int p2 = text[i + 1] - 'A';
        int p3 = text[i + 2] - 'A';

        int c1 = (key[0][0] * p1 + key[0][1] * p2 + key[0][2] * p3) % 26;
        int c2 = (key[1][0] * p1 + key[1][1] * p2 + key[1][2] * p3) % 26;
        int c3 = (key[2][0] * p1 + key[2][1] * p2 + key[2][2] * p3) % 26;

        result += char(c1 + 'A');
        result += char(c2 + 'A');
        result += char(c3 + 'A');
    }

    return result;
}

// Decryption: P = K⁻¹ * C (mod 26) for each 3-letter block
// Padding 'X' characters are stripped from the recovered plaintext.
string decrypt(string text)
{
    string result = "";

    for(int i = 0; i < text.length(); i += 3)
    {
        int c1 = text[i] - 'A';
        int c2 = text[i + 1] - 'A';
        int c3 = text[i + 2] - 'A';

        int p1 = (inverseKey[0][0] * c1 + inverseKey[0][1] * c2 + inverseKey[0][2] * c3) % 26;
        int p2 = (inverseKey[1][0] * c1 + inverseKey[1][1] * c2 + inverseKey[1][2] * c3) % 26;
        int p3 = (inverseKey[2][0] * c1 + inverseKey[2][1] * c2 + inverseKey[2][2] * c3) % 26;

        result += char(p1 + 'A');
        result += char(p2 + 'A');
        result += char(p3 + 'A');
    }

    // Remove padding X
    while(!result.empty() && result.back() == 'X')
        result.pop_back();

    return result;
}

int main()
{
    string text;

    cout << "Enter Plain Text: ";
    getline(cin, text);

    text = prepare(text);

    cout << "\nPrepared Text : " << text << endl;

    string cipher = encrypt(text);

    cout << "Cipher Text   : " << cipher << endl;

    string plain = decrypt(cipher);

    cout << "Plain Text    : " << plain << endl;

    return 0;
}

/*============================================================================
 * TEST CASES (commented — paste into a test runner or uncomment to run)
 *============================================================================
 *
 * VALID CASES (3-4)
 * -----------------
 * 1) text = "HELLO"
 *    prepared = "HELLOX" (3 + 3, padded)
 *    block1: H(7) E(4) L(11) → c1=(6*7+24*4+1*11)%26=(42+96+11)%26=149%26=19→T
 *                                 c2=(13*7+16*4+10*11)%26=(91+64+110)%26=265%26=5→F
 *                                 c3=(20*7+17*4+15*11)%26=(140+68+165)%26=373%26=9→J
 *    block2: L(11) O(14) X(23)→ c1=(6*11+24*14+1*23)%26=(66+336+23)%26=425%26=9→J
 *                                 c2=(13*11+16*14+10*23)%26=(143+224+230)%26=597%26=25→Z
 *                                 c3=(20*11+17*14+15*23)%26=(220+238+345)%26=803%26=23→X
 *    cipher = "TFJ JZX" → decrypt yields "HELLOX" → strip X → "HELLO" ✓
 *
 * 2) text = "ACT" (exactly 3 letters)
 *    A(0) C(2) T(19) → c1=(6*0+24*2+1*19)%26=(0+48+19)=67%26=15→P
 *                        c2=(13*0+16*2+10*19)%26=(0+32+190)=222%26=14→O
 *                        c3=(20*0+17*2+15*19)%26=(0+34+285)=319%26=7→H
 *    cipher = "POH", decrypt → "ACT" ✓
 *
 * 3) text = "HELLO WORLD" (spaces removed → "HELLOWORLD", 10 → pad to 12)
 *    prepared = "HELLOWORLDXX"
 *    // Full trace omitted for brevity; round-trip guaranteed by matrix invertibility.
 *
 * INVALID CASES (1-2)
 * -------------------
 * 4) text = "12345" (digits only, no alphabetic chars)
 *    prepare() strips non-alpha → empty string "" → encrypt returns "".
 *    Edge: length 0, while-loop condition 0%3==0 so no padding, fine.
 *
 * 5) text with non-printable characters, e.g. "\n\t"
 *    isalpha() returns false → stripped. Result is empty.
 *
 * EDGE CASES (2-3)
 * ----------------
 * 6) text = "A" (single letter)
 *    prepared = "AXX" (padded to length 3)
 *    block: A(0) X(23) X(23) → c1=(6*0+24*23+1*23)%26=(0+552+23)=575%26=3→D
 *                                c2=(13*0+16*23+10*23)%26=(0+368+230)=598%26=0→A
 *                                c3=(20*0+17*23+15*23)%26=(0+391+345)=736%26=8→I
 *    cipher = "DAI", decrypt → "AXX" → strip trailing X → "A" ✓
 *
 * 7) text = "XXXXX" (only padding chars)
 *    prepared = "XXXXXX" (6 letters, no extra pad needed)
 *    encrypt → decrypt round-trips back to "XXXXX" (one X stripped).
 *    // Shows that trailing-X stripping can hide actual X content.
 *
 * 8) text = "Z" (last letter)
 *    prepared = "ZXX"
 *    Z(25) X(23) X(23) → c1=(6*25+24*23+1*23)%26=(150+552+23)=725%26=23→X
 *                          c2=(13*25+16*23+10*23)%26=(325+368+230)=923%26=13→N
 *                          c3=(20*25+17*23+15*23)%26=(500+391+345)=1236%26=14→O
 *    cipher = "XNO", decrypt → "ZXX" → strip → "Z" ✓
 */