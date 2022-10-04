#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <stack>
#include <array>
#include <regex>

using namespace std;

class Symbol {

  public:
    string name;
    double freq;
    string code;
    bool leaf;
    Symbol *left, *right;

	Symbol(): name(""),freq(0),code(""),leaf(true), left(NULL), right(NULL){}
	Symbol(string newName, double newFreq, string newCode="", bool newLeaf=true, Symbol* newLeft=NULL, Symbol* newRight=NULL):
  name(newName), freq(newFreq),code(newCode), leaf(newLeaf), left(newLeft), right(newRight){}
 
 	// To Do: Code Here
  // Complete the class
  std::string toString() {
    std::ostringstream oss;
    oss << name << ":" << freq << std::endl;
    std::string ss(oss.str());
    return ss;
  }
};

// foncteur priority queue
class compareSymbol {
  public:
    bool operator()(Symbol * s1, Symbol * s2) {
      return s1->freq > s2->freq;
    }
};

//Rajout code sur sous-arbre
void fillCode(Symbol * root, std::string c) {

    if(root != NULL) {
      stack<Symbol *> stack;
      Symbol * currNode = root;
      stack.push(root);

      while(!stack.empty()) {
        currNode = stack.top();
        stack.pop();

        if(currNode->leaf) {
          currNode->code = c + currNode->code;
        }

        if(currNode->right != NULL) {
          stack.push(currNode->right);
        }
        if(currNode->left != NULL) {
          stack.push(currNode->left);
        }
      }
    }
}

Symbol* CreateHuffmanCode(vector<Symbol*> &alphabet)
{
  // To Do: Code Here
  // Build the Huffman code tree
  std::priority_queue<Symbol*, std::vector<Symbol*>, compareSymbol> pq;
  for(int i = 0; i < int(alphabet.size()); ++i) { 
    pq.push(alphabet[i]);
  }

  while(pq.size() != 1) {
    Symbol * s1 = pq.top(); pq.pop();
    Symbol * s2 = pq.top(); pq.pop();

    Symbol * node = new Symbol(s1->name+s2->name, s1->freq+s2->freq, "", false, s1, s2);
    
    fillCode(s1, "0");
    fillCode(s2, "1");
    pq.push(node);
  }

  return pq.top();
}

void CreateAlphabet(vector<Symbol*>& alphabet, bool Proba=true, std::string filename="")
{
  if(Proba)
  {
    // Probability of french letters
    alphabet.push_back(new Symbol("a",8.11));
    alphabet.push_back(new Symbol("b",0.81));
    alphabet.push_back(new Symbol("c",3.38));
    alphabet.push_back(new Symbol("d",4.28));
    alphabet.push_back(new Symbol("e",17.69));
    alphabet.push_back(new Symbol("f",1.13));
    alphabet.push_back(new Symbol("g",1.19));
    alphabet.push_back(new Symbol("h",0.74));
    alphabet.push_back(new Symbol("i",7.24));
    alphabet.push_back(new Symbol("j",0.18));
    alphabet.push_back(new Symbol("k",0.02));
    alphabet.push_back(new Symbol("l",5.99));
    alphabet.push_back(new Symbol("m",2.29));
    alphabet.push_back(new Symbol("n",7.68));
    alphabet.push_back(new Symbol("o",5.2));
    alphabet.push_back(new Symbol("p",2.92));
    alphabet.push_back(new Symbol("q",0.83));
    alphabet.push_back(new Symbol("r",6.43));
    alphabet.push_back(new Symbol("s",8.87));
    alphabet.push_back(new Symbol("t",7.44));
    alphabet.push_back(new Symbol("u",5.23));
    alphabet.push_back(new Symbol("v",1.28));
    alphabet.push_back(new Symbol("w",0.06));
    alphabet.push_back(new Symbol("x",0.53));
    alphabet.push_back(new Symbol("y",0.26));
    alphabet.push_back(new Symbol("z",0.12));
  }
  else
  {

    // To Do: Code Here
    // 1/ Take the probabilty of letters in french langage (default code)
    // 2/ Compute the probabilty by the text to encode (text.txt)
    std::ifstream fichier;
    std::array<int, 26> arr{};
    fichier.open(filename.c_str());
    double text_size = 0;

    while(!fichier.eof()) {
      char c;
      fichier >> c;
      arr[c - 97] += 1;
      text_size++;
    }
    
    for(size_t i = 0; i < arr.size(); ++i) {
      alphabet.push_back(new Symbol(std::string{char(i+97)}, arr[i]/text_size));
    }

    std::cout << std::endl;
    fichier.close();
    } 
}

void DeleteMemory(vector<Symbol*>& alphabet, Symbol* root)
{
  // To Do: Code Here
  // Clear the memory	
  if(root != NULL) {
      stack<Symbol *> stack;
      Symbol * currNode = root;
      
      stack.push(root);

      while(!stack.empty()) {
        currNode = stack.top();
        stack.pop();
        if(currNode->right != NULL) {
          stack.push(currNode->right);
        }
        if(currNode->left != NULL) {
          stack.push(currNode->left);
        }
        delete currNode;
      }
  }  
}


std::string decodeHuffman(std::string huffman_code, Symbol * root) {
  //huffman decod
  std::string result{""};

  Symbol * currNode = root;
  for(char lettre : huffman_code) {

    if(lettre == '0') {
      currNode = currNode->left;
    } else {  
      currNode = currNode->right;
    }

    if(currNode->leaf) {
      result += currNode->name;
      currNode = root;
    }
  }

  return result;
}

std::string writeInHuffman(std::string message, vector<Symbol *> & alphabet) {

  for(size_t i=0;i<alphabet.size();++i) {
    message = std::regex_replace(message, std::regex(alphabet[i]->name), alphabet[i]->code);
  }
  std::string result{message};
  return result;
}

double entropie() {

}

double longueurMoyenneHuffman() {

}


int main()
{
 vector<Symbol*> alphabet;

 // Compute the frequencies of the symbol
 CreateAlphabet(alphabet, true, "text.txt");

 // Build the Huffman code tree 
 Symbol* root = CreateHuffmanCode(alphabet);

 // Display the result
 for(size_t i=0;i<alphabet.size();++i) {
  cout << "Name : " << alphabet[i]->name << " ; Code : " << alphabet[i]->code << " ; freq : " << alphabet[i]->freq << endl;
 }
  //huffman decode
  std::string ss{"hey"};
  ss = writeInHuffman(ss, alphabet);
  
  std::cout << ss << std::endl;
  std::string result = decodeHuffman("01001100001000010", root);
  std::cout << result << std::endl;

 // Clear the memory
 DeleteMemory(alphabet,root);
 return 0;
}

