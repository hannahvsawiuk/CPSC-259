/*
 File:              pagerank.c
 Purpose:           Computes the page ranks of a web of hyperlinked pages using MATLAB
 Author:			Hannah Sawiuk & Joseph Canlas
 Student #s:		51196160 & 35276154
 CS Accounts:		s1u0b & b8j0b
 Date:				December 5th, 2016
 */ 

#include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include "engine.h"

#define  BUFSIZE 256 //change that to big number
#define  web "web.txt"

//function prototypes
int matrix_dimension ( FILE * pointer );
char ** read_file ( FILE * filepointer, int dimension );

int main (void)
{
		//Variables 
		FILE * infile					= NULL;
		FILE * dim_infile				= NULL; //separate file pointer for matrix_dimension
		int error						= 0;
		int dimension					= 0;
		char ** matrix_pointer			= NULL;
		double tempMatrix				[BUFSIZE][BUFSIZE];
		double matrixone				[BUFSIZE*235]; //one dimensional array of tempMatrix
		char buffer						[BUFSIZE + 1];
		
		//counters
		int row							= 0;
		int column						= 0;
		int i							= 0;
		int j							= 0;
		int k							= 0;

		//MATLAB variables
		Engine * ep = NULL; // A pointer to a MATLAB engine object
        mxArray * result = NULL;
		mxArray * webMatrix = NULL; //declare matrices
		

		//Open file and read
		error = fopen_s(&infile, web ,"r");
		if ( error == 0) 
		{
			printf("The file was opened \n\n");
			fopen_s(&dim_infile, web ,"r");
		}
		else 
		{
			printf("The file was not opened \n");
			printf("No page ranks available to extract\n\n");
		}

		//get dimension of the file
		dimension = matrix_dimension(dim_infile); 
		
		//if file not empty, read
		if (dimension > 0)
		{
			matrix_pointer = read_file(infile, dimension);
			for(row = 0; row < dimension; row ++)
			{
				for(column = 0; column < dimension; column++)
				{
					tempMatrix[row][column] = (double)matrix_pointer[row][column]-48;
				}
			}
		}

		if (infile != NULL) 
		{
			printf("File closed\n\n");
			fclose( infile );
		}
		
		//Starts a MATLAB process 
        if ( !(ep = engOpen(NULL)) ) {
          fprintf(stderr, "\nCan't start MATLAB engine\n");
          system("pause");
          return 1;
        }

		//create matrix
		webMatrix = mxCreateDoubleMatrix(dimension, dimension, mxREAL);
	
		//one dimensionalize 2D array that contains the text file (connectivity matrix)
		for (i = 0; i < dimension; i++)
		{
			for (j = 0; j < dimension; j++)
			{
				matrixone[k] = tempMatrix[j][i]; //i and j flipped since MATLAB is columns/rows 
				k++;
			}
			j = 0;
		}
		//copy new 1D array into the matlab matrix (webMatrix)
		memcpy((void*) mxGetPr(webMatrix), (void *)matrixone, dimension*dimension*sizeof(double)); // populates webMatrix with tempMatrix
		
		//print connectivity matrix (with same format as the text file: not in the MATLAB form)
		{
		  size_t sizeOfwebMatrix = mxGetNumberOfElements(webMatrix);
          size_t a = 0;
		  size_t b = 0;
		  printf("Dimension = %d\n\n", dimension);
          printf("The Connectivity Matrix is:\n\n");
          for (a = 0; a < sizeOfwebMatrix/dimension; a++) 
		  {
			  for (b = 0; b < sizeOfwebMatrix; b+=dimension)
			  {
				   printf(" %1.0f  ", *(mxGetPr(webMatrix) + a + b) );
			  }
			  printf("\n");
		  }
		}
		 
		 printf("\n");
		
		//places the arrays into MATLAB workspace: webMatrix = connectivity matrix
		if ( engPutVariable(ep, "webMatrix", webMatrix) ) {
          fprintf(stderr, "\nCannot write webMatrix to MATLAB \n");
          system("pause");
          exit(1); // Same as return 1;
		}

		//get row and column dimensions of connectivity matrix (webMatrix)
		if ( engEvalString(ep, "[rows, columns] = size(webMatrix)")  ) {
          fprintf(stderr, "\nError calculating rows and columns  \n");
          system("pause");
          exit(1);
        }

		//gets dimension of (square) matrix: since square matrix, 1 gets dimension of row which = dimension column
		if ( engEvalString(ep, "dimension = size(webMatrix , 1)" )  ) {
          fprintf(stderr, "\nError calculating dimension  \n");
          system("pause");
          exit(1);
        }
		//calculates sum of each column and stores in array
		if ( engEvalString(ep, "columnsums = sum(webMatrix, 1)" )  ) {
          fprintf(stderr, "\nError calculating columnsums  \n");
          system("pause");
          exit(1);
        }
		//initializes probability factor
		if ( engEvalString(ep, "p = 0.85" )  ) {
          fprintf(stderr, "\nError entering probability factor \n");
          system("pause");
          exit(1);
        }
		//finds the indices of the columns whose sums are != 0
		if ( engEvalString(ep, "zerocolumns = find(columnsums~=0)" )  ) {
          fprintf(stderr, "\nError finding columnsums  zero sum indices  \n");
          system("pause");
          exit(1);
        }
		//gets inverse matrix D: gets the inverse vector of the sums. Note "." signifies that the operations will be applied to each element of the matrix
		if ( engEvalString(ep, "D = sparse( zerocolumns, zerocolumns, 1./columnsums(zerocolumns), dimension, dimension )" )  ) {
          fprintf(stderr, "\nError calculating sparse (inverse) matrix  \n");
          system("pause");
          exit(1);
        }
		//gets the stochastic matrix (transition matrix)
		if ( engEvalString(ep, "StochasticMatrix = webMatrix * D" )  ) {
          fprintf(stderr, "\nError calculating Stochastic matrix  \n");
          system("pause");
          exit(1);
        }
		//finds the zero columns in the original webMatrix
		if ( engEvalString(ep, "[row, column] = find(columnsums==0)" )  ) {
          fprintf(stderr, "\nError calculating  \n");
          system("pause");
          exit(1);
        }
		//generates the Stochastic matrix (again note "." siginifies application of operation to each element of array)
		if ( engEvalString(ep, "StochasticMatrix(:, column) = 1./dimension" )  ) {
          fprintf(stderr, "\nError generating Stochastic matrix \n");
          system("pause");
          exit(1);
        }
		//generates matrix with equal dimensions and entries of the inverse of the D matrix (the ones matrix)
		if ( engEvalString(ep, "Q = ones(dimension, dimension)" )  ) {
          fprintf(stderr, "\nError calculating  Q \n");
          system("pause");
          exit(1);
        }
		//generates Transition matrix
		if ( engEvalString(ep, "TransitionMatrix = p * StochasticMatrix + (1 - p) * (Q/dimension)" )  ) {
			fprintf(stderr, "\nError calculating Transition matrix \n");
			system("pause");
			exit(1);
		}
		//
		if ( engEvalString(ep, "PageRank = ones(dimension, 1)" )  ) {
          fprintf(stderr, "\nError calculating Page rank \n");
          system("pause");
          exit(1);
        }
		//multiplies column vector by Transition matrix until values cease to change (reach the final values)
		if ( engEvalString(ep, "for i = 1:100 PageRank = TransitionMatrix * PageRank; end" )  ) {
          fprintf(stderr, "\nError calculating for Page rank until column vectors stopped changing \n");
          system("pause");
          exit(1);
        }
		//normalize page rank vector values
		if ( engEvalString(ep, "PageRank = PageRank / sum(PageRank)" )  ) {
          fprintf(stderr, "\nError calculating normalized Page rank vector \n");
          system("pause");
          exit(1);
        }

		printf("\nRetrieving Page Ranks....\n\n");

        if ((result = engGetVariable(ep,"PageRank")) == NULL) {
          fprintf(stderr, "\nFailed to retrieve eigenvalue vector\n");
          system("pause");
          exit(1);
        } 
        else {
          size_t sizeOfResult = mxGetNumberOfElements(result);
          size_t i = 0;
          printf("The Page Ranks are:\n\n");
          for (i = 0; i < sizeOfResult; i++ ) {
            printf(" Page %d: %f \n", i+1 , *(mxGetPr(result) + i) );
          }
        }

		printf("\n");

		if ( engOutputBuffer(ep, buffer, BUFSIZE ) ) {
          fprintf(stderr, "\nCan't create buffer for MATLAB output\n");
          system("pause");
          return 1;
        }

		//finishes MATLAB
		buffer[BUFSIZE] = '\0';
		engEvalString(ep, "whos"); // whos is a handy MATLAB command that generates a list of all current variables
        printf("%s\n", buffer);
        printf("\n");
		mxDestroyArray(webMatrix);
		mxDestroyArray(result);
        webMatrix = NULL;
        result = NULL;

        if ( engClose(ep) ) {
          fprintf(stderr, "\nFailed to close MATLAB engine\n");
		  }     

		printf("Thanks :)\n\n");
		system("pause"); // So the terminal window remains open long enough for you to read it
        return 0; // Because main returns 0 for successful completion

}
	
