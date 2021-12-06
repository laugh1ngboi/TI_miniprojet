#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
//#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace std;
//using namespace cv::xfeatures2d;


Mat hue_frag, gray_frag,hue_fresque;
int bins = 25;
void Hist_and_Backproj(int, void*);

int main(int argc, char** argv)
{
    // lire un fichier en tant qu'image
    //Mat image = imread("./fresque0.ppm", IMREAD_GRAYSCALE); // choissisez le bon chemin de votre image
   Mat image_templet = imread("./fresque0.ppm", IMREAD_COLOR); // choissisez le bon chemin de votre image

    // afficher une erreur si le fichier de l'image n'existe pas
    if (image_templet.empty()) {
        cout << "Image non trouvee" << endl;
        // attendre qu'un bouton soit appuy?
        cin.get();
        return -1;
    }
    /*********************Debut de coder***********************/
    
    Mat image_frag = imread("./frag_eroded/frag_eroded_0007.ppm", IMREAD_COLOR);
    if (image_frag.empty()) {
        cout << "Image non trouvee" << endl;
        // attendre qu'un bouton soit appuy?
        cin.get();
        return -1;
    }
   //imshow("nom de l'image", image_frag);
    /*
    vector<Mat> bgr_templet, bgr_frag;
    split(image_templet, bgr_templet);
    split(image_frag, bgr_frag);
    int histSize = 256;
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange = { range };
    bool uniform = true, accumulate = false;
    Mat b_hist_templet, g_hist_templet, r_hist_templet;
    Mat b_hist_frag, g_hist_frag, r_hist_frag;
    calcHist(&bgr_frag[0], 1, 0, Mat(), b_hist_frag, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_frag[1], 1, 0, Mat(), g_hist_frag, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_frag[2], 1, 0, Mat(), r_hist_frag, 1, &histSize, &histRange, uniform, accumulate);
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    normalize(b_hist_frag, b_hist_frag, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(g_hist_frag, g_hist_frag, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(r_hist_frag, r_hist_frag, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    for (int i = 1; i < histSize; i++)
    {
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist_frag.at<float>(i - 1))),
            Point(bin_w * (i), hist_h - cvRound(b_hist_frag.at<float>(i))),
            Scalar(255, 0, 0), 2, 8, 0);
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_hist_frag.at<float>(i - 1))),
            Point(bin_w * (i), hist_h - cvRound(g_hist_frag.at<float>(i))),
            Scalar(0, 255, 0), 2, 8, 0);
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_hist_frag.at<float>(i - 1))),
            Point(bin_w * (i), hist_h - cvRound(r_hist_frag.at<float>(i))),
            Scalar(0, 0, 255), 2, 8, 0);
    }
    imshow("nom de l'image", image_frag);
    imshow("calcHist Demo", histImage);
    */
    Mat hsv_frag,hsv_fresque;
    cvtColor(image_frag, hsv_frag, COLOR_BGR2HSV);
    cvtColor(image_frag, gray_frag, COLOR_BGR2GRAY);
    cvtColor(image_templet, hsv_fresque, COLOR_BGR2HSV);
    //Mat hue_frag;
    hue_frag.create(hsv_frag.size(),hsv_frag.depth());
    hue_fresque.create(hsv_fresque.size(), hsv_fresque.depth());
    int fromto[] = { 0,0 };
    mixChannels(&hsv_frag,1,&hue_frag,1,fromto,1);
    mixChannels(&hsv_fresque, 1, &hue_fresque, 1, fromto, 1);
    const char* window_image = "Source image";
    namedWindow(window_image);
    createTrackbar("* Hue  bins: ", window_image, &bins, 180, Hist_and_Backproj);
    Hist_and_Backproj(0, 0);
    imshow(window_image, image_frag);
    //imshow("image gray", gray_frag);
    /*
    int minHessian = 400;
    Ptr<SURF> detector = SURF::create(minHessian);
    std::vector<KeyPoint> keypoints;
    detector->detect(src, keypoints);
    //-- Draw keypoints
    Mat img_keypoints;
    drawKeypoints(src, keypoints, img_keypoints);
    //-- Show detected (drawn) keypoints
    imshow("SURF Keypoints", img_keypoints);
    */

    /*********************Fin de coder*************************/

    // afficher l'image dans une fenetre avec un nom au choix
    //imshow("nom de l'image", image_templet);
    // attendre qu'un bouton soit appuy?pour arreter
    waitKey(0);
    return 0;
}

void Hist_and_Backproj(int, void*)
{
    int histSize = MAX(bins, 2);
    float hue_range[] = { 0, 180 };
    const float* ranges = { hue_range };
    Mat hist;
    calcHist(&hue_frag, 1, 0, gray_frag, hist, 1, &histSize, &ranges, true, false);
    //calcHist(&hue_frag, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false);
    normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());
    Mat backproj;
    //calcBackProject(&hue_frag, 1, 0, hist, backproj, &ranges, 1, true);
    calcBackProject(&hue_fresque, 1, 0, hist, backproj, &ranges, 1, true);
    imshow("BackProj", backproj);
    int w = 400, h = 400;
    int bin_w = cvRound((double)w / histSize);
    Mat histImg = Mat::zeros(h, w, CV_8UC3);
    for (int i = 0; i < bins; i++)
    {
        rectangle(histImg, Point(i * bin_w, h), Point((i + 1) * bin_w, h - cvRound(hist.at<float>(i) * h / 255.0)),
            Scalar(0, 0, 255), FILLED);
    }
    imshow("Histogram", histImg);
}