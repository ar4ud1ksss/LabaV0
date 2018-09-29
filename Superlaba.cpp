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
#include <math.h>


//===================================================================================

                                        /// Number measurements for everyone length
const int NMeas           = 12;            
                                        /// Long of wire 1 (cm)
const float Length1       = 20;            
                                        /// Long of wire 2 (cm)
const float Length2       = 30;            
                                        /// Long of wire 3 (cm)
const float Length3       = 50;            
                                        /// Max value ampermeter (mA)
const int MaxI            = 301;           
                                        /// Max value voltmeter (mV)
const int MaxU            = 601;
                                        /// Tolerance 
const float Tolerance     = 1e-4;
                                        /// Code of error №1 (problem with open files)
const int ERROR1          = 1; 
                                        /// Code of error №2 (problem with data in ibput file)
const int ERROR2          = 2;    
                                        /// String for read name of files from user
const int String          = 20;
                                        /// 1st string of 1st 12 measurements
const int NUM1            = 0;                     
                                        /// 1st string of 2nd 12 measurements
const int NUM2            = 12;
                                        /// 1st string of 3ed 12 measurments
const int NUM3            = 24;
    					                /// Max value resistance
const float MaxResistance = 10; 
    					                /// Max value error
const float MaxError      = 1; 
    					                /// Error voltmeter (mV)  
const float ErrorU        = 1.2;             		
    					                /// Error ampermeter (mA) 
const float ErrorI        = 1.2; 
                                        /// Error diametre (mm)
const float ErrorD        = 0.01;                  	
					                    /// Error length (cm)
const float ErrorL        = 0.1;             		
					                    /// Number value of diameter
const int   NumD          = 10; 
					                    /// Resistance voltmeter (Om) 
const int   RU            = 4000;  
    					                /// Number Pi
const float Pi            = 3.14;       				

//=============================================================================

int Compute (FILE* input, FILE* output, float length, int num, FILE* CSV, float square ,float errorS, float* ro, float* errorRo);
int ReadAndCheck (float U[], float I[], int size, FILE* input);
void Print (float R[], FILE* output, float length);
int CheckData (float U[], float I[]);
void PrintCSV (float U[], float I[], FILE* CSV);
int Diameter (float* s, float* errorS, FILE* diameter);
void Processing (float U[], float I[], float* sumUI, float* sumUU, float* sumII);
void Avg (float sumUI, float sumII, float* resistanceAvg);
void RandomError (float resistanceAvg, float sumUU,float sumII, float* errorRrandom);
void System (float maxU, float maxI, float resistanceAvg, float* errorRsystem);
void TotalErrorR (float errorRrandom, float errorRsystem ,float* errorR);
void TotalR (float resistanceAvg, float* rTotal, float* ro, float s, int length);
void TotalErrorRo (float ro, float errorR, float rTotal, float* errorRo, float errorS, int length);


//=============================================================================

#include "laba.h"

