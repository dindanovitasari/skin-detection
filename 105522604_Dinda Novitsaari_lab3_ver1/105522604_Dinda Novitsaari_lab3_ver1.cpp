// 105522604_Dinda Novitsaari_lab3_ver1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<math.h>
#include<iostream>
using namespace std;
using namespace cv;


int main(void)
{
	Mat frame, gaussBlurImg, ycbImage;

	std::vector<std::vector<Point>> contours; // init contours

	string filename = "input.mp4";
	VideoCapture capture(filename);

	namedWindow("Original video", 1);
	for (;;)
	{

		Mat newCut;

		capture >> frame;
		if (frame.empty())
			break;

		int frameOf = capture.get(CV_CAP_PROP_POS_FRAMES);

		//Read the video, and use Gaussian Blur to get smooth video. 
		//kernel size must k%2=1
		GaussianBlur(frame, gaussBlurImg, Size(3, 3), 2, 0);

		//Convert the color space to YCbCr.
		cvtColor(gaussBlurImg, ycbImage, COLOR_BGR2YCrCb);

		//Extract the skin color pixels with the values in Cb channel and Cr channel.
		//lower 0 133 98 upper 255 177 122
		inRange(ycbImage, cv::Scalar(0, 140, 98), cv::Scalar(255, 177, 122), ycbImage);

		//frame copy
		frame.copyTo(newCut, ycbImage);

		//finding contour
		//retrieves only the extreme outer contours
		//compresses horizontal, vertical, and diagonal segments and leaves only their end points
		findContours(ycbImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); //calculate contours
		vector<Rect> boundRect(contours.size());
		for (int i = 0; i<contours.size(); i++)
		{
			double n = contourArea(contours[i]); //calculate  area of contour

			//thresholding
			if (n>10000) {

				//draw contours[i] on image_gbr
				drawContours(newCut, contours, i, Scalar(255, 255, 255));
				
				boundRect[i] = boundingRect(Mat(contours[i]));

				rectangle(newCut, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 1, 8, 0);

				//convert to string
				std::ostringstream strs;
				strs << n;
				std::string str = strs.str();

				//drawing text
				putText(newCut, str, boundRect[i].tl(),
					FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 255, 0), 1, CV_AA);		
			}// end of thresholding
		}

		//showing video
		imshow("Original video", newCut);

		//save images
		if (frameOf == 100) {
			imwrite("result100.jpg", newCut);
		}
		else if (frameOf == 200) {
			imwrite("result200.jpg", newCut);
		}
		waitKey(20);
	}
	waitKey(0);

}




