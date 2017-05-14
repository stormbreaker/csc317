/*
    Author: Benjamin Kaiser
    Class: CSC 317 - Computer Organization and Architecture
    Program: Program 3 -- B17 Disassembly/Simulation
    Professor: Dr. Christer Karlsson
    Due Date: March 23, 2016
    Known Bugs: No known Bugs
    Description:  This program is a disassembly of textual object files which
        simulates the B17 computer.  It takes the object file in, which contains
        several lines.  Each lines starts with an address and this if followed
        by a number which indicates how many values and contiguous memory spots
        there are.  The very final value in the file is the memory address at
        which to start the file.  It should then run from there until a HALT
        operation is encountered or something illegal happens.
    Compile: use the accompanying Makefile or alternatively use the following command:
	   g++ -std=c++11 b17.cpp -o b17
    Run: ./b17 <inputfile>
	     ./b17 example.obj
*/
#include <cstdint>
#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <stdint.h>
#include <string>
#include <cmath>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <bitset>

using namespace std;

//array to check for printing
static const string INSTRUCTION_CODES[22] = {"HALT","NOP", "LD", "ST", "EM", "LDX", "STX",
"EMX", "ADD", "SUB", "CLR", "COM", "AND", "OR", "XOR", "ADDX", "SUBX", "CLRX", "J", "JZ",
"JN", "JP"};

//parallel array to get associated opcode
static const int INSTRUCTIONOPCODE[22] = {0, 1, 16, 17, 18, 24, 25, 26, 32, 33, 34, 35,
36, 37, 38, 40, 41, 42, 48, 49, 50, 51};

//struct to decide whether insturction or not
//this constitutes the RAM
struct object
{
    bool instruction = false;
    string value;
};

//memory IN HEX
object RAM[4096]; //0 through 4095
//memory address register BINARY
string MAR = "";
//instruction counter BINARY
string IC = "";
//index registers HEX
string X[4] = {"000", "000", "000", "000"};
//address bus BINARY
string ABUS = "";
//memory data register BINARY
string MDR = "";
//accumulator HEX
string AC = "000000";
//arithmetic logic unit BINARY
string ALU = ""; //not really being used since have ALUcompute
//instruction register BINARY
string IR = "";
//data bus BINARY
string DBUS = "";
//memory addres to start the program IN HEX
string PROGRAMSTART = "";

/*
Name: getCategoryIndicatorFromInstruction
Description: This function takes a binary representation of an
    instruction and extracts the operation category bits from it.
Author: Benjamin Kaiser
Parameters:
	binaryValue: this is a bitstring.  Should be at least 24 bits
        but in reality takes anyting.
Returns:
	A two bit string
*/
//opcode -- COMPLETE
string getCategoryIndicatorFromInstruction(string binaryValue)
{
    return binaryValue.substr(12, 2);
}

/*
Name: getOperationSpecifierFromInstruction
Description: This function takes a binary representation of an
    instruction and extracts the operation specifier bits from it.
Author: Benjamin Kaiser
Parameters:
	binaryValue: this is a bitstring.  Should be at least 24 bits
        but in reality takes anyting.
Returns:
	A four bit string
*/
//opcode -- COMPLETE
string getOperationSpecifierFromInstruction(string binaryValue)
{
    return binaryValue.substr(14, 4);
}

/*
Name: getRegisterSpecifierFromInstruction
Description: This function takes a binary representation of an
    instruction and extracts the index register specifier bits from it.
Author: Benjamin Kaiser
Parameters:
	binaryValue: this is a bitstring.  Should be at least 24 bits
        but in reality takes anyting.
Returns:
	A two bit string
*/
//addressing mode -- COMPLETE
string getRegisterSpecifierFromInstruction(string binaryValue)
{
    return binaryValue.substr(22, 2);
}

/*
Name: getAddressingModeFromInstruction
Description: This function takes a binary representation of an
    instruction and extracts the addressing mode bits from it.
Author: Benjamin Kaiser
Parameters:
	binaryValue: this is a bitstring.  Should be at least 24 bits
        but in reality takes anyting.
Returns:
	A four bit string
*/
//addressing mode -- COMPLETE
string getAddressingModeFromInstruction(string binaryValue)
{
    return binaryValue.substr(18, 4);
}

