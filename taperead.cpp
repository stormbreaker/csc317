#include <fstream>
#include <iostream>

using namespace std;

int read(fstream& tape)
{
   int data;
   tape.read(reinterpret_cast<char*>(&data), sizeof(int));
   return data;
}

int main(int argc, char** argv)
{
    fstream tape;
    tape.open(argv[1], ios::in | ios::binary);

    int data;

    do
    {
        data = read(tape);
        cout << data << " ";
    } while (!tape.eof());

    cout << endl;
    tape.close();

}
