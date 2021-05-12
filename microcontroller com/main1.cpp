/*
// This program allows for the arduino to send encoder volumes to the PC
// The volumes based on application selection on the touchscreen
// can be combind with volume control and immage upload in 2 ways
// 1) through a shared file
// 2) combine 'main' files 
*/

//inclluded libraries
#include <iostream>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <fstream>
#include"SerialPort.h"
#include "Image.h"
using namespace std;


//global varables
bool getInput(char *c);
char incomingData[MAX_DATA_LENGTH];
char output[MAX_DATA_LENGTH];

char volume[4];
char sel[1];
char outputTMP[MAX_DATA_LENGTH];

// change the name of the port with the port name of your computer
// must remember that the backslashes are essential so do not remove them

//const char *port;// = "\\\\.\\COM4";

int main()
{
	//opens serial port
	//port = AutodetectArduinoPort();
	string portNum = "\\\\.\\COM2";
	char * port = new char[portNum.size() + 2];

	copy(portNum.begin(), portNum.end(), port);
	port[portNum.size()] = '\0';


	for(int i = 0; i < 100; i++)
	{
		if(i < 10)
		{
			port[7] = i+48;
		}
		else
		{
			port[7] = (i/10)+48;
			port[8] = (i%10)+48;
			port[9] = '\0';
		}
		SerialPort arduino(port);
		if(arduino.isConnected())
		{
			Sleep(5);
			arduino.readSerialPort(output, 1);
			if(output[0] == 'C')
			{
				i = 100;
			}
			
		}
	}

	output[0] = '\0';
	SerialPort arduino(port);
	

	ShellExecute(NULL, "open", "ExtractMasterLevel.exe", NULL, NULL, SW_SHOWDEFAULT);

	//varables
	int tmp1 = 0;	//volume temp
	int selNum = 0;	//application selection
	int volumeOut = 0;	//volume that gets updated
	char current[3];
	int startingVol;

	//this will be changed to read current volume from file
	//130 represents 0 so the volume read in will need to have 130 added to it
	//the first value is not used to simplify things (tmp2[1] - tmp2[6] store volume data) 
	//then the data will need to be sent to the microcontroller
	int tmp2[7] = {130, 130, 220, 220, 220, 220, 220}; // stores volume for each application 


	//this is where the volume update of (tmp2[1] - tmp2[6]} should take place (read from file)
	//-------------------------------------------------------------------------------------

	

	//-------------------------------------------------------------------------------------

	//detects if there is volumetric connected
	if(arduino.isConnected()){
		cout<<"Connection made" <<endl;
	}
	else{
		cout<<"Error in port name"<<endl;
	}

	//this is where the immage upload should take place
	//-------------------------------------------------------------------------------------


	Image immage(0,0);

	immage.Read("Back.bmp");
	immage.sendB(arduino);

    immage.Read("Program1.bmp");
    immage.send(arduino);

	immage.Read("Program2.bmp");
    immage.send(arduino);

	immage.Read("Program3.bmp");
    immage.send(arduino);

	immage.Read("Program4.bmp");
    immage.send(arduino);

	immage.Read("Program5.bmp");
    immage.send(arduino);

	immage.Read("Program6.bmp");
    immage.send(arduino);

	//-------------------------------------------------------------------------------------

	//opens levels.txt for reading
	string line;
	fstream myfile;
	myfile.open ("level.txt", ios::in);

	if (myfile)
  	{
		//reads defult volume for level.txt
    	myfile >> current[0];
		myfile >> current[1];
		current[2] = '\0';
		startingVol = atoi(current);
		cout<<"Master Vol: "<< startingVol <<endl;

		//sets current volume for every application to that volume
		tmp2[1] += startingVol;
  	}
  	else cout << "Unable to open file all volumes set to Zero";

	myfile.close();

	//sends starting volume to arduino
	output[0] = static_cast<unsigned char>(startingVol);
	arduino.writeSerialPort(output, 1);

	cout<<"Ready to Recieve data: " <<endl;

	//runs while volumetric is connected
	while(arduino.isConnected())
    {

		//reads data sent from arduino
		while(volume[2] == output[3] && arduino.isConnected())
		{
			arduino.readSerialPort(output, 4);
			Sleep(1);
		}

		sel[0] = output[0];
		volume[0] = output[1];
		volume[1] = output[2];
		volume[2] = output[3];
		volume[3] = '\0';

		tmp1 = atoi(volume);
		selNum = atoi(sel);

		//this statement makes sure the program doesnt glitch out and jump in volume
		if(tmp1 < tmp2[selNum] - 13 || tmp1 > tmp2[selNum] + 13)
		{
			tmp1 = 0;
		}

		//these statements make sure the data is only updated on a change to prevent noise 
		if(tmp1 > tmp2[selNum] && tmp1 != 0 && tmp1 < 230)
		{
				//adjusts the varables that keep track of the 
				tmp2[selNum] = tmp1;
				volumeOut = tmp2[selNum] - 130;
		
				//couts are for debugging and can be deleted later
				cout<<"app: "<<output[0] <<" - Vol: "<< volumeOut <<endl;

				//Can output to file from here (need to do both places)
				//or change volume directly from here
				//volume is the adjusted volume varable
				//selNum is the selected application varable (gives a number 1-6)

				//this is where the volume update to the file should take place
				//-------------------------------------------------------------------------------------
				fstream VolumeFile;
				fstream AppFile;

				VolumeFile.open ("level.txt", ios::out);
				AppFile.open ("number.txt", ios::out);

				if (VolumeFile && AppFile)
				{
					VolumeFile << volumeOut;
					AppFile << selNum - 1;
					VolumeFile.close();
					AppFile.close();

					ShellExecute(NULL, "open", "VolumeControlApp.exe", NULL, NULL, SW_SHOWDEFAULT);
					Sleep(1);
					
				}
				else
				{
					cout << "File edit error" << endl;
					VolumeFile.close();
					AppFile.close();
				}
				//-------------------------------------------------------------------------------------

		}
		else if(tmp1 < tmp2[selNum]  && tmp1 > 130)
		{
				tmp2[selNum] = tmp1;
				volumeOut = tmp2[selNum] - 130;
		
				//couts are for debugging and can be deleted later
				cout<<"app: "<<output[0] <<" - Vol: "<< volumeOut <<endl;
		
				//Can output to file from here (need to do both places)
				//or change volume directly from here
				//volume is the adjusted volume varable
				//selNum is the selected application varable (gives a number 1-6)

				//this is where the volume update to the file should take place
				//-------------------------------------------------------------------------------------
				fstream VolumeFile;
				fstream AppFile;

				VolumeFile.open ("level.txt", ios::out);
				AppFile.open ("number.txt", ios::out);

				if (VolumeFile && AppFile)
				{
					VolumeFile << volumeOut;
					AppFile << selNum - 1;
					VolumeFile.close();
					AppFile.close();

					ShellExecute(NULL, "open", "VolumeControlApp.exe", NULL, NULL, SW_SHOWDEFAULT);
					Sleep(1);
				}
				else
				{
					cout << "File edit error" << endl;
					VolumeFile.close();
					AppFile.close();
				}
				//-------------------------------------------------------------------------------------

		} 

	}

	delete[] port;
	
}