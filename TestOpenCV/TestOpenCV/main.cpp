#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

class Backproj {//Backproj for an image of 3 channels from an RGB image (also calculate the corresponding histograms)
public:
    Backproj(Mat input, Mat _masque, Mat _frag){
        src = input;
        frag = _frag;
        masque = _masque;
        bins = 25;
        init_hist_tab2();
        imshow("backprojection", backproj);
    }
protected://attributes:
    Mat dest;//destination array -> hue 
    Mat src;//fresque
    Mat frag; // fragment

    //for hist :
    Mat masque;//supprime le pic ?0 au début des histogrammes
    int ch[2];//tab of the channels
    Mat hist_tab[3];//hist_tab[0] = histrogram for channel 0
    Mat hist_concat;
    int bins;
    const float* ranges;

    //for backproj:
    Mat backproj_tab[3];
    Mat backproj_concat;
    Mat hist;
    Mat backproj;
protected://methods
    void set_tab_channel(int src_ch, int dest_ch) 
    {
        ch[0] = src_ch;//channel from source array = input
        ch[1] = dest_ch;//channel for destination array
    }

    void init_hist_tab2() 
    {
        /*for (int i = 0; i < 3; i++) {
            hist_tab[i].create(frag.size(),frag.depth());
        }*/
        /*frag[0]->hist_tab[0]
        * frag[1]->hist_tab[1]
        * frag[2]->hist_tab[2]
        */
       //int fromto[] = { 0,0, 1,1, 2,2 };
        //mixChannels(&frag, 1, hist_tab, 1, fromto, 3);
        int channels[] = {0,1,2};
        int histSize[] = {bins,bins,bins};
        float hR0[] = { 0 , 180 };
        float hR12[] = { 0, 256 };
        const float* ranges[] = { hR0, hR12, hR12 };
        calcHist(&frag, 1, channels, masque, hist, 3, histSize, ranges, true, false);
        calcBackProject(&src, 1, channels, hist, backproj, ranges, 1, true);//fresque complete et histogramme du fragment
    }

    void init_hist_tab() //create histogram
    {
        dest.create(frag.size(), frag.depth());//initialize the destination array as we need it later on

        

        int histSize = MAX(bins, 2);
        float hR0[] = { 0 , 180 };
        float hR12[] = { 0, 256 };
        const float* r[] = { hR0, hR12, hR12 };

        for (int i = 0; i < 3; i++) {
            ranges = r[i];

            //hist
            set_tab_channel(i, 0);
            mixChannels(&src, 1, &dest, 1, ch, 1);
            calcHist(&dest, 1, 0, masque, hist_tab[i], 1, &histSize, &ranges, true, false);//parametre masque dans cette fonction
            normalize(hist_tab[i], hist_tab[i], 0, 255, NORM_MINMAX, -1, Mat());

            calcBackProject(&src, 1, 0, hist_tab[i], backproj_tab[i], &ranges, 1, true);//fresque complete et histogramme du fragment

        }

        cv::vconcat(hist_tab, 3 , hist_concat);

        cout << "Taille de hist_tab[0] = " << hist_tab[0].size() << endl;
        cout << "Taille de hist_tab[1] = " << hist_tab[1].size << endl;
        cout << "Taille de hist_tab[2] = " << hist_tab[2].size << endl;
        cout << "Taille de hist_concat = " << hist_concat.size << endl;
        cout << "histsize : " << histSize << endl;
    }

public://methods
    void show_hist()
    {
        int histSize = MAX(3*bins, 2);
        String windStr;

        //drawing histogram
        int w = 800, h = 600;
        int bin_w = cvRound((double)w / histSize);
       
        Mat histImg = Mat::zeros(h, w, CV_8UC3);
       
        for (int i = 0; i < 3*bins; i++)
        {
            rectangle(histImg, Point(i * bin_w, h), Point((i + 1) * bin_w, h - cvRound(hist_concat.at<float>(i) * h / 255.0)), Scalar(0, 0, 255), FILLED);
        }
        windStr = cv::format("Histogram diagram for 3 channels of HSV");
        imshow(windStr, histImg);
    }

    void show_backproj() {
        String windStr;
        windStr = cv::format("Backprojection diagram for 3 channels of HSV");
        imshow(windStr, backproj_concat);
    }
};
/*
class KeyPoint {
private://attributes

    //keypoint detection :
    Ptr<BRISK> Briskdetector;
    std::vector<KeyPoint> keypoints;
    int thresh, octaves;
    float patternScale;
public:
    KeyPoint(Mat& m, int _t=30, int _o=3, float _p=1.0f) : patternScale(_t), octaves(_o), patternScale(_p)
    {

        //-- Step 1: Detect the keypoints using SURF Detector
        Briskdetector = BRISK::create(thresh, octaves, patternScale);
        //Briskdetector->detect(m, keypoints);
    }
};*/

int main()
{
    // lire un fichier en tant qu'image
    //Mat image = imread("./fresque0.ppm", IMREAD_GRAYSCALE); // choissisez le bon chemin de votre image
    Mat fresque_rgb= imread("./fresque0.ppm", IMREAD_COLOR); // choissisez le bon chemin de votre image

    // afficher une erreur si le fichier de l'image n'existe pas
    if (fresque_rgb.empty()) {
        cout << "Image fresque non trouvee" << endl;

        // attendre qu'un bouton soit appuy?
        cin.get();
        return -1; 
    }

    Mat frag_rgb = imread("./frag_eroded/frag_eroded_0041.ppm", IMREAD_COLOR); // choissisez le bon chemin de votre image

    // afficher une erreur si le fichier de l'image n'existe pas
    if (frag_rgb.empty()) {
        cout << "Image fragment non trouvee" << endl;

        // attendre qu'un bouton soit appuy?
        cin.get();
        return -1;
    }

    /*********************Debut de coder***********************/
    Mat frag_hsv, fresque_hsv, masque;
    cvtColor(frag_rgb, masque, COLOR_BGR2GRAY); // 
    cvtColor(fresque_rgb, fresque_hsv, COLOR_BGR2HSV);
    cvtColor(frag_rgb, frag_hsv, COLOR_BGR2HSV);

    Backproj h(fresque_hsv, masque, frag_hsv);
    //h.show_hist();
    //h.show_backproj();
    /*********************Fin de coder*************************/

    // afficher l'image dans une fenetre avec un nom au choix
    imshow("nom de l'image", fresque_rgb); 
    imshow("frag", frag_rgb);
    // attendre qu'un bouton soit appuy?pour arreter
    waitKey(0);
    return 0;
}
