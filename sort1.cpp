#include <iostream>
#include <fstream>
#include <iomanip>
#include <unistd.h>

using namespace std;

double time_count = 0;

/*
Name: read
This is the read function provided by Dr. Karlsson.  I added the usleep which I
commented out because I prefered to use a global time_count variable to count
operations instead.  In read, every time it gets called I add 1 to that variable.

The tape variable is a file stream which contains the file that I am working with.
*/
int read(fstream& tape)
{
   int data;
   tape.read(reinterpret_cast<char*>(&data), sizeof(int));
	//usleep(1000);
	time_count += 1;
   return data;
}

/*
Name: rewind
This is the read function provided by Dr. Karlsson.  I added the usleep which I
commented out because I prefered to use a global time_count variable to count
operations instead.  In rewind, every time it gets called I add 1/8 of the number of
elements that I rewind on to that variable.

The tape variable is a file stream which contains the file that I am working with.
The arg variable carries a string of the file which I am working with.
The n variable contains the number of items which I rewound backwards to be used
in counting the number of operations which have been used.
*/
void rewind(fstream& tape, char *arg, int n)
{
   tape.close();
   tape.open(arg,ios::in | ios::out | ios::binary);
	//usleep(250*n);
	time_count += ((1/8) * n);
}

/*
Name: write
This is the read function provided by Dr. Karlsson.  I added the usleep which I
commented out because I prefered to use a global time_count variable to count
operations instead.  In write, every time it gets called I add 1 to that variable.

The tape variable is a file stream which contains the file that I am working with.
The data variable is the value which needs to be written to the tape.
*/
void write(fstream& tape, int data)
{
   tape.write(reinterpret_cast<char*>(&data), sizeof(int));
	//usleep(1000);
	time_count += 1;
}

/*
This is the main function for the program.  What this function does is consists
of a few loops.  The first goes through and counts how long the list is.
After that, I have a for loop nested within a while loop which loops through and
controls when to write at the end.  The while loop controls the length of the
list and shortens it by one each time through.  At the end "TIME" prints out the
time_count to set the operation count.  
*/
int main(int argc, char** argv)
{
    fstream tape;
    tape.open(argv[1], ios::in | ios::out | ios::binary);

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
