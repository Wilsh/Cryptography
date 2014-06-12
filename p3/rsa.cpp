/*******************************************************************************
* Course:      CSC555
* Assignment:  #3
* Author:      Andrew Seligman
* Date:        April 30, 2014
* File:        rsa.cpp
* Purpose:     This program simulates the encryption and decryption processes
*               of RSA. After generating p, q, n, phi(n), and d, the user is
*               asked for a numerical value that is then encrypted and
*               decrypted.
*******************************************************************************/

#include <iostream>
#include <openssl/bn.h>

using namespace std;

bool generatePrimes(BIGNUM*, BIGNUM*);
bool calculateMod(BIGNUM*, BIGNUM*, BIGNUM*, BIGNUM*);
bool getInverse(BIGNUM*, BIGNUM*, BIGNUM*);

int main()
{
    BIGNUM *p, *q, *n, *modulus, *pubKey, *privKey, *msg, *ciphertext, *decrypt;
    p = BN_new();
    q = BN_new();
    n = BN_new();
    modulus = BN_new();
    pubKey = BN_new();
    privKey = BN_new();
    msg = BN_new();
    ciphertext = BN_new();
    decrypt = BN_new();
    BN_CTX* store = BN_CTX_new();
    unsigned long int e = 65537L;
    unsigned long int message;
    
    if(!BN_set_word(pubKey, e))
    {
        cout << "Error setting the public key" << endl;
        return -1;
    }
    
    if(!generatePrimes(p, q))
        return -1;
   
    if(!calculateMod(p, q, n, modulus))
        return -1;

    cout << "Public Key:\n" << BN_bn2dec(pubKey) << "\n\n\n";
    
    if(!getInverse(pubKey, modulus, privKey))
        return -1;
        
    cout << "Private Key:\n" << BN_bn2dec(privKey) << "\n\n\n";
    cout << "Enter a message: ";
    cin >> message;
    
    if(!BN_set_word(msg, message))
    {
        cout << "Error converting message to BIGNUM" << endl;
        return -1;
    }

    if(!BN_mod_exp(ciphertext, msg, pubKey, n, store))
    {
        cout << "Error generating ciphertext" << endl;
        return -1;
    }
    
    cout << "\nCiphertext:\n" << BN_bn2dec(ciphertext) << "\n\n";
    
    if(!BN_mod_exp(decrypt, ciphertext, privKey, n, store))
    {
        cout << "Error generating plaintext" << endl;
        return -1;
    }
    
    cout << "Decrypt:\n" << BN_bn2dec(decrypt) << endl;
    
    BN_clear_free(p);
    BN_clear_free(q);
    BN_clear_free(n);
    BN_clear_free(modulus);
    BN_clear_free(pubKey);
    BN_clear_free(privKey);
    BN_clear_free(msg);
    BN_clear_free(ciphertext);
    BN_clear_free(decrypt);
    BN_CTX_free(store);
    return 0;
}

/*******************************************************************************
* Function name:  generatePrimes
*                                                                             
* Description:    Generate two 512-bit prime numbers. Returns false if an error
*                   occurs while generating either number.
*                                                                             
* Parameters:     BIGNUM* p - EXPORT - 512-bit prime number
*				  BIGNUM* q - EXPORT - 512-bit prime number
*
* Return Value:   true or false
*******************************************************************************/
bool generatePrimes(BIGNUM* p, BIGNUM* q)
{
    if(BN_generate_prime_ex(p, 512, false, NULL, NULL, NULL))
        cout << "Generating p:\n" << BN_bn2dec(p) << "\n\n\n";
    else
    {
        cout << "Error generating p" << endl;
        return false;
    }
        
    if(BN_generate_prime_ex(q, 512, false, NULL, NULL, NULL))
        cout << "Generating q:\n" << BN_bn2dec(q) << "\n\n\n";
    else
    {
        cout << "Error generating q" << endl;
        return false;
    }
    
    return true;
}

