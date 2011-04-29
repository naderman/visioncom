
#include "Producer.h"

#include <Ice/Ice.h>

int main(int argc, char* argv[])
{
    vision::Producer app;

    // need to create an ice pointer here to be able to use "this" in app
    Ice::ObjectPtr obj = &app;
    obj->__setNoDelete(true);

    return app.main(argc, argv);
}
