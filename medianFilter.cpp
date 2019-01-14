
// CSC382 Project Part II
// Vincent Ippolito
// Carl Mendez
// Nick Wilson

#include<iostream> 
#include<algorithm>			//for sort
#include<climits> 
#include <chrono>			//for clock
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>


using namespace std;
using namespace std::chrono;
using namespace cv;



int i = 0; //declaring it globally allows for it to be accessed anywhere 



// Median of 3 elements
int findMedian(int arr[], int n) {
	sort(arr, arr + n);  // Sort the array 
	return arr[n / 2];   // Return middle element 
}

/* Median of medians algorithm
* Divides arr[] into groups of size 3, gets element 1 (the median) and stores it in medians[] array.
*/
void MedianOfMedians(int arr[], int medians[], int n, int start) {
	for (i = 0; i < n / 3; ::i++) {
		medians[i] = findMedian(arr + start + i * 3, 3);
	}

	if (i * 3 < n) { //For last group with less than 3 elements 
		medians[i] = findMedian(arr + start + i * 3, n % 3);
		::i++;
	}
}

void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}
int partition(int arr[], int start, int end, int mid) {

	//Find element mid and move it to the end
	for (i = start; i < end; i++)
		if (arr[i] == mid)
			break;
	swap(&arr[i], &arr[end]);

	//Quick Sort Algorithm
	i = start;
	for (int j = start; j <= end - 1; j++) {
		if (arr[j] <= mid) {
			swap(&arr[i], &arr[j]);
			::i++;
		}
	}
	swap(&arr[i], &arr[end]);

	return i;	//pivot position is returned
}


int kthSmallest(int arr[], int start, int end, int k) {

	// k needs to be from 1 to the number of elements in the array
	if (k > 0 && k <= end - start + 1)
	{
		int n = end - start + 1; //number of elements in arr

								 
		int *medians = new int[3];

		//Find all the medians
		MedianOfMedians(arr, medians, n, start);

		// Find median of all medians using recursive call if median has more than 1 element
		int medOfMed = (i == 1) ? medians[i - 1] :
			kthSmallest(medians, 0, i - 1, i / 2);

		//Partition the array around pivot and store the pivot position
		int pivotPos = partition(arr, start, end, medOfMed);

		//If pivotPos is k
		if (pivotPos - start == k - 1)
			return arr[pivotPos];

		//If pivotPos > k, take left part 
		if (pivotPos - start > k - 1)
			return kthSmallest(arr, start, pivotPos - 1, k);

		//If pivotPos < k, take right part
		return kthSmallest(arr, pivotPos + 1, end, k - pivotPos + start - 1);
	}

	//If k is is out of bounds
	return INT_MAX;
}

int main()
{

	Mat src, dst;

	// Load an image
	src = imread("test.png", IMREAD_GRAYSCALE);

	if (!src.data)
	{
		return -1;
	}

	//create a sliding window of size 9
	int window[9];

	int n = 9;
	int *medians = new int[3];
	int median;
	int	medOfMed;

	dst = src.clone();
	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
			dst.at<uchar>(y, x) = 0.0;

	for (int y = 1; y < src.rows - 1; y++) {
		for (int x = 1; x < src.cols - 1; x++) {

			// Pick up window element

			window[0] = src.at<uchar>(y - 1, x - 1);
			window[1] = src.at<uchar>(y, x - 1);
			window[2] = src.at<uchar>(y + 1, x - 1);
			window[3] = src.at<uchar>(y - 1, x);
			window[4] = src.at<uchar>(y, x);
			window[5] = src.at<uchar>(y + 1, x);
			window[6] = src.at<uchar>(y - 1, x + 1);
			window[7] = src.at<uchar>(y, x + 1);
			window[8] = src.at<uchar>(y + 1, x + 1);

			// sort the window to find median using medians of median algorithm
			medOfMed = kthSmallest(window, 0, n, 5); //median is the fifth smallest element in size 9

			// assign the median to centered element of the matrix
			dst.at<uchar>(y, x) = medOfMed;
		}
	}
	//Used to create output image
	vector<int> compressionParams;
	compressionParams.push_back(IMWRITE_PNG_COMPRESSION);
	compressionParams.push_back(9);
	imwrite("final.png", dst, compressionParams);

	namedWindow("final");
	imshow("final", dst);

	namedWindow("initial");
	imshow("initial", src);

	waitKey();

	system("pause");
	return 0;
}