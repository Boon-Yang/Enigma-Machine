class Plugboard;
class Rotor;
class Reflector;
class Enigma;


class Rotor { 
  int rotationsPerformed = 0; //Helps to avoid actually rotating the mappings
  int currentNotchPositions[26] = {0};

  /*Internal Mappings and the state of encoding before and after mappping*/
  int shifts[26];

  int rotorId = 0; //To facilitate setting up config

  /*Pointers to the connected components, all of these components are optional*/
  Rotor* rightRotorPtr = nullptr;
  Rotor* leftRotorPtr = nullptr;

public:
  /*Gives permission to plugboard and reflector to change the state of characters*/
  
  /*Rotor Constructor*/
  Rotor(int rotorId, Rotor* rightRotorPtr=nullptr);

  /*initConfig initiates the settings of rotor*/
  void initConfig(char* rotFilename, char* posFilename);

  /*rotate updates the rotary state of the rotor(update rotationsPerformed), turnover when not setting up*/
  void rotate(int numberOfRotations, bool settingUp);

  /*backward encoding*/
  int mapLeftToRight(int code);

  /*forward encoding*/
  int mapRightToLeft(int ch);

  /*Getters for the left and right rotor pointer, used when connecting the rotors in Enigma*/
  Rotor* getRightPtr();
  Rotor* getLeftPtr();

};


//----------------------------Reflector--------------------------------------
class Reflector {

  /*mappings and state of characters before and after mapping*/
  int mappings[26];

public:


  /*Helps to initiate the configuration of a reflector*/
  void initConfig(char* rfFilename);

  /*execute the reflection*/
  int reflectToNextComponent(int code);
};

//------------------------------Plugboard-------------------------------------------
class Plugboard {
  /*initialise mappings with default case without plugcables*/
  int mappings[26] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,14, 15, 16, 17, 18, 19, 20, 21, 22,23, 24, 25};

public:
  /*helps to set up the configuration of plugborad*/
  void initConfig(char const* filename);

  /*send the mapped encoding to either rotor or reflector*/
  void sendToNextComponent(int encoding);

  /*Input(from the input keyboard) and output(encoding to the output board)*/
  int readAndComputeSignal(char ch);
  void outputSignal(int code);
};

//--------------------------------Enigma-------------------------------------------
class Enigma {

  /*Rotors*/
  Rotor* leftmostRotorPtr = nullptr;
  Rotor* rightmostRotorPtr = nullptr;
  int numberOfRotors = 0;

  /*Reflector*/
  Reflector* reflector;

  /*Plugboard*/
  Plugboard* plugboard;

  /*Internal functions*/
  void initRotors(char** argv);
  Rotor* createRotor(int rotorId, Rotor* rotorPtr=nullptr);
  void initReflector(char** argv);
  void initPlugboard(char** argv);
public:
  /*Constructor of an enigma machine*/
  Enigma(int argc, char** argv);

  /*encode the input from the user(enigma machine operator)*/
  void encode(char ch);

  /*For Debugging*/
  void checkRotorsConnectedProperly();

  /*Destructor: free all memory allocated on heap(by utilising on the components ptr)*/
  ~Enigma();
};
