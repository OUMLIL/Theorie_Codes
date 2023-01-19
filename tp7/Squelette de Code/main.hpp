#pragma once

#include <gmp.h>

void generatePrime(mpz_t &p);
void generateRandom(mpz_t &s, mpz_t p);
void generateCoefs(mpz_t *tab, mpz_t p, int k);
void init_tab_mpz(mpz_t * tab, int t);
void computeShares(mpz_t *x, mpz_t *y, mpz_t *a, mpz_t s, int k, int n);
unsigned compute_image(mpz_t *a, int i, int k, mpz_t s);