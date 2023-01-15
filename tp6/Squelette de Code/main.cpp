//
//  TP6_RSA
//  

#include "main.hpp"
#define BITSTRENGTH  14              /* size of modulus (n) in bits */
#define PRIMESIZE (BITSTRENGTH / 2)  /* size of the primes p and q  */

/* Declare global variables */
mpz_t d,e,n;
mpz_t M,c;
gmp_randstate_t state;


/* Main subroutine */
int main()
{   
    // Encrypt, Decrypt using GMP
    char plain[7] = "44";
    char cipher[1000];
    
    std::cout << "------------------ using GMP Library ------------------------" << std::endl << std::endl;
    // Creating keys
    setup_keys();
    encrypt(plain, cipher);
    decrypt(cipher);


    std::cout << "--------------- using crafted functions ------------------------" << std::endl << std::endl;
    setup_keys_home();
    Home_encrypt(plain, cipher);
    Home_decrypt(cipher);

    // Clearing gmp integers
    clear_gmp_integers();
}

// Encryption of a message (< n) using GMP preconstructed functions
void encrypt(char * message, char * chiffr_str) {
    // Setting up and printing message to encrypt
    mpz_init_set_str(M, message, 0);
    size_t size_M = mpz_sizeinbase(M, 10);
    char M_str[1000];
    mpz_get_str(M_str,10,M);
    std::cout << "M = " << M_str << std::endl << std::endl;

    // Encryption
    mpz_t chiffr;
    mpz_init(chiffr);
    mpz_powm(chiffr, M, e, n);
    mpz_get_str(chiffr_str, 10, chiffr);
    std::cout << "M chiffré = " << chiffr_str << std::endl << std::endl;
}

// Decryption of a message
void decrypt(char * chiffr_str) {
    mpz_t dechiffr, chiffr;
    mpz_inits(dechiffr, chiffr, NULL);
    mpz_init_set_str(chiffr, chiffr_str, 0);
    mpz_powm(dechiffr, chiffr, d, n);

    char dechiffr_str[1000];
    mpz_get_str(dechiffr_str, 10, dechiffr);
    std::cout << "M déchiffré = " << dechiffr_str << std::endl << std::endl;
}

// Encryption of a message (< n) using GMP preconstructed functions
void Home_encrypt(char * message, char * chiffr_str) {
    // Setting up and printing message to encrypt
    mpz_init_set_str(M, message, 0);
    size_t size_M = mpz_sizeinbase(M, 10);
    char M_str[1000];
    mpz_get_str(M_str,10,M);
    std::cout << "M = " << M_str << std::endl << std::endl;

    // Encryption
    mpz_t chiffr;
    mpz_init(chiffr);
    powm(chiffr, M, e, n);
    mpz_get_str(chiffr_str, 10, chiffr);
    std::cout << "M chiffré = " << chiffr_str << std::endl << std::endl;
}

// Decryption of a message
void Home_decrypt(char * chiffr_str) {
    mpz_t dechiffr, chiffr;
    mpz_inits(dechiffr, chiffr, NULL);
    mpz_init_set_str(chiffr, chiffr_str, 0);
    powm(dechiffr, chiffr, d, n);

    char dechiffr_str[1000];
    mpz_get_str(dechiffr_str, 10, dechiffr);
    std::cout << "M déchiffré = " << dechiffr_str << std::endl << std::endl;
}