/*
Name: getOpCodeFromInstruction
Description: This function takes a binary representation of an
    instruction and extracts the entire opcode bits from it.
Author: Benjamin Kaiser
Parameters:
	binaryValue: this is a bitstring.  Should be at least 24 bits
        but in reality takes anyting.
Returns:
	A six bit string
*/
//opcode -- COMPLETE
string getOpCodeFromInstruction(string binaryValue)
{
    return binaryValue.substr(12, 6);
}

/*
Name: getAddressFromInstruction
Description: This function takes a binary representation of an
    instruction and extracts the address bits from it.
Author: Benjamin Kaiser
Parameters:
	binaryValue: this is a bitstring.  Should be at least 24 bits
        but in reality takes anyting.
Returns:
	A twelve bit string
*/
//address -- COMPLETE
string getAddressFromInstruction(string binaryValue)
{
    return binaryValue.substr(0,12);
}

/*
Name: convertBinaryDecimal
Description: This function takes a bit string and converts it
    into the appropriate decimal value.
Author: Benjamin Kaiser
Parameters:
	binaryValue: this is a bitstring.  This can be of any length
Returns:
	An integer
*/
//binary to decimal conversion -- COMPLETED
int convertBinaryDecimal(string binaryValue)
{
    bitset<24> decimalValue(binaryValue);
    return decimalValue.to_ulong();
}

/*
Name: convertDecimalBinary
Description: This function takes a decimal value and converts it into
    the appropriate bit string.
Author: Benjamin Kaiser
Parameters:
	decimalValue: a value to be converted into a bit string
Returns:
	A bit string representation of the decimal value
*/
//convert decimal to Binary -- COMPLETED
string convertDecimalBinary(int decimalValue)
{
    bitset<24> binaryValue(decimalValue);
    return binaryValue.to_string();
}

/*
Name: convertHexDecimal
Description: This function takes a hexadecimal string and converts it
    into a decimal value.
Author: Benjamin Kaiser
Parameters:
	hexValue: a value to be converted into a decimal value
Returns:
	A decimal integer of the hex string
*/
//hexadecimal to to decimal conversion -- COMPLETED
int convertHexDecimal(string hexValue)
{
    int decimalValue;
    decimalValue = strtol(hexValue.c_str(), 0, 16);
    return decimalValue;

}

/*
Name: convertDecimalHex
Description: This function takes a decimal value and turns it into a string
    of hex digits
Author: Benjamin Kaiser
Parameters:
    decimalValue: a decimal number which needs to be turned into a
    string of hex digits
Returns:
	A string of hex digits
*/
//decimal to hexadecimal -- COMPLETED
string convertDecimalHex(unsigned int decimalValue)
{
    char hexValue[24];
    sprintf(hexValue, "%x", decimalValue);
    return hexValue;
}

/*
Name: convertHexBinary
Description: This function takes a hexadecimal string and converts it
    into a bit string representation
Author: Benjamin Kaiser
Parameters:
	hexValue: a value to be converted into a decimal value
Returns:
	A bit string version of the given hex value
*/
//convert straight from hexadecimal to binary -- COMPLETED
string convertHexBinary(string hexValue)
{
    int decimalValue = convertHexDecimal(hexValue);
    return convertDecimalBinary(decimalValue);
}

/*
Name: convertBinaryHex
Description: This function takes a bit string and converts it directly
    into a hexadecimal string.
Author: Benjamin Kaiser
Parameters:
	binaryValue: a bit string to be turned into a hexadecimal digit string
Returns:
	A hexadecimal string o the bit string
*/
//convert straight from binary to hex -- COMPLETED
string convertBinaryHex(string binaryValue)
{
    int decimalValue = convertBinaryDecimal(binaryValue);
    return convertDecimalHex(decimalValue);
}

