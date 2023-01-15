# TP Théories des code

**Groupe** :
- ABBOUD Oussama
- OUMLIL Abdellah
# Remplacement des fonctions GMP
## 1. Remplacement de *mpz_pown()* par l'algorithm d'exponentiation rapide:
```cpp
    void powm(mpz_t &g, mpz_t &k, mpz_t &p, mpz_t &m);
```
L'algorithme d'exponentiation rapide est utilisé pour calculer les puissances de grands nombres entiers positifs d'un nombre. Il permet de chiffrer un message en utilisant un message de base **(g)**, un exposant de chiffrement **(k)** et un module de chiffrement **(p)**. 

## 2. Remplacement de *mpz_nextprime*:
```cpp
    bool PrimalityTest(int accuracy, mpz_t & odd_integer);
    void nextprime(mpz_t &p, mpz_t s);
```
Nous avons utiliser le  test de primalité de **Rabin-Miller** pour déterminer si un nombre est premier. Cette fonction est ensuite utilisée dans ***nextprime()*** pour tester si un nombre premier est généré et ne s'arrêté que si c'est le cas

## 3. Remplacement de *mpz_invert*:
```cpp
    void extended_GCD(const mpz_t a, const mpz_t b, mpz_t & k1, mpz_t & k2);
    void invert(mpz_t & d, const mpz_t e, const mpz_t x);
```
L'algorithme d'Euclide étendu permet de calculer de manière efficace le plus grand diviseur commun (PGCD) de deux nombres. Il est utilisé dans notre cas pour calculer l'exposant de déchiffrement **(d)** pour la création de la clé privée. Il repose sur l'idée d'effectuer les mêmes étapes que l'algorithme d'Euclide classique, mais en exprimant à chaque itération le reste comme une combinaison linéaire des deux entiers fournis (**a** et **b**). Le dernier reste obtenu est le PGCD et il est exprimé comme une combinaison linéaire de **a** et **b**.
