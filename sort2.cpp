#include <iostream>
#include <iomanip>
#include <fstream>
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
	time_count +=1;
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
void rewind(fstream& tape, const char *arg, int n)
{
	tape.close();
	tape.open(arg, ios::in | ios::out | ios::binary);
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
	//usleep(250*n);
	time_count += 1;
}

/*
Name: radixSort
This implements a base 2 radix sort.  I use two other tapes which were allowed
for sort2.  Tape1 is passed in and contains the main list.  The other two tapes
are hard-coded and are used for the base 0 and 1 respectively.  I loop through
each mask and compare to see if that bit was 0 or 1.  When tape1 is empty,
We write tape2 back to it immediately followed by tape3.  This is repeated 32
times because ints are 4 bytes.
*/
void radixSort(fstream& tape1, int length, char* arg)
{
	fstream tape2, tape3;
	tape2.open("tape2.bin", ios::in | ios::out | ios::trunc | ios::binary);
	tape3.open("tape3.bin", ios::in | ios::out | ios::trunc | ios::binary);

	int data;
	int t2length = 0;
	int t3length = 0;
	unsigned int mask = 1;
	int temp;

	//cout << "legnth: " << length << endl;

	for (int j = 0; j < 32; j++)
	{
		//cout << "in first loop";
		t2length = 0;
		t3length = 0;

		for (int i = 0; i < length; i++)
		{
			//cout << "int second loop";
			data = read(tape1);
			//cout << "data " << data << endl;
			temp = data & mask;
			temp = temp >> j;
			if (temp == 1)
			{
				write(tape3, data);
				t3length++;
				//cout << "writing to 3" << endl;
			}
			else
			{
				write(tape2, data);
				t2length++;
			}
		}
		mask = mask << 1;

		rewind(tape1, arg, length);
		rewind(tape2, "tape2.bin", t2length);
		rewind(tape3, "tape3.bin", t3length);
		for (int i = 0; i < t2length; i++)
		{
			data = read(tape2);
			write(tape1, data);
		}
		for (int i = 0; i < t3length; i++)
		{
			data = read(tape3);
			write(tape1, data);
		}
		rewind(tape1, arg, length);
		rewind(tape2, "tape2.bin", t3length);
		rewind(tape3, "tape3.bin", t2length);
	}
}
/*
This is the main function.  It finds the length of the tape that I am working with
then I am calling the radix sort on it.  After I complete the sort, I print out
the number of operations as "TIME: <time_count>".
*/
int main(int argc, char** argv)
{
	fstream tape, tape1, tape2;
	tape.open(argv[1], ios::in | ios::out | ios::binary);

	int tapelength = 0;
	int data = 0;

	//did this to get the compiler to quit giving me crap about "not using data"
	data = data;

	//get length of the list
	do
	{
		data = read(tape);
		tapelength++;
	} while(!tape.eof());

	tapelength--;
	//rewind to prep for sort
	rewind(tape, argv[1], tapelength);


	radixSort(tape, tapelength, argv[1]);

	cout << "TIME: " << time_count << endl;

	tape.close();

}
