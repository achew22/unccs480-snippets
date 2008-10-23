#include <string>

//#include "socketConnection.h"
#include "HTTPConnection.h"

/**
 * Create it an init port to 0
 */
HTTPConnection::HTTPConnection() {
    port = 0;
}

/**
 * Download a file via HTTP and put it out in a string
 *
 * @param URI to get the file from
 */
std::string HTTPConnection::get(std::string URL) {

    int i = 0;
    if (URL.substr(0,7) == "http://") {
        i = 7;
    }

    /**
        This code parses the URL and turns it into port, path and host

        Proper format for a url is http://www.domain.com:123/path/to/whatever/you/want

        where 123 is the port you want to connect on
    */
    std::string path;
    bool foundSlash = false;
    while((!foundSlash) && (i < URL.length())) {
        if (URL[i] == '/') {
            foundSlash = true;
            break;
        } else if (URL[i] == ':') {
            i++;
            while ((i < URL.length()) && (URL[i] != '/')) {
                port = port * 10;
                port += URL[i] - '0';
                i++;
            }
            //Since we step again at the bottom of this step back so that it doesn't mess it up
            i--;
        } else {
            host += URL[i];
        }
        i++;
    }

    while (i < URL.length()) {
        path += URL[i];
        i++;
    }


    if (connect()) {
        char requestString[2048];
        sprintf(requestString, "GET http://%s%s HTTP/1.1\n\n", host.c_str(), path.c_str());
        if (write(requestString)){
            //Pull some data
            std::string buffer;
            bool done = false;
            std::string fileBody;
            while (!done) {
                //We are reading still
                buffer = read(1024);
                fileBody += buffer;
                if (buffer.length() != 1024) {
                    done = true;
                }


            }

            //We have everything
            return fileBody.substr(fileBody.find("\r\n\r\n", 0) + 4);
        }
    }

    //Bad things happend
    return "";

}
