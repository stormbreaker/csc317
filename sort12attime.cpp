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
    int data;

	do
    {
        data = read(tape);
        tapelength++;
    } while (!tape.eof());

	tapelength--;
//	tapelength--;



	cout << tapelength << endl;

	int largeI = -1; //where I found large
	int large = 0;

    int large2 = 0;
    int large2I = -1; //where I found large2

	int clob = 0;
	int clobI = -1; //where I need to put clob

    int clob2 = 0;
    int clob2I = -1; //where I need to put clob2
/*

NEED TO SPECIAL CASE WHEN SECOND HIGHEST IS AT END!!!! or not.... what is wrong here... losing clobber


*/
	//rewind(tape, argv[1]);
	while (tapelength > 0)
	{
		large = 0;
        large2 = 0;
		rewind(tape, argv[1], tapelength);
		for (int i = 0; i < tapelength; i++)
		{
			if (i == clob2I)
			{
                cout << "placeing small clob" << endl;
				write(tape, clob2);
                clob2I = -1;
				//rewind(tape, argv[1], i);
				//i = 0;
			}
            if (i == clobI)
            {
                cout << "placing large clob" << endl;
                write(tape, clob);
                //rewind(tape, argv[1], i);
                clobI = -1;
                //i = 0;
            }
			data = read(tape);

			//cout << "data " << data << " large " << large << " tape " << tapelength << " i " << i << " largeI " << largeI << " clob " << clob << "clob2 " << clob2 << endl;
            //cout << "large2 " << large2 << "large2I " << large2I << endl;
			//cin.ignore();
			if (data > large2)
			{
                large2 = data;
				large2I = i;
                if (large2 > large)
                {
                    int temp, tempI;
                    temp = large;
                    large = large2;
                    large2 = temp;
                    tempI = largeI;
                    largeI = large2I;
                    large2I = tempI;
                }
                //cout << "large " << large << "large2 " << large2 << endl;
	        }
			if (i == tapelength - 2)
			{
                if (clob2I == -1)
                {
                    clob2 = data;
                    clob2I = i;
                }
                if(clobI = -1)
                {
                    clob = read(tape);
                    clobI = i++;
                }
                cout << "clob " << clob << endl;
                cout << "clob2 " << clob2 << endl;

                cout << "end of tape" << endl;
                //special case this so that if the old clobs fall within the new clobs, things don't go wonky
                //the old large index overlaps with the new clob index for both
                if ((tapelength - clobI) == 1 && (tapelength - clob2I) == 2)
                {
                    clob2I = large2I;
                    clobI = largeI;
                    data = read(tape);
                    i += 2;
                    cout << "need to move both clobs down one more" << endl;
                }
                //the old large index overlaps with the new clob index for the larger large
                else if ((tapelength - clobI) == 1)
                {
                    clob2 = data;
                    clob2I = large2I;
                    clobI = largeI;
                    i++;
                    cout << "need to move large clob down one more" << endl;
                }
                //the old large index overlaps with the new clob index for the smaller large
                else if ((tapelength - clob2I) == 2)
                {
                    clob2I = large2I;
                    clob = read(tape);
                    clobI = largeI;
                    i += 2;
                    cout << "need to move small clob down one more" << endl;
                }
                //both of the larges were already in place
                else if (clob2I == tapelength - 2 && clobI == tapelength - 1)
                {
                    clob = 0;
                    clobI = -1;
                    clob2 = 0;
                    clob2I = -1;
                    i += 2;
                    cout << "both larges already in place" << endl;
                }
                //the larger large was already in place
                else if (clobI == tapelength - 1)
                {
                    clob2 = data;
                    clob2I = large2I;
                    clob = 0;
                    clobI = -1;
                    i++;
                    cout << "large large already in place" << endl;
                }
                //the smaller large was already in place
                else if (clob2I == tapelength - 2)
                {
                    clob2 = 0;
                    clob2I = -1;
                    clob = read(tape);
                    clobI = largeI;
                    i += 2;
                    cout << "small large already in place" << endl;
                }
                //end special cases?
                else
                {
                    rewind(tape, argv[1], i);
                    for (int j = 0; j < tapelength  - 2; j++)
                    {
                        data = read(tape);
                        //cout << "data " << data << endl;
                    }
                    write(tape, large2);
                    write(tape, large);
                }


				//tapelength -= 2;
                cout << tapelength << endl;
			}
		}
        tapelength -=2;
	}

    tape.close();
	cout << "TIME: " << time_count << endl;
}
