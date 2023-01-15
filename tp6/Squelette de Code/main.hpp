#pragma once


#include <stdio.h>
#include <iostream>
#include <gmp.h>


void clear_gmp_integers();
void setup_keys();
void setup_keys_beta();
void nextprime(mpz_t &p, mpz_t s);
void decrypt(char * chiffr_str);
void encrypt(char * message, char * chiffr_str);
bool PrimalityTest(int accuracy, mpz_t & odd_integer);
void GCD(mpz_t & result, mpz_t a, mpz_t b);
void powm(mpz_t &res, mpz_t &g, mpz_t &k, mpz_t &p);
void invert(mpz_t & d, const mpz_t e, const mpz_t x);
void extended_GCD(mpz_t e, mpz_t x, mpz_t & k1, mpz_t & k2);