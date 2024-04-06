#include "ImageProcessor.h"

#include <iostream>
#include <fstream>

ImageProcessor::ImageProcessor()
{
}

bool ImageProcessor::ReadImage(const std::string& imagePath, cv::Mat& image)
{
    // 이미지 파일의 확장자를 확인하여 OpenCV에서 지원하는 형식인지 검사
    if (imagePath.substr(imagePath.find_last_of(".") + 1) != "bmp" &&
        imagePath.substr(imagePath.find_last_of(".") + 1) != "jpg" &&
        imagePath.substr(imagePath.find_last_of(".") + 1) != "jpeg" &&
        imagePath.substr(imagePath.find_last_of(".") + 1) != "png")
    {
        std::cerr << "지원되지 않는 이미지 형식입니다. 이미지 경로 : " << imagePath << std::endl;
        return false;
    }

    // 이미지 파일이 실제로 존재하는지 확인
    std::ifstream file(imagePath);
    if (!file.good())
    {
        std::cerr << "이미지 파일이 존재하지 않습니다. 이미지 경로 : " << imagePath << std::endl;
        return false;
    }

    // 이미지를 읽어옴
    image = cv::imread(imagePath);

    // 이미지 크기가 0x0인 경우 오류 처리
    if (IsImageVaild(image))
    {
        return true;
    }
    return false;
}

bool ImageProcessor::SaveImage(const std::string& imagePath, const cv::Mat& image)
{
    if (!IsImageVaild(image))
    {
        std::cerr << "이미지 파일이 유효하지 않습니다." << image << std::endl;
        return false;
    }

    if (imwrite(imagePath, image)) 
    {
        std::cerr << "이미지가 성공적으로 저장되었습니다: " << imagePath << std::endl;
        return true;
    }

    else 
    {
        std::cerr << "이미지 저장 실패: " << imagePath << std::endl;
        return false;
    }
}

void ImageProcessor::ShowImage(const cv::Mat& image, const std::string name)
{
    if (!IsImageVaild(image))
    {
        std::cerr << "이미지 오류, 이미지 출력 불가" << std::endl;
    }
    else
    {
        cv::imshow(name, image);;
    }
}

bool ImageProcessor::BinaryImage(const cv::Mat& inputImage, cv::Mat& binaryImage, int thresholdValue)
{
    if (!IsImageVaild(inputImage))
    {
        return false;
    }

    cv::threshold(inputImage, binaryImage, thresholdValue, 255, cv::THRESH_BINARY);

    if (!IsImageVaild(binaryImage))
    {
        return false;
    }
    return true;
}

bool ImageProcessor::Morphology(const cv::Mat& inputImage, cv::Mat& outputImage, MorphologyType type, cv::Size kernelSize)
{
    // 입력 이미지 유효성 검사
    if (!IsImageVaild(inputImage))
    {
        std::cerr << "입력 이미지가 유효하지 않습니다." << std::endl;
        return false;
    }

    // 커널 생성
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, kernelSize);

    // 모폴로지 연산 수행
    switch (type)
    {
    case MorphologyType::DILATE:
        cv::dilate(inputImage, outputImage, kernel);
        break;
    case MorphologyType::ERODE:
        cv::erode(inputImage, outputImage, kernel);
        break;
        // 필요한 다른 모폴로지 연산도 추가할 수 있습니다.
    default:
        std::cerr << "지원되지 않는 모폴로지 연산 유형입니다." << std::endl;
        return false;
    }

    // 결과 이미지 유효성 검사
    if (!IsImageVaild(outputImage))
    {
        std::cerr << "모폴로지 연산 결과 이미지가 유효하지 않습니다." << std::endl;
        return false;
    }

    return true;
}

bool ImageProcessor::ContourDetection(const cv::Mat& inputImage, cv::Mat& outputImage)
{
    // 입력 이미지 유효성 검사
    if (!IsImageVaild(inputImage)) {
        std::cerr << "유효하지 않은 입력 이미지입니다." << std::endl;
        return false;
    }

    // 가장자리 검출
    cv::Mat edges;
    cv::Canny(inputImage, edges, 100, 200);

    // 윤곽선 검출
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 검출된 윤곽선 그리기
    outputImage = inputImage.clone();
    cv::drawContours(outputImage, contours, -1, cv::Scalar(0, 255, 0), 2);

    return true;
}

bool ImageProcessor::SegmentImage(const cv::Mat& inputImage, cv::Mat& outputImage)
{
    // 입력 이미지 유효성 검사
    if (!IsImageVaild(inputImage)) {
        std::cerr << "유효하지 않은 입력 이미지입니다." << std::endl;
        return false;
    }

    // 가장자리 검출
    cv::Mat edges;
    cv::Canny(inputImage, edges, 100, 200);

    // 영역 분할을 위한 연결 요소 레이블링
    cv::Mat labels;
    cv::connectedComponents(edges, labels);

    // 각 레이블을 다른 색으로 랜덤하게 표시
    cv::Mat coloredLabels = cv::Mat::zeros(labels.size(), CV_8UC3);
    for (int label = 1; label < labels.rows; ++label) {
        cv::Mat mask = labels == label;
        cv::Vec3b color(rand() & 255, rand() & 255, rand() & 255);
        coloredLabels.setTo(color, mask);
    }

    // 결과 이미지 생성
    coloredLabels.copyTo(outputImage);

    return true;
}

bool ImageProcessor::IsImageVaild(const cv::Mat& inputImage)
{
    // 이미지가 비어 있는지 확인
    if (inputImage.empty())
    {
        std::cerr << "이미지가 비어 있습니다." << std::endl;
        return false;
    }

    // 이미지의 크기가 유효한지 확인 (너비 또는 높이가 0이면 유효하지 않음)
    if (inputImage.cols == 0 || inputImage.rows == 0)
    {
        std::cerr << "이미지의 크기가 유효하지 않습니다." << std::endl;
        return false;
    }

    // 이미지의 데이터 타입이 유효한지 확인
    if (inputImage.type() != CV_8UC1 && inputImage.type() != CV_8UC3)
    {
        std::cerr << "지원되지 않는 이미지 형식입니다." << std::endl;
        return false;
    }

    // 모든 검사를 통과하면 이미지가 유효함
    return true;
}

cv::Mat ImageProcessor::CombineImagesHorizontally(const cv::Mat& image1, const cv::Mat& image2)
{
    
     // 이미지 수평 결합
     cv::Mat combinedImage;
     cv::hconcat(image1, image2, combinedImage);
     return combinedImage;
    
}

cv::Mat ImageProcessor::CombineImagesVertically(const cv::Mat& image1, const cv::Mat& image2)
{
    
    // 이미지 수직 결합
    cv::Mat combinedImage;
    cv::vconcat(image1, image2, combinedImage);
    return combinedImage;
    
}
