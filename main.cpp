/////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <stdlib.h> 
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <Windows.h> //sleep
#include <stdio.h> // isdigit
#include <climits> // for INT_MAX limits
#include <conio.h> // for _getch()


using namespace std;
using namespace cv;

void help(); // deklaracja, zeby inne funkcje widzialy helpa




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
void vision_manipulate()
{
VideoCapture stream(0); // class ~getting the video from camera(Number 'id' refers to camera's id integer, i have only one camera so its id is 0)
string window_name[] = { "Kamera", "Binary", "Kontur" }; // window names (interesting way of creation with a array)
Mat frame, img, hsv_img, binary, cont; // creating Mat object
vector<Mat> hsv_split; // creating vector; space for hsv channels 
for (int i = 0; i < 2; i++) namedWindow(window_name[i], CV_WINDOW_AUTOSIZE);    // creating two windows using simple 'for' loop
int lowerb = 100, upperb = 109; // determination of initial values for thresholding
createTrackbar("Need to know", window_name[1], &lowerb, 255, NULL); // nazwa przy suwaku, nazwa okna, zmienna ktora bedzie zmieniana za pomoca suwaka, odwolanie do innej funkcji
//Utworzenie trackbar'ow 
createTrackbar("This too mate", window_name[1], &upperb, 255, NULL);
while (waitKey(20) != 27) // 20ms waitkey 
{
	stream.read(frame); // getting the frame 
	frame.copyTo(img); // copy frame to Mat img using copyTo
	frame.copyTo(hsv_img); // copy frame to Mat img using copyTo
	cvtColor(img, hsv_img, CV_BGR2HSV); // conversion to HSV
	split(hsv_img, hsv_split); // splitting the hsv on individual channels 
	inRange(hsv_split[0], lowerb, upperb, binary);                        //Progowanie zgodnie z wartosciami lowerb, i upperb 
	Mat element(3, 3, CV_8U, Scalar(1));    //Okreslenie opcji erozji 
	blur(binary, binary, Size(3, 3));        // blur - rozmycie
	erode(binary, binary, element);            // Erozja 


	vector<vector<Point> > contours;
	vector<Point> contours_poly;
	Rect boundRect;
	binary.copyTo(cont);
	findContours(cont, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	int max = 0, i_cont = -1;
	Mat drawing = Mat::zeros(cont.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		if (abs(contourArea(Mat(contours[i]))) > max)
		{
			max = abs(contourArea(Mat(contours[i])));
			i_cont = i;
		}
	}
	if (i_cont >= 0)
	{
		approxPolyDP(Mat(contours[i_cont]), contours_poly, 3, true);
		boundRect = boundingRect(Mat(contours_poly));
		fillConvexPoly(img, contours_poly, contours_poly.size());
		rectangle(img, boundRect.tl(), boundRect.br(), Scalar(125, 250, 125), 2, 8, 0);
		line(img, boundRect.tl(), boundRect.br(), Scalar(250, 125, 125), 2, 8, 0);
		line(img, Point(boundRect.x + boundRect.width, boundRect.y), Point(boundRect.x, boundRect.y + boundRect.height), Scalar(250, 125, 125), 2, 8, 0);
		string s;
		stringstream out;
		out << boundRect.x + boundRect.width / 2 << "x" << boundRect.y + boundRect.height / 2;
		s = out.str();
		putText(img, s, Point(50, 50), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(20, 40, 80), 3, 8);
		drawContours(drawing, contours, i_cont, Scalar(125, 125, 250), 2);
	}
	imshow(window_name[1], drawing);



	imshow(window_name[0], img);            //Obrazek Orginalny 
	imshow(window_name[1], binary);            //Obraz binarny 
}
stream.release();                        //Zwolnienie uchwytu kamery 
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
	cout << "1. OPEN IMAGE " << endl << "2. CAMERA LIVE" << endl << "3. YOUR OWN TEXT" << endl << "4. AlLPA.visionCONTROl";
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

		case '4':
			cout << " \n Thats ultra alpha mate, i haven't written it";
			vision_manipulate();
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
//	cout << "_______ ________ _______ OpenCV's simple console APP by Daniel B. _______ _________ ________";

	help();

	return 0;
}
