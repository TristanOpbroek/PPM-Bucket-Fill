/** *********************************************************************
 * @file
 *
 * @brief   Memory allocation / free operations.
 ***********************************************************************/
#include "netpbm.h"

 /** *********************************************************************
  * @author Tristan Opbroek
  *
  * @par Description: This function takes a 2d, and allocates 2 arrays
  * based on the cols and rows of an image. It will throw a warning message and terminate safely if it fails.
  *
  *
  * @param[out] ptr[][] - a 2d array of points
  * @param[in] cols - cols of 2d array
  * @param [in] rows - rows of 2d array
  * @param [in] ptr - ptr to pixel array
  * @par Example:
	@verbatim
  * pixel** cPtr;
  * int rows = 3
  * int cols = 3
  * createArray(cPtr, cols, rows);
  * cPtr[2][2] // created 3x3 array.
	@endverbatim
  ***********************************************************************/
void createArray(pixel**& ptr, int cols, int rows)
{
	//Allocated number of rows
	ptr = new (nothrow) pixel * [rows];
	if (ptr == nullptr)
	{
		cout << "Memory Allocation Error" << endl;
		exit(0);
	}

	//------------------------------------------------------- plz


	//Allocate each row it's own columns
	for (int i = 0; i < rows; i++)
	{
		ptr[i] = new (nothrow) pixel[cols];
		if (ptr[i] == nullptr)
		{
			cout << "Memory Allocation Error" << endl;
			exit(0);
		}
	}
}
/** *********************************************************************
 * @author Tristan Opbroek
 *
 * @par Description: This function allocates memory for a 2D array of bools
 * based on the cols and rows of an image. It will throw a warning message and terminate safely if it fails.
 *
 *
 * @param[out] ptr[][] - a 2d array of points
 * @param[in] cols - cols of 2d array
 * @param [in] rows - rows of 2d array
 * @param[in] ptr - Ptr to boolean array.
 * @par Example:
   @verbatim
 * bool** cPtr;
 * int rows = 3
 * int cols = 3
 * createArray(cPtr, cols, rows);
 * cPtr[2][2] // created 3x3 array.
   @endverbatim
 ***********************************************************************/
void createBoolArray(bool**& ptr, int cols, int rows)
{
	ptr = new (nothrow) bool* [rows];
	if (ptr == nullptr)
	{
		cout << "Memory Allocation Error" << endl;
		exit(0);
	}
	for (int i = 0; i < rows; i++)
	{
		ptr[i] = new (nothrow) bool[cols];
		if (ptr[i] == nullptr)
		{
			cout << "Memory Allocation Error" << endl;
			exit(0);
		}
	}
}
/** *********************************************************************
 * @author Tristan Opbroek
 *
 * @par Description: 
 * Cleans up memory allocated for this program.
 *
 * @param[in] arr1 - 2D boolean arry
 * @param [in] picture - Image, and accompanying arrays.
 * @par Example:
   @verbatim
 * bool** cPtr;
 * image picture;
 * cleanUp(cPtr, picture); //Frees up memory
   @endverbatim
 ***********************************************************************/
void cleanUp(bool** arr1, image picture)
{
	delete [] arr1;
	delete [] picture.blue;
	delete [] picture.redgray;
	delete[] picture.green;
}