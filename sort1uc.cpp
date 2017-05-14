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
	int large2I = 0;
	int clobbered = 0;
	int clobberI = -1;

	//rewind(tape, argv[1]);
	while (tapelength > 0)
	{
		int i;
		large = 0;
		rewind(tape, argv[1]);
		cout << "rewound after reaching end of tape" << endl;
		for (i = 0; i < tapelength; i++)
		{
			if (i == clobberI)
			{
				write(tape, clobbered);
				cout << "wrote swapped" << endl;
				clobberI = -1;
				rewind(tape, argv[1]);
				i = 0;
				cout << "rewound after swap" << endl;
			}

			//need to read a value to get it clobbered but then I need to write it the next time through
			//is this a highest and second highest thing? that would make sense...
		
			//or we could cut off two numbers at a time.

			if (i < tapelength - 1)
			{
				data = read(tape);
			}
		//	cout << "data " << data << " large " << large << " tape " << tapelength << " i " << i << " largeI " << largeI << " clobber " << clobbered << " clobberI " << clobberI << endl;
		//	cin.ignore();
			if (data > large)
			{
				large = data;
				largeI = i;
			}
		}
		clobbered = data;
		clobberI = largeI;
		/*rewind(tape, argv[1]);
		for (int j = 0; j < tapelength  - 1; j++)
		{
			data = read(tape);
		}*/
		write(tape, large);
		cout << "wrote at end" << endl;

		tapelength--;
	}

    tape.close();
}
