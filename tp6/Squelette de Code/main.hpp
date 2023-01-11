#pragma once


#include <stdio.h>
#include <iostream>
#include <gmp.h>


void clear_gmp_integers();
void setup_keys();
void decrypt(char * chiffr_str);
void encrypt(char * message, char * chiffr_str);
bool PrimalityTest(int accuracy, mpz_t & odd_integer);
void GCD(mpz_t & result, mpz_t a, mpz_t b);
void powm(mpz_t &res, const mpz_t &g, const mpz_t &k, const mpz_t &p);