/*
Name: readObject
Description: This function take the name of a file, opens it and reads
    the entire thing into memory.  This is basically like loading the binary
    file into memory on a real machine.
Author: Benjamin Kaiser
Parameters:
	objectfile: a string which contains the name of the object file
Returns:
	nothing
*/
//load into RAM -- COMPLETED
void readObject(string objectfile)
{
    ifstream fin;
    int contiguousMem;
    int memAddress;
    string tempHold = "";

    fin.open(objectfile);

    while (fin >> tempHold && fin >> contiguousMem)
    {
        memAddress = convertHexDecimal(tempHold);
        for (int i = memAddress; i < memAddress + contiguousMem; i++)
        {
            fin >> tempHold;
            RAM[i].value = tempHold;
            if (tempHold.length() == 6)
            {
                RAM[i].instruction = true;
            }
        }
    }
    PROGRAMSTART = tempHold;
    fin.close();
}

/*
Name: exitSimulation
Description: This function prints the reason for terminating the
    machine and then it calls a system exit to end the C++ program.
Author: Benjamin Kaiser
Parameters:
	reason: a string which is appened to "Machine halted - " to
    explain why the simulation ended.
Returns:
	Nothing
*/
//terminate wtih appropriate message -- COMPLETED
void exitSimulation(string reason)
{
    cout << "Machine halted - " << reason << endl;
    exit(0);
}

/*
Name: loadMARMDR
Description: This function is basically the transferr of data from
    the ABUS into the MAR and the DBUS into the MDR.  This gets called
    a lot because we have to do this to get in direct and other addressing
    besides immediate.
Author: Benjamin Kaiser
Parameters:
	address:  this is the address that the thing needs to go to in memory
Returns:
	Nothing
*/
//load the MAR and MDR -- COMPLETED
void loadMARMDR(string address)
{
    MAR = ABUS = address;
    MDR = convertHexBinary(RAM[convertBinaryDecimal(MAR)].value);
}

/*
Name: extractActual
Description: This function fills the MAR and MDR up to the point of getting the
    actual address and data in the instruction that was passed in.  This is
    how I deal with the different modes of addressing.
Author: Benjamin Kaiser
Parameters:
	instruction: this is the instruction that needs to be dealt with to get the
    actual values from direct, indirect, and various forms of indexed addressing.
Returns:
	Nothing
*/
//get the actual value from memory -- COMPLETED?
void extractActual(string instruction)
{
    string modeSpecifier = getAddressingModeFromInstruction(instruction);
    string registerSpecifier = getRegisterSpecifierFromInstruction(instruction);
    string address = getAddressFromInstruction(instruction);

    //direct
    if (modeSpecifier == "0000")
    {
        loadMARMDR(address);
    }
    //immediate
    else if (modeSpecifier == "0001")
    {
        MDR = getAddressFromInstruction(instruction);
    }
    //indexed
    else if (modeSpecifier == "0010")
    {
        if (registerSpecifier == "00")
        {
            int decimalAddress = convertHexDecimal(X[0]) + convertBinaryDecimal(getAddressFromInstruction(instruction));
            loadMARMDR(convertDecimalBinary(decimalAddress));
        }
        else if (registerSpecifier == "01")
        {
            int decimalAddress = convertHexDecimal(X[1]) + convertBinaryDecimal(getAddressFromInstruction(instruction));
            loadMARMDR(convertDecimalBinary(decimalAddress));
        }
        else if (registerSpecifier == "10")
        {
            int decimalAddress = convertHexDecimal(X[2]) + convertBinaryDecimal(getAddressFromInstruction(instruction));
            loadMARMDR(convertDecimalBinary(decimalAddress));
        }
        else if (registerSpecifier == "11")
        {
            int decimalAddress = convertHexDecimal(X[3]) + convertBinaryDecimal(getAddressFromInstruction(instruction));
            loadMARMDR(convertDecimalBinary(decimalAddress));
        }
    }
    //indirect
    else if (modeSpecifier == "0100")
    {
        //go out to memory and then go to memory
        loadMARMDR(getAddressFromInstruction(instruction));
        loadMARMDR(getAddressFromInstruction(IR));
    }
    //indexed indirect
    else if (modeSpecifier == "0110")
    {
        if (registerSpecifier == "00")
        {
            int decimalAddress = convertHexDecimal(X[0]) + convertBinaryDecimal(getAddressFromInstruction(instruction));
            loadMARMDR(convertDecimalBinary(decimalAddress));
            loadMARMDR(getAddressFromInstruction(IR));
        }
        else if (registerSpecifier == "01")
        {
            int decimalAddress = convertHexDecimal(X[1]) + convertBinaryDecimal(getAddressFromInstruction(instruction));
            loadMARMDR(convertDecimalBinary(decimalAddress));
            loadMARMDR(getAddressFromInstruction(IR));
        }
        else if (registerSpecifier == "10")
        {
            int decimalAddress = convertHexDecimal(X[2]) + convertBinaryDecimal(getAddressFromInstruction(instruction));
            loadMARMDR(convertDecimalBinary(decimalAddress));
            loadMARMDR(getAddressFromInstruction(IR));
        }
        else if (registerSpecifier == "11")
        {
            int decimalAddress = convertHexDecimal(X[3]) + convertBinaryDecimal(getAddressFromInstruction(instruction));
            loadMARMDR(convertDecimalBinary(decimalAddress));
            loadMARMDR(getAddressFromInstruction(IR));
        }
    }
}

