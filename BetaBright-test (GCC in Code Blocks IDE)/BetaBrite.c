
// test for sending commands to BetaBrite LED display over serial port
// BetaBrite uses ALPHA sign communications protocol
// Transmission format:
//    NUL(min of 5), SOH, Type Code, Sign Add, STX, cmd code, data field, EOT
// Data Field format: ESC, Disp Pos, Mode Code, Special Specifier, ASCII message


#include <stdio.h>
#include <stdlib.h>
#include "rs232.h"
#include <windows.h>

#define NUL 0x00
#define SOH 0x01 //Start Of Header
#define STX 0x02 //Start Of Text
#define EOT 0x04 //End Of Transmission
#define ESC 0x1B //escape

#define Port 0
#define BAUD 9600


int main()
{
    int i;

    printf("Send bytes to serial port\n");

//----------------------------------------------

    RS232_OpenComport(Port,BAUD);

    for(i=0;i<10;i++)
    {
        RS232_SendByte(Port,NUL); //send 10 NULLs (sync comm port on display)
    }
    RS232_SendByte(Port,SOH);
    RS232_cputs(Port, "Z00");  //Z=all sign types, 00=any address
    RS232_SendByte(Port,STX);
    RS232_SendByte(Port,'A');  //command, A=write text
    RS232_SendByte(Port,'A');  //A=file label
    RS232_SendByte(Port,ESC);  //start of Data Field, ESC always precedes other data
    RS232_SendByte(Port,'0');  //position code, 0=Fill and center vert.
    RS232_SendByte(Port,'b');  //mode code, b=HOLD (msg is stationary)
    RS232_SendByte(Port,0x1C); //control code, 1C=select char color
    RS232_SendByte(Port,'2');  //color specifier, 2=green
    RS232_cputs(Port, "OPEN"); //actual text to display
    RS232_SendByte(Port,EOT);

    RS232_CloseComport(Port);

    Sleep(1000);

//-----------------------------------------

    RS232_OpenComport(Port,BAUD);

    for(i=0;i<10;i++)
    {
        RS232_SendByte(Port,NUL);
    }
    RS232_SendByte(Port,SOH);
    RS232_cputs(Port, "Z00");  //Z=all sign types, 00=any address
    RS232_SendByte(Port,STX);
    RS232_SendByte(Port,'A');  //command, A=write text
    RS232_SendByte(Port,'A');  //A=file label
    RS232_SendByte(Port,ESC);  //start of Data Field, ESC always precedes other data
    RS232_SendByte(Port,'0');  //position code, 0=Fill and center vert.
    RS232_SendByte(Port,'b');  //mode code, b=HOLD (msg is stationary)
    RS232_SendByte(Port,0x1C); //control code, 1C=select char color
    RS232_SendByte(Port,'1');  //color specifier, 1=red
    RS232_cputs(Port, "CLOSED"); //actual text to display
    RS232_SendByte(Port,EOT);

    RS232_CloseComport(Port);

    Sleep(1000);

//--------------------------------------------

    RS232_OpenComport(Port,BAUD);

    for(i=0;i<10;i++)
    {
        RS232_SendByte(Port,NUL);
    }
    RS232_SendByte(Port,SOH);
    RS232_cputs(Port, "Z00");  //Z=all sign types, 00=any address
    RS232_SendByte(Port,STX);
    RS232_SendByte(Port,'A');  //command, A=write text
    RS232_SendByte(Port,'A');  //A=file label
    RS232_SendByte(Port,ESC);  //start of Data Field, ESC always precedes other data
    RS232_SendByte(Port,'0');  //position code, 0=Fill and center vert.
    RS232_SendByte(Port,'n');  //mode code, n=Special (next char defines)
    RS232_SendByte(Port,'0');  //special mode code, 0=Twinkle, 1=Sparkle, 2=Snow
    RS232_SendByte(Port,0x1C); //control code, 1C=select char color
    RS232_SendByte(Port,'B');  //color specifier, B=Color Mix (ea char is a diff color)
    RS232_cputs(Port, "WELCOME TO MAKE SPACE"); //actual text to display
    RS232_SendByte(Port,EOT);

    RS232_CloseComport(Port);

    Sleep(1000);


    return 0;
}
