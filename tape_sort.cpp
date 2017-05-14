/*
 * File:     tape_sort.cpp
 * Purpose:  Simulating sorting of an array of
 *           positive integerson a tape
 * Input:
 *    A:     file with elements of array A
 * Output:
 *    A:     file elements of A after sorting
 *
 * Compile:  g++ -g -Wall -std=c++11 -o tape_sort tape_sort.cpp
 * Run:
 *    ./tape_sort <filename>
 *
 * Notes:
 * 1.  Optional -DDEBUG compile flag for verbose output
 *     g++ -g -Wall -std=c++11 -DDEBUG -o tape_sort tape_sort.cpp
 */
#include<iostream>
#include<fstream>
#include <unistd.h>

using namespace std;

double time_count = 0;
// User defined functions
// Function to read a single element from the tape
int read(fstream& tape)
{
   int data;
   tape.read(reinterpret_cast<char*>(&data), sizeof(int));
	//usleep(1000);
	time_count += 1;
   return data;
}
// Function to rewaind the tape to the beginning
void rewind(fstream& tape, char *arg, int n)
{
   tape.close();
   tape.open(arg,ios::in | ios::out | ios::binary);
	//usleep(250*n);
	time_count += ((1/8) * n);
}
// Function to write a single element to the tape
void write(fstream& tape, int data)
{
   tape.write(reinterpret_cast<char*>(&data), sizeof(int));
	//usleep(1000);
	time_count += 1;
}

// Main function
int main(int argc, char *argv[])
{
	// User defined varaible
   int data, length=0;
   int current_value, current_index=0;
   int low_value, low_index = 0;
   int swap_value, swap_index = 0;

   // Make sure we a commandline argument for file-name
   if ( argc != 2 )
   {
      /* We print argv[0] assuming it is the program name */
      printf( "usage: %s <filename>\n", argv[0] );
      return(0);
   }

   //Open a binary file-stream for in and output
   fstream tape;
   tape.open(argv[1],ios::in | ios::out | ios::binary);

   // First read of the tape
   if(tape.is_open()) // Were we able to open the tape?
   {	// If so read the data on the tape
      cout << "Pass 0     : ";
      // Read until the end of the file
      while(!tape.eof())
      {
         // Read one element at the time
         data = read(tape);
         // If the data is greater than zero, we have valid data
         if(data > 0)
         {	// Is it the first element on the tape?
            if(length == 0)
                current_value = low_value = data;	// Set first element to current and low
            else if(data < low_value)
            {	// It was not the first check to see if we are lowest value
               low_value = data;
               low_index = length;
            }
            cout << data << " ";							// Print Original Array to screen
            length++;										// Increment the length
         }
      }
      // Rewind the tape
      rewind(tape, argv[1], length);

	}
   // Could not read the tape
   else
   {
      cout << "Unable to open file" << endl;
      return(0);
   }

   /* We have now read the tape, we know the length and the lowest value
      Start sorting the tape */
   for(current_index=0; current_index<length-1; current_index++)
   {

      int j;
      swap_value = current_value;			// Value in current position
      swap_index = low_index;				// Position with the lowest value

      /* Read off the sorted numbers
         All values before current_position are sorted */
      for(j=0; j<current_index; j++)
      {
         data = read(tape);				// Read off the sorted values

      }
      // Write lowest number from previous pass

      write(tape, low_value);				// Write lowest value in current position
      j++;

      // Check the first element that is unsorted on the tape
      if(j==swap_index)
      {	// Was this were the lowest value came from?

          write(tape, swap_value);				// Write to the postion that had lowest value
          current_value = low_value = swap_value;		// Set this to current and low
      }
      else
      {	// Lowest value was not the first unsorted value

          data = read(tape);				// Read value at current position
          if(data)
             current_value = low_value = data;		// Set this value to current and low
      }
      low_index = j;					// Current index is the low index

      // Loop through the remaing elements on the tape
      for(j++; j<length; j++)
      {	// Was this were the previous lowest value came from?
          if(j==swap_index)
          {
             write(tape, swap_value);			// Write the value with swapped with
             data = swap_value;				// Set data to the swapped value
          }
         else
         {	// We were not the lowest value from previous pass
             data = read(tape);				// Read value in current position
         }

         if(data < low_value)
         {	/*	Check to see if we are lower than current lowest value
			If so update the lowest value and index */
             low_value = data;
             low_index = j;
         }
      }

     rewind(tape, argv[1], current_index);				// Rewind the tape
   }
   // The tape is sorted
   cout << endl << "Sorted tape: ";
   // rewind(tape, argv[1]);				// Rewind the tape
   for(int k=0; k<length; k++)
   {
      data = read(tape);
      if(data)
         cout << data << " ";
   }
   cout << endl;
   tape.close();
	cout << "TIME: " << time_count << endl;					// Close the tape

   return(0);
}