void setup_keys() {
    /* Initialize the GMP integers */
    mpz_init(d);
    mpz_init(e);
    mpz_init(n);
    
    mpz_init(M);
    mpz_init(c);

    // Initializing the global pseudo random generator
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));
    
 
    /* This function creates the keys. The basic algorithm is...
     *
     *  1. Generate two large distinct primes p and q randomly
     *  2. Calculate n = pq and x = (p-1)(q-1)
     *  3. Select a random integer e (1<e<x) such that gcd(e,x) = 1
     *  4. Calculate the unique d such that ed = 1(mod x)
     *  5. Public key pair : (e,n), Private key pair : (d,n)
     *
     */
    
    /*
     *  Step 1 : Get two large primes.
     */
    mpz_t p, q, p_temp, q_temp;
    mpz_init(p); mpz_init(q); mpz_init(p_temp); mpz_init(q_temp);

    // Making p and q random prime numbers
    mpz_urandomb(p_temp, state, PRIMESIZE); // Render random number between 0 and 2 ^ PRIMESIZE
    mpz_nextprime(p, p_temp); // Selecting first prime number after the random number generated
    mpz_urandomb(q_temp, state, PRIMESIZE);
    mpz_nextprime(q, q_temp);

    // Printing results
    char p_str[1000];
    char q_str[1000];
    mpz_get_str(p_str, 10, p); // Converting int to string in base 10
    mpz_get_str(q_str, 10, q);
    
    std::cout << "Random Prime 'p' = " << p_str <<  std::endl;
    std::cout << "Random Prime 'q' = " << q_str <<  std::endl;
    std::cout << "is prime ?: " << PrimalityTest(10, p) << std::endl;
    std::cout << std::endl;
    
    /*
     *  Step 2 : Calculate n (=pq) ie the 1024 bit modulus
     *  and x (=(p-1)(q-1)).
     */
    char n_str[1000];
    mpz_t x;
    mpz_init(x);

    /* Calculate n... */
    mpz_mul(n,p,q);
    mpz_get_str(n_str,10,n);
    std::cout << "\t n = " << n_str << std::endl;
    
    
    /* Calculate x... */
    mpz_t p_minus_1,q_minus_1;
    mpz_init(p_minus_1);
    mpz_init(q_minus_1);

    mpz_sub_ui(p_minus_1,p,(unsigned long int)1);
    mpz_sub_ui(q_minus_1,q,(unsigned long int)1);

    mpz_mul(x,p_minus_1,q_minus_1);
    char phi_str[1000];
    mpz_get_str(phi_str,10,x);
    std::cout << "\t x = phi(n) = " << phi_str << std::endl;


    /*
     *  Step 3 : Get small odd integer e such that gcd(e,x) = 1.
     */
    mpz_t e_temp, pgcd;
    mpz_init(e_temp);
    mpz_init(pgcd);

    // Selecting e
    do {
        mpz_urandomb(e_temp, state, PRIMESIZE);
        mpz_init_set_str(e, std::to_string(mpz_get_ui(e_temp) % mpz_get_ui(x)).c_str(), 0);
        mpz_gcd(pgcd, e, x);
    } while (mpz_get_ui(pgcd) != 1);

    // Printing results
    char e_str[1000];
    mpz_get_str(e_str, 10, e);
    std::cout << "\t e = " << e_str << std::endl;

    /*
     *  Step 4 : Calculate unique d such that ed = 1(mod x)
     */
    mpz_invert(d, e, x);
    char d_str[1000];
    mpz_get_str(d_str,10,d);
    std::cout << "\t d = " << d_str << std::endl << std::endl;

    /*
     *  Step 5 : Print the public and private key pairs...
     */
    std::cout << "Public Keys  (e,n): ( " << e_str <<" , " << n_str << " )" << std::endl;
    std::cout << "Private Keys (d,n): ( " << d_str <<" , " << n_str << " )" << std::endl;

    /* Clean up the GMP integers */
    mpz_clear(p_minus_1);
    mpz_clear(q_minus_1);
    mpz_clear(x);
    mpz_clear(p);
    mpz_clear(q);
}

bool PrimalityTest(int accuracy, mpz_t & n) {
    if(mpz_get_si(n) <= 2 || !mpz_odd_p(n)) {
        return false;
    }

    
    // Writing n − 1 as t × 2^s by factoring powers of 2 from n − 1
    mpz_t t, s;
    mpz_inits(t, s, NULL);

    mpz_sub_ui(t, n, 1);
    mpz_set_ui(s, 0);

    while (mpz_even_p(t)) {
        mpz_fdiv_q_ui(t, t, 2);
        mpz_add_ui(s, s, 1);
    }
    mpz_t a, x, n_1;
    mpz_init(a);
    mpz_init(x);
    mpz_init(n_1);

    for(int i = 0; i < accuracy; ++i) {
        do {
            mpz_urandomm(a, state, n);
        } while(mpz_get_ui(a) < 2);

        mpz_powm(x, a, t, n);
        mpz_sub_ui(n_1, n, 1);

        if(mpz_get_ui(x) == 1 || mpz_get_ui(x) == mpz_get_ui(n_1)) {
            continue;
        }

        for(int r = 1; r < mpz_get_ui(s); ++r) {
            mpz_mul(x, x, x);
            mpz_mod(x, x, n);

            if(mpz_get_ui(x) == 1) {
                return false;
            }

            if(mpz_get_ui(n_1) == mpz_get_ui(x)) {
                continue;
            }
        }
        return false;
    }
    return true;
}  

