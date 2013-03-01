#include "PCH.h"
#include "NetworkMessage.h"

struct DelegateList
{
     bool parse(double d) { }
     int f2(double d) { }
};

typedef int (DelegateList::* DelegateType)(double d);