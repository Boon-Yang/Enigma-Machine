#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include "components.h"
#include "errors.h"

using namespace std;

int checkComponentConfigError(char* filename, int caseNumber, int argc);
int getPosOfDuplicate(int array[26], int number);
string getFileType(char* filename);
  

int main(int argc, char** argv) {

  int errorCode = 0;
  //ErrorCode 1: Check if sufficient number of parameters
  if (argc < 4) {
    cerr<<"usage: enigma plugboard-file reflector-file (<rotor-file>)* rotor-positions"<<endl;
    return INSUFFICIENT_NUMBER_OF_PARAMETERS;
  }
  
  //  Errors related to the config files contents
  for (int i=1; i<argc; i++) {
    errorCode = checkComponentConfigError(argv[i], i, argc);
    if (errorCode) {
      return errorCode;
    }
  }
  /*------------------------------------- Encryption starts here --------------------------------------*/
  char ch='A';
  Enigma enigma(argc, argv);

  while (true) {
    cin >> ch;
    if (cin.eof()) {break;}
    if (!(ch-'A'>=0 && ch-'A'<26)) {
      cerr<<ch<<" is not a valid input character (input characters must be upper case letters A-Z)!"<<endl; //Invalid character error handling
      return INVALID_INPUT_CHARACTER;
      break;
    }
    enigma.encode(ch);
  }
  return 0;
}


int checkComponentConfigError(char* filename, int caseNumber, int argc) {
  
  int numerical;
  ifstream in;

  int signalCount = 0;
  int appearedArray[26] = {0};
  int tempArray[26] = {0};
  
  in.open(filename);

  //Error Code 11
  if (in.fail()) {   
    in.close();
    cerr<< "Error opening config file, "<<filename<<endl;
    return (ERROR_OPENING_CONFIGURATION_FILE);
  }

  // Error Code 3, 4, 
  while (!(in>>numerical).eof()) {
    if (signalCount>=26 && getFileType(filename) == "rotor") {
      break;
    }
    // Error Code 4
    if (in.fail()) {
      in.close();
      cerr<<"Non-numeric character ";
      if (getFileType(filename) == "rotor")
	cerr<<"for mapping ";
      cerr<< "in "<< getFileType(filename)  <<" file "<<filename<<endl;
      return (NON_NUMERIC_CHARACTER);
    }
    // Error Code 3
    if (numerical < 0||numerical > 25) {
      in.close();
      cerr<<"Invalid numeric character is found in config file, "<<filename<<endl;
      return (INVALID_INDEX);
    }
    tempArray[signalCount] = numerical;
    appearedArray[numerical] += 1;
    signalCount++;
    
    // Error Code 5, 7, 9 (deal with .pb, .rf and .rot files, duplicated signal within 26 counts)
    if (caseNumber > 0 && caseNumber < argc-1 && appearedArray[numerical]>1 && signalCount<=26) {
      switch (caseNumber) {
      case 1: {cerr<<"Duplicated plugboard entries"<<numerical<<". First instance at index "<<getPosOfDuplicate(tempArray, numerical)<<endl; return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;}
      case 2: {cerr<<"Duplicated reflector entries"<<numerical<<". First instance at index "<<getPosOfDuplicate(tempArray, numerical)<<endl; return INVALID_REFLECTOR_MAPPING;}
      default: {cerr<<"Invalid mapping of input "<<signalCount-1<<" to output "<<numerical<<" (output "<<numerical;
	  cerr<<" is already mapped to from input "<<getPosOfDuplicate(tempArray, numerical) <<") in file "<<filename<<endl; return INVALID_ROTOR_MAPPING;}
      }
    }
  }
     
  in.close();
     
  //Error Code 6, 7, 10 (deal with .pb, .rf and .rot files, incorrect number of signal mappings or parameters)
  if (caseNumber > 0 && caseNumber < argc-1) {
    switch (caseNumber) {
    case 1:if (signalCount%2) {cerr<<"Incorrect number of parameters in plugboard file "<<filename<<endl; return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;} break;
    case 2:if (signalCount != 26) {
      if (signalCount%2 == 1 && signalCount != 26) {
	  cerr<<"Incorrect (odd) number of parameters in reflector file "<<filename<<endl;
	}
      else if (signalCount%2==0 && signalCount < 26) {
	cerr<<"Insufficient number of mappings in reflector file: "<<filename<<endl;
      }
      else if (signalCount%2==0 && signalCount > 26) {
	cerr<<"Incorrect number of mappings in reflector file: "<<filename<<endl;
      }
      return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
      }
      break;
    default:if (signalCount<26||signalCount>32) {
      if (signalCount<26) {
	cerr<<"Not all inputs mapped in rotor file: "<<filename<<endl;
      }
      else if (signalCount>32) {
	cerr<<"Incorrect number of notches parameters in rotor file: "<<filename<<endl;
      }
      return INVALID_ROTOR_MAPPING;
      }
      break;
    }
  }
  else if (caseNumber == argc-1 && signalCount < argc-4) {
    cerr<<"No starting position for rotor "<<signalCount<<" in rotor position file: "<<filename<<endl;
    return NO_ROTOR_STARTING_POSITION;
  }
  return 0;
}

int getPosOfDuplicate(int array[26], int number) {

  for (int i=0; i<26; i++) {
    if (array[i] == number) {
      return i;
    }
  }
  return -1;
}


string getFileType(char* filename) {
  string curr = " ";
  curr += filename;
  if (curr.find(".rot") != string::npos) {
    return "rotor";
  }
  else if (curr.find(".pb") != string::npos) {
    return "plugboard";
  }
  else if (curr.find(".rf") != string::npos) {
    return "reflector";
  }
  return "rotor positions";
}
