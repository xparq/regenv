#ifndef _DBG_WRAPPER_H_
#define _DBG_WRAPPER_H_

#include "import/dbg.h"

// Just a convenience test helper
#define PAUSE { cout<<"Press Enter...\n"; while (cin.get() != '\n'); }
//#define PAUSE { char pause; cin >> pause; } // <-- this will not stop at an empty line!

#endif