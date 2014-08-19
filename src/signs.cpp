#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
//#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include "opencv2/video/tracking.hpp"

using namespace cv;
using namespace std;

string intToString(int number)
{

	std::stringstream ss;
	ss << number;
	return ss.str();
}

class Symbol
{

public:
	Mat img;
	string name;

};



void sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center)
{
	std::vector<cv::Point2f> top, bot;

	for (unsigned int i = 0; i < corners.size(); i++)
	{
		if (corners[i].y < center.y)
			top.push_back(corners[i]);
		else
			bot.push_back(corners[i]);
	}

	cv::Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
	cv::Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
	cv::Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
	cv::Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];

	corners.clear();
	corners.push_back(tl);
	corners.push_back(tr);
	corners.push_back(br);
	corners.push_back(bl);
}



int readRefImages(Symbol *symbols)
{

	symbols[0].img = imread("arrowL.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!symbols[0].img.data)
		return -1;
	threshold(symbols[0].img, symbols[0].img, 100, 255, 0);
	symbols[0].name = "Left 90";

	symbols[1].img = imread("arrowR.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!symbols[1].img.data)
		return -1;
	threshold(symbols[1].img, symbols[1].img, 100, 255, 0);
	symbols[1].name = "Right 90";

	symbols[2].img = imread("arrowT.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!symbols[2].img.data)
		return -1;
	threshold(symbols[2].img, symbols[2].img, 100, 255, 0);
	symbols[2].name = "Turn Around";

	symbols[3].img = imread("arrowB.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!symbols[3].img.data)
		return -1;
	threshold(symbols[3].img, symbols[3].img, 100, 255, 0);
	symbols[3].name = "Ball";

	symbols[4].img = imread("arrowL45.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!symbols[4].img.data)
		return -1;
	threshold(symbols[4].img, symbols[4].img, 100, 255, 0);
	symbols[4].name = "Left 45";

	symbols[5].img = imread("arrowR45.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!symbols[5].img.data)
		return -1;
	threshold(symbols[5].img, symbols[5].img, 100, 255, 0);
	symbols[5].name = "Right 45";

	symbols[6].img = imread("arrowStop.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!symbols[6].img.data)
		return -1;
	threshold(symbols[6].img, symbols[6].img, 100, 255, 0);
	symbols[6].name = "Stop";

	symbols[7].img = imread("arrowGo.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!symbols[7].img.data)
		return -1;
	threshold(symbols[7].img, symbols[7].img, 100, 255, 0);
	symbols[7].name = "Go";

	return 0;
}


int lowThreshold;


void CannyThreshold(int, void*)
{
}



int main_(int argc, char** argv)
{
	// check http://docs.opencv.org/doc/tutorials/features2d/table_of_content_features2d/table_of_content_features2d.html
	// for OpenCV general detection/matching framework details

	VideoCapture cap;

	cap.open("file.h264");

	namedWindow("A", CV_WINDOW_AUTOSIZE);
	namedWindow("B", CV_WINDOW_AUTOSIZE);
	namedWindow("C", CV_WINDOW_AUTOSIZE);

	Mat camera;
	Mat greyImg;

	Symbol symbols[10];
	if (readRefImages(symbols) == -1)
	{
		printf("Error reading reference symbols\n");
		return -1;
	}

	createTrackbar("Min Threshold:", "A", &lowThreshold, 100, CannyThreshold);

	double t = (double) getTickCount();
	// do something ...
	//t = ((double)getTickCount() - t)/getTickFrequency();
	//cout << "Times passed in seconds: " << t << endl;

	cap >> camera;

	Mat new_image;

	while (1)
	{

		t = (double) getTickCount() - t;

		cap >> camera;

		cvtColor(camera, greyImg, CV_RGB2GRAY);

		//threshold(greyImg, greyImg, 100, 255, 0);

		Mat canny_output;
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		GaussianBlur(greyImg, greyImg, Size(9, 9), 2, 2);

		/// Detect edges using canny
		Canny(greyImg, canny_output, lowThreshold, lowThreshold * 3, 3);

		//	imshow("B",canny_output);
		/// Find contours
		findContours(canny_output, contours, hierarchy, CV_RETR_TREE,
				CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		vector<Point> approxRect;

		for (size_t i = 0; i < contours.size(); i++)
		{
			approxPolyDP(contours[i], approxRect,
					arcLength(Mat(contours[i]), true) * 0.05, true);
			if (approxRect.size() == 4)
			{
				float area = contourArea(contours[i]);

				if (area > 10000)
				{
					std::vector<cv::Point2f> corners;

					vector<Point>::iterator vertex;
					vertex = approxRect.begin();
					//vertex++;
					circle(camera, *vertex, 2, Scalar(0, 0, 255), -1, 8, 0);
					corners.push_back(*vertex);
					vertex++;
					circle(camera, *vertex, 2, Scalar(0, 0, 255), -1, 8, 0);
					corners.push_back(*vertex);
					vertex++;
					circle(camera, *vertex, 2, Scalar(0, 0, 255), -1, 8, 0);
					corners.push_back(*vertex);
					vertex++;
					circle(camera, *vertex, 2, Scalar(0, 0, 255), -1, 8, 0);
					corners.push_back(*vertex);

					Moments mu;
					mu = moments(contours[i], false);
					Point2f center(mu.m10 / mu.m00, mu.m01 / mu.m00);

					sortCorners(corners, center);

					// Define the destination image
					Mat correctedImg = ::Mat::zeros(195, 271, CV_8UC3);

					// Corners of the destination image
					std::vector<cv::Point2f> quad_pts;
					quad_pts.push_back(Point2f(0, 0));
					quad_pts.push_back(Point2f(correctedImg.cols, 0));
					quad_pts.push_back(
							Point2f(correctedImg.cols, correctedImg.rows));
					quad_pts.push_back(Point2f(0, correctedImg.rows));

					// Get transformation matrix
					Mat transmtx = getPerspectiveTransform(corners, quad_pts);

					// Apply perspective transformation
					warpPerspective(camera, correctedImg, transmtx,
							correctedImg.size());

					Mat correctedImgBin;

					cvtColor(correctedImg, correctedImgBin, CV_RGB2GRAY);

					//equalizeHist(correctedImgBin, correctedImgBin);

					correctedImgBin.copyTo(new_image);

					threshold(correctedImgBin, correctedImgBin, 140, 255, 0);

					imshow("B", correctedImgBin);

					double minVal, maxVal, medVal;

					minMaxLoc(new_image, &minVal, &maxVal);

					medVal = (maxVal - minVal) / 2;

					threshold(new_image, new_image, medVal, 255, 0);

					imshow("C", new_image);

					Mat diffImg;

					int match, minDiff, diff;
					minDiff = 12000;
					match = -1;

					for (int i = 0; i < 8; i++)
					{
						//diffImg = symbols[i].img-correctedImgBin;

						bitwise_xor(new_image, symbols[i].img, diffImg,
								noArray());

						diff = countNonZero(diffImg);
						if (diff < minDiff)
						{
							minDiff = diff;
							match = i;
						}

						if (i == 0)
						{
							//	imshow("B",diffImg);

						}
					}

					//imshow("B", correctedImg);

					if (match != -1)
					{
						putText(camera, symbols[match].name, Point(320, 30), 1,
								2, Scalar(0, 255, 0), 2);
					}

					//break;

				}

			}
		}

		imshow("A", camera);
		waitKey(20);
	}
}
