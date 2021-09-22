#include <string>
using namespace std;

class remoteMutex
{
private:
    /* data */
public:
    remoteMutex(string coord_addr);
    int acquire();
    int release();
    ~remoteMutex();
};
