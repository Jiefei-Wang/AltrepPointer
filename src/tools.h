#include <string>
using std::string;
#define DEBUG(x) x;
#define charPtr(x) CHAR(Rf_asChar(x))
#define PACKAGE_NAME "AltrepPointer"

void errorHandle(std::string msg);
void errorHandle(const char* fmt, ...);
void warningHandle(std::string msg);
void warningHandle(const char* fmt, ...);

void messageHandle(std::string msg);
void messageHandle(const char* fmt, ...);