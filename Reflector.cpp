#include <iostream>
#include <fstream>
#include "components.h"

using namespace std;

/*Constructor for reflector*/
Reflector::Reflector(Rotor* connectedRotorPtr)
  : connectedRotorPtr(connectedRotorPtr) {
  if (connectedRotorPtr != nullptr) {
    connectedRotorPtr->connectedReflectorPtr = this;
  }
}

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
void Reflector::reflectToNextComponent() {
  charAfterMapping = mappings[charBeforeMapping];
  if (connectedRotorPtr != nullptr) {
    connectedRotorPtr->charBeforeMapping = this->charAfterMapping;
  }
  else if (connectedPlugboardPtr != nullptr) {
    connectedPlugboardPtr->charBeforeMapping = this->charAfterMapping;
  }
}