/*
Name: printLine
Description: This is the print function that prints the line in the output file
    to the format specified by Dr. Karlsson.
Author: Benjamin Kaiser
Parameters:
	textualOpCode: The textual version of the opcode that was extracted from the
        instruction.
    addressingMode: The textual version of the addressing mode that was extracted
        from the instruction.
Returns:
	Nothing
*/
//print a line of the output file -- COMPLETED
void printLine(string textualOpCode, string addressingMode)
{
    cout << setw(3) << convertBinaryHex(IC) << ":  ";
    cout << setw(6) << setfill('0') << convertBinaryHex(IR) << "  " << setw(4) << setfill(' ') << textualOpCode << "  ";
    if (addressingMode == "0001")
    {
        cout << "IMM";
    }
    else if (addressingMode == "???")
    {
        cout << "???";
    }
    else
    {
        cout << setw(3) << setfill('0') << convertBinaryHex(getAddressFromInstruction(IR));
    }
    cout << "  AC[" << setw(6) << setfill('0') << AC.substr(0, 6) << "]  X0[" << X[0].substr(0, 3) << "]  X1[" << X[1].substr(0, 3) << "]  X2[";
    cout << X[2].substr(0, 3) << "]  X3[" << X[3].substr(0, 3) << "]" << endl;
}


/*******************************************************************************
The following instructions are the actual implementation of the B17 assembly
instructions.  At least the ones that were provided in the problem description
*******************************************************************************/
//end the simulation correctly.  -- COMPLETED
void HALT()
{
    exitSimulation("HALT - instruction executed");
}

//does nothing -- COMPLETED
void NOP()
{
    //just do things so compiler doesn't gripe
    int tempVal  = convertBinaryDecimal(IC);
    tempVal++;
}