/*******************************************************************************
* Function name:  calculateMod
*                                                                             
* Description:    Calculate n and phi(n) from primes p and q. Returns false if
*                   any of the BN function calls fail.
*                                                                             
* Parameters:     BIGNUM* p       - IMPORT - 512-bit prime number
*				  BIGNUM* q       - IMPORT - 512-bit prime number
*                 BIGNUM* n       - EXPORT - p * q
*				  BIGNUM* modulus - EXPORT - phi(n)
*
* Return Value:   true or false
*******************************************************************************/
bool calculateMod(BIGNUM* p, BIGNUM* q, BIGNUM* n, BIGNUM* modulus)
{
    BIGNUM* temp1 = BN_new();
    BIGNUM* temp2 = BN_new();
    BIGNUM* oneBN = BN_new();
    BN_CTX* store = BN_CTX_new();
    BN_one(oneBN);
    
    if(!BN_sub(temp1, p, oneBN))
    {
        cout << "Error calculating p - 1" << endl;
        return false;
    }
    
    if(!BN_sub(temp2, q, oneBN))
    {
        cout << "Error calculating q - 1" << endl;
        return false;
    }
    
    if(!BN_mul(modulus, temp1, temp2, store))
    {
        cout << "Error calculating phi(n)" << endl;
        return false;
    }
        
    if(!BN_mul(n, p, q, store))
    {
        cout << "Error calculating n" << endl;
        return false;
    }

    cout << "Common modulus:\n" << BN_bn2dec(modulus) << "\n\n\n";
    
    BN_clear_free(temp1);
    BN_clear_free(temp2);
    BN_CTX_free(store);
    return true;
}

/*******************************************************************************
* Function name:  getInverse
*                                                                             
* Description:    Calculate the modular inverse of e and phi(n). Returns false
*                   if any of the BN function calls fail.
*                                                                             
* Parameters:     BIGNUM* pubKey  - IMPORT - e from public key
*				  BIGNUM* phiOfN  - IMPORT - phi(n)
*                 BIGNUM* inverse - EXPORT - the modular inverse
*                                                                             
* Return Value:   true or false
*******************************************************************************/
bool getInverse(BIGNUM* pubKey, BIGNUM* phiOfN, BIGNUM* inverse)
{
    BIGNUM* a = BN_new();
    BIGNUM* b = BN_new();
    BIGNUM* x = BN_new();
    BIGNUM* u = BN_new();
    BIGNUM* m = BN_new();
    BIGNUM* q = BN_new();
    BIGNUM* r = BN_new();
    BIGNUM* temp1 = BN_new();
    BN_CTX* store = BN_CTX_new();
    
    BN_copy(a, pubKey);
    BN_copy(b, phiOfN);
    BN_one(u);
    BN_zero(x);

    //extended Euclidean algorithm
    while(!BN_is_zero(a))
    {
        //q = b / a
        //r = b % a
        if(!BN_div(q, r, b, a, store))
        {
            cout << "Error calculating gcd: BN_div(q, r, b, a, store)" << endl;
            return false;
        }
        
        //m = x - u * q
        if(!BN_mul(temp1, u, q, store))
        {
            cout << "Error calculating gcd: BN_mul(temp1, u, q, store)" << endl;
            return false;
        }
        if(!BN_sub(m, x, temp1))
        {
            cout << "Error calculating gcd: BN_sub(m, x, temp1)" << endl;
            return false;
        }

        BN_copy(b, a);
        BN_copy(a, r);
        BN_copy(x, u);
        BN_copy(u, m);
    }

    if(!BN_is_one(b))
    {
        cout << "Modular inverse does not exist" << endl;
        return false;
    }
    
    //calculate modular inverse
    if(!BN_nnmod(inverse, x, m, store))
    {
        cout << "Error calculating nnmod" << endl;
        return false;
    }
    
    BN_clear_free(x);
    BN_clear_free(u);
    BN_clear_free(m);
    BN_clear_free(q);
    BN_clear_free(r);
    BN_clear_free(temp1);
    BN_CTX_free(store);
    return true;
}
