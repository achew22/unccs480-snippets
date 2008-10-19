#include <fstream>
#include <stdio.h>
#include "textureManager.h"

extern TextureManager* TextureManager::pinstance = NULL;

/**
 * @param infile is a pointer to the FILE to read from
 * @return Returns the ushort read from the file
 */
unsigned short Texture::getShort(FILE* infile) {
	char ic;
	unsigned short ip;
	ic = fgetc(infile);
	ip = ic;
	ic = fgetc(infile);
	ip |= ((unsigned short)ic << 8);
	return ip;
}

/**
 * @param infile is a pointer to the FILE to read from
 * @return Returns the ulong read from the file
 */
unsigned long Texture::getLong(FILE* infile) {
	char ic = 0;
	unsigned long ip = 0;
	unsigned char uc = ic;
	ic = fgetc(infile);
	uc = ic;
	ip = uc;
	ic = fgetc(infile);
	uc = ic;
	ip |= ((unsigned long)uc << 8);
	ic = fgetc(infile);
	uc = ic;
	ip |= ((unsigned long)uc << 16);
	ic = fgetc(infile);
	uc = ic;
	ip |= ((unsigned long)uc << 24);
	return ip;
}

/**
 * This function heavily based off of a funciton in the book
 * "Computer Graphics Using OpenGL" by F.S. Hill, Jr.
 *
 * This function takes in a filename, then loads the BMP file
 * given by the filename. It puts all of the pixels for the
 * file into the pixels array. Use with setTexture
 * to get a useful texture out of it. This is a private function
 * which should only be called by TextureManager.
 *
 * @param filename is the name of the bitmap file to load.
 * @return Returns 1 if successful, 0 otherwise.
 */
int Texture::loadBMP(std::string filename) {
	FILE* infile = NULL;
	infile = fopen(filename.c_str(), "rb");
	if (!infile) {
		printf("OH NO!\n");
		return 0;
	}

	int k, row, col, numPadBytes, nBytesInRow;
	char ch1, ch2;

    //This next part reads in some header file information
	BMPHeader header;
	fread(&header, 14, 1, infile);

    //And this is where we get all of the info that we are actually interested in
    BMPInfoHeader infoHeader;
    fread(&infoHeader, 40, 1, infile);

	if(infoHeader.bitsPerPixel != 24) {
		printf("Error, should be 24 bits\n");
		fclose(infile);
		return 0;
	}

	nBytesInRow = ((3 * infoHeader.nCols + 3)/4) * 4;
	numPadBytes = nBytesInRow - 3 * infoHeader.nCols;
	numRows = infoHeader.nRows;
	numCols = infoHeader.nCols;
	pixels = new texRGB[numRows * numCols];

    //Read data in to pixels
	long count = 0;
	char dum;
	for (row = 0; row < numRows; row++) {
		for (col = 0; col < numCols; col++) {
			char r,g,b;
			b = fgetc(infile);
			g = fgetc(infile);
			r = fgetc(infile);
			pixels[count].r = r;
			pixels[count].g = g;
			pixels[count].b = b;
			count++;
		}
		for (k = 0; k < numPadBytes; k++) {
			dum = fgetc(infile);
		}
	}
	fclose(infile);
	return 1;
}

/**
 * This function heavily based off of a funciton in the book
 * "Computer Graphics Using OpenGL" by F.S. Hill, Jr.
 *
 * This function sets up the texture information stored in pixels
 * so that a valid texture is created in OpenGL's memory. After
 * creating the texture, releases the memory which was being held
 * by pixels. This is a private function
 * which should only be called by TextureManager.
 *
 * @param textureName is a GLuint which will be the identifier for this texture according to OpenGL.
 */
void Texture::setTexture(GLuint textureName) {
    name = textureName;
    glBindTexture(GL_TEXTURE_2D, textureName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, numCols, numRows, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    //Now that we've set the texture, we can actually delete pixels
    delete [] pixels;
}

/**
 * A static function to automatically free all memory used
 * by TextureManager. Deletes all textures from local memory
 * and the graphics card memory. Essentially gives you a
 * fresh, new class.
 */
void TextureManager::cleanUp() {
    TextureManager* instance = pinstance;
    for (std::map<std::string, Texture*>::iterator iter = instance->texMap.begin(); iter != instance->texMap.end(); iter++) {
        if (iter->second != NULL) {
            delete iter->second;
        }
    }
    instance->texMap.clear();
    instance->texNameMap.clear();
}

/**
 * Always and only use this function in order to obtain
 * an instance of TextureManager. Do not use the constructor,
 * as this is a singleton class.
 *
 * @return Returns a pointer to an instance of TextureManager.
 */
TextureManager* TextureManager::getInstance() {
    if (TextureManager::pinstance == NULL) {
        pinstance = new TextureManager;
    }
    return pinstance;
}

/**
 * Always and only use this function in order to obtain a
 * valid OpenGL texture. It checks to see if the file mentioned
 * by filename has already been loaded. If it has, it simply
 * returns an ID handle for that texture. If not, it will
 * create one.
 *
 * @param filename is the name of the file to load.
 * @return Returns an ID for a valid OpenGL texture.
 */
int TextureManager::loadBMP(std::string filename) {
    std::map<std::string, Texture*>::iterator toFind = texMap.find(filename);
    if (toFind != texMap.end()) {
        toFind->second->numUsers++;
        printf("Number of users increased: now %i\n", toFind->second->numUsers);
        return toFind->second->name;
    } else {
        Texture* newTexture = new Texture;
        newTexture->loadBMP(filename);
        newTexture->numUsers++;
        newTexture->filename = filename;
        glGenTextures(1, &(newTexture->name));
        newTexture->setTexture(newTexture->name);

        texMap[filename] = newTexture;
        texNameMap[newTexture->name] = newTexture;

        printf("Number of users set: now %i\n", newTexture->numUsers);
        return newTexture->name;
    }
}

/**
 * This function should be used when one no longer has need for a
 * texture. Because TextureManager keeps track of how many times
 * a given texture has been requested, it will determine whether
 * or not the texture is still in use, and delete/not delete it
 * accordingly. To be effective, must be called every time a
 * given object no longer needs the use of a texture.
 *
 * @param filename is the name of the file from which the bitmap was loaded.
 */
void TextureManager::unLoadBMP(std::string filename) {
    std::map<std::string, Texture*>::iterator toFind = texMap.find(filename);
    if (toFind == texMap.end()) {
        return;
    } else {
        toFind->second->numUsers--;
        printf("Number of users decreased: now %i\n", toFind->second->numUsers);
        if (toFind->second->numUsers <= 0) {
            texNameMap.erase(toFind->second->name);
            delete toFind->second;
            texMap.erase(toFind);
            printf("Texture erased\n");
        }
    }
}

/**
 * This function should be used when one no longer has need for a
 * texture. Because TextureManager keeps track of how many times
 * a given texture has been requested, it will determine whether
 * or not the texture is still in use, and delete/not delete it
 * accordingly. To be effective, must be called every time a
 * given object no longer needs the use of a texture.
 *
 * @param id is the id of the texture to be freed.
 */
void TextureManager::unLoadBMP(int id) {
    std::map<int, Texture*>::iterator toFind = texNameMap.find(id);
    if (toFind == texNameMap.end()) {
        return;
    } else {
        toFind->second->numUsers--;
        printf("Number of users decreased: now %i\n", toFind->second->numUsers);
        if (toFind->second->numUsers <= 0) {
            texMap.erase(toFind->second->filename);
            delete toFind->second;
            texNameMap.erase(toFind);
            printf("Texture erased\n");
        }
    }
}
