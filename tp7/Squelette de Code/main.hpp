#pragma once

#include <gmp.h>

void generatePrime(mpz_t &p);
void generateRandom(mpz_t &s, mpz_t p);
void generateCoefs(mpz_t *tab, mpz_t p, int k);
//void computeShares(mpz_t * x[], mpz_t * y[], mpz_t a[]);