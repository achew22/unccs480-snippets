#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <string>
#include <map>
#include <GL/gl.h>
#include "point.h"

/**
 * This class heavily based off of a class in the book
 * "Computer Graphics Using OpenGL" by F.S. Hill, Jr.
 *
 * Stores three chars, one each for a red, green, and blue
 * value of a given pixel. Used in combination with
 * loading bitmaps.
 */
class texRGB {
public:
    unsigned char r, g, b;  ///<The colors for red, green, and blue.
    texRGB() {  ///< Default constrctor - all zeros.
        r = g = b = 0;
    }
    texRGB(unsigned char rr, unsigned char gg, unsigned char bb) {  ///< Assignment function
        r = rr;
        g = gg;
        b = bb;
    }
};

/**
 * A structure to facilitate the easy reading of bitmaps. Nothing
 * to be concerned with or used except by Texture and TextureManager
 * classes.
 */
struct BMPHeader {
    char ch1;
	char ch2;
	unsigned long fileSize;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned long offBits;
};

/**
 * A structure to facilitate the easy reading of bitmaps. Nothing
 * to be concerned with or used except by Texture and TextureManager
 * classes.
 */
struct BMPInfoHeader {
	unsigned long headerSize;
	unsigned long nCols;
	unsigned long nRows;
	unsigned short planes;
	unsigned short bitsPerPixel;
	unsigned long compression;
	unsigned long imageSize;
	unsigned long xPels;
	unsigned long yPels;
	unsigned long numLUTentries;
	unsigned long impColors;
};

//Forward declaration
class TextureManager;

/**
 * A class for loading textures from bitmaps. This class
 * is friend to the TextureManager class, and should largely
 * only be used by said class.
 */
class Texture {
private:
    friend class TextureManager;

    texRGB* pixels; ///< All pixel data stored here.
    int numRows;    ///< Number of rows, in pixels.
    int numCols;    ///< Number of columns, in pixels.

    std::string filename;   ///< The filename that this texture was loaded from.
    GLuint name;    ///< The unique ID for this texture.
    int numUsers;   ///< The number of other objects currently using this texture.

    unsigned short getShort(FILE* infile);  ///< File reading utility function.
    unsigned long getLong(FILE* infile);    ///< File reading utility function.

    int loadBMP(std::string filename);  ///< Load a BMP file.
    void setTexture(GLuint textureName);    ///< Register the texture with OpenGL.
public:
    Texture() { ///< Default constructor.
        numRows = numCols = numUsers = name = 0;
        filename = "";
    }
    ~Texture() {    ///< Destructor, frees all memory allocated.
        freeMemory();
    }
    void freeMemory() { ///< Free up all the memory allocated.
        if (pixels != NULL) {
            delete [] pixels;
        }
        numRows = numCols = 0;
        GLuint GLid = name;
        glDeleteTextures(1, &GLid);
    }
    int getName() { ///< Return the unique texture ID.
        return name;
    }
};

/**
 * A singleton type class to manage all texture loading. It
 * ensures that the same texture is never loaded into memory
 * multiple times at once. It also keeps track of how many
 * objects are using each texture, and when the number drops
 * to zero, it frees all of the memory used. When finished,
 * remember to call TextureManager::cleanUp() to free all
 * textures left in memory.
 *
 * Important: to be used succesfully, all instances should
 * be obtained using getInstance(), and never the constructor.
 *
 * Example: Suppose that you wish to load up a texture
 * for some spaceship from the file "shipTex1.bmp". Simply
 * use the following:
 *
 * int ship1ID = TextureManager::getInstance()->loadBMP("shipTex1.bmp");
 *
 * Set the texture that want to use with glBindTexture(GL_TEXTURE_2D, ship1ID),
 * and then draw the ship. If you need to make another copy of the ship,
 * using the exact same texture, simply use the same call again.
 *
 * int ship2ID = TextureManager::getInstance()->loadBMP("shipTex1.bmp");
 *
 * Now the texture manager will make sure that the texture is only loaded
 * into memory once, so you don't have to worry about that. When one ship
 * is destroyed, you should let the TextureManager know that it is no longer
 * using the texture by calling
 *
 * TextureManager::getInstance()->unLoadBMP("shipTex1.bmp");
 *
 * or alternately,
 *
 * TextureManager::getInstance()->unLoadBMP(ship2ID);
 *
 * The texture will still be in memory, since ship1 is still using it. However,
 * if the call were made a second time, with
 *
 * TextureManager::getInstance()->unLoadBMP(ship1ID);
 *
 * (or TextureManager::getInstance()->unLoadBMP("shipTex1.bmp");)
 *
 * Then TextureManager would automatically erase all traces of that texture from
 * memory, freeing it to be used by other things. It is important that you
 * always load and unload all textures using these methods, though, otherwise
 * it will not free memory properly.
 */
class TextureManager {
private:
    static TextureManager* pinstance;   ///< Pointer to the instance of the TextureManager.
    std::map<std::string, Texture*> texMap; ///< Map associating file names with textures.
    std::map<int, Texture*> texNameMap; ///< Map associating texture IDs with textures.
public:
    TextureManager() {} ///< Default constructor.
    ~TextureManager() { ///< Deconstructor.
        TextureManager::cleanUp();
    }
    static void cleanUp();  ///< Call to clean up all textures.
    static TextureManager* getInstance();   ///< Get an instance of the TextureManager.
    int loadBMP(std::string filename);  ///< Load a bitmap from a file.
    void unLoadBMP(std::string filename);   ///< Unload a bitmap with the given filename.
    void unLoadBMP(int id); ///< Unload a bitmap whose texture ID is given.
};


#endif  //TEXTUREMANAGER_H
