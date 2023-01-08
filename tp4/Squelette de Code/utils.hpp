#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <cstdlib>
#include <ctime>



const int N = 2;
const int K = 1;
const int R = 4;


//perturbation du canal : nombre d'erreurs ajoutées
const int nombre_erreurs = 2;

// #define DEBUG
using namespace std;


class Chemin {
    public:
        bitset<R> registre;
        vector<bitset<K>> message_decoded;
        int distance_totale;

        Chemin() = default;
        Chemin(bitset<R> reg, int dst) : registre(reg), distance_totale(dst) {}

        friend bool operator<(const Chemin & c1, const Chemin & c2) {
            return c1.distance_totale < c2.distance_totale;
        }
};


/*
    Calcul de la distance de hamming entre deux bitsets
*/
inline int hammingDstCalcule(bitset<N> bitset1, bitset<N> bitset2) {
    int cpt = 0;
    cpt = (bitset1[0] != bitset2[0]) ? cpt + 1 : cpt;
    cpt = (bitset1[1] != bitset2[1]) ? cpt + 1 : cpt;
    return cpt;
}

/*
    Calcul de la sortie 2 bits à partir d'un état 5 bits
*/
inline bitset<N> code(const bitset<R + 1> & registre)
{
  bitset<N> output;
  int g0, g1;
  bitset<R + 1> G0(25);
  bitset<R + 1> G1(27);
  g0 = (registre & G0).count() % 2; 
  g1 = (registre & G1).count() % 2;

  output.reset();
  output.set(0, g0);
  output.set(1, g1);

  return output;
}


////////////////////////////////////////////////////////////
//      template<int bits> bitset<bits> randBitset()      //
//                                                        //
//               Generate random bitset                   //
////////////////////////////////////////////////////////////
template <int bits>
inline bitset<bits> randBitset()
{
  bitset<bits> r(rand());
  for (int i = 0; i < bits / 16 - 1; i++)
  {
    r <<= 16;
    r |= bitset<bits>(rand());
  }
  return r;
}

////////////////////////////////////////////////////////////
// vector< bitset<N> > GSM_code(vector< bitset<K> > mess) //
//                                                        //
//     Convolutional coding of a message (GSM norm)       //
////////////////////////////////////////////////////////////
inline vector<bitset<N>> GSM_code(vector<bitset<K>> mess)
{
  int i = 0, g0, g1;
  vector<bitset<N>> mess_out;

  bitset<N> cod_out;
  bitset<R + 1> G0(25);
  bitset<R + 1> G1(27);
  bitset<R + 1> registre;
  registre.reset();

#ifdef DEBUG
  cout << "-------------------- Debug Informations (Coding) --------------------" << endl
       << endl;
  cout << "Initial register ( u(i-4)  u(i-3)  u(i-2)  u(i-1)  u(i)  ): " << registre << endl;
  cout << "Polynom G0       ( g0(i-4) g0(i-3) g0(i-2) g0(i-1) g0(i) ): " << G0 << endl;
  cout << "Polynom G1       ( g1(i-4) g1(i-3) g1(i-2) g1(i-1) g1(i) ): " << G1 << endl
       << endl;
#endif

  for (vector<bitset<K>>::iterator it = mess.begin(); it != mess.end(); ++it)
  {
    registre = registre << 1;            // Left shifting of the initial register
    registre.set(0, (*it).count()); // Setting the first bit of the initial register if the current bit scanned is set

    g0 = (registre & G0).count() % 2;
    g1 = (registre & G1).count() % 2;

    cod_out.reset();
    cod_out.set(0, g0);
    cod_out.set(1, g1);

    mess_out.push_back(cod_out); // Pushing 2-bits binary words at a time in the final message (to output)

#ifdef DEBUG
    cout << "Block number: " << ++i << " - In frame: " << *it << endl;
    cout << "\t Current status of registers: " << registre << endl;
    cout << "\t Out : " << cod_out << endl;
#endif
  }

#ifdef DEBUG
  cout << "------------------------------------------------------------------" << endl
       << endl;
#endif

  return mess_out;
}

/////////////////////////////////////////////////////////////////////////
// vector< bitset<N> >  GSM_transmission(vector< bitset<N> > mess_cod) //
//                                                                     //
//         Simulation of a transmission channel => adding errors       //
/////////////////////////////////////////////////////////////////////////
inline vector<bitset<N>> GSM_transmission(vector<bitset<N>> mess_cod)
{
    vector<bitset<N>> mess_tra = mess_cod;

    for (int i = 0; i < nombre_erreurs; i++) {                                                                            
        int bitset_a_changer = rand() % mess_cod.size();                                 
        int bit_a_changer = rand() % N;                                                
        mess_tra[bitset_a_changer][bit_a_changer] = !mess_tra[bitset_a_changer][bit_a_changer];
    }

  return mess_tra;
}
//////////////////////////////////////////////////////////////////
// vector< bitset<K> > GSM_decode(vector< bitset<N> > mess_tra) //
//                                                              //
//     Convolutional decoding of a message (GSM norm)           //
//////////////////////////////////////////////////////////////////
inline vector<bitset<K>> GSM_decode(vector<bitset<N>> mess_tra)
{
    vector<bitset<K>> message_decode;

    vector<Chemin> chemins;    
    vector<Chemin> nv_chemins;

    
    Chemin chemin_initial(0000, 0); //premier chemin du graphe
    chemins.push_back(chemin_initial);

    // Pour chaque mot de N bits dans le message transmis
    for (vector<bitset<N>>::iterator it_message = mess_tra.begin(); it_message != mess_tra.end(); it_message++) { 
        nv_chemins = chemins;
        chemins.clear();

        // Pour chaque chemin
        for (vector<Chemin>::iterator it_chemin = nv_chemins.begin(); it_chemin != nv_chemins.end(); it_chemin++) { 
            
            // 2 possibilités +0 ou +1
            Chemin tmp_chem1 = *it_chemin;
            Chemin tmp_chem2 = *it_chemin;

            // possibilité +0
            bitset<R + 1> possibilite_1(tmp_chem1.registre.to_string() + "0"); 
            bitset<N> code_possibilite1 = code(possibilite_1);                             
            int dst_hamming_chem1 = hammingDstCalcule(code_possibilite1, (*it_message));

            // possibilité +1
            bitset<R + 1> possibilite_2(tmp_chem2.registre.to_string() + "1");
            bitset<N> code_possibilite2 = code(possibilite_2);
            int dst_hamming_chem2 = hammingDstCalcule(code_possibilite2, (*it_message));

            // Mise à jour distance pour chaque chemin
            tmp_chem1.distance_totale += dst_hamming_chem1;
            tmp_chem2.distance_totale += dst_hamming_chem2;

            // Mise à jour des registre
            tmp_chem1.registre = (tmp_chem1.registre << 1); 
            tmp_chem2.registre = (tmp_chem2.registre << 1);
            tmp_chem1.registre[0] = 0;
            tmp_chem2.registre[0] = 1;

            // Mise à jours du message decodé pour chaque chemin
            tmp_chem1.message_decoded.push_back(0);
            tmp_chem2.message_decoded.push_back(1);

            // Ajout des deux chemin potentiels
            chemins.push_back(tmp_chem1);
            chemins.push_back(tmp_chem2);
        }
    }

    // Recuperer le chemin ayant la plus petite distance de hamming et le message correspondant
    message_decode = min_element(chemins.begin(), chemins.end())->message_decoded;

    return message_decode;
}