int main ()
    {
    printf ("#Laba 1.1.1 \n"
                "#(c) Kirill Shcherbina 2k!8\n\n");

    char fileinput [String] ;
    char fileoutput[String] ;
    char filediameter[String] ;
    char fileCSV[String] ;


    printf ("Please enter name of file with data\n");
    scanf ("%s", fileinput);

    FILE* input  = fopen (fileinput,"r");

    int message = Message (input, fileinput);

    assert ((message == 0) || (message == ERROR1));
        
    if (message == ERROR1) return (ERROR1);

    
    printf ("\nPlease enter name of file with dimetre\n");
    scanf ("%s", filediameter);

    FILE* diameter  = fopen (filediameter,"r");

    message = Message (diameter, filediameter);

    assert ((message == 0) || (message == ERROR1));
        
    if (message == ERROR1) return (ERROR1);


    printf ("\nPlease enter name of CSV file wiith sorted data\n");
    scanf ("%s", fileCSV);

    FILE* CSV  = fopen (fileCSV,"w");

    message = Message (CSV, fileCSV);

    assert ((message == 0) || (message == ERROR1));
        
    if (message == ERROR1) return (ERROR1);  
    
    
    printf ("\nPlease enter name of file with result\n");
    
    scanf ("%s", fileoutput);    

    
    FILE* output = fopen (fileoutput,"w");
	
    message = Message (input, fileoutput);

    assert ((message == 0) || (message == ERROR1));
        
    if (message == ERROR1) return (ERROR1);

    
    float square = 0, errorS = 0;    
    int d = Diameter (&square, &errorS , diameter);
    
    assert ((d == 0) || (d == ERROR2));    
    
    if (d == ERROR2) return (ERROR2);


    float ro1 = 0, errorRo1 = 0, ro2 = 0, errorRo2 = 0, ro3 = 0, errorRo3 = 0;    

    int correct = Compute (input, output, Length1, NUM1, CSV, square, errorS, &ro1, &errorRo1);

    assert (correct >= 0);

    if (correct > 0) return (ERROR2);
   
    
    correct = Compute (input, output, Length2, NUM2, CSV, square, errorS, &ro2, &errorRo2);

    assert (correct >= 0);

    if (correct > 0) return (ERROR2);


    correct = Compute (input, output, Length3, NUM3, CSV, square, errorS, &ro3, &errorRo3);
    
    assert (correct >= 0);
    
    if (correct > 0) return (ERROR2);

    float roTotal = (ro1 + ro2 + ro3)/3;
	float errorRoTotal = (errorRo1 + errorRo2 + errorRo3)/3;

 
    printf ("\nResult saved in file '%s'\n", fileoutput);

    fprintf (output,"\n\nTotal value resistivity (%3.2f+-%3.2f) * 10^(-4)Om * cm", roTotal, errorRoTotal); 

    fclose (input);
    fclose (output);
    fclose (diameter);
    fclose (CSV);    
    
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
    \param num 
    Multiplier for number of measurements
    \param CSV
    Name of .csv file
    \param square 
    Square of wire
    \param errorS
    Value of error of square
    \param ro
    Value of resistivity
    \param errorRo
    Value of Error of resistivity   
*/

int Compute (FILE* input, FILE* output, float length, int num, FILE* CSV, float square ,float errorS, float* ro, float* errorRo)
    {
    assert ((Length1 <= length) && (length <= Length3));

    float U[NMeas], I[NMeas], R[NMeas];
    ZeroArray (U, NMeas);
    ZeroArray (I, NMeas);
    ZeroArray (R, NMeas);
	
    int checkread = ReadAndCheck (U, I, NMeas, input);

    assert (checkread >= 0);
    
    if (checkread > 0)
        {
        printf ("ERROR in line %d\n", checkread + num);
        return (ERROR2); 
        }
    
    int check = CheckData (U, I);	

    assert (check >= 0);

    if (check > 0)
        {
        printf ("ERROR in line %d\n",check + num);
        return (ERROR2); 
        }
		
    Sort (U, NMeas);
    Sort (I, NMeas);
    float maxU = U[NMeas - 1]; 
    float maxI = I[NMeas - 1]; 
    
    float sumII = 0, sumUI = 0, sumUU = 0;
    Processing (U, I, &sumUI, &sumUU, &sumII); 

    float resistanceAvg = 0;
    Avg (sumUI, sumII, &resistanceAvg);

    float errorRrandom = 0;   
    RandomError (resistanceAvg, sumUU, sumII, &errorRrandom);
     
    float errorRsystem = 0;
    System (maxU, maxI, resistanceAvg, &errorRsystem);

    float errorR = 0;
    TotalErrorR (errorRrandom, errorRsystem, &errorR);
    
    float rTotal = 0;
	TotalR (resistanceAvg, &rTotal, ro, square , length);

    TotalErrorRo (*ro, errorR, rTotal, errorRo, errorS, length);
    
    fprintf (output,"\nFor length = %3.0f cm\n",length);
    fprintf (output,"Ravg = %5.3f Om\n", resistanceAvg); 
    fprintf (output,"Rtot = %5.3f Om\n", rTotal);
    fprintf (output,"ErrorRand = %5.3f Om\n", errorRrandom);
    fprintf (output,"ErrorSys = %5.3f Om\n", errorRsystem);
    fprintf (output,"ErrorTot = %5.3f Om\n", errorR);
    fprintf (output,"Ro = %5.2f\t, ErrorRo = %5.2f\n\n", *ro, *errorRo); 


    PrintCSV (U, I, CSV);
	
    return (0);

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

int ReadAndCheck (float U[], float I[], int size, FILE* input)
    {
    assert (size > 0);

    char str[1024] = "";

    for (int line = 0; line < size; line++)
	    {
	    assert ((0 <= line) && (line < size));

        fgets (str, sizeof (str) - 1, input);

        int check = sscanf (str,"%f %f", &U[line], &I[line]);

        assert ((check >= -1) && (check <= 2));

        if (check != 2) return (line + 1);
         
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
	    return (line + 1);
	    }
        }

    return (0);
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
    
    }

//=============================================================================

/*!
	\brief
	Funcion that count square of wire
	\details Funcion read file with value of diameter and process data
	\param S 
	Value square of wire
	\param errorS 
	Value error for square
    \param diameter
    File with data of diameter   
*/

int Diameter (float* s, float* errorS, FILE* diameter)
	{	
    float D[NumD] = {} ;
	float sumD = 0, dAvg = 0;
    
    char str[1024] = "";
	
    for (int line = 0; line < NumD; line++)
		{
        fgets (str, sizeof (str) - 1, diameter);

		int check = sscanf (str,"%f", &D[line]);

        assert (check >= -1);        

        if (check != 1)
            {
            printf ("ERROR in file with data of diameter on line №%d",check + 1);
            return (ERROR2);
            }        
            
		sumD += D[line];
		}
	
	dAvg = sumD / NumD;
	*s = Pi * dAvg * dAvg * 100 / 4;
	*errorS = 2 * ErrorD / dAvg;
    
    return (0);
	}

//=============================================================================

/*!
    \brief
    Funcion that print sorted data in .csv file
    \param U[]
    Array with data of voltage
    \param I[]
    Array with data of current
    \param CSV
    Output file
*/

void PrintCSV (float U[], float I[], FILE* CSV)
	{
	for (int i = 0; i < NMeas; i++)
		{
		assert ((0 <= i) && (i < NMeas));
		
		fprintf (CSV,"%4.1f %5.2f\n", U[i], I[i]);
		}
    fprintf (CSV,"\n");
	}



//=============================================================================

/*!
	\brief
	Funcion that count summ of U^2, I^2, U * I 
	\param U[]
    Array with data of voltage
    \param I[]
    Array with data of current
	\param sumUI 
	ammount of multiplication current and voltage
	\param sumII 
	ammount of current^2
	\param resistanceAvg 
	Averadge value resistance 
*/

void Processing (float U[], float I[], float* sumUI, float* sumUU, float* sumII)
	{
	for (int i = 0; i < NMeas; i++)                                       
		{
		*sumUI += U[i] * I[i];
		*sumII += I[i] * I[i];
		*sumUU += U[i] * U[i]; 
		}
	}

//=============================================================================

/*!
	\brief
	Funcion that count average resistance of wire 
	\details Funcion uses the lest squares method for obtain average resistance
	\param sumUI 
	ammount of multiplication current and voltage
	\param sumII 
	ammount of current^2
	\param resistanceAvg 
	Averadge value resistance 
*/

void Avg (float sumUI, float sumII, float* resistanceAvg)
	{
	*resistanceAvg = sumUI / sumII;

	assert ((0 < *resistanceAvg) && (*resistanceAvg < MaxResistance)); 
	}

//=============================================================================

/*!
	\brief
	Funcion that count random error for resistance of wire  
	\param sumUU 
	ammount of voltage^2
	\param sumII 
	ammount of current^2
	\param resistanceAvg 
	Averadge value resistance
	\param errorRrandom
	Value of random error for resistance 
*/
void RandomError (float resistanceAvg, float sumUU,float sumII, float* errorRrandom)
	{
	float sqrtN = sqrt(NMeas);
	*errorRrandom = (sqrt((sumUU / sumII) - (resistanceAvg * resistanceAvg))) / sqrtN;

	assert ((0 < *errorRrandom) && (*errorRrandom < MaxError));
	}

//=============================================================================

/*!
	\brief
	Funcion that count systematic error for resistance of wire
	\param maxU 
	Max value of voltage 
	\param maxI 
	Max value of current 
	\param resistanceAvg 
	Averadge value resistance
	\param errorRsystem
	Value of systematic error for resistance   
*/

void System (float maxU, float maxI, float resistanceAvg, float* errorRsystem)
	{
	assert ((maxU > 0) && (maxI > 0));	
	*errorRsystem = resistanceAvg * sqrt((ErrorU / maxU) * (ErrorU / maxU) + (ErrorI / maxI) * (ErrorI / maxI));
	
	assert ((0 < *errorRsystem) && (*errorRsystem < MaxError));
	}

//=============================================================================

/*!
	\brief
	Funcion that count total error for resistance of wire
	\param errorRrandom 
	Value of random error for resistance 
	\param errorRsystem
	Value of systematic error for resistance 
	\param errorR 
	Total value error for resistance
*/
void TotalErrorR (float errorRrandom, float errorRsystem ,float* errorR)
	{
	*errorR = sqrt(errorRrandom * errorRrandom + errorRsystem * errorRsystem);
	}

//=============================================================================

/*!
	\brief
	Funcion that count total resistance and resistivity of wire (l=20cm)  
	\param resistanceAvg 
	Averadge value resistance
	\param rTotal 
	Total resistance of wire
	\param ro
	Value resistivity of wire
	\param s
	Value square of wire
    \param length 
    Length of wire
*/
void TotalR (float resistanceAvg, float* rTotal, float* ro, float s, int length)
	{
	*rTotal = resistanceAvg + (resistanceAvg * resistanceAvg) / RU;
	*ro = *rTotal * s / length;
	}

//=============================================================================

/*!
	\brief
	Funcion that count total error for resistivity of wire (l=20cm)  
	\param ro
	Value resistivity of wire
	\param errorR
	Total value error for resistance of wire
	\param Rtotal 
	Total value resistance of wire
	\param errorRo
	Value error for resistivity
	\param errorS
	Value error for square of wire
    \param length
    Length of wire (cm)
    
*/

void TotalErrorRo (float ro, float errorR, float rTotal, float* errorRo, float errorS, int length)
	{
	*errorRo = ro * sqrt((errorR / rTotal) * (errorR / rTotal) + (errorS * errorS) + (ErrorL / length) * (ErrorL / length));
	}

//=============================================================================


