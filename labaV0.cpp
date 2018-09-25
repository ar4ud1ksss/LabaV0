#include <stdio.h>
#include <assert.h>


const int NMeas    = 12;            //Number all measurements
const int Square   = 10;            //Square of wire (10^(-4) ñm^2)
const float Length1  = 20;            //Long of wire 1 (cm)
const float Length2  = 30;            //Long of wire 2 (cm)
const float Length3  = 50;            //Long of wire 3 (cm)
const int MaxI     = 600;           //Max value ampermeter (mA)
const int MaxU     = 600;           //Max value voltmeter (mV)


int Compute (FILE* input, FILE* output, float length);
void ZeroArray (float array[], int size);
void Read (float U[], float I[], int num, FILE* input);
void CountResistivity (float U[], float I[], float R[], float length);
void Print (float R[], FILE* output, float length);
int Count (FILE* input, FILE* output);
int Meassage (FILE* input, FILE* output);
int CheckData (float U[], float I[]);

int main ()
	{
    printf ("#Laba 1.1.1 \n"
                "#(c) Kirill Shcherbina 2k!8\n\n");

    FILE* input  = fopen ("laba.txt",        "r");
    FILE* output = fopen ("Babka-labka.txt", "w");
	
    int meassage = Meassage (input, output);

    assert ((meassage == 0) || (meassage == 1));
        
    if (meassage == 1) return (1);  
    
    int correct1 = Compute (input, output, Length1);
    if (correct1 == 1) return (1);
   
    int correct2 = Compute (input, output, Length2);
    if (correct2 == 1) return (1);

    int correct3 = Compute (input, output, Length3);
    if (correct3 == 1) return (1);
 

	printf ("Result saved in file 'Babka-labka.txt'\n");
    
    return (0);
    	
	}

//=============================================================================

int Compute (FILE* input, FILE* output, float length)
	{
	float U[NMeas], I[NMeas], R[NMeas];
	ZeroArray (U, NMeas);
	ZeroArray (I, NMeas);
	ZeroArray (R, NMeas);
	
    if (input == NULL) return (-1);	
    
    Read (U, I, NMeas, input);

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

void ZeroArray (float array[], int size)
	{
	for (int i = 0; i < size; i++)
        	{
        	assert (0 <= i && i < size);

        	array[i] = 0;
		}
	}

//=============================================================================

void Read (float U[], float I[], int size, FILE* input)
	{
	for (int line = 0; line < size; line++)
		{
		assert ((0 <= line) && (line < size));

		fscanf (input, "%f %f", &U[line], &I[line]);
		}
	}

//=============================================================================

int CheckData (float U[], float I[])
	{	
	for (int line = 0; line < NMeas; line++)
		{
		assert ((0 <= line) && (line < NMeas));

		if ((U[line] <= 0) || (I[line] <= 0) || (U[line] > MaxU) || (I[line] > MaxI))
			{
            
			return (line+1);
			}
        }
		
        return (0);
	}
    
//=============================================================================

void CountResistivity (float U[], float I[], float R[], float length)
	{
	for (int i = 0; i < NMeas; i++)
		{
		assert ((0 <= i) && (i < NMeas));

			{
			R[i] = U[i] * Square/(I[i] * length);
			}
		

		}
	}

//=============================================================================

void Print (float R[], FILE* output, float length) 
	{
    fprintf (output,"\nValue resistivity for %3.0fcm\n", length);
	for (int i = 0; i < NMeas; i++)
		{
		assert ((0 <= i) && (i < NMeas));
		

		if ((i + 1) < 10)	fprintf (output,"%d.  %5.2f *10^(-4) Om*cm^2\n",i + 1, R[i]);
				
		else			    fprintf (output,"%d. %5.2f *10^(-4) Om*cm^2\n",i + 1, R[i]);
                        
		}
	}

//=============================================================================

int Meassage (FILE* input, FILE* output)
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
