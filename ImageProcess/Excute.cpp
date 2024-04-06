#include <iostream>
#include "ImageProcess/ImageProcessor.h"

int main()
{
    std::string imagePath = "Resource/Sample.png";
    cv::Mat image;
    ImageProcessor imageProcessor;

    // �̹��� �б�
    if (!imageProcessor.ReadImage(imagePath, image))
    {
        std::cerr << "�̹����� ���� �� �����ϴ�." << std::endl;
        return 1;
    }

    // �׷��̽����� ��ȯ
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

    // �̹��� ����ȭ
    cv::Mat binaryImage;
    imageProcessor.BinaryImage(grayImage, binaryImage, 128);

    // �̹��� ��â
    cv::Mat dilateImage;
    imageProcessor.Morphology(grayImage, dilateImage, MorphologyType::DILATE, { 3,3 });

    // �̹��� ħ��
    cv::Mat erodeImage;
    imageProcessor.Morphology(grayImage, erodeImage, MorphologyType::DILATE, { 3,3 });

    //�̹��� �ܰ���
    cv::Mat ContourDetectionImage;
    imageProcessor.ContourDetection(grayImage, ContourDetectionImage);

    //�̹��� ��������
    cv::Mat SegmentImage;
    imageProcessor.SegmentImage(grayImage, SegmentImage);

    // �̹��� ǥ��
    {
        cv::Mat combinedHorizontal1 = imageProcessor.CombineImagesHorizontally(grayImage, binaryImage);
        cv::Mat combinedHorizontal2 = imageProcessor.CombineImagesHorizontally(dilateImage, erodeImage);

        cv::Mat combinedVertical = imageProcessor.CombineImagesVertically(combinedHorizontal1, combinedHorizontal2);

        imageProcessor.ShowImage(combinedVertical, "�̹��� ����ȭ �� �������");
        imageProcessor.ShowImage(ContourDetectionImage, "�ܰ��� ����");
        imageProcessor.ShowImage(SegmentImage, "���� ����");
        cv::waitKey(0);
    }

    return 0;
}