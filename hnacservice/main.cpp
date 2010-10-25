#include "service.h"

int main(int argc, char *argv[])
{
    Service service(argc, argv);
    return service.exec();
}
