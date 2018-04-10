/////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdlib.h> 
#include <Windows.h> //sleep
#include <climits> // for INT_MAX limits
#include <conio.h> // for _getch()

using namespace std;
using namespace cv;

void help(); // deklaracja, zeby inne funkcje widzialy helpa


//function which displays image on the screen
void show_image(string path) // show_image(point path to the image)
{
//	 opencv ___________   IplImage* image = cvLoadImage(img); // loading image
	 Mat image = imread(path, CV_LOAD_IMAGE_COLOR);

//	 opencv ___________  if (image == 0)  // checking if the img was loaded
	 if (!image.data)
	{
		system("cls");
		cout << "You did not load the image!";
		Sleep(2000);
		getchar(); // waiting for key
		help();
	}

	namedWindow("That's image", WINDOW_AUTOSIZE);  // creating window with a name
	startWindowThread(); 
//	opencv __________  cvShowImage("That's image", image); // showing image within the window
	imshow("That's image", image);


	while (waitKey(0) != 27) // waiting for key (otherwise it would close immediately) btw. 27 is space in ESCAPE
	{
		system("cls");
		cout << "press ESCAPE to exit" << endl;
	}

	image.release();  // releasing header (naglowek) and image data (isnt really needed when we are operating cv2's Mat objects)
	destroyAllWindows(); // just destroying window

	/*
//	  opencv ___________  cvReleaseImage(&image);
	*/
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// function that displays live video from the camera onboard
void camera_output(const int id = 0) //camera_output(camera's id; default->0)
{
	VideoCapture stream(id); // class ~ getting the video from camera (Number 'id' refers to camera's id integer, i have only one camera so its id is 0)
	// LUB _______ stream.open(0);
	stream.set(CAP_PROP_FRAME_WIDTH, 840); // szerokosc okna
	stream.set(CAP_PROP_FRAME_HEIGHT, 640); // wysokosc okna

	if (!stream.isOpened()) // checking if we successed
	{
		system("cls");
		cout << "Video capture failed! Check your camera :)";
		getchar(); // waiting for key
		Sleep(2000);
		help();
	//	exit(EXIT_FAILURE);
	}


	Mat image; // class which handle images, it treats images as a Matrix(macierz) - Two-Dimensional Array(Tablica dwuwymiarowa)
	namedWindow("Camera output", CV_WINDOW_AUTOSIZE); // creating window (does not work without it)
	

	while (true)
	{
		try
		{ 
			stream.read(image); //reading video
			putText(image, "That's you", cvPoint(30, 250), FONT_HERSHEY_COMPLEX_SMALL, 6, cvScalar(124, 252, 0), 3, CV_AA);  //displaying text 
			imshow("Camera output", image); // showing the video to the user, on a screen 
		}

		catch (Exception& error) // error implementation
		{
			const char* error_msg = error.what();
			cout << "Something went wrong!" << '\n' << error_msg;
		}

		waitKey(30); // we have to use waitKey otherwise video_output won't work due to time that operating system takes to display the image
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//function that displays your text on a solid colour image
void show_text(string wName, string text, int size = 1, cv::Scalar bgColor = Scalar(24, 53, 12), cv::Scalar txtColor = Scalar(243, 232, 225)) // show_text(window name, your text, text size, text colour, backgroud colour)
{
	namedWindow(wName, 1);


	Mat image(480, 640, CV_8UC3, bgColor); // height. width, type CV_8UC3 means 8-bit unsigned int, 3 channels, BGR format colour |
										   //CV_[The number of bits per item][Signed or Unsigned][Type Prefix]C[The channel number]
	putText(image, text, cvPoint(30, 100), FONT_HERSHEY_COMPLEX_SMALL, size, txtColor);  // insert a text into Mat image
	imshow(wName, image);
	while (waitKey(0) != 27) // waiting for key (otherwise it would close immediately) btw. 27 is ESCAPE
	{
		system("cls");
		cout << "press ESCAPE to exit" << endl;
	}
	destroyAllWindows(); // just destroying windows
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void help()
{
	// variables (user will input them)
	int id, size;
	string path, wName, text;
	char n;

	// Basic MENU using switch statement
	cout << " \n What you wanna do? \n";
	cout << "1. OPEN IMAGE " << endl << "2. CAMERA LIVE" << endl << "3. YOUR OWN TEXT \n";
	n = _getch();

	switch(n)
	{
		case '1':
					cout << " \n Input image's PATH ### Ex. image.png \n ";
					cin >> path;
					show_image(path);
		break;

		case '2':
					cout << " \n Input camera's id ### Default -> 0 \n ";
					cout << "...in range 0-9 \n ";
					cin >> id;
					if (id >= 0 && id <= 9) // checking if the id is <= 9, otherwise program will crash af
					{
						camera_output(id);
					}
					else
					{
						cin.clear(); // clear input buffer to restore cin to a usable state
						cin.ignore(INT_MAX, '\n'); // ignore last input
						system("cls");
						cout << "ERROR, id range is 0-9! \n ";
						Sleep(2000);
						help();
					}
		break;

		case '3':
					cout << " \n *Input Window name \n ";
					getline(cin, wName); // dunno why but it doesn't work when single getline is there
					getline(cin, wName);
					cout << " \n *Input YourText \n ";
					getline(cin, text);
					cout << " \n *Input size ### Ex. 1-10 \n ";
					cin >> size;

					show_text(wName, text, size);
		break;


		case 'q':
				exit(0);
		break;


		case 'quit':
				exit(0);
		break;

		 
		default:
			system("cls");
			help();
	}
}


int main(int argc, char** argv)
{
	 cout << "_______ ________ _______ OpenCV's simple console APP by Daniel B. _______ _________ ________";
	 help();

	return 0;
}