//LD instruction: load accumulator from memory -- COMPLETED
void LD()
{
    extractActual(IR);
    //this isn't quite working to load instruction in
    AC = convertBinaryHex(MDR);
}
//ST instruction: store accumulator into memory -- COMPLETED
void ST()
{
    //if immediate we are illegal
    if (getAddressingModeFromInstruction(IR) == "0001")
    {
        printLine("ST", "???");
        exitSimulation("illegal addressing mode");
    }
    //otherwise store accumulator to memory
    ABUS = MAR; //in case the addressing is indirect
    DBUS = convertHexBinary(AC);
    extractActual(IR);
    RAM[convertBinaryDecimal(MAR)].value = convertBinaryHex(DBUS);
}
//EM instruction: swap the memory and the accumulator -- COMPLETED
void EM()
{
    //if immediate we are illegal
    if (getAddressingModeFromInstruction(IR) == "0001")
    {
        printLine("EM", "???");
        exitSimulation("illegal addressing mode");
    }
    //otherwise switch the memory and accumulator
    ABUS = MAR;
    DBUS = convertHexBinary(AC); //put AC in temp;
    extractActual(IR);
    LD(); //put memory in AC
    RAM[convertBinaryDecimal(MAR)].value = convertBinaryHex(DBUS);
}
//LDX instruction: put upper half of memory in index register -- COMPLETED
void LDX()
{
    string registerSpecifier = "";
    //if indexed then we're illegal
    if (getAddressingModeFromInstruction(IR) == "0010")
    {
        printLine("LDX", "???");
        exitSimulation("illegal addressing mode");
    }
    //if indirect then we're illegal
    if (getAddressingModeFromInstruction(IR) == "0100")
    {
        printLine("LDX", "???");
        exitSimulation("illegal addressing mode");
    }
    //if indexed indirect then we're illegal
    if (getAddressingModeFromInstruction(IR) == "0110")
    {
        printLine("LDX", "???");
        exitSimulation("illegal addressing mode");
    }
    extractActual(IR);
    registerSpecifier = getRegisterSpecifierFromInstruction(IR);
    X[convertBinaryDecimal(registerSpecifier)] = convertBinaryHex(getAddressFromInstruction(MDR));
}
//STX instruction: put index register in upper half of memory word -- COMPLETED
void STX()
{
    string registerSpecifier = "";
    //if anything but direct we are illegal
    if (getAddressingModeFromInstruction(IR) != "0000")
    {
        printLine("STX", "???");
        exitSimulation("illegal addressing mode");
    }
    extractActual(IR);
    registerSpecifier = getRegisterSpecifierFromInstruction(IR);
    RAM[convertBinaryDecimal(MAR)].value = RAM[convertBinaryDecimal(MAR)].value.replace(0, 3, X[convertBinaryDecimal(registerSpecifier)]);
}
//EMX instruction: swap upper half of memory word with index register -- COMPLETED
void EMX()
{
    string registerSpecifier = "";
    //if anything but direct we are illegal
    if (getAddressingModeFromInstruction(IR) != "0000")
    {
        printLine("EMX", "???");
        exitSimulation("illegal addressing mode");
    }
    ABUS = MAR;
    DBUS = convertHexBinary(X[convertBinaryDecimal(registerSpecifier)]); //put AC in temp;
    registerSpecifier = getRegisterSpecifierFromInstruction(IR);
    extractActual(IR);
    LDX(); //load index register
    RAM[convertBinaryDecimal(MAR)].value = RAM[convertBinaryDecimal(MAR)].value.replace(0, 3, convertBinaryHex(DBUS));
}
//ADD instruction: add memory contents to accumulator -- COMPLETED
void ADD()
{
    extractActual(IR);
    AC = convertDecimalHex(convertHexDecimal(AC) + convertBinaryDecimal(MDR));
}
//SUB instruction: subtract memory contents from accumulator -- COMPLETED
void SUB()
{
    extractActual(IR);
    AC = convertDecimalHex(convertHexDecimal(AC) - convertBinaryDecimal(MDR));
}
//clear the accumulator register -- COMPLETED
void CLR()
{
    AC = convertDecimalHex(0) + "0" + "0" + "0" + "0" + "0";
}
//COM: instruction complement the accumulator register -- COMPLETED
void COM()
{
    AC = convertDecimalHex(~convertHexDecimal(AC) + 1);
}
//AND instruction: and the memory into the accumulator -- COMPLETED
void AND()
{
    extractActual(IR);
    AC = convertDecimalHex(convertHexDecimal(MDR) & convertHexDecimal(AC));
}
//OR instruction: or the memory into the accumulator -- COMPLETED
void OR()
{
    extractActual(IR);
    AC = convertDecimalHex(convertHexDecimal(MDR) | convertHexDecimal(AC));
}
//XOR instruction: xor the memory into the accumulator -- COMPLETED
void XOR()
{
    extractActual(IR);
    AC = convertDecimalHex(convertHexDecimal(MDR) ^ convertHexDecimal(AC));
}
//ADDX instruction: add the memory to the specified index register -- COMPLETED
void ADDX()
{
    string registerSpecifier = getRegisterSpecifierFromInstruction(IR);
    //if indexed then we're illegal
    if (getAddressingModeFromInstruction(IR) == "0010")
    {
        printLine("ADDX", "???");
        exitSimulation("illegal addressing mode");
    }
    //if indirect then we're illegal
    if (getAddressingModeFromInstruction(IR) == "0100")
    {
        printLine("ADDX", "???");
        exitSimulation("illegal addressing mode");
    }
    //if indexed indirect then we're illegal
    if (getAddressingModeFromInstruction(IR) == "0110")
    {
        printLine("ADDX", "???");
        exitSimulation("illegal addressing mode");
    }
    registerSpecifier = getRegisterSpecifierFromInstruction(IR);
    extractActual(IR);
    X[convertBinaryDecimal(registerSpecifier)] = convertDecimalHex(convertHexDecimal(X[convertBinaryDecimal(registerSpecifier)]) + convertBinaryDecimal(MDR));
}
//SUBX instruction: subtract memory from the specified index register -- COMPLETED
void SUBX()
{
    string registerSpecifier = getRegisterSpecifierFromInstruction(IR);
    //if indexed then we're illegal
    if (getAddressingModeFromInstruction(IR) == "0010")
    {
        printLine("SUBX", "???");
        exitSimulation("illegal addressing mode");
    }
    //if indirect then we're illegal
    if (getAddressingModeFromInstruction(IR) == "0100")
    {
        printLine("SUBX", "???");
        exitSimulation("illegal addressing mode");
    }
    //if indexed indirect then we're illegal
    if (getAddressingModeFromInstruction(IR) == "0110")
    {
        printLine("SUBX", "???");
        exitSimulation("illegal addressing mode");
    }
    registerSpecifier = getRegisterSpecifierFromInstruction(IR);
    extractActual(IR);
    X[convertBinaryDecimal(registerSpecifier)] = convertDecimalHex(convertHexDecimal(X[convertBinaryDecimal(registerSpecifier)]) - convertBinaryDecimal(MDR));
}
//CLRX instruction: clear specified index register -- COMPLETED
void CLRX()
{
        X[convertBinaryDecimal(getRegisterSpecifierFromInstruction(IR))] = convertDecimalHex(0) + "0" + "0";
}
//J instruction: jump to memory address -- COMPLETED
void J()
{
    //if we are immediate then we are illegal
    if (getAddressingModeFromInstruction(IR) == "0001")
    {
        printLine("J", "???");
        exitSimulation("illegal addressing mode");
    }
    extractActual(IR);
    IC = MAR;
}
//JZ instruction: jump to memory address if zero -- COMPLETED
void JZ()
{
    //if immediate we're illegal
    if (getAddressingModeFromInstruction(IR) == "0001")
    {
        printLine("JZ", "???");
        exitSimulation("illegal addressing mode");
    }
    if (convertHexDecimal(AC) == 0)
    {
        extractActual(IR);
        IC = MAR;
    }
}
//JN instruction: jump to memory address if negative -- COMPLETED
void JN()
{
    //if immediate we're illegal
    if (getAddressingModeFromInstruction(IR) == "0001")
    {
        printLine("JN", "???");
        exitSimulation("illegal addressing mode");
    }
    if (convertHexDecimal(AC) < 0)
    {
        extractActual(IR);
        IC = MAR;
    }
}
//JP instruction: jump to memeory address if positive -- COMPLETED
void JP()
{
    //if immediate we're illegal
    if (getAddressingModeFromInstruction(IR) == "0001")
    {
        printLine("JP", "???");
        exitSimulation("illegal addressing mode");
    }
    if (convertHexDecimal(AC) > 0)
    {
        extractActual(IR);
        IC = MAR;
    }
}

