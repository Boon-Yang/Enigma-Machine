#include <iostream>
#include <fstream>
#include <cassert>
#include "components.h"

using namespace std;

/*Constructor for plugboard*/
Plugboard::Plugboard(Rotor* connectedRotorPtr, Reflector* connectedReflectorPtr)
  :connectedRotorPtr(connectedRotorPtr), connectedReflectorPtr(connectedReflectorPtr) {
  if (connectedRotorPtr != nullptr)
    connectedRotorPtr->connectedPlugboardPtr = this;
  else if (connectedReflectorPtr != nullptr)
    connectedReflectorPtr->connectedPlugboardPtr = this;
}

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

/*Send the mapped signal to the next component, either a rotor or a reflector*/
void Plugboard::sendToNextComponent() {
  if (connectedRotorPtr != nullptr)
    connectedRotorPtr->charBeforeMapping = charAfterMapping;
  else if (connectedReflectorPtr != nullptr)
    connectedReflectorPtr->charBeforeMapping = charAfterMapping;
}

/*Read in from the input keyboard and map appropriately*/
void Plugboard::readAndComputeSignal(char ch){
  charBeforeMapping = (int)ch-'A';
  charAfterMapping = mappings[charBeforeMapping];
}

/*Output to the outputboard*/
void Plugboard::outputSignal() {
  charAfterMapping = mappings[charBeforeMapping];
  cout<<(char)(charAfterMapping+'A');
}



