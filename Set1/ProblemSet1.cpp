#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//NOTE:
// Bear with me here, this is the first set, so I wasn't much for wrapping it nicely.
// C is more suitable for these problems since C++ is somewhat overkill... BUT the string representation does make it easier to debug
// So, nothing fancy, just the first problem set.

//Converts an arbitrary string of hex chars to a readable string
//Since C++ strings boil down to char arrays, can be used as an array of bytes
string HexSequenceToString(string rawInput){
	string retVal;
	for(int i=0; i<rawInput.size()-1; i+=2){
		stringstream buf;
		int rawChar;
		buf.str(rawInput.substr(i,2));
		buf >> std::hex >> rawChar;
		retVal+=(char)rawChar;
	}
	return retVal;
}

string HexVectorToString(vector<int> rawInput){
	stringstream buf;
	for(int i=0; i<rawInput.size(); i++){
		buf << (char)((rawInput[i]) );
	}
	cout<<endl;
	return buf.str();
}

vector<int> EncodeInBase64(string input){
	vector<int> realVector;
	cout<<"In: "<<input<<endl;
	//Check if the given string is divisble by three
	int neededBytes = input.size() % 3;
	if(neededBytes){
		input.append(string(neededBytes,'\0'));
	}

	for(int i=0; i < input.size() - 2; i+=3){
		unsigned int byteTrio = ((int)input[i] << 16) + ((int)input[i+1] << 8) + ((int)input[i+2]);

		realVector.push_back( (byteTrio>>18) & 0x3F);
		realVector.push_back( (byteTrio>>12) & 0x3F);
		realVector.push_back( (byteTrio>>6) & 0x3F);
		realVector.push_back( (byteTrio) & 0x3F);
	}
	return realVector;
}

void PrintAsBase64(vector<int> input){
	//Check size to add the padding at the end
	//TODO: add padding at the end rather than "A"
	cout<<"Out: ";
	for(int i=0; i<input.size(); ++i){
		int offsetInAscii=0;
		if(input[i] >= 0 && input[i]<= 25){ offsetInAscii = (int)'A'; }
		else if(input[i] >= 26 && input[i] <= 51){ offsetInAscii = (int)'a'-26; }
		else if(input[i] >= 52 && input[i] <=61){ offsetInAscii = (int)'0'-52; }
		else if(input[i] == 62) { offsetInAscii = (int)'+'-62; }
		else if(input[i] == 63) { offsetInAscii = (int)'/'-63; }
		cout<<((char)(input[i]+offsetInAscii));
	}

	cout<<endl;
}

void PrintByteVector(vector<int> input){
	for(int i=0; i< input.size(); ++i){
		cout<<std::hex<<input[i];
	}
	cout<<endl;
}

vector<int> FixedXOR(string byteSetA, string byteSetB){
	if(byteSetA.size()!=byteSetB.size()){
		cout<<"Input strings not of same size! Aborting!"<<endl;
	}
	
	//Do the conversion into vector<int> format
	string bytesA = HexSequenceToString(byteSetA);
	string bytesB = HexSequenceToString(byteSetB);
	vector<int> retVal;
	
	for(int i=0; i<bytesA.size(); ++i){
		retVal.push_back(((int)bytesA[i]) ^ ((int)bytesB[i]));
	}
	
	return retVal;
}

void Challenge1(){
	string test = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	//string test = "a2";
	//cout<<HexSequenceToString(test)<<endl;
	PrintAsBase64(EncodeInBase64(HexSequenceToString(test)));
	PrintAsBase64(EncodeInBase64("any carnal pleasure."));
}

void Challenge2(){
	vector<int> result = FixedXOR("1c0111001f010100061a024b53535009181c","686974207468652062756c6c277320657965");
	PrintByteVector(result);
	cout<<HexVectorToString(result)<<endl;
}

int main(){
	//cout<<"Challenge 1: Hex to Base64"<<endl;
	//Challenge1();
	//cout<<endl<<endl;
	
	cout<<"Challenge 2: Fixed size XOR"<<endl;
	Challenge2();
	cout<<endl<<endl;
}
