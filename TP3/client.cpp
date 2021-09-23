#include <string>
using namespace std;

class remoteMutex
{
private:
    /* data */
public:
    remoteMutex(string coord_ip);
    int acquire();
    int release();
    ~remoteMutex();
};
