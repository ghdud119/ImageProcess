#include<opencv2/opencv.hpp>

enum class MorphologyType
{
	DILATE, // 팽창
	ERODE   // 침식
};

class ImageProcessor
{
public:
	ImageProcessor();

	bool ReadImage(const std::string& imagePath, cv::Mat& image);
	bool SaveImage(const std::string& imagePath, const cv::Mat& image);

	void ShowImage(const cv::Mat& image, const std::string name);
	//이미지 처리
	bool BinaryImage(const cv::Mat& inputImage, cv::Mat& binaryImage, int thresholdValue);
	bool Morphology(const cv::Mat& inputImage, cv::Mat& outputImage, MorphologyType type, cv::Size kernelSize);
	bool ContourDetection(const cv::Mat& inputImage, cv::Mat& outputImage);
	bool SegmentImage(const cv::Mat& inputImage, cv::Mat& outputImage);

	bool IsImageVaild(const cv::Mat& inputImage);


	cv::Mat CombineImagesHorizontally(const cv::Mat& image1, const cv::Mat& image2);
	cv::Mat CombineImagesVertically(const cv::Mat& image1, const cv::Mat& image2);

};