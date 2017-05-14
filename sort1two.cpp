#include <iostream>
#include <fstream>
#include <iomanip>


using namespace std;

// Function to read a single element from the tape
int read(fstream& tape)
{
   int data;
   tape.read(reinterpret_cast<char*>(&data), sizeof(int));
   return data;
}

// Function to rewaind the tape to the beginning
void rewind(fstream& tape, char *arg)
{
   tape.close();
   tape.open(arg,ios::in | ios::out | ios::binary);
}

// Function to write a single element to the tape
void write(fstream& tape, int data)
{
   tape.write(reinterpret_cast<char*>(&data), sizeof(int));
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
	int large2 = 0;
	int large2I = -1;
	int clobbered = 0;
	int clobberI = -1;
	int clobbered2 = 0;
	int clobberI2 = -1;

	//rewind(tape, argv[1]);
	while (tapelength > 0)
	{
		large = 0;
		large2 = 0;
		rewind(tape, argv[1]);
		for (int i = 0; i < tapelength; i++)
		{
			if (i == clobberI2)
			{
				write(tape, clobbered2);
				clobberI = -1;
				write(tape, clobbered);
				clobberI2 = -1;
				rewind(tape, argv[1]);
				i = 0;
			}
			data = read(tape);
			//cout << "data " << data << " large " << large << " tape " << tapelength << " i " << i << " largeI " << largeI << " clobber " << clobbered << endl;
			//cin.ignore();
			if (data > large2)
			{
				large2 = data;
				largeI2 = i;
				if (//no idea....confused as heck
			}
			if (i == tapelength - 2)
			{
				clobbered2 = data;
				clobberI2 = largeI;
				data = read(tape);
				clobbered = data;
				clobberI = clobberI + 1;
				rewind(tape, argv[1]);
				for (int j = 0; j < tapelength  - 1; j++)
				{
					data = read(tape);
				}
				write(tape, large2);
				write(tape, large);
				tapelength--;
				tapelength--;
			}
		}
	}

    tape.close();
}
