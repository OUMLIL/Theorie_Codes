//
//  TP7_Shamir's Secret Sharing Scheme
//  

#include <stdio.h>
#include <iostream>
#include <gmp.h>
#include <cmath>
#include "main.hpp"

#define BITSTRENGTH  14              /* size of prime number (p) in bits */
#define DEBUG true

gmp_randstate_t state;

void generatePrime(mpz_t &p) {
    mpz_init(p);
    mpz_t p_tmp; mpz_init(p_tmp);
    mpz_urandomb(p_tmp, state, BITSTRENGTH);
    mpz_nextprime(p, p_tmp);
}

void generateRandom(mpz_t &s, mpz_t p) {
    mpz_init(s);
    mpz_t s_tmp; mpz_init(s_tmp);
    mpz_urandomb(s_tmp, state, BITSTRENGTH);
    mpz_mod(s, s_tmp, p);
}

void generateCoefs(mpz_t *tab, mpz_t p, int k) {
    init_tab_mpz(tab, k);
    for(int i = 0; i < k; i++){
        generateRandom(tab[i], p);
    }
}
/*
bool check_coefs_different(mpz_t *tab, int taille, mpz_t e) {
    bool res = false;
    for(int i=0; i < taille; ++i) {
        if(mpz_cmp(tab[i], e) == 0) res = true;
    }
    return res;
}
*/

void compute_image(mpz_t &image, mpz_t *a, mpz_t x, int k, mpz_t s) {
    mpz_set(image, s);
    mpz_t tmp; mpz_init(tmp);
    for(int j=0; j < k; j++) {
        //image += mpz_get_ui(a[j])*pow(mpz_get_ui(x),j+1);
        //std::cout << "a" << j << ": " << mpz_get_ui(a[j]) << " x^" << j << "= " << pow(mpz_get_ui(x),j) << std::endl;
        mpz_pow_ui(tmp, x, j+1);
        mpz_mul(tmp, a[j], tmp);
        mpz_add(image, image, tmp);
    }
}

void computeShares(mpz_t *x, mpz_t *y, mpz_t *a, mpz_t s, int k, int n) {
    init_tab_mpz(x, n);
    init_tab_mpz(y, n);
    for(int i = 0; i < n; ++i) {
        mpz_t image; mpz_init(image);
        mpz_set_ui(x[i], i+1);
        compute_image(image, a, x[i], k, s);
        mpz_set(y[i], image);
    }
}


void init_tab_mpz(mpz_t * tab, int t) {
    for(int i = 0; i < t; ++i) {
        mpz_init(tab[i]);
    }
}

void clear_tab_mpz(mpz_t * tab, int t) {
    for(int i = 0; i < t; ++i) {
        mpz_clear(tab[i]);
    }
}

void computeLagrange(mpz_t *alpha, mpz_t * x, int k, int n) {
    init_tab_mpz(alpha, k);
    mpz_t prod; mpz_init(prod);
    mpz_t tmp; mpz_init(tmp);
    for(int i = 0; i < k; i++) {
        mpz_set_str(prod, "1", 0);
        for(int j = 0; j < n; j++) {
            if(i != j) {
                mpz_sub(tmp, x[j], x[i]);
                mpz_div(tmp, x[j], tmp);
                mpz_mul(prod, prod, tmp);
            }
        }
        mpz_set(alpha[i], prod);
        char p_str[1000]; mpz_get_str(p_str,10,alpha[i]);
        std::cout << "alpha" << i << " = " << p_str << std::endl;
    }
}

void reconstructSecret(mpz_t * alpha, mpz_t *y, mpz_t Sr, mpz_t p, int k) {
    mpz_init(Sr); mpz_init_set_str(Sr, "0", 0);
    mpz_t temp; mpz_init(temp);
    for(int i = 0; i < k; i++) {
        mpz_mul(temp,alpha[i],y[i]);
        mpz_add(Sr, Sr, temp);
    }
    mpz_mod(Sr, Sr, p );
}

