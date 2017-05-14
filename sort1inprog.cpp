#include <iostream>
#include <fstream>
#include <iomanip>
#include <unistd.h>

using namespace std;
double time_count = 0;
// Function to read a single element from the tape
int read(fstream& tape)
{
   int data;
   tape.read(reinterpret_cast<char*>(&data), sizeof(int));
 	usleep(1000);
	time_count += 1;
  return data;
}

// Function to rewaind the tape to the beginning
void rewind(fstream& tape, char *arg, int n)
{
   tape.close();
   tape.open(arg,ios::in | ios::out | ios::binary);
	usleep(250*n);
	time_count += ((1/8)*n);
}

// Function to write a single element to the tape
void write(fstream& tape, int data)
{
   tape.write(reinterpret_cast<char*>(&data), sizeof(int));
	usleep(1000);
	time_count += 1;
}

void sort(fstream& tape, char *arg)
{
    tape.open(arg, ios::in | ios::out | ios::binary);

    int tapelength = 0;
    int data;

    do
    {
        data = read(tape);
        tapelength++;
    } while (!tape.eof());

	tapelength--;
//	tapelength--;



	cout << tapelength << endl;

	int largeI = -1;
	int large = 0;
	int clobbered = 0;
	int clobberI = -1;

	//rewind(tape, argv[1]);
	while (tapelength > 0)
	{
		large = 0;
		rewind(tape, arg, tapelength);
		for (int i = 0; i < tapelength; i++)
		{
			if (i == clobberI)
			{
				write(tape, clobbered);
				clobberI = -1;
				rewind(tape, arg, i);
				i = 0;
			}
			data = read(tape);
			//cout << "data " << data << " large " << large << " tape " << tapelength << " i " << i << " largeI " << largeI << " clobber " << clobbered << endl;
			//cin.ignore();
			if (data > large)
			{
				large = data;
				largeI = i;
			}
			if (i == tapelength - 1)
			{
				clobbered = data;
				clobberI = largeI;
				rewind(tape, arg, i);
				for (int j = 0; j < tapelength  - 1; j++)
				{
					data = read(tape);
				}
				write(tape, large);
				tapelength--;
			}
		}
	}
}

int main(int argc, char** argv)
{
	fstream tape;

	tape.open(argv[1], ios::in | ios::out | ios::binary);
	
	int tapelength = 0;
	int data;
	int large = 0;
	int largeI = -1;
	int clob;

	int nxtLarge = 0;
	int nxtLargeI = -1;


	while (!tape.eof())
	{
		data = read(tape);
		/*if (data > large)
		{
			large = data;
			cout << "large: " << large << endl;
			largeI = tapelength;
		}*/
		tapelength++;
		//clob = data;	
	}
	tapelength--;

	rewind(tape, argv[1], tapelength);
	//go through and get the highest and the clobber value.  Why the second loop?  Because the loop above fails if the highest val is at the end.
	//That's all I can gather.
	for (int i = 0; i < tapelength; i++)
	{
		data = read(tape);
		if (data > large)
		{
			large = data;
			cout << "large: " << large << endl;
			largeI = i;
		}
		clob = data;
	}

	cout << "largest: " << large << " largeIN: " << largeI << " clob: " << clob << endl;
	//loop which moves values
	while (tapelength > 0)
	{
		rewind(tape, argv[1], tapelength);
		for (int i = 0; i < tapelength; i++)
		{
			if (i == largeI)
			{
				write(tape, clob);
				i++;
			}
			if (i == tapelength - 1)
			{
				write(tape, large);
				large = nxtLarge;
				largeI = nxtLargeI;
				nxtLarge = 0;
				nxtLargeI = -1;
				i++;
			}
			cout << "i: " << i << endl;
			if (i < tapelength)
			{
				data = read(tape); //this reads even if i gets incremented to 8 cuz precondition check. Fixed with the if around it
			}
				if (data > nxtLarge)
				{
					nxtLarge = data;
					nxtLargeI = i;
				}
				cout << "data: " << data << endl;
				clob = data;
		
		}
		tapelength--;	
	}
	//cout << tapelength << endl;


	// sort(tape, argv[1]);

	tape.close();
	cout << "TIME: " << time_count << endl;
}