/*
    Name:  ALUcompute
    Author: Benjamin Kaiser
    Description:  This function basically performs the same thing the ALU in the
        physical machine should perform.  It simply consists of a giant if statement
    Parameters:
        instruction_code: a textual representation of the opCode that was extracted
            from the instruction.
        instruction: the actual instruction being executed
    Returns:
        a boolean of whether or not a jump instruction was executed
*/
//this is the process instruction -- COMPLETED
bool ALUcompute(string instruction_code, string instruction)
{
    bool jump = false;
    if (instruction_code == "HALT")
    {
        HALT();
    }
    else if (instruction_code == "NOP")
    {
        NOP();
    }
    else if (instruction_code == "LD")
    {
        LD();
    }
    else if (instruction_code == "ST")
    {
        ST();
    }
    else if (instruction_code == "EM")
    {
        EM();
    }
    else if (instruction_code == "LDX")
    {
        LDX();
    }
    else if (instruction_code == "STX")
    {
        STX();
    }
    else if (instruction_code == "EMX")
    {
        EMX();
    }
    else if (instruction_code == "ADD")
    {
        ADD();
    }
    else if (instruction_code == "SUB")
    {
        SUB();
    }
    else if (instruction_code == "CLR")
    {
        CLR();
    }
    else if (instruction_code == "COM")
    {
        COM();
    }
    else if (instruction_code == "AND")
    {
        AND();
    }
    else if (instruction_code == "OR")
    {
        AND();
    }
    else if (instruction_code == "XOR")
    {
        XOR();
    }
    else if (instruction_code == "ADDX")
    {
        ADDX();
    }
    else if (instruction_code == "SUBX")
    {
        SUBX();
    }
    else if (instruction_code == "CLRX")
    {
        CLRX();
    }
    else if (instruction_code == "J")
    {
        J();
        jump = true;
    }
    else if (instruction_code == "JZ")
    {
        JZ();
        jump = true;
    }
    else if (instruction_code == "JN")
    {
        JN();
        jump = true;
    }
    else if (instruction_code == "JP")
    {
        JP();
        jump = true;
    }
    return jump;
}