void GCD(mpz_t & result, mpz_t a, mpz_t b) {
    if(mpz_get_ui(b) == 0) {
        char a_str[1000];
        mpz_set(result, a);
        mpz_get_str(a_str, 10, result);
        std::cout << "GCD = " << a_str << std::endl;
    } else {
        mpz_t tmp_a; mpz_init(tmp_a);
        mpz_set(tmp_a, a);
        mpz_set(a, b);
        mpz_mod(b, tmp_a, b);
        
        GCD(result, a, b);
    }
}

void powm(mpz_t &res, mpz_t &g, mpz_t &k, mpz_t &p){
    if(mpz_get_ui(k) < 0){
        mpz_t temp1,temp2;
        mpz_init(temp1);
        mpz_set_ui(temp1, 1);
        mpz_divexact_ui(temp1, temp1,mpz_get_ui(g));
        mpz_set(g, temp1);
        mpz_init(temp2);
        mpz_set_ui(temp2, -1);
        mpz_mul(temp2, temp2, k);
        mpz_set(k, temp2);
    }
    if(mpz_get_ui(k) == 0) mpz_set_str(res,"1",0);
    mpz_t y;
    mpz_init(y);
    mpz_set_str(y,"1",0);
    while(mpz_get_ui(k) > 1) {
        if(mpz_even_p(k)) {
            mpz_mul(g,g,g);
            mpz_mod(g,g,p);
            mpz_divexact_ui(k,k,2);
        }
        else {
            mpz_mul(y,g,y);
            mpz_mul(g,g,g);
            mpz_sub_ui(k,k,1);
            mpz_divexact_ui(k,k,2);
        }
    }
    mpz_mul(g,g,y);
    mpz_mod(res,g,p);
}

void nextprime(mpz_t &p, mpz_t s) {
    if(mpz_even_p(s)) {
        mpz_add_ui(s, s, 1);
    }
    mpz_set(p, s);
    while(!PrimalityTest(10000, p)) {
        mpz_add_ui(p, p, 2);
    }
}

void extended_GCD(const mpz_t a, const mpz_t b, mpz_t & k1, mpz_t & k2) {

    mpz_t x1_tmp; mpz_init(x1_tmp); mpz_set_ui(x1_tmp, 1);
    mpz_t x2_tmp; mpz_init(x2_tmp); mpz_set_ui(x2_tmp, 0);

    mpz_t y1_tmp; mpz_init(y1_tmp); mpz_set_ui(y1_tmp, 0);
    mpz_t y2_tmp; mpz_init(y2_tmp); mpz_set_ui(y2_tmp, 1);

    mpz_t a_tmp; mpz_init(a_tmp); mpz_set(a_tmp, a);
    mpz_t b_tmp; mpz_init(b_tmp); mpz_set(b_tmp, b);

    while(mpz_get_ui(b_tmp) != 0) {

        mpz_t q; mpz_init(q);
        mpz_div(q, a_tmp, b_tmp);

        mpz_t r; mpz_init(r);
        mpz_mod(r, a_tmp, b_tmp);

        mpz_t tmp; mpz_init(tmp);
        mpz_t tmp2; mpz_init(tmp2);

        mpz_mul(tmp, q, x1_tmp);
        mpz_mul(tmp2, q, y1_tmp);

        mpz_sub(k1, x2_tmp, tmp);
        mpz_sub(k2, y2_tmp, tmp2);

        mpz_set(a_tmp, b_tmp);
        mpz_set(b_tmp, r);

        mpz_set(x2_tmp, x1_tmp);
        mpz_set(x1_tmp, k1);

        mpz_set(y2_tmp, y1_tmp);
        mpz_set(y1_tmp, k2);
    }

    mpz_set(k1, x2_tmp);
    mpz_set(k2, y2_tmp);
}

void invert(mpz_t & d, const mpz_t e, const mpz_t x) {
    mpz_t k1; mpz_init(k1);
    mpz_t k2; mpz_init(k2);

    extended_GCD(e, x, k1, k2);

    mpz_t tmp; mpz_init(tmp);

    char k2_s[1000];
    mpz_get_str(k2_s,10,k2);
    char k1_s[1000];
    mpz_get_str(k1_s,10,k1);
    char x_s[1000];
    mpz_get_str(x_s,10,x);
    std::cout << "\t k1 , k2, x = " << k1_s << ", " << k2_s << ", " << x_s << std::endl << std::endl;


    mpz_mod(tmp, k2, x);
    mpz_add(tmp, tmp, x);
    mpz_mod(tmp, tmp, x);
    mpz_set(d, tmp);
}

