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
		rewind(tape, arg);
		for (int i = 0; i < tapelength; i++)
		{
			if (i == clobberI)
			{
				write(tape, clobbered);
				clobberI = -1;
				rewind(tape, arg);
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
				rewind(tape, arg);
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

    sort(tape, argv[1]);

    tape.close();
}
