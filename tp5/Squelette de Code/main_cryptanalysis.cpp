#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <array>
#include <cmath>

using namespace std;
 
typedef array<pair<char, double>, 26> FreqArray;
 
class VigenereCryptanalysis
{
private:
  array<double, 26> targets;
  array<double, 26> sortedTargets;
  
  // TO COMPLETE
  std::string to_upper_Case(std::string & input) {
    // Modifying all characters other than uppercase : lowercase -> uppercase, other -> delete
    std::string out{};
    for(unsigned int i = 0; i < input.length(); ++i)
    {
      if(input[i] >= 'A' && input[i] <= 'Z')
        out += input[i];
      else if(input[i] >= 'a' && input[i] <= 'z')
        out += input[i] + 'A' - 'a';
    }
    return out;
  }
   
 
public:
  VigenereCryptanalysis(const array<double, 26>& targetFreqs) 
  {
    targets = targetFreqs;
    sortedTargets = targets;
    sort(sortedTargets.begin(), sortedTargets.end());
  }

  double calculIC(string &ciphered_input) {
    double s = 0.0;
    ciphered_input = to_upper_Case(ciphered_input);
    double size = ciphered_input.size()*1.0;
    for(char i = 'A'; i < 'Z'; i++) {
        auto n = std::count(ciphered_input.begin(), ciphered_input.end(), i);
        s += (n*(n-1))/(size*(size-1));
    }
    return s;
  }

  vector<string> subsequences(int period, string &&ciphered_input) {
    vector<string> v;
    for(int i = 0; i < period; i++) {
        string s = "";
        for(int j=i; j<ciphered_input.size(); j+=period){
          s += ciphered_input[j];
        }
        v.push_back(s);
    }
    return v;
  }

  double averageIC(vector<string> &v) {
      double result;
      for(auto s : v) {
        result += calculIC(s);
      }
      return result/v.size();
  }

  double chi_squared(string sequence) {
    double result = 0.0;
    sequence = to_upper_Case(sequence);
    for(char l = 'A'; l <= 'Z'; ++l) {
      double C = std::count(sequence.begin(), sequence.end(), l);
      double E = targets[l - 'A'] * sequence.size();

      result += pow((C - E), 2) / E;
    }
    return result;
  }

  int candidateLetter(string subsequence) {

    vector<double> chi_squares;
    subsequence = to_upper_Case(subsequence);
    string decrypted = subsequence;

    for(char l = 'A'; l <= 'Z'; ++l) {
      for(int i = 0 ; i < subsequence.size(); ++i) {
        decrypted[i] = (subsequence[i] - l + 2 * 'A') % 26;
        decrypted[i] = decrypted[i] + 'A';
      }
      chi_squares.push_back(chi_squared(decrypted));
    }

    auto min = std::min_element(chi_squares.begin(), chi_squares.end());
    int index = min - chi_squares.begin();
    return index;
  }

  string find_key_letters(string & cipher_text, int num_sub_seqs) {

    std::map<double, vector<string>> avgIC_subseq_map;
    vector<int> key_candidate{};
    vector<std::string> key_candidates{};

    // generating 15 subesquences
    for(int i = 1; i<=num_sub_seqs;i++) {
      vector<string> v = subsequences(i, std::move(cipher_text));
      double avg = averageIC(v);
      avgIC_subseq_map.insert({avg, v});
    }

    // finding the highest avg and its subsequences
    auto compLambda = [](decltype(avgIC_subseq_map)::value_type & k1, decltype(avgIC_subseq_map)::value_type & k2) -> bool {
      return k1.first < k2.first;
    };
    auto max_avg_subseq = std::max_element(avgIC_subseq_map.begin(), avgIC_subseq_map.end(), compLambda);

    //Finding the key
    for(auto subseq : max_avg_subseq->second) {
      int index = candidateLetter(subseq);
      key_candidate.push_back(index);
    }

    std::string candidate_key{};
    for(auto const & e : key_candidate) {
      candidate_key += (e + 'A');
      char x = e + 'A';
    }
    key_candidates.push_back(candidate_key);
    std::cout << candidate_key << std::endl;
    return candidate_key;
  }

  string decrypt(string text,string key)
  {
    string out;
    out = text;
    // ADD THE VIGENERE DECRYPTION 
    for(unsigned int i=0; i < text.length(); ++i) {
      out[i] = (out[i] - key[i%key.size()] + 2*'A') % 26;
      out[i] = out[i] + 'A';
    }
    return out;
  }

   pair<string, string> analyze(string input) 
  {
    string key = "ISIMA PERHAPS";
    string result = "I CAN NOT DECRYPT THIS TEXT FOR NOW :-)" + input;
    input = to_upper_Case(input);
    key = find_key_letters(input, 5);
    result = decrypt(input, key);
    return make_pair(result, key);
  }
};
 
int main() 
{
  string input = "iefomntuohenwfwsjbsfftpgsnmhzsbbizaomosiuxycqaelrwsklqzekjvwsivijmhuvasmvwjewlzgubzlavclhgmuhwhakookakkgmrelgeefvwjelksedtyhsgghbamiyweeljcemxsohlnzujagkshakawwdxzcmvkhuwswlqwtmlshojbsguelgsumlijsmlbsixuhsdbysdaolfatxzofstszwryhwjenuhgukwzmshbagigzzgnzhzsbtzhalelosmlasjdttqzeswwwrklfguzl";
  array<double, 26> english = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228,
    0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025,
    0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 0.05987,
    0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150,
    0.01974, 0.00074};

  array<double, 26> french = {
    0.0811,  0.0081,  0.0338,  0.0428,  0.1769,  0.0113,
    0.0119,  0.0074,  0.0724,  0.0018,  0.0002,  0.0599, 
    0.0229,  0.0768,  0.0520,  0.0292,  0.0083,  0.0643,
    0.0887,  0.0744,  0.0523,  0.0128,  0.0006,  0.0053,
    0.0026,  0.0012};	
 
  VigenereCryptanalysis vc_en(english);
  pair<string, string> output_en = vc_en.analyze(input);
 
  cout << "Key: "  << output_en.second << endl;
  cout << "Text: " << output_en.first << endl;

  VigenereCryptanalysis vc_fr(french);
  pair<string, string> output_fr = vc_fr.analyze(input);
 
  cout << "Key: "  << output_fr.second << endl;
  cout << "Text: " << output_fr.first << endl;
}
