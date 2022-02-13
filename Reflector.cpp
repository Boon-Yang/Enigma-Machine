#include <iostream>
#include <fstream>
#include "components.h"

using namespace std;

/*Configure the reflector*/
void Reflector::initConfig(char* rfFilename) {
  int signalOne;
  int signalTwo;
  ifstream in;

  in.open(rfFilename);

  while (!(in >> signalOne).fail()&&!(in >> signalTwo).fail()) {
    mappings[signalOne] = signalTwo;
    mappings[signalTwo] = signalOne;
  }
  in.close();
}


/*Reflect current character and sendthe output to the next component(leftmost rotor or plugboard)*/
int Reflector::reflectToNextComponent(int code) {
  return mappings[code];
}