/* Main subroutine */
int main()
{
    /* Declare variables */
    int n = 4;          // Numbers of users (max)
    int k = 3;          // Threshold : minimal number of users => secret

    mpz_t p;            // Prime number
    mpz_t S;            // Secret
    mpz_t Sr;           // Reconstruction of the Secret

    mpz_t  a[k-1];       // Coefficients of polynom
    mpz_t alpha[k];  // Lagrangian polynomials in zero

    mpz_t x[n];  // Login users
    mpz_t y[n];  // Shares of users
    //initialize seed
    gmp_randinit_mt(state); gmp_randseed_ui(state, time(NULL));

    /* This function creates the shares computation. The basic algorithm is...
    *
    *  1. Initialize Prime Number : we work into Z/pZ
    *  2. Initialize Secret Number : S
    *  3. Compute a random polynom of order k-1
    *  4. Shares computation for each users (xi, yi) for i in [1,n]
    *  5. Reconstruct the secret with k users or more
    *
    */
    
    /*
     *  Step 1: Initialize Prime Number : we work into Z/pZ
     */

    //mpz_init(p); mpz_init_set_str(p, "11", 0);
    
    //TODO: Delete this part and compute a prime number randomly
    generatePrime(p);
    
    if (DEBUG)
    {
        char p_str[1000]; mpz_get_str(p_str,10,p);
        std::cout << "Random Prime 'p' = " << p_str <<  std::endl;
    }

    /*
     *  Step 2: Initialize Secret Number
     */

    //mpz_init(S); mpz_init_set_str(S, "5", 0);
    
    //TODO: Delete this part and compute the secret randomly ( warning: inside Z/pZ )
    generateRandom(S, p);
    
    if (DEBUG)
    {
        char S_str[1000]; mpz_get_str(S_str,10,S);
        std::cout << "Secret number 'S' = " << S_str <<  std::endl;
    }

    /*
     *  Step 3: Initialize Coefficient of polynom
     */
    //mpz_init(a1); generateRandom(a1, p);//mpz_init_set_str(a1, "3", 0);
    //mpz_init(a2); generateRandom(a2, p);//mpz_init_set_str(a2, "10", 0);
    //TODO: Delete this part and compute the coeffiecients randomly ( warning: inside Z/pZ )//done 
    generateCoefs(a, p, k-1);
    
    if (DEBUG)
    {
        char a1_str[1000]; mpz_get_str(a1_str,10,a[0]);
        char a2_str[1000]; mpz_get_str(a2_str,10,a[1]);
        char S_str[1000];  mpz_get_str(S_str,10,S);
        std::cout << "Polynom 'P(X)' = " << a2_str << "X^2 + " << a1_str << "X + " << S_str << std::endl;
    }
    
    /*
     *  Step 4: Shares computation for each users (xi, yi)
     */
    /*
    mpz_init(x1); mpz_init_set_str(x1, "2", 0);
    mpz_init(x2); mpz_init_set_str(x2, "4", 0);
    mpz_init(x3); mpz_init_set_str(x3, "6", 0);
    mpz_init(x4); mpz_init_set_str(x4, "8", 0);

    mpz_init(y1); mpz_init_set_str(y1, "7", 0);
    mpz_init(y2); mpz_init_set_str(y2, "1", 0);
    mpz_init(y3); mpz_init_set_str(y3, "9", 0);
    mpz_init(y4); mpz_init_set_str(y4, "9", 0);
    */
    //TODO: Delete this part and compute the shares of all users with public login
    computeShares(x, y, a, S, k-1, n);

    
    if (DEBUG)
    {
        char x1_str[1000]; mpz_get_str(x1_str,10,x[0]);
        char x2_str[1000]; mpz_get_str(x2_str,10,x[1]);
        char x3_str[1000]; mpz_get_str(x3_str,10,x[2]);
        char x4_str[1000]; mpz_get_str(x4_str,10,x[3]);

        char y1_str[1000]; mpz_get_str(y1_str,10,y[0]);
        char y2_str[1000]; mpz_get_str(y2_str,10,y[1]);
        char y3_str[1000]; mpz_get_str(y3_str,10,y[2]);
        char y4_str[1000]; mpz_get_str(y4_str,10,y[3]);
        
        std::cout << "Login and share of each users : " << "( x1="<< x1_str << " ; y1=" << y1_str << " ) , "  << "( x2="<< x2_str << " ; y2=" << y2_str << " ) , "  << "( x3="<< x3_str << " ; y3=" << y3_str << " ) , "  << "( x4="<< x4_str << " , y4=" << y4_str << " )" << std::endl;
    }

    /*
     *  Step 5: Sample for reconstruct the secret with 3 users (x1, x2, x3)
     */
    /*
    mpz_init(alpha1); mpz_init_set_str(alpha1, "3", 0);
    mpz_init(alpha2); mpz_init_set_str(alpha2, "8", 0);
    mpz_init(alpha3); mpz_init_set_str(alpha3, "1", 0);
    */
    //TODO: Delete this part and automatically compute the secret with k or more shares
    computeLagrange(alpha, x, k, n);
    // Compute Secret = sum_{i=1}^{k} alpha_i x y_i
    /*
    mpz_init(Sr); mpz_init_set_str(Sr, "0", 0);
    mpz_t temp; mpz_init(temp);
    
    mpz_mul(temp,alpha[0],y[0]);
    mpz_add(Sr, Sr, temp);
    mpz_mul(temp,alpha[1],y[1]);
    mpz_add(Sr, Sr, temp);
    mpz_mul(temp,alpha[2],y[2]);
    mpz_add(Sr, Sr, temp);
    mpz_mod(Sr, Sr, p );
    */
   
   reconstructSecret(alpha, y, Sr, p, k);
    if (DEBUG)
    {
        char Sr_str[1000]; mpz_get_str(Sr_str,10,Sr);
        std::cout << "Reconstruction of the secret : S = " << Sr_str << std::endl;
    }
    /* Clean up the GMP integers */
    //mpz_clear(y1);mpz_clear(y2);mpz_clear(y3);mpz_clear(y4);
    //mpz_clear(x1);mpz_clear(x2);mpz_clear(x3);mpz_clear(x4);
    //mpz_clear(alpha1);mpz_clear(alpha2);mpz_clear(alpha3);
    //mpz_clear(a1);mpz_clear(a2);
    clear_tab_mpz(x, n);
    clear_tab_mpz(y, n);
    clear_tab_mpz(alpha, k);
    clear_tab_mpz(a, k-1);
    //mpz_clear(temp);
    mpz_clear(Sr);
    mpz_clear(S);
    mpz_clear(p);
}

