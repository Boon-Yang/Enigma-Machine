class Plugboard;
class Rotor;
class Reflector;
class Enigma;


class Rotor { 
  int rotationsPerformed = 0; //Helps to avoid actually rotating the mappings
  int currentNotchPositions[26] = {0};

  /*Internal Mappings and the state of encoding before and after mappping*/
  int charBeforeMapping;
  int charAfterMapping;
  int shifts[26];

  int rotorId = 0; //To facilitate setting up config

  /*Pointers to the connected components, all of these components are optional*/
  Rotor* rightRotorPtr = nullptr;
  Rotor* leftRotorPtr = nullptr;
  Reflector* connectedReflectorPtr = nullptr;
  Plugboard* connectedPlugboardPtr = nullptr;

public:
  /*Gives permission to plugboard and reflector to change the state of characters*/
  friend class Plugboard;
  friend class Reflector;
  
  /*Rotor Constructor*/
  Rotor(int rotorId, Rotor* rightRotorPtr=nullptr);

  /*initConfig initiates the settings of rotor*/
  void initConfig(char* rotFilename, char* posFilename);

  /*rotate updates the rotary state of the rotor(update rotationsPerformed), turnover when not setting up*/
  void rotate(int numberOfRotations, bool settingUp);

  /*backward encoding*/
  void mapLeftToRight();

  /*forward encoding*/
  void mapRightToLeft();

  /*Getters for the left and right rotor pointer, used when connecting the rotors in Enigma*/
  Rotor* getRightPtr();
  Rotor* getLeftPtr();

};


//----------------------------Reflector--------------------------------------
class Reflector {

  /*mappings and state of characters before and after mapping*/
  int mappings[26];
  int charBeforeMapping;
  int charAfterMapping;

  /*Pointers to connected components, both rotor and plugboard(zero rotor case) are optional*/
  Rotor* connectedRotorPtr = nullptr;
  Plugboard* connectedPlugboardPtr = nullptr;
public:
  friend class Rotor;
  friend class Enigma;
  friend class Plugboard;
  /*Constructor for reflector*/
  Reflector(Rotor* connectedRotorPtr);

  /*Helps to initiate the configuration of a reflector*/
  void initConfig(char* rfFilename);

  /*execute the reflection*/
  void reflectToNextComponent();
};

//------------------------------Plugboard-------------------------------------------
class Plugboard {
  /*initialise mappings with default case without plugcables*/
  int mappings[26] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,14, 15, 16, 17, 18, 19, 20, 21, 22,23, 24, 25};

  /*state of encodings before and after encoding*/
  int charBeforeMapping;
  int charAfterMapping;

  /*Pointers to rotor and reflector (in case of zero rotor)*/
  Rotor* connectedRotorPtr = nullptr;
  Reflector* connectedReflectorPtr = nullptr;

public:
  friend class Rotor;
  friend class Reflector;
  friend class Enigma;

  /*constructor of plugboard*/
  Plugboard(Rotor* connectedRotor=nullptr, Reflector* connectedReflector=nullptr);

  /*helps to set up the configuration of plugborad*/
  void initConfig(char const* filename);

  /*send the mapped encoding to either rotor or reflector*/
  void sendToNextComponent();

  /*Input(from the input keyboard) and output(encoding to the output board)*/
  void readAndComputeSignal(char ch);
  void outputSignal();
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
