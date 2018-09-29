

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
    Funcion that print ERROR if fils wasn't opened
    \param filecondition
    Condition of file
    \param name
    Name of file
*/ 

int Message (FILE* filecondition, char* name)
    {
    if (filecondition == NULL)
        {
        printf ("File '%s' wasn't opened\n", name);
        return (ERROR1);        
        }

    return (0);
    }

//=============================================================================

/*!
	\brief
	Funcion that sorts array 
	\param Array[]  
	Input array
	\param size
	Size of array
*/

void Sort (float Array[], int size)
	{	
	float saveValue = 0;
	
    for (int j = 0; j < (size - 1); j++)
		{
		for (int i = j + 1; i < size; i++)
			{
			if (Array[i] < Array[j])
				{
				saveValue = Array[j];
				Array[j] = Array[i];
				Array[i] = saveValue;	
				}
			}
		}
    }






