// *****************************
// Name = Ismail Deha
// Surname = Kose
// Student ID = 2072544
// Task = Lab 4 
// |->Used StarWars picture which have 4 patch
// ****************************


#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

// Function to prompt the user to enter the number of patches
int getNumPatches() {  //Used StarWars picture which have 4 patch
    int num_patches;
    std::cout << "Enter the number of patches: ";
    std::cin >> num_patches;
    return num_patches;
}

// Function to load the corrupted image and the patches
void loadImages(cv::Mat& image, std::vector<cv::Mat>& patches, int num_patches) {
    image = cv::imread("image_to_complete.jpg");
    patches.resize(num_patches);
    for (int i = 0; i < num_patches; i++) {
        std::string patch_name = "patch_" + std::to_string(i + 1) + ".jpg";
        patches[i] = cv::imread(patch_name);
    }
}

// Function to convert the image and patches to grayscale
void convertToGrayscale(cv::Mat& image, std::vector<cv::Mat>& patches, cv::Mat& gray_image, std::vector<cv::Mat>& gray_patches) {
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    gray_patches.resize(patches.size());
    for (int i = 0; i < patches.size(); i++) {
        cv::cvtColor(patches[i], gray_patches[i], cv::COLOR_BGR2GRAY);
    }
}

// Function to detect keypoints and compute descriptors for the image and patches
void detectAndCompute(cv::Mat& gray_image, std::vector<cv::Mat>& gray_patches, cv::Ptr<cv::SIFT>& sift, std::vector<cv::KeyPoint>& keypoints_image, cv::Mat& descriptors_image, std::vector<std::vector<cv::KeyPoint>>& keypoints_patches, std::vector<cv::Mat>& descriptors_patches) {
    sift = cv::SIFT::create();
    sift->detectAndCompute(gray_image, cv::noArray(), keypoints_image, descriptors_image);
    keypoints_patches.resize(gray_patches.size());
    descriptors_patches.resize(gray_patches.size());
    for (int i = 0; i < gray_patches.size(); i++) {
        sift->detectAndCompute(gray_patches[i], cv::noArray(), keypoints_patches[i], descriptors_patches[i]);
    }
}

// Function to compute matches between the image and patches
void computeMatches(cv::Mat& descriptors_image, std::vector<cv::Mat>& descriptors_patches, cv::BFMatcher& matcher, std::vector<std::vector<cv::DMatch>>& matches) {
    matches.resize(descriptors_patches.size());
    for (int i = 0; i < descriptors_patches.size(); i++) {
        matcher.match(descriptors_patches[i], descriptors_image, matches[i]);
    }
}

// Function to refine the matches
void refineMatches(std::vector<std::vector<cv::DMatch>>& matches, double ratio, std::vector<std::vector<cv::DMatch>>& good_matches) {
    good_matches.resize(matches.size());
    for (int i = 0; i < matches.size(); i++) {
        for (int j = 0; j < matches[i].size(); j++) {
            if (matches[i][j].distance < ratio * matches[i][0].distance) {
                good_matches[i].push_back(matches[i][j]);
            }
        }
    }
}

// Function to find the homography matrix using RANSAC
void findHomography(std::vector<std::vector<cv::KeyPoint>>& keypoints_patches, std::vector<cv::KeyPoint>& keypoints_image, std::vector<std::vector<cv::DMatch>>& good_matches, std::vector<cv::Mat>& homographies) {
    std::vector<std::vector<cv::Point2f>> points1(good_matches.size()), points2(good_matches.size());
    homographies.resize(good_matches.size());
    for (int i = 0; i < good_matches.size(); i++) {
        for (int j = 0; j < good_matches[i].size(); j++) {
            points1[i].push_back(keypoints_patches[i][good_matches[i][j].queryIdx].pt);
            points2[i].push_back(keypoints_image[good_matches[i][j].trainIdx].pt);
        }
        homographies[i] = cv::findHomography(points1[i], points2[i], cv::RANSAC);
    }
}

// Function to overlay the patches over the image to fix the corrupted regions
void overlayPatches(cv::Mat& image, std::vector<cv::Mat>& patches, std::vector<cv::Mat>& homographies) {
    cv::Mat result;
    for (int i = 0; i < patches.size(); i++) {
        cv::warpPerspective(patches[i], result, homographies[i], image.size());
        result.copyTo(image, result != 0);
    }
}

// Function to display the result
void displayResult(cv::Mat& image) {
    cv::imshow("Result", image);
    cv::waitKey(0);
}

int main() {
    // Prompt the user to enter the number of patches
    int num_patches = getNumPatches(); //Used StarWars picture which have 4 patch

    // Load the corrupted image and the patches
    cv::Mat image;
    std::vector<cv::Mat> patches;
    loadImages(image, patches, num_patches);

    // Convert the image and patches to grayscale
    cv::Mat gray_image;
    std::vector<cv::Mat> gray_patches;
    convertToGrayscale(image, patches, gray_image, gray_patches);

    // Detect keypoints and compute descriptors for the image and patches
    cv::Ptr<cv::SIFT> sift;
    std::vector<cv::KeyPoint> keypoints_image;
    cv::Mat descriptors_image;
    std::vector<std::vector<cv::KeyPoint>> keypoints_patches;
    std::vector<cv::Mat> descriptors_patches;
    detectAndCompute(gray_image, gray_patches, sift, keypoints_image, descriptors_image, keypoints_patches, descriptors_patches);

    // Compute matches between the image and patches
    cv::BFMatcher matcher(cv::NORM_L2);
    std::vector<std::vector<cv::DMatch>> matches;
    computeMatches(descriptors_image, descriptors_patches, matcher, matches);

    // Refine the matches
    double ratio = 0.75;
    std::vector<std::vector<cv::DMatch>> good_matches;
    refineMatches(matches, ratio, good_matches);

    // Find the homography matrix using RANSAC
    std::vector<cv::Mat> homographies;
    findHomography(keypoints_patches, keypoints_image, good_matches, homographies);

    // Overlay the patches over the image to fix the corrupted regions
    overlayPatches(image, patches, homographies);

    // Display the result
    displayResult(image);

    return 0;
}
