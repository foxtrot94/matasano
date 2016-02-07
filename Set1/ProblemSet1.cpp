#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define INT_MAX 4294967295

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

string SingleByteXORCypher(string encrypted, char key){
	string retVal(encrypted.size(),'\0'); //Wouldn't want to miss those null characters, eh?
	
	for(int i=0; i<encrypted.size(); ++i){
		retVal[i] = encrypted[i] ^ key;
	}
	return retVal;
}

string BruteForceSingleByteXOR(string cypher, bool printAttempt){
	char key = 0;
	string decodedMessage;
	int heuristic=0;
	//We'll brute force the password, it's only 128 possibilities for a single char.
	for(int i=0; i<128; i++){
		string decodeAttempt = SingleByteXORCypher(cypher,(char) i);
	
		if(printAttempt)
			cout<<i<<". "<<decodeAttempt<<endl;
		
		//Our Heuristic will slow us down since we have to count letters.
		//My initial guess was pretty bad. I was trying to count the letter 'e'. That's the most common one in english words...
		//However, it turns out that for a full, well structured message, spaces (' ') are an infinitely better heuristic!
		int hits=0;
		for(int j=0; j<decodeAttempt.size(); ++j){
			if(decodeAttempt[j]>=65 && decodeAttempt[j]<=122){ 
				hits++; 
			}
			if(decodeAttempt[j]==' '){
				hits++;
			}
		}
		if(heuristic < hits){
			heuristic = hits;
			decodedMessage = decodeAttempt;
			key = (char) i;
		}
	}
	
	if(printAttempt){
		cout<<"Message was: "<<decodedMessage<<endl;
		cout<<"Key was: "<<(int)key<<" '"<<key<<"'"<<endl;
	}
	return decodedMessage;
}

void PrintAsHexChars(string input){
	for(int i=0; i<input.size(); ++i){
		cout<<std::hex<<(int)input[i];
	}
}

string RepeatingKeyXOR(string plaintext, string key){
	int keyLength = key.size(), cypherLength = plaintext.size();
	
	if(keyLength<1) { return ""; }
	
	string cypher(cypherLength,'\0');
	for(int i=0; i<cypherLength; ++i){
		cypher[i] = plaintext[i] ^ key[i%keyLength];
	}
	return cypher;
}

int HammingDistance(string inputA, string inputB){
	if(inputA.size()!=inputB.size()){
		return INT_MAX;
	}

	int distance=0;
	for(int i=0; i<inputA.size(); ++i){
		int partial = inputA[i] ^ inputB[i];
		while(partial){
			distance += partial & 1;
			partial = partial >>1;
		}
	}
	return distance;
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

void Challenge3(){
	string secret = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	string plainChars = HexSequenceToString(secret);
	cout<<"Interpreted "<<plainChars<<endl;
	
	BruteForceSingleByteXOR(plainChars,true);
}

void Challenge4(){
	//Open file and read line-by-line
	fstream inputFile;
	int heuristic=0, lineNum=0;
	string line, decodeAttempt, likelyDecode;
	inputFile.open("4.txt");
	
	for(int i=1; !inputFile.eof(); ++i){
		getline(inputFile,line);
		decodeAttempt = BruteForceSingleByteXOR(HexSequenceToString(line),0);
		cout<<decodeAttempt<<endl;
		int hits=0;
		for(int j=0; j<decodeAttempt.size(); ++j){
			if(decodeAttempt[j]>=65 && decodeAttempt[j]<=122){ 
				hits++; 
			}
			if(decodeAttempt[j]==' '){ 
				hits++; 
			}
		}
		if(heuristic < hits){
			heuristic = hits;
			likelyDecode = decodeAttempt;
			lineNum = i;
		}
	}
	
	cout<<"The message was: "<<likelyDecode<<endl;
	cout<<"At line "<<lineNum<<endl;

	//Read the input file one line at a time.
	
}

void Challenge5(){
	string plaintext = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
	string key = "ICE";
	cout<<"Encoding: "<<plaintext<<endl;
	string cypher = RepeatingKeyXOR(plaintext,key);
	PrintAsHexChars(cypher);
}

void Challenge6(){
	cout<<HammingDistance("this is a test","wokka wokka!!!")<<endl;
}

void Test(){
	BruteForceSingleByteXOR(HexSequenceToString("7b5a4215415d544115415d5015455447414c155c46155f4058455c5b523f"),1);
}

int main(){
	//cout<<"Challenge 1: Hex to Base64"<<endl;
	//Challenge1();
	//cout<<endl<<endl;
	
	//cout<<"Challenge 2: Fixed size XOR"<<endl;
	//Challenge2();
	//cout<<endl<<endl;
	
	//cout<<"Challenge 3: Single Byte XOR"<<endl;
	//Challenge3();
	//cout<<endl<<endl;
	
	//cout<<"Challenge 4: Single Byte XOR decypher"<<endl;
	//Challenge4();
	//cout<<endl<<endl;
	
//	cout<<"Challenge 5: Repeating Key XOR"<<endl;
//	Challenge5();
//	cout<<endl<<endl;

	cout<<"Challenge 6: Break Repeating Key XOR"<<endl;
	Challenge6();
	cout<<endl<<endl;
}
