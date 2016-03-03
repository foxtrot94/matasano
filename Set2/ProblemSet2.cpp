#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <openssl/aes.h>

#define INT_MAX 4294967295

using namespace std;

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

void Challenge9(){
	string test = "YELLOW SUBMARINE";
	int padding = 20; //bytes
	PrintSafely(AddPadding(test,padding));
}

int main(){
	cout<<"Challenge 9: PKCS#7 Padding"<<endl;
	Challenge9();
	cout<<endl<<endl;

}
