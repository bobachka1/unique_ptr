#include <iostream>
#include "unique_ptr.h"

int main()
{
	unique_ptr<int> a(new int(1));
    unique_ptr<int> b;
    b.swap(a);
    b.reset();
    return 0;
}