/** *********************************************************************
 * @file
 *
 * @brief   Image input output functions
 ***********************************************************************/
#pragma once
#include "netPBM.h"

 /** *********************************************************************
  * @author Tristan Opbroek
  *
  * @par Description: This function opens a file to the ifstream fin in binary.
  *
  *
  * @param[in, out] fin - ifstream that data from file will pass through
  * @param[in] file - File that fin references.
  *
  * @par Example:
  *  @verbatim
  * ifstream fin;
  * openInput(fin, "file.txt");
  * cout << getline(fin) << endl; //fin outputs from file.txt
  *
  * @endverbatim
  ***********************************************************************/
void openInput(ifstream& fin, string file)
{
	fin.open(file, ios::in | ios::binary);

	if (!fin.is_open())
	{
		cout << "Unable to open file: " << file << endl;
		exit(0);
	}
}
/** *********************************************************************
 * @author Tristan Opbroek
 *
 * @par Description: This function opens a file to output to, in either binary or ascii
 * depending on the requested type.
 *
 *
 * @param[in, out] fout - ofstream that data from file will pass through
 * @param[in] file - File that fout references.
 * @param[in] type - type of method to output file, either --ascii or --binary

 * @par Example:
 *  @verbatim
 * ofstream fout;
 * openOutput(fout, "file.txt", "--binary"); //opens file.txt, and can now write to it in binary
 *
 * @endverbatim
 ***********************************************************************/
void openOutput(ofstream& fout, string file, string type)
{
	if (type == "P6")
	{
		fout.open(file, ios::out | ios::binary);
	}
	else if (type == "P3")
	{
		fout.open(file, ios::out);
	}
	else
	{
		cout << "Invalid Option" << endl;
		cout << "thpe01.exe [option] --outputtype basename image.ppm" << endl;
		cout << endl;
		cout << "Output Type" << endl;
		cout << " --ascii pixel values will be written in text form"
			<< endl;
		cout << " --binary pixel values will be written in binary form"
			<< endl;
		cout << endl;
		cout << "Option Code" << endl;
		cout << " --negate" << endl;
		cout << " --brighten #" << endl;
		cout << " --sharpen" << endl;
		cout << " --smooth" << endl;
		cout << " --grayscale" << endl;
		cout << " --contrast" << endl;
	}


	if (!fout.is_open())
	{
		cout << "Unable to open file: " << file << endl;
		exit(0);
	}
}


/** *********************************************************************
 * @author Tristan Opbroek
 *
 * @par Description: This function gets all of the pixel data from a
 * P6 .ppm file.
 *
 *
 * @param[in, out] picture - struct containing picture data
 * @param[in, out] fin - ifstream that data from image comes from.

 * @par Example:
 *  @verbatim
 * image picture = *some picture*
 * ifstream fin = *some image file*
 * getPixelsP6(picture, fin); //Loads picture struct with data from fin.
 * @endverbatim
 ***********************************************************************/
void getPixelsP6(image& picture, ifstream& fin)
{
	pixel temp;
	for (int i = 0; i < picture.rows; i++)
	{
		for (int j = 0; j < picture.cols; j++)
		{
			fin.read((char*)&temp, sizeof(pixel));
			picture.redgray[i][j] = temp;
			fin.read((char*)&temp, sizeof(pixel));
			picture.green[i][j] = temp;
			fin.read((char*)&temp, sizeof(pixel));
			picture.blue[i][j] = temp;
		}
	}
}


/** *********************************************************************
 * @author Tristan Opbroek
 *
 * @par Description: This function gets all of the pixel data from a
 * P3 .ppm file.
 *
 *
 * @param[in, out] picture - struct containing picture data
 * @param[in, out] fin - ifstream that data from image comes from.
 * @par Example:
 *  @verbatim
 * image picture = *some picture*
 * ifstream fin = *some image file*
 * getPixelsP3(picture, fin); //Loads picture struct with data from fin.
 * @endverbatim
 ***********************************************************************/
 //Gets pixels from ascii files DON'T TOUCH IT FINALLY WORKS
