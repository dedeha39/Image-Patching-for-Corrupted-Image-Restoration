# Image Patching for Corrupted Image Restoration

This repository contains the implementation of an image restoration algorithm using homography estimation. The aim of this project is to restore a corrupted image by overlaying patches obtained from reference images. The algorithm utilizes computer vision techniques, such as keypoint detection, feature matching, and homography estimation, to align and blend the patches with the corrupted image.

## Project Details

The program uses the following libraries and tools:
- **OpenCV:** an open-source computer vision library that provides a wide range of tools for image and video processing.
- **C++:** a general-purpose programming language used for developing high-performance applications.

### Functions:
- **getNumPatches:** Prompts the user to enter the number of patches and returns the value.
- **loadImages:** Loads the corrupted image and the patches into `cv::Mat` objects and stores them in a `std::vector`.
- **convertToGrayscale:** Converts the image and patches to grayscale.
- **detectAndCompute:** Detects keypoints and computes descriptors for the image and patches using the SIFT algorithm.
- **computeMatches:** Computes matches between the image and patches using the BFMatcher class provided by OpenCV.
- **refineMatches:** Refines the matches by selecting the matches with a distance less than `ratio * min_distance`.
- **findHomography:** Finds the homography matrix using RANSAC.
- **overlayPatches:** Overlays the patches over the image to fix the corrupted regions.
- **displayResult:** Displays the result.

## How It Works

1. **Loading the Corrupted Image and Patches:** The algorithm starts by loading the corrupted image and the corresponding patches into memory using OpenCV.
2. **Converting to Grayscale:** Both the image and patches are converted to grayscale for simplifying operations.
3. **Keypoint Detection and Descriptor Computation:** The Scale-Invariant Feature Transform (SIFT) algorithm detects keypoints and computes descriptors for both the grayscale image and patches.
4. **Match Computation:** Brute-force matching based on Euclidean distance is used to compute matches between the descriptors of the patches and the image.
5. **Match Refinement:** Matches are refined by filtering out unreliable matches based on distance ratio.
6. **Homography Estimation:** Using the RANSAC algorithm, homographies are estimated between the keypoints of the patches and the keypoints of the image.
7. **Overlaying the Patches:** The patches are overlaid onto the image by warping them according to the estimated homographies.
8. **Displaying the Result:** The final repaired image with the patches overlaid is displayed to the user.

## Conclusion

In this project, an image patching algorithm was implemented using homography estimation. The algorithm effectively repaired corrupted regions in an image by overlaying relevant patches. By leveraging the Scale-Invariant Feature Transform (SIFT) for keypoint detection and descriptor computation, along with refining matches and estimating homographies, the algorithm accurately aligned the patches with the corrupted regions. The program can also be extended to work with images that have been subject to transformations.

## References

- [OpenCV Documentation](https://docs.opencv.org/4.x/)

For detailed usage instructions and examples, please refer to the project documentation.
