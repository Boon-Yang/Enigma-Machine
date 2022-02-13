#include <iostream>
#include <fstream>
#include <cassert>
#include "components.h"

using namespace std;



/*Initialise the plugboard config*/
void Plugboard::initConfig(char const* filename) {
  ifstream in;
  int index1;
  int index2;
  int tempVar;
  in.open(filename);
  while (!(in >> index1).fail()&&!(in >> index2).fail()) {
    tempVar = mappings[index1];
    mappings[index1] = mappings[index2];
    mappings[index2] = tempVar;
  }
  in.close();
}


/*Read in from the input keyboard and map appropriately*/
int Plugboard::readAndComputeSignal(char ch){
  return mappings[(int)ch-'A'];
}

/*Output to the outputboard*/
void Plugboard::outputSignal(int code) {
  cout<<(char)( mappings[code]+'A');
}



