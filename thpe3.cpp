/** *********************************************************************
 * @file
 *
 * @brief This program takes an .ppm, a row and col, and 3 color channel values. It "bucket fills"
 * every same color from our inital point and replaces it with the new color provided from the 
 * 3 channel values.
 ***********************************************************************/

 /** ********************************************************************
 *
 * @mainpage THPE3 - Bucket Fill
 *
 * @section CSC 215 M02
 *
 * @authors Tristan Opbroek
 *
 * @date November 27th, 2022
 *
 * @par Instructor:
 *         Roger Schrader
 *
 * @par Course:
 *         CSC 215 - Section 2 - 1:00 pm
 *
 * @par Location:
 *         Classroom Building - Room 205
 *
 * @section program_section Program Information
 *
 * @details This program takes a .ppm file, a pixel location (Given in row and column), and a color value
 * (Given in 3 seperate color channels, red, green, blue) and "Bucket Fills" around the specified
 * pixel recursively, replacing old pixels with the new provided color. The program starts by reading
 * all pixel data into memory. After storing the entire image, it recursively steps through every connected pixel,
 * painting over them as necessary. Once the program runs out of pixels (ie. every connected, color-matching pixel
 * has been painted), then the program rewrites the image data to the originally specified file.
 *
 * @section compile_section Compiling and Usage
 *
 * @par Compiling Instructions:
 *      Stack Reserve Size & Stack Commit Size set to 4000000000; To accommodate large number of recusions.
 *
 * @par Usage:
 *  @verbatim
 *  minimal run:
 *  c:\> thpe3.exe imageFile row col redValue greenValue blueValue
 *
 * where imageFile is a valid .ppm image;
 *       row is the row of a pixel in a spot to bucket fill
 *       col is the column of a pixel in a spot to bucket fill
 *       redValue is the color of the red channel for the replacement color
 *       greenValue is the color of the green channel for the replacement color
 *       blueValue is the color of the blue channel for the replacement color
 *
 *
 *  @endverbatim
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 *
 * @bug The program wouldn't handle the same file twice, failing to process the magic number
 * in spite of the correct string being passed in. Was likely an issue with my solution file
 * and not the code and seems to have disappeared.
 * @bug The program won't handle invalid pixel positions or colors, ie. row and col -5 & -25, and color
 * R: 546, G: -5, B: 3.2.
 * @todo Although not required, the program doesn't handle aliasing in any way, so the pixel values must match
 * exactly for any bucket fill operation to have the expected result.
 *
 * @par Modifications and Development Timeline:
 *  Gitlab commit log, <a href = "https://gitlab.cse.sdsmt.edu/101078202/csc215f22programs/-/commits/main">
 *
 ***********************************************************************/

#include "netpbm.h"

 /** *********************************************************************
  * @author Tristan Opbroek
  *
  * @par Description:
  * Starting point to the program.  Based on user input it calls the necessary functions to
  * get, store, process, and then output data.
  *
  *
  * @param[in] argc - the number of arguments from the command prompt.
  * @param[in] argv - a 2d array of characters containing the arguments.
  *
  * @returns 0 -- includes text output if anything failed
  *
  * @verbatim
    see usage statement above.
    @endverbatim
  *
  ***********************************************************************/
