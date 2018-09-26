/*!
	\file
	Program is helper with laba 1.1.1
	
        
	//\details
	It read file with data, process them and write result in file.
	The data in the file should be stored in the format of 36 pairs of numbers of current and voltage.
	If input or output file not fond, program print you error.
	If the data file has error, then program print you error in the file.
*/

#include <stdio.h>
#include <assert.h>

//===================================================================================

                                        ///Number all measurements
const int NMeas        = 12;            
                                        ///Square of wire (10^(-4) ñm^2)
const int Square       = 10;            
                                        ///Long of wire 1 (cm)
const float Length1    = 20;            
                                        ///Long of wire 2 (cm)
const float Length2    = 30;            
                                        ///Long of wire 3 (cm)
const float Length3    = 50;            
                                        ///Max value ampermeter (mA)
const int MaxI         = 301;           
                                        ///Max value voltmeter (mV)
const int MaxU         = 601;
                                        ///Tolerance 
const float Tolerance  = 1e-4;           


int Compute (FILE* input, FILE* output, float length);
void ZeroArray (float array[], int size);
int Read (float U[], float I[], int num, FILE* input);
void CountResistivity (float U[], float I[], float R[], float length);
void Print (float R[], FILE* output, float length);
int Message (FILE* input, FILE* output);
int CheckData (float U[], float I[]);

int main ()
    {
    printf ("#Laba 1.1.1 \n"
                "#(c) Kirill Shcherbina 2k!8\n\n");

    FILE* input  = fopen ("laba.txt",        "r");
    FILE* output = fopen ("Babka-labka.txt", "w");
	
    int message = Message (input, output);

    assert ((message == 0) || (message == 1));
        
    if (message == 1) return (1);  
    
    int correct1 = Compute (input, output, Length1);

    assert ((correct1 == 0) || (correct1 == 1));

    if (correct1 == 1) return (1);
   
    int correct2 = Compute (input, output, Length2);

    assert ((correct2 == 0) || (correct2 == 1));

    if (correct2 == 1) return (1);

    int correct3 = Compute (input, output, Length3);
    
    assert ((correct3 == 0) || (correct3 == 1));
    
    if (correct3 == 1) return (1);
 
    printf ("Result saved in file 'Babka-labka.txt'\n");

    fclose (input);
    fclose (output);    
    
    return (0);
    	
    }

//=============================================================================

/*!
    \brief
    Funcion that process data and print result 
    \param input
    Input file
    \param output
    Output file
    \param length
    Length of wire 
*/

int Compute (FILE* input, FILE* output, float length)
    {
    assert ((Length1 <= length) && (length <= Length3));

    float U[NMeas], I[NMeas], R[NMeas];
    ZeroArray (U, NMeas);
    ZeroArray (I, NMeas);
    ZeroArray (R, NMeas);
	
    int checkread = Read (U, I, NMeas, input);
    
    assert (checkread >= 0);

    if (checkread > 0)
        {
        printf ("ERROR in data\n");
        return (1); 
        }
    
    int check = CheckData (U, I);	

    assert (check >= 0);

    if (check > 0)
        {
        printf ("ERROR in data\n");
        return (1); 
        }
		
    CountResistivity (U, I, R, length);
    Print (R, output, length);
	
    return (0);

    }

//=============================================================================

/*!
    \brief
    Funcion that zero array 
    \param array[] 
    Input array
    \param size 
    Size of array
*/ 

void ZeroArray (float array[], int size)
    {
    assert (size > 0);

    for (int i = 0; i < size; i++)
        {
        assert (0 <= i && i < size);

        array[i] = 0;
	}
    }

//=============================================================================

/*!
    \brief
    Funcion that read file 
    \param U[]
    Array with value of voltage
    \param I[] 
    Array with value of current
    \param size 
    Number of value
    \param input 
    Input file
*/ 

int Read (float U[], float I[], int size, FILE* input)
    {
    assert (size > 0);

    for (int line = 0; line < size; line++)
	{
	assert ((0 <= line) && (line < size));

        int check = fscanf (input, "%f %f", &U[line], &I[line]);
    
        if (check != 2) return (1);
         
	}
    return (0);
    }

//=============================================================================

/*!
    \brief
    Funcion that check data
    \param U[]
    Array with value of voltage
    \param I[] 
    Array with value of current
*/ 

int CheckData (float U[], float I[])
    {	
    for (int line = 0; line < NMeas; line++)
        {
	assert ((0 <= line) && (line < NMeas));

	if ((U[line] < 0) || ((I[line] > 0.0 - Tolerance) && (I[line] < 0.0 + Tolerance)) || (U[line] > MaxU + Tolerance) || (I[line] > MaxI + Tolerance))
	    {  
            printf ("%f", U[line]);
	    return (1);
	    }
        }

    return (0);
    }
    
//=============================================================================

/*!
    \brief
    Funcion that compute resistivity 
    \param U[]
    Array with value of voltage
    \param I[] 
    Array with value of current
    \param R[] 
    Array with value of resistivity	
    \param length 
    Long of wire
*/ 

void CountResistivity (float U[], float I[], float R[], float length)
    {
    assert ((Length1 <= length) && (length <= Length3));

    for (int i = 0; i < NMeas; i++)
        {
	assert ((0 <= i) && (i < NMeas));

	R[i] = U[i] * Square/(I[i] * length);
        }
    }

//=============================================================================

/*!
    \brief
    Funcion that print result 
    \param R[] 
    Array with value of resistivity
    \param output
    Output file	
    \param length 
    Long of wire
*/ 

void Print (float R[], FILE* output, float length) 
    {
    assert ((Length1 <= length) && (length <= Length3));

    fprintf (output,"\nValue resistivity for %3.0fcm\n", length);
    for (int i = 0; i < NMeas; i++)
	{
	assert ((0 <= i) && (i < NMeas));
		
        if ((i + 1) < 10)    fprintf (output,"%d.  %5.2f *10^(-4) Om*cm^2\n",i + 1, R[i]);
				
	else    fprintf (output,"%d. %5.2f *10^(-4) Om*cm^2\n",i + 1, R[i]);
                        
	}
    }

//=============================================================================

/*!
    \brief
    Funcion that print ERROR if files weren't opened
    \param input
    Input file
    \param output
    Output file
*/ 

int Message (FILE* input, FILE* output)
    {
    if ((input == NULL) && (output == NULL))
        {
        printf ("File 'laba.txt' and 'Babka-labka.txt' weren't opend\n");
        return (1);        
        }

    if (input == NULL) 
        {
        printf ("File 'laba.txt' wasn't opend\n");
        return (1);        
        }

    if (output == NULL)
        {
        printf ("File 'Babka-labka.txt' wasn't opend\n");
        return (1);           
        }

    return (0);
    }
