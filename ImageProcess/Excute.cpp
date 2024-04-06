#include <iostream>
#include "ImageProcess/ImageProcessor.h"

int main()
{
    std::string imagePath = "Resource/Sample.png";
    cv::Mat image;
    ImageProcessor imageProcessor;

    // 이미지 읽기
    if (!imageProcessor.ReadImage(imagePath, image))
    {
        std::cerr << "이미지를 읽을 수 없습니다." << std::endl;
        return 1;
    }

    // 그레이스케일 변환
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

    // 이미지 이진화
    cv::Mat binaryImage;
    imageProcessor.BinaryImage(grayImage, binaryImage, 128);

    // 이미지 팽창
    cv::Mat dilateImage;
    imageProcessor.Morphology(grayImage, dilateImage, MorphologyType::DILATE, { 3,3 });

    // 이미지 침식
    cv::Mat erodeImage;
    imageProcessor.Morphology(grayImage, erodeImage, MorphologyType::DILATE, { 3,3 });

    //이미지 외곽선
    cv::Mat ContourDetectionImage;
    imageProcessor.ContourDetection(grayImage, ContourDetectionImage);

    //이미지 영역분할
    cv::Mat SegmentImage;
    imageProcessor.SegmentImage(grayImage, SegmentImage);

    // 이미지 표시
    {
        cv::Mat combinedHorizontal1 = imageProcessor.CombineImagesHorizontally(grayImage, binaryImage);
        cv::Mat combinedHorizontal2 = imageProcessor.CombineImagesHorizontally(dilateImage, erodeImage);

        cv::Mat combinedVertical = imageProcessor.CombineImagesVertically(combinedHorizontal1, combinedHorizontal2);

        imageProcessor.ShowImage(combinedVertical, "이미지 이진화 및 모노폴리");
        imageProcessor.ShowImage(ContourDetectionImage, "외곽선 추출");
        imageProcessor.ShowImage(SegmentImage, "영역 추출");
        cv::waitKey(0);
    }

    return 0;
}