/*
    Name:  incrementIC
    Author: Benjamin Kaiser
    Description:  This function simply increments the instruction counter to the
        next memory address.
    Parameters:
        Nothing
    Returns:
        Nothing
*/
//increments the IC -- COMPLETED
void incrementIC()
{
    IC = convertDecimalBinary(convertBinaryDecimal(IC) + 1);
}

/*
    Name:  getTextualOpcode
    Author: Benjamin Kaiser
    Description:  This function takes a decimal version of the
        extracted opcode and indexes into the INSTRUCTION_CODES array
        to get the textual version.
    Parameters:
        opcode: a decimal version of the opcode extracted from an instruction
    Returns:
        The text version of the opcode. If it wasn't found, it simply returns
        four question marks.
*/
//turns the decimal version of opcode into text -- COMPLETED
string getTextualOpcode(int opcode)
{
    for (int i = 0; i < 22; i++)
    {
        if (INSTRUCTIONOPCODE[i] == opcode)
        {
            return INSTRUCTION_CODES[i];
        }
    }
    return "????";
}

/*
    Name:  main
    Author: Benjamin Kaiser
    Description:  This is the main function of the C++ program.
        It takes the command line argument and loads it into memory.  It then
        enters an infite loop.  This loop starts at the original memory address
        indicated in the object file.  It then increments by 1 unless a jump
        instruction was executed.  It then calls the ALUcompute function which
        executes each instruction.
    Parameters:
        argc,argv - for command line arguments
    Returns:
        Nothing
*/
int main(int argc, char** argv)
{
    //Read the object file into memory
    readObject(argv[1]);

    bool jump = false;

    int opCodeIndex = 0;

    string textualOpCode = "";
    int decimalOpCode = 0;
    string addressingMode = "";

    IC = convertHexBinary(PROGRAMSTART);

    //run program
    while(true)
    {
        loadMARMDR(IC);
        IR = MDR;
        //extract opcode then get the textual opcode
        decimalOpCode = convertBinaryDecimal(getOpCodeFromInstruction(MDR));
        textualOpCode = getTextualOpcode(decimalOpCode);
        addressingMode = getAddressingModeFromInstruction(MDR);

        if (textualOpCode == "????")
        {
            printLine(textualOpCode, addressingMode);
            exitSimulation("undefined opcode");
        }

        jump = ALUcompute(textualOpCode, MDR);//needs to have the textual opcode passed
        printLine(textualOpCode, addressingMode);

        if (!jump)
        {
            incrementIC();
        }

    }

    return 0;
}
