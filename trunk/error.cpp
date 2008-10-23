#include <stdarg.h>
#include <string>

#include "error.h"


//Init it to 0 so that we don't freak out
Error * Error::pinstance = 0;

/**
 * Get the current instance
 */
extern Error * Error::getInstance() {
    if (Error::pinstance == 0) {
        Error::pinstance = new Error();
    }
    return Error::pinstance;
}


/**
 * Constructor
 */
Error::Error() {
    level = 2;
}

/**
 * Write a line to the file NOT DONE
 */
void Error::writeLine(char * output) {
    //printf("TODO: implement output in %s on line %d\n", __FILE__, __LINE__);
}


/**
 * Take in arguments like a printf,
 *
 * \n\n this is POSSIX shell color 41 (Red back, white text)
 *
 * @param theError The text to output in the error
 */
void Error::error(std::string theError, ...) {
    //Take all the args in, type cast them do all that fun stuff then put it in a buffer of 1024 chars (no mas please
    const char * msg = theError.c_str();
    va_list args;
    va_start(args,msg);
    char buffer[1024];
    char writeBuffer[1024];
    vsprintf(buffer,msg,args);

    #ifdef _WIN32
        sprintf(writeBuffer, "%s: %s", __FUNCTION__, buffer);
    #else
        sprintf(writeBuffer, "%s: \033[41;1m%s %s\033[0m\r\n", __FUNCTION__, buffer);
    #endif
    printf(writeBuffer);
    Error::getInstance()->writeLine(buffer);
}
/**
 * Warn the user of whatever happened
 *
 * \n\n this is POSSIX shell color 43 (Orange with white text)
 *
 * @param theError The text to output in the error
 */
void Error::warn(std::string theError, ...) {
    //Take all the args in, type cast them do all that fun stuff then put it in a buffer of 1024 chars (no mas please
    const char * msg = theError.c_str();
    va_list args;
    va_start(args,msg);
    char buffer[1024];
    char writeBuffer[1024];
    vsprintf(buffer,msg,args);

    #ifdef _WIN32
        sprintf(writeBuffer, "%s: %s", __FUNCTION__, buffer);
    #else
        sprintf(writeBuffer, "%s: \033[43;1m%s %s\033[0m\r\n", __FUNCTION__, buffer);
    #endif
    printf(writeBuffer);
    Error::getInstance()->writeLine(buffer);
}
/**
 * Log something for us
 *
 * \n\n this is POSSIX shell color  34 (Blue text)
 *
 * @param theError The text to output in the error
 */
void Error::log(std::string theError, ...) {
    //Take all the args in, type cast them do all that fun stuff then put it in a buffer of 1024 chars (no mas please
    const char * msg = theError.c_str();
    va_list args;
    va_start(args,msg);
    char buffer[1024];
    char writeBuffer[1024];
    vsprintf(buffer,msg,args);

    #ifdef _WIN32
        sprintf(writeBuffer, "%s: %s", __FUNCTION__, buffer);
    #else
        sprintf(writeBuffer, "%s: \033[34;1m%s %s\033[0m\r\n", __FUNCTION__, buffer);
    #endif
    printf(writeBuffer);
    Error::getInstance()->writeLine(buffer);
}
/**
 * Put out some debug info (not included in release version)
 *
 * \n\n this is POSSIX shell color 37
 *
 * @param theError The text to output in the error
 */
void Error::debug(std::string theError, ...) {
    //Take all the args in, type cast them do all that fun stuff then put it in a buffer of 1024 chars (no mas please
    const char * msg = theError.c_str();
    va_list args;
    va_start(args,msg);
    char buffer[1024];
    char writeBuffer[1024];
    vsprintf(buffer,msg,args);

    #ifdef _WIN32
        sprintf(writeBuffer, "%s: %s", __FUNCTION__, buffer);
    #else
        //sprintf(writeBuffer, "%s: \033[37;1m%s %s\033[0m\r\n", __FUNCTION__, buffer);
        sprintf(writeBuffer, "%s: %s", __FUNCTION__, buffer);
    #endif
    printf(writeBuffer);
    Error::getInstance()->writeLine(buffer);
}