void getPixelsP3(image& picture, ifstream& fin)
{
	int temp;
	for (int i = 0; i < picture.rows; i++)
	{
		for (int j = 0; j < picture.cols; j++)
		{
			fin >> temp;
			picture.redgray[i][j] = (pixel)temp;
			fin >> temp;
			picture.green[i][j] = (pixel)temp;
			fin >> temp;
			picture.blue[i][j] = (pixel)temp;
		}
	}
}
/** *********************************************************************
 * @author Tristan Opbroek
 *
 * @par Description: This function write data from a struct to a file in the specified
 * format
 *
 *
 * @param[in] type - file type to output as, acceptable inputs as "--binary" or "--ascii", MUST match fout open type.
 * @param[in, out] fout - ofstream containing valid opened file to write to
 * @paramp[in, out] picture - Struct containing picture data
 *
 * @par Example:
 *  @verbatim
 * image picture = *some picture*;
 * string type = "--ascii";
 * ofstream fout = *some output file, opened in the same as type*
 * writeFile(type, fout, picture); //writes all data through fout to the file
 * @endverbatim
 ***********************************************************************/
void writeFile(string type, ofstream& fout, image& picture)
{
	if (type == "P6")
	{
		fout << "P6" << endl;
		fout << picture.comment;
		fout << picture.cols << " " << picture.rows << endl;
		fout << "255" << endl;
		int i = 0;
		int j = 0;
		for (int i = 0; i < picture.rows; i++)
		{
			for (int j = 0; j < picture.cols; j++)
			{
				fout.write((char*)&picture.redgray[i][j], sizeof(pixel));

				fout.write((char*)&picture.green[i][j], sizeof(pixel));

				fout.write((char*)&picture.blue[i][j], sizeof(pixel));

			}
		}
	}
	//Outputs in Ascii format DON'T TOUCH IT FINALLY WORKS uhh
	if (type == "P3")
	{
		fout << "P3" << endl;
		fout << picture.comment;
		fout << picture.cols << " " << picture.rows << endl;
		fout << "255" << endl;
		for (int i = 0; i < picture.rows; i++)
		{
			for (int j = 0; j < picture.cols; j++)
			{
				fout << (int)picture.redgray[i][j] << ' '
					<< (int)picture.green[i][j] << ' '
					<< (int)picture.blue[i][j] << endl;
			}
		}
	}
}
/** *********************************************************************
 * @author Tristan Opbroek
 *
 * @par Description: This function write data from a struct to a file in the specified
 * format, however it is specific to P5 and P2 image types, therefore grayscale.
 *
 *
 * @param[in] type - file type to output as, acceptable inputs as "--binary" or "--ascii", MUST match fout open type.
 * @param[in, out] fout - ofstream containing valid opened file to write to
 * @paramp[in, out] picture - Struct containing picture data
 *
 * @par Example:
 *  @verbatim
 * image picture = *some picture, in Grayscale*;
 * string type = "--ascii";
 * ofstream fout = *some output file, opened in the same as type*
 * writeFileGray(type, fout, picture); //writes all data through fout to the file
 * @endverbatim
 ***********************************************************************/
void writeFileGray(string type, ofstream& fout, image& picture)
{
	if (type == "--binary")
	{
		fout << "P5" << endl;
		fout << picture.comment;
		fout << picture.cols << " " << picture.rows << endl;
		fout << "255" << endl;
		int i = 0;
		int j = 0;
		for (int i = 0; i < picture.rows; i++)
		{
			for (int j = 0; j < picture.cols; j++)
			{
				fout.write((char*)&picture.redgray[i][j], sizeof(pixel));

			}
		}
	}
	//Outputs in Ascii format DON'T TOUCH IT FINALLY WORKS
	if (type == "--ascii")
	{
		fout << "P2" << endl;
		fout << picture.comment;
		fout << picture.cols << " " << picture.rows << endl;
		fout << "255" << endl;
		for (int i = 0; i < picture.rows; i++)
		{
			for (int j = 0; j < picture.cols; j++)
			{
				fout << (int)picture.redgray[i][j] << ' ';
			}
		}
	}
}