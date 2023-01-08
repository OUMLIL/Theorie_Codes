
#include "utils.hpp"


int main()
{
  int NbMot = 12;

  vector<bitset<K>> mess;
  vector<bitset<N>> mess_cod;
  vector<bitset<N>> mess_tra;
  vector<bitset<K>> mess_dec;

  // Random initialization message
  srand((unsigned)time(NULL));
  for (int i = 0; i < NbMot; ++i)
    mess.push_back(randBitset<K>());
  for (int i = 0; i < R; ++i)
    mess.push_back(bitset<K>(0));

  // Coding of the message => mess_cod
  mess_cod = GSM_code(mess);

  // Simulation of a transmission (errors) => mess_tra
  mess_tra = GSM_transmission(mess_cod);

  // Decoding of the transmitted message => mess_dec
  mess_dec = GSM_decode(mess_tra);

  cout << "Source Message   : ";
  for (vector<bitset<K>>::iterator it = mess.begin(); it != mess.end(); ++it)
    cout << ' ' << *it;
  cout << '\n';

  cout << "Coded Message    : ";
  for (vector<bitset<N>>::iterator it = mess_cod.begin(); it != mess_cod.end(); ++it)
    cout << ' ' << *it;
  cout << '\n';

  cout << "Received Message : ";
  for (vector<bitset<N>>::iterator it = mess_tra.begin(); it != mess_tra.end(); ++it)
    cout << ' ' << *it;
  cout << '\n';

  cout << "Decoded Message  : ";
  for (vector<bitset<K>>::iterator it = mess_dec.begin(); it != mess_dec.end(); ++it)
    cout << ' ' << *it;
  cout << '\n';
}
