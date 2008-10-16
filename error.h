#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include <string>
#include <stdarg.h>

class Error {
public:
    static Error * pinstance; ///Hold the instance
    static Error * getInstance();

    int level; ///Holds the current log level

    Error();

    static void errorLevel(int newLevel);

    //File * myFile;
    static void error(std::string theError, ...);
    static void warn(std::string theError, ...);
    static void log(std::string theError, ...);
    static void debug(std::string theError, ...);

    void writeLine(char * toWrite);

};

#endif // ERROR_H_INCLUDED
