#include <iostream>
#include <fstream>
#include "components.h"

using namespace std;

/*constructor for rotor*/
Rotor::Rotor(int rotorId, Rotor* rightRotorPtr)
  : rotorId(rotorId), rightRotorPtr(rightRotorPtr) {
  
  if (rightRotorPtr != nullptr) {
    rightRotorPtr->leftRotorPtr = this;
  }
}

/*Init config for rotor*/
void Rotor::initConfig(char* rotFilename, char* posFilename) {

  ifstream rotIn;
  int rotorRightSignal = 0;
  int rotorLeftSignal;
  int notchValue;
  int numberOfNotches = 0;
  rotIn.open(rotFilename);
  
  //configure internal wirings
  while (rotorRightSignal<26 && !(rotIn >> rotorLeftSignal).fail()) {
    shifts[rotorRightSignal] = rotorLeftSignal-rotorRightSignal;
    rotorRightSignal++;
  }
  //configure the turnover notches
  while (numberOfNotches<26 && !(rotIn >> notchValue).fail()) {
    currentNotchPositions[notchValue] = 1;
    numberOfNotches++;
  }
  rotIn.close();
  //configure the initial position
  ifstream posIn;
  int initialPos = 0;
  int count = 0;
  posIn.open(posFilename);
  while (!(posIn >> initialPos).fail() && (count != rotorId)) {
    count++;
  }
  posIn.close();
  rotate(initialPos, true);
  
}

/*Rotate current rotor by numberOfRotations times, neglect turnover effect when setting up*/
void Rotor::rotate(int numberOfRotations, bool settingUp) {

  //rotate current rotor
  rotationsPerformed += numberOfRotations;
  rotationsPerformed %= 26;

  if (currentNotchPositions[rotationsPerformed] && !settingUp && leftRotorPtr!=nullptr) {
    leftRotorPtr->rotate(1, false);
  }
}

/*shift[adj_i] corresponds to the shift AFTER rotation, where adj_i is the adjusted index*/
int Rotor::mapLeftToRight(int code) {
  int toFind; // the char on the LHS of the rotor ie the char that we need to find
  int adj_i; // relative frame of reference to deal with rotations performed
  for (int i=0; i<26; i++) {
    adj_i = (i + rotationsPerformed)%26;
    toFind = (i + shifts[adj_i])%26;
    if (toFind < 0)
      toFind += 26;
    if (toFind==code) {
      code = i;
      break;
    }
  }
  return code;
}

/*Maps character and send to the left rotor*/
int Rotor::mapRightToLeft(int code) {
  int i = (code + rotationsPerformed)%26;
  code = (code + shifts[i])%26;
  // Wrap around(+26) if negative output
  return  (code<0) ? (code+26) : (code);
}

/*Getter functions to get the pointer to the next or prev rotor*/
Rotor* Rotor::getLeftPtr() {
  // can return nullptr
  return this->leftRotorPtr;
}

Rotor* Rotor::getRightPtr() {
  // can return null ptr
  return this->rightRotorPtr;
}


