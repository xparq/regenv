#ifndef __ListPart_h__
#define __ListPart_h__

const char ELEMENT_SEPARATOR = ';';

#define ENUM_CLASS
enum ENUM_CLASS LIST_TYPE { NORMAL, PATH };


// Just a convenience test helper
#define PAUSE { cout<<"Press Enter...\n"; while (cin.get() != '\n'); }
//#define PAUSE { char pause; cin >> pause; } // <-- this will not stop at an empty line!



#endif //__ListPart_h__
