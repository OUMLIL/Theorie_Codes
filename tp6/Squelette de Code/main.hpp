#pragma once


#include <stdio.h>
#include <iostream>
#include <gmp.h>


void clear_gmp_integers();
void setup_keys();
void decrypt(char * chiffr_str);
void encrypt(char * message, char * chiffr_str);