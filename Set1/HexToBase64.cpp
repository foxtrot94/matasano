#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

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
	for(int i=0; i<input.size()-padding; ++i){
		int offsetInAscii=0;
		if(input[i] >= 0 && input[i]<= 25){ offsetInAscii = (int)'A'; }
		else if(input[i] >= 26 && input[i] <= 51){ offsetInAscii = (int)'a'-26; }
		else if(input[i] >= 52 && input[i] <=61){ offsetInAscii = (int)'0'-52; }
		else if(input[i] == 62) { offsetInAscii = (int)'+'-62; }
		else if(input[i] == 63) { offsetInAscii = (int)'/'-63; }
		cout<<((char)(input[i]+offsetInAscii));
	}
	for(int i=0; i<padding; ++i){
		cout<<"=";
	}
	cout<<endl;
}


int main(){
	string test = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	//string test = "a2";
	//cout<<HexSequenceToString(test)<<endl;
	PrintAsBase64(EncodeInBase64(HexSequenceToString(test)));
	PrintAsBase64(EncodeInBase64("any carnal pleasure."));
}
