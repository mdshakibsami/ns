#include<bits/stdc++.h>
using namespace std;

int findPrivateKey(int e, int phi)
{
    for(int d=1;d<phi;d++)
    {
        if((d*e)%phi == 1) return d;
    }
    return -1;
}


long long powerMod (long long base, long long exp, long long mod)
{
    long long result = 1;

    while (exp>0)
    {
        result = (result*base) %mod;
        exp--;
    }
    
    return result;
}
int main()
{
    int p,q,e,d,message;
    cout << "Enter two prime numbers: ";

    cin>>p>>q;

    int n = p*q;
    int phi = (p-1)*(q-1);

    cout<<"Enter public key e: ";
    cin>>e;

    if(gcd(e,phi)!=1)
    {
        cout<<"Invalid Public Key";
        return 0;
    }

    d = findPrivateKey(e,phi);

    if(d==-1)
    {
        cout<<"Private Key can't be generated";
        return 0;
    }

    cout << "\nPublic Key : (" << e << "," << n << ")";
    cout << "\nPrivate Key: (" << d << "," << n << ")";

      cout << "\n\nEnter message(number): ";
    cin>>message;
    long long cipher = powerMod(message,e,n);
    long long plain = powerMod(cipher,d,n);

    cout << "Cipher Text: " << cipher << endl;

    cout << "Plain Text : " << plain << endl;

}