int matrix_dimension ( FILE * pointer )
{

	char  line_buffer      [BUFSIZE];

	if (fgets( line_buffer, BUFSIZE, pointer))
	{
		return (int)strlen(line_buffer)/2;
	}
	return 0; //if NULL file*/
}

char ** read_file( FILE * file_pointer, int dimension)
{
	//counters
	int i                  = 0;
	int j				   = 0;
	int k				   = 0;
	//allocate mem space for line_buffer
	char line_buffer[BUFSIZE];

	//allocate space for the **
	char ** matrix =  (char**)calloc( dimension, dimension*sizeof(char));
										
    //allocate space for each * of the **
	for( i = 0; i < dimension; i++)
	{
		matrix[i] = ( char* )calloc( dimension, dimension*sizeof(char)); 
	}
	
	//reuse counter
	i = 0;

	//Copies the file, line by line, to line buffer using fgets in a while loop
	while ( fgets ( line_buffer, BUFSIZE, file_pointer) != NULL && i < dimension) 
	{
			for (j = 0; j < dimension*2 - 1; j++)
			{
				if(line_buffer[j] =='0' || line_buffer[j] =='1')//if element is a valid char ( '1' or '0'), copy into double**
				{
					matrix[i][k] = line_buffer[j]; 
					k++;
				}		
			}
	  i++;
	  //reuse counters
	  j = 0;
	  k = 0;
    }
	return matrix;
}
