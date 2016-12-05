/*

	extra credit project input generator, ver 1.0

	usuage a.exe (or a.out) 5 10
	
	5 is the number of items
	10 is max weight can be carried
	
	output data.txt
*/


#include <stdio.h>
#include <stdlib.h>

int IntRand (int nMin, int nMax) {
	double retVal = ((double) rand () / (double) RAND_MAX) * ((double) (nMax - nMin)) + (double) nMax;
	
	return (int) retVal;
};

int main (int argc, char* argv [])
{
	srand (0);
	
	int nItems = atoi (argv [1]);
	int nMaxTotalWeight = atoi (argv [2]);
	char strBuf [1000];
	
	FILE* pFile = fopen ("data.txt", "w+");
	
	fprintf (pFile, "%d\n", nItems);
	fprintf (pFile, "%d\n", nMaxTotalWeight);
	
	for (int i = 0; i < nItems; i++) {
	        // originaly upper bound of 100
		fprintf (pFile, "%d %d\n", IntRand (1, 5), IntRand (1, 5));
	}
	
	fclose (pFile);
	
	return 0;
}
