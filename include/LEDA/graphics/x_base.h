#undef B0
#define B0    {}

#undef B1
#define B1(T) { return (T)0; }

namespace leda {

class x_base {

public:

#include <LEDA/graphics/x_basic.h>

};

extern x_base* XPTR;

}
