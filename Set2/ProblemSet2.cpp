#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <openssl/aes.h>

#define INT_MAX 4294967295

using namespace std;

//Function imported from ProblemSet1.cpp
string Base64toString(string input){

	//First, translate the encoded base64 into their respective byte values
	for(int i=0; i<input.size(); ++i){
		char rawVal = input[i];
		if(rawVal >='A' && rawVal <='Z'){
			rawVal = rawVal - 65;
		}
		else if(rawVal >='a' && rawVal <='z'){
			rawVal += -97 + 26;
		}
		else if(rawVal>='0' && rawVal<='9'){
			rawVal += -48 + 52;
		}
		else if(rawVal=='+'){
			rawVal = 62;
		}
		else if(rawVal=='/'){
			rawVal = 63;
		}

		input[i]=rawVal;
	}

	//Now that we have it at the byte level, make the original string
	//Keep in mind 4 Base64 chars are 3 ASCII chars

	string plaintext( ((input.size())*3)/4,'\0'); //We can expect the output to be slightly larger with more unused characters...
	int index=0;
	for(int i=0; i<input.size() - 3; i+=4){
		unsigned int bitVal = (input[i] << 18) + (input[i+1] << 12) + (input[i+2] << 6) + (input[i+3]);

		plaintext[index] = (char) ((bitVal >> 16) & 0xFF);
		plaintext[index+1] = (char) ((bitVal >> 8) & 0xFF);
		plaintext[index+2] = (char) (bitVal & 0xFF);

		index+=3;
	}

	return plaintext;
}

string AddPadding(string input, int desiredLength){
	if(input.size() < desiredLength){
		//Allocate a string with the added padding
		string output(desiredLength,(char)(desiredLength-input.size()));

		for(int i=0; i<input.size(); ++i){
			output[i] = input[i];
		}
		return output;
	}
	else{
		return "1";
	}
}

void PrintSafely(string input){
	for(int i=0; i<input.size(); i++){
		if(input[i] < 32 || input[i] > 126){
			cout<<"\\x"<<std::hex<<setfill('0')<<setw(2)<<(unsigned int)input[i];
		}
		else{
			cout<<std::dec<<input[i];
		}
	}
	cout<<std::dec<<endl;
}

string XOR(string a, string b){
	if(a.size()!=b.size()) { return ""; }
	string output(a.size(),'\0');

	for(int i=0; i< a.size(); ++i){
		output[i] = ((unsigned int)a[i]) ^ ((unsigned int)b[i]);
	}
	return output;
}

string DecryptAESinCBCMode(string cyphertext, string key, string IV, int bitMode=128){
	AES_KEY cryptoKey;
	string output, currentBlock;
	int bytes=bitMode/8;
	int repetitions=cyphertext.size()/bytes;

	AES_set_decrypt_key((const unsigned char*)key.c_str(),bitMode,&cryptoKey);

	//Validate the IV
	if(IV.size()<bytes){
		IV = AddPadding(IV,16-IV.size());
	}

	//Set up the output char array
	unsigned char* decrypted = new unsigned char[cyphertext.size()+2];
	decrypted[cyphertext.size()]='\0';
	unsigned char* outBlock = new unsigned char[bytes+1]; //fixed size of 16 bytes
	outBlock[bytes]='\0';

	for(int i=0; i<repetitions; ++i){
		currentBlock = cyphertext.substr(i*bytes,bytes);
		AES_ecb_encrypt((const unsigned char*)(currentBlock.c_str()), outBlock, &cryptoKey, AES_DECRYPT);
		output += XOR(string((const char*)outBlock),IV);
		IV = currentBlock;
	}

	delete outBlock;
	delete decrypted;
	return output;

}

void Challenge9(){
	string test = "YELLOW SUBMARINE";
	int padding = 20; //bytes
	PrintSafely(AddPadding(test,padding));
}

void Challenge10(){
	fstream Base64;
	string cyphertext;
	string output;
	string key="YELLOW SUBMARINE";
	string IV="\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"; //16 null chars
	Base64.open("10.txt");

	while(!Base64.eof()){
		string line;
		getline(Base64,line);
		if(line.size() > 0)
			cyphertext += Base64toString(line);
	}
	Base64.close();

	output = DecryptAESinCBCMode(cyphertext,key,IV);
	cout<<output<<endl;

}

int main(){
//	cout<<"Challenge 9: PKCS#7 Padding"<<endl;
//	Challenge9();
//	cout<<endl<<endl;

	cout<<"Challenge 10: CBC Implementation"<<endl;
	Challenge10();
	cout<<endl<<endl;

}
