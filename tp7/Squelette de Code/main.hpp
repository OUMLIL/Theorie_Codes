#pragma once

#include <gmp.h>

void generatePrime(mpz_t &p);
void generateSecret(mpz_t &s, mpz_t p);