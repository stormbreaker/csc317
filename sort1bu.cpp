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
	time_count += ((1/8) * n);
}

// Function to write a single element to the tape
void write(fstream& tape, int data)
{
   tape.write(reinterpret_cast<char*>(&data), sizeof(int));
	usleep(1000);
	time_count += 1;
}

int main(int argc, char** argv)
{
    fstream tape;
    tape.open(argv[1], ios::in | ios::out | ios::binary);

    int tapelength = 0;
    int b17mem[8];

	do
    {
        b17mem[0] = read(tape);
        tapelength++;
    } while (!tape.eof());

	tapelength--;
//	tapelength--;

	int data;

	cout << tapelength << endl;

	int largeI = -1;
	int large = 0;
	int clobbered = 0;
	int clobberI = -1;

	//rewind(tape, argv[1]);
	while (tapelength > 0)
	{
		large = 0;
		rewind(tape, argv[1], tapelength);
		for (int i = 0; i < tapelength; i++)
		{
			if (i == clobberI)
			{
				write(tape, clobbered);
				clobberI = -1;
				rewind(tape, argv[1], i);
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
				rewind(tape, argv[1], i);
				for (int j = 0; j < tapelength  - 1; j++)
				{
					data = read(tape);
				}
				write(tape, large);
				tapelength--;
			}
		}
	}

    tape.close();
	cout << "TIME: " << time_count << endl;
}
