#include <iostream>      
#include <fstream>
#include <string>         
#include <bitset>        
#include <vector>

/**
 * Hamming (7,4)
 **/ 
#define N 4
#define HAMMING_7 7

/**
 * Debug macros
 **/ 
#define DEBUG_RF false // Debug Information: Read File
#define DEBUG_HE true // Debug Information: Hamming Encoding

using namespace std; 

/**
 * Matrice génératrice
*/
	//todo fill G

/**
 * vector<bitset<N> > readFile(string filename)
 * Read a file in binary and create a vector of bitset wih a width of 4 for each bitset
 * Return a vector bitset
 **/ 
vector<bitset<N> > readFile(string filename)
{
	vector<bitset<N> > content;
	ifstream reader;
	char buffer;
	reader.open(filename.c_str(), ios::binary|ios::in);

	if(DEBUG_RF)
		cout << "Read : \t";

	if(reader != NULL && reader.is_open())
	{
		while(!reader.eof())
		{
			reader.read(&buffer, 1);
			bitset<N> bsBufferLSB(buffer);
			bitset<N> bsBufferMSB(buffer>>4);
			
			content.push_back(bsBufferMSB);
			content.push_back(bsBufferLSB);
	
			if(DEBUG_RF)
			{
				cout << " |" << bsBufferMSB.to_string();
				cout << " |" << bsBufferLSB.to_string();
			}
		}
	}
	
	if(DEBUG_RF)
		cout << endl;

	reader.close();
	return content;
} 

/**
 * vector<bitset<HAMMING_7> > HammingEncoding(vector<bitset<N> > bitsetVector)
 * Convert a vector of bitset<4> into a hamming vector of bitset<7>
 **/ 
vector<bitset<HAMMING_7> > HammingEncoding(vector<bitset<N> > bitsetVector)
{
	vector<bitset<HAMMING_7> > encodedBitset;
	if(DEBUG_HE)
		std::cout << "Encode : \t";
		
	for(vector<bitset<N> >::iterator i = bitsetVector.begin(); i != bitsetVector.end(); ++i)
	{	
		//loop through G step += 4
		//for each step
			//fill buffer
		bitset<N> inBuffer = *i;
		bitset<HAMMING_7> outBuffer;
		cout << " | " << inBuffer.to_string();

		// outBuffer[0] = inBuffer[0]^inBuffer[1]^inBuffer[3];
		// outBuffer[1] = inBuffer[0]^inBuffer[2]^inBuffer[3];
		// outBuffer[2] = inBuffer[0];
		// outBuffer[3] = inBuffer[1]^inBuffer[2]^inBuffer[3];
		// outBuffer[4] = inBuffer[1];
		// outBuffer[5] = inBuffer[2];
		// outBuffer[6] = inBuffer[3];

		outBuffer[0] = inBuffer[0];
		outBuffer[1] = inBuffer[1];
		outBuffer[2] = inBuffer[2];
		outBuffer[3] = inBuffer[3];
		outBuffer[4] = inBuffer[0]^inBuffer[1]^inBuffer[3]; 
		outBuffer[5] = inBuffer[0]^inBuffer[2]^inBuffer[3]; 
		outBuffer[6] = inBuffer[1]^inBuffer[2]^inBuffer[3];
		
		if(DEBUG_HE)
			cout << " | " << outBuffer.to_string() << endl;
		
		encodedBitset.push_back(outBuffer);
	}
	
	if(DEBUG_HE)
		cout << endl;
	
	return encodedBitset;
}

// to find which bit was modified commpare the syndrom with each row of the matrix
	// the ith row matched means the error is in the ith position
vector<bitset<N> > HammingDecoding(vector<bitset<HAMMING_7> > bitsetVector)
{
	vector<bitset<N> > decodedBitset;
	vector<bitset<3> > s;
	if(DEBUG_HE)
		std::cout << "Decode : \t";
		
	for(vector<bitset<HAMMING_7> >::iterator i = bitsetVector.begin(); i != bitsetVector.end(); ++i)
	{	
		bitset<HAMMING_7> inBuffer = *i;
		if(DEBUG_HE) {
			cout << " no error : " << inBuffer.to_string();
		}
		inBuffer[3] = 1;
		if(DEBUG_HE) {
			cout << " error pos 0 : " << inBuffer.to_string();
		}
		bitset<N> outBuffer;
		bitset<3> tempBuffer;
		
		tempBuffer[2] = inBuffer[0]^inBuffer[1]^inBuffer[3]^inBuffer[4];
		tempBuffer[1] =	inBuffer[0]^inBuffer[2]^inBuffer[3]^inBuffer[5];
		tempBuffer[0] = inBuffer[1]^inBuffer[2]^inBuffer[3]^inBuffer[6];
		s.push_back(tempBuffer);
		if(DEBUG_HE) {
			cout << " res | " << tempBuffer.to_string();
		}
		decodedBitset.push_back(outBuffer);
	}
	
	if(DEBUG_HE)
		cout << endl;
	
	return decodedBitset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                     Main                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
std::bitset<N> b1(std::string("0001"));
 vector< bitset<N> > input_data;
 vector< bitset<HAMMING_7> > encode_data; 

 // Read data to encode
 //input_data = readFile("test.txt");
	input_data.push_back(b1);
 // Encode by Hamming (7,4) coding
 encode_data = HammingEncoding(input_data);

 // Inject error
 // TODO

 // Decode
 HammingDecoding(encode_data);

}



