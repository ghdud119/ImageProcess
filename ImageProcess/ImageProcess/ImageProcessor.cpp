#include "ImageProcessor.h"

#include <iostream>
#include <fstream>

ImageProcessor::ImageProcessor()
{
}

bool ImageProcessor::ReadImage(const std::string& imagePath, cv::Mat& image)
{
    // �̹��� ������ Ȯ���ڸ� Ȯ���Ͽ� OpenCV���� �����ϴ� �������� �˻�
    if (imagePath.substr(imagePath.find_last_of(".") + 1) != "bmp" &&
        imagePath.substr(imagePath.find_last_of(".") + 1) != "jpg" &&
        imagePath.substr(imagePath.find_last_of(".") + 1) != "jpeg" &&
        imagePath.substr(imagePath.find_last_of(".") + 1) != "png")
    {
        std::cerr << "�������� �ʴ� �̹��� �����Դϴ�. �̹��� ��� : " << imagePath << std::endl;
        return false;
    }

    // �̹��� ������ ������ �����ϴ��� Ȯ��
    std::ifstream file(imagePath);
    if (!file.good())
    {
        std::cerr << "�̹��� ������ �������� �ʽ��ϴ�. �̹��� ��� : " << imagePath << std::endl;
        return false;
    }

    // �̹����� �о��
    image = cv::imread(imagePath);

    // �̹��� ũ�Ⱑ 0x0�� ��� ���� ó��
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
        std::cerr << "�̹��� ������ ��ȿ���� �ʽ��ϴ�." << image << std::endl;
        return false;
    }

    if (imwrite(imagePath, image)) 
    {
        std::cerr << "�̹����� ���������� ����Ǿ����ϴ�: " << imagePath << std::endl;
        return true;
    }

    else 
    {
        std::cerr << "�̹��� ���� ����: " << imagePath << std::endl;
        return false;
    }
}

void ImageProcessor::ShowImage(const cv::Mat& image, const std::string name)
{
    if (!IsImageVaild(image))
    {
        std::cerr << "�̹��� ����, �̹��� ��� �Ұ�" << std::endl;
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
    // �Է� �̹��� ��ȿ�� �˻�
    if (!IsImageVaild(inputImage))
    {
        std::cerr << "�Է� �̹����� ��ȿ���� �ʽ��ϴ�." << std::endl;
        return false;
    }

    // Ŀ�� ����
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, kernelSize);

    // �������� ���� ����
    switch (type)
    {
    case MorphologyType::DILATE:
        cv::dilate(inputImage, outputImage, kernel);
        break;
    case MorphologyType::ERODE:
        cv::erode(inputImage, outputImage, kernel);
        break;
        // �ʿ��� �ٸ� �������� ���굵 �߰��� �� �ֽ��ϴ�.
    default:
        std::cerr << "�������� �ʴ� �������� ���� �����Դϴ�." << std::endl;
        return false;
    }

    // ��� �̹��� ��ȿ�� �˻�
    if (!IsImageVaild(outputImage))
    {
        std::cerr << "�������� ���� ��� �̹����� ��ȿ���� �ʽ��ϴ�." << std::endl;
        return false;
    }

    return true;
}

bool ImageProcessor::ContourDetection(const cv::Mat& inputImage, cv::Mat& outputImage)
{
    // �Է� �̹��� ��ȿ�� �˻�
    if (!IsImageVaild(inputImage)) {
        std::cerr << "��ȿ���� ���� �Է� �̹����Դϴ�." << std::endl;
        return false;
    }

    // �����ڸ� ����
    cv::Mat edges;
    cv::Canny(inputImage, edges, 100, 200);

    // ������ ����
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // ����� ������ �׸���
    outputImage = inputImage.clone();
    cv::drawContours(outputImage, contours, -1, cv::Scalar(0, 255, 0), 2);

    return true;
}

bool ImageProcessor::SegmentImage(const cv::Mat& inputImage, cv::Mat& outputImage)
{
    // �Է� �̹��� ��ȿ�� �˻�
    if (!IsImageVaild(inputImage)) {
        std::cerr << "��ȿ���� ���� �Է� �̹����Դϴ�." << std::endl;
        return false;
    }

    // �����ڸ� ����
    cv::Mat edges;
    cv::Canny(inputImage, edges, 100, 200);

    // ���� ������ ���� ���� ��� ���̺�
    cv::Mat labels;
    cv::connectedComponents(edges, labels);

    // �� ���̺��� �ٸ� ������ �����ϰ� ǥ��
    cv::Mat coloredLabels = cv::Mat::zeros(labels.size(), CV_8UC3);
    for (int label = 1; label < labels.rows; ++label) {
        cv::Mat mask = labels == label;
        cv::Vec3b color(rand() & 255, rand() & 255, rand() & 255);
        coloredLabels.setTo(color, mask);
    }

    // ��� �̹��� ����
    coloredLabels.copyTo(outputImage);

    return true;
}

bool ImageProcessor::IsImageVaild(const cv::Mat& inputImage)
{
    // �̹����� ��� �ִ��� Ȯ��
    if (inputImage.empty())
    {
        std::cerr << "�̹����� ��� �ֽ��ϴ�." << std::endl;
        return false;
    }

    // �̹����� ũ�Ⱑ ��ȿ���� Ȯ�� (�ʺ� �Ǵ� ���̰� 0�̸� ��ȿ���� ����)
    if (inputImage.cols == 0 || inputImage.rows == 0)
    {
        std::cerr << "�̹����� ũ�Ⱑ ��ȿ���� �ʽ��ϴ�." << std::endl;
        return false;
    }

    // �̹����� ������ Ÿ���� ��ȿ���� Ȯ��
    if (inputImage.type() != CV_8UC1 && inputImage.type() != CV_8UC3)
    {
        std::cerr << "�������� �ʴ� �̹��� �����Դϴ�." << std::endl;
        return false;
    }

    // ��� �˻縦 ����ϸ� �̹����� ��ȿ��
    return true;
}

cv::Mat ImageProcessor::CombineImagesHorizontally(const cv::Mat& image1, const cv::Mat& image2)
{
    
     // �̹��� ���� ����
     cv::Mat combinedImage;
     cv::hconcat(image1, image2, combinedImage);
     return combinedImage;
    
}

cv::Mat ImageProcessor::CombineImagesVertically(const cv::Mat& image1, const cv::Mat& image2)
{
    
    // �̹��� ���� ����
    cv::Mat combinedImage;
    cv::vconcat(image1, image2, combinedImage);
    return combinedImage;
    
}