int main(int argc, char** argv)
{
    ifstream fin;
    ofstream fout;
    image picture;
    string tempString;
    color ogColor;
    color newColor;
    bool** adjPixels;

    if (argc != 7)
    {
        cout << "Invalid Number of Arguments!" << endl;
        cout << "Usage:" << endl;
        cout << "thpe03.exe imageFile row col redValue greenValue blueValue";
        return 0;
    }

    openInput(fin, argv[1]); //Open File

    //----------------Image operations--------------
    fin >> picture.magicNumber;
    fin.ignore(1000, '\n');
    //getline(fin, picture.magicNumber, '\n'); //Extract magicnumber
    while (fin.peek() == 35) //Extract Comments
    {
        getline(fin, tempString);
        picture.comment += tempString + '\n';
    }
    getline(fin, tempString, ' '); //Get cols, put in struct
    picture.cols = stoi(tempString);
    getline(fin, tempString, '\n'); //Get rows, put in struct
    picture.rows = stoi(tempString);
    getline(fin, tempString, '\n'); //Skip Newline

    createArray(picture.redgray, picture.cols, picture.rows); //allocate memory.
    createArray(picture.green, picture.cols, picture.rows);
    createArray(picture.blue, picture.cols, picture.rows);

    if (picture.magicNumber == "P3")
    {
        getPixelsP3(picture, fin);
    }
    else if (picture.magicNumber == "P6")
    {
        getPixelsP6(picture, fin);
    }
    else
    {
        cout << "Unrecognized magic number of " << picture.magicNumber << endl;
        cout << "Note: This might be an error";
        return 0;
    }
    //----------------------------------------------------------
    newColor.redValue = stoi(argv[4]);
    newColor.greenValue = stoi(argv[5]);
    newColor.blueValue = stoi(argv[6]);
    ogColor.redValue = (int)picture.redgray[stoi(argv[2])][stoi(argv[3])];
    ogColor.greenValue = (int)picture.green[stoi(argv[2])][stoi(argv[3])];
    ogColor.blueValue = (int)picture.blue[stoi(argv[2])][stoi(argv[3])];

    createBoolArray(adjPixels, picture.cols, picture.rows);

    for (int i = 0; i < picture.rows; i++)
    {
        for (int j = 0; j < picture.cols; j++)
        {
            adjPixels[i][j] = false;
        }
    }
    //-------------------------------------
    bucketFill(picture, newColor, ogColor, stoi(argv[2]), stoi(argv[3]), adjPixels);
    fin.close();
    openOutput(fout, argv[1], picture.magicNumber);
    writeFile(picture.magicNumber, fout, picture);
    fout.close();
    cleanUp(adjPixels, picture);
    return 0;
}


//Functions go here
 /** *********************************************************************
  * @author Tristan Opbroek
  *
  * @par Description:
  * Compares two color structures and determines if they are equal.
  *
  *
  * @param[in] color1 - The first color in the comparison
  * @param[in] color2 - The second color in the comparison
  *
  * @returns true - Colors are equal
  * @returns false - Colors are not equal
  *
 * @par Example:
   @verbatim
    color c1;
    color c2;
    c1.redValue = 1
    c1.greenValue = 1
    c1.blueValue = 1
    
    c2.redValue = 1
    c2.greenValue = 1
    c2.blueValue = 1
    
    cout << isEqual(c1, c2) << endl; //Outputs true.
   @endverbatim
  ***********************************************************************/
bool isEqual(color color1, color color2)
{
    if (color1.redValue == color2.redValue &&
        color1.greenValue == color2.greenValue &&
        color1.blueValue == color2.blueValue)
        return true;
    else
        return false;
}
/** *********************************************************************
 * @author Tristan Opbroek
 *
 * @par Description:
 * Recursively steps through an image and replaces the color of all pixels of the same color that are touching, within bounds of the image.
 * 
 *
 *
 * @param[in] picture - The image struct being passed in
 * @param[in] newColor - The color to replace pixels with.
 * @param[in] ogColor - The original pixel color (target pixel)
 * @param[in] row - Target pixel row
 * @param[in] col - Target pixel col
 * @param[in] adjPixels - 2D array of used pixels.
 * 
 * 
 *
* @par Example:
  @verbatim
  image picture; //Contains good image data
  color newColor; //Contains 255, 0, 0; or the color red
  color ogCOlor; //Contains color of pixel at row, col, in image
  int row = 0; //Start bucket fill in top left corner
  int col = 0;
  bool** adjPixels; //2D array of size picture.row * picture.col, containing all false.

   cout << isEqual(c1, c2) << endl; //Outputs true.
  @endverbatim
 ***********************************************************************/
void bucketFill(image& picture, color newColor, color ogColor, int row, int col, bool**& adjPixels)
{

    color currColor;

    currColor.redValue = picture.redgray[row][col];
    currColor.greenValue = picture.green[row][col];
    currColor.blueValue = picture.blue[row][col];
    if (adjPixels[row][col] == false &&
        isEqual(currColor, ogColor))
    {
        adjPixels[row][col] = true;
        picture.redgray[row][col] = newColor.redValue;
        picture.green[row][col] = newColor.greenValue;
        picture.blue[row][col] = newColor.blueValue;

        if (row != 0)
            bucketFill(picture, newColor, ogColor, row - 1, col, adjPixels);
        if (row != picture.rows - 1)
            bucketFill(picture, newColor, ogColor, row + 1, col, adjPixels);
        if (col != 0)
            bucketFill(picture, newColor, ogColor, row, col - 1, adjPixels);
        if (col != picture.cols - 1)
            bucketFill(picture, newColor, ogColor, row, col + 1, adjPixels);
    }
    adjPixels[row][col] = true;
}
