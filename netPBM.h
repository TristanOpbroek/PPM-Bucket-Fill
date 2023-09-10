/** *********************************************************************
 * @file
 *
 * @brief  Prototypes
 ***********************************************************************/
#pragma once
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

#ifndef __NETPBM__H__
#define __ NETPBM__H__
typedef unsigned char pixel;

/** *********************************************************************
 * @author Tristan Opbroek
 *
 * @par Description:
 * Image struct, that holds our Magic Number, Comments, rows and columns, and all of our pixel values for an image
 *
 *
 ***********************************************************************/
struct image // do not change the names of the structure
{ // you may not add other fields to this
// structure.
    string magicNumber; /**<Image type; P6 for binary, P3 for ascii>*/
    string comment; /**Comments in image file, seperated by their own newline*/
    int rows; /** Row dimension of the image*/
    int cols; /** Column dimension of the image */
    pixel** redgray; /** Array of red pixel values*/
    pixel** green;/** Array of green pixel values*/
    pixel** blue;/** Array of blue pixel values*/
};
/** *********************************************************************
 * @author Tristan Opbroek
 *
 * @par Description:
 * Color struct, Combines 3 seperate channels into one data type for easier handling.
 *
 *
 ***********************************************************************/
struct color
{
    int redValue; /** Red pixel value*/
    int greenValue;/** Green pixel value*/
    int blueValue; /** Blue pixel value*/
};

/************************************************************************
 *                         Function Prototypes
 ***********************************************************************/
void openInput(ifstream& fin, string file);
void openOutput(ofstream& fout, string file, string type);

void createArray(pixel**& ptr, int cols, int rows);
void createBoolArray(bool**& ptr, int cols, int row);

void getPixelsP6(image& picture, ifstream& fin);
void getPixelsP3(image& picture, ifstream& fin);

void writeFile(string type, ofstream& fout, image& picture);
void writeFileGray(string type, ofstream& fout, image& picture);

void cleanUp(bool** arr1, image picture);

void bucketFill(image& picture, color newColor, color ogColor, int row, int col, bool**& adjPixels);
bool isEqual(color color1, color colorc2);
#endif