void setup_keys_home() {
    /* Initialize the GMP integers */
    mpz_init(d);
    mpz_init(e);
    mpz_init(n);
    
    mpz_init(M);
    mpz_init(c);

    // Initializing the global pseudo random generator
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));
    
 
    /* This function creates the keys. The basic algorithm is...
     *
     *  1. Generate two large distinct primes p and q randomly
     *  2. Calculate n = pq and x = (p-1)(q-1)
     *  3. Select a random integer e (1<e<x) such that gcd(e,x) = 1
     *  4. Calculate the unique d such that ed = 1(mod x)
     *  5. Public key pair : (e,n), Private key pair : (d,n)
     *
     */
    
    /*
     *  Step 1 : Get two large primes.
     */
    mpz_t p, q, p_temp, q_temp;
    mpz_init(p); mpz_init(q); mpz_init(p_temp); mpz_init(q_temp);

    // Making p and q random prime numbers
    mpz_urandomb(p_temp, state, PRIMESIZE); // Render random number between 0 and 2 ^ PRIMESIZE
    nextprime(p, p_temp); // Selecting first prime number after the random number generated
    mpz_urandomb(q_temp, state, PRIMESIZE);
    nextprime(q, q_temp);

    // Making sure p and q are different
    if (mpz_cmp(q, p) == 0) {
        mpz_urandomb(q_temp, state, PRIMESIZE);
        nextprime(q, q_temp);
    }

    // Printing results
    char p_str[1000];
    char q_str[1000];
    mpz_get_str(p_str, 10, p); // Converting int to string in base 10
    mpz_get_str(q_str, 10, q);
    
    std::cout << "Random Prime 'p' = " << p_str <<  std::endl;
    std::cout << "Random Prime 'q' = " << q_str <<  std::endl;
    std::cout << "is prime ?: " << PrimalityTest(10, p) << std::endl;
    std::cout << std::endl;
    
    /*
     *  Step 2 : Calculate n (=pq) ie the 1024 bit modulus
     *  and x (=(p-1)(q-1)).
     */
    char n_str[1000];
    mpz_t x;
    mpz_init(x);

    /* Calculate n... */
    mpz_mul(n,p,q);
    mpz_get_str(n_str,10,n);
    std::cout << "\t n = " << n_str << std::endl;
    
    
    /* Calculate x... */
    mpz_t p_minus_1,q_minus_1;
    mpz_init(p_minus_1);
    mpz_init(q_minus_1);

    mpz_sub_ui(p_minus_1,p,(unsigned long int)1);
    mpz_sub_ui(q_minus_1,q,(unsigned long int)1);

    mpz_mul(x,p_minus_1,q_minus_1);
    char phi_str[1000];
    mpz_get_str(phi_str,10,x);
    std::cout << "\t x = phi(n) = " << phi_str << std::endl;


    /*
     *  Step 3 : Get small odd integer e such that gcd(e,x) = 1.
     */
    mpz_t e_temp, pgcd;
    mpz_init(e_temp);
    mpz_init(pgcd);

    // Selecting e
    do {
        mpz_urandomb(e_temp, state, PRIMESIZE);
        mpz_init_set_str(e, std::to_string(mpz_get_ui(e_temp) % mpz_get_ui(x)).c_str(), 0);
        mpz_gcd(pgcd, e, x);
    } while (mpz_get_ui(pgcd) != 1);

    // Printing results
    char e_str[1000];
    mpz_get_str(e_str, 10, e);
    std::cout << "\t e = " << e_str << std::endl;

    /*
     *  Step 4 : Calculate unique d such that ed = 1(mod x)
     ed = 1 + K*x
     ed +(- k)*x = 1
     d = (1 + K*x)/e
     extended_euclide(e, x) => e , -k
     */
    invert(d, e, x);
    char d_str[1000];
    mpz_get_str(d_str,10,d);
    std::cout << "\t d = " << d_str << std::endl << std::endl;
    

    /*
     *  Step 5 : Print the public and private key pairs...
     */
    std::cout << "Public Keys  (e,n): ( " << e_str <<" , " << n_str << " )" << std::endl;
    std::cout << "Private Keys (d,n): ( " << d_str <<" , " << n_str << " )" << std::endl;

    /* Clean up the GMP integers */
    mpz_clear(p_minus_1);
    mpz_clear(q_minus_1);
    mpz_clear(x);
    mpz_clear(p);
    mpz_clear(q);
}

void clear_gmp_integers() {
    mpz_clear(d);
    mpz_clear(e);
    mpz_clear(n);
    mpz_clear(M);
    mpz_clear(c);
}
