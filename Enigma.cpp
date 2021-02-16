#include "components.h"
#include <string>
#include <iostream>
using namespace std;

/*Constructor*/
Enigma::Enigma(int argc, char** argv) {
  string rot{ ".rot" };
  string currFilename;

  /*count the number of rotors*/
  for (int i=0; i<argc; i++) {
    currFilename = ' ';
    currFilename += argv[i];
    /* if .rot is in the filename */
    if (currFilename.find(rot) != string::npos) {
      numberOfRotors++;
    }
  }
  /*init the config of components*/
  initRotors(argv);
  initReflector(argv);
  initPlugboard(argv);
}

/*Destructor: dealloates the heap memory*/
Enigma::~Enigma() {

  /*For the essential components*/
  delete plugboard;
  delete reflector;

  /*For the rotors linked list*/
  if (numberOfRotors>0) {
    Rotor* currRotorPtr = leftmostRotorPtr;
    if (leftmostRotorPtr->getRightPtr() == nullptr) {
      delete leftmostRotorPtr;
      return;
    }
    Rotor* nextRotorPtr = leftmostRotorPtr->getRightPtr();
    while (nextRotorPtr != nullptr) {
      delete currRotorPtr;
      currRotorPtr = nextRotorPtr;
      nextRotorPtr = nextRotorPtr->getRightPtr();
    }
    delete currRotorPtr;
  }
}


void Enigma::encode(char ch) {
  plugboard->readAndComputeSignal(ch);
  plugboard->sendToNextComponent();

  if (numberOfRotors>0) {
    Rotor* currRotor = rightmostRotorPtr;
    // Rotate by once before mapping the character
    currRotor->rotate(1, false);
    
    // forward rotor pass from right to left
    while (currRotor->getLeftPtr() != nullptr) {
      currRotor->mapRightToLeft();
      currRotor = currRotor->getLeftPtr();
    }

    // send and receive mapped signal from reflector
    currRotor->mapRightToLeft();
    reflector->reflectToNextComponent();
    
    // backward rotor pass from left to right
    while (currRotor != nullptr) {
      currRotor->mapLeftToRight();
      currRotor = currRotor->getRightPtr();
    }
  }
  else {
    // 0 rotor case
    reflector->reflectToNextComponent();
  }
  plugboard->outputSignal();
}



/*---------------------------Reflector related functions------------------------------*/
/*initialise the refelctor with the reflector config file*/
void Enigma::initReflector(char** argv) {
  reflector = new Reflector(leftmostRotorPtr);
  reflector->initConfig(argv[2]);
}
/*---------------------------Plugboard related function-------------------------------*/
/*initialise the plugboard with the plugboard config file*/
void Enigma::initPlugboard(char** argv) {
  plugboard = new Plugboard(rightmostRotorPtr, reflector);
  plugboard->initConfig(argv[1]);
}
/*-----------------------------Rotors related functions-------------------------------*/
/*create a new rotor and connect it to the rotor to its right, 
returns the pointer to the (currently leftmost)rotor created*/
Rotor* Enigma::createRotor(int rotorId, Rotor* RotorPtr) {
  Rotor *newRotorPtr = new Rotor(rotorId, RotorPtr);
  return newRotorPtr;
}

/*initialise rotors from left to right*/
void Enigma::initRotors(char** argv) {
  int argCount = numberOfRotors + 2;
  int rotorId = numberOfRotors - 1;
  int posFileIdx = numberOfRotors + 3;
  if (numberOfRotors>0) {
    Rotor* right; // just a temporary rotor pointer for rotors linked list connection and config initialisation
    rightmostRotorPtr = createRotor(rotorId--, nullptr);
    rightmostRotorPtr->initConfig(argv[argCount--], argv[posFileIdx]);
    right = rightmostRotorPtr;
    
    while (rotorId>=0) {
      right = createRotor(rotorId--, right);
      right->initConfig(argv[argCount--], argv[posFileIdx]);
    }
    leftmostRotorPtr = right;
  }
}


/*Debugging helper function: Ensure all rotors are connected*/
void Enigma::checkRotorsConnectedProperly() {
  if (numberOfRotors>0) {
    // check for both directions
    Rotor* right = rightmostRotorPtr;
    int rightToLeftCount=0;
    if (rightmostRotorPtr!=nullptr) {
      rightToLeftCount=1;
    }
    while (right->getLeftPtr() != nullptr) {
      rightToLeftCount++;
      right = right->getLeftPtr();
    }
    cout<< "The numbers of rotors traversing from right to left is "<<rightToLeftCount<<endl;

    Rotor* left = leftmostRotorPtr;
    int leftToRightCount = 0;
    if (leftmostRotorPtr!=nullptr) {
      leftToRightCount = 1;
    }
    while (left->getRightPtr() != nullptr) {
      leftToRightCount++;
      left = left->getRightPtr();
    }
    cout<< "The numbers of rotors traversing from left to right is "<<leftToRightCount<<endl;
  }
  else {
    cout<<"There is no rotor"<<endl;
  }
}
