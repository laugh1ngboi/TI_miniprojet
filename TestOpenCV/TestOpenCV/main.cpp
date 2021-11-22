#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;


int main()
{
    // lire un fichier en tant qu'image
    //Mat image = imread("./fresque0.ppm", IMREAD_GRAYSCALE); // choissisez le bon chemin de votre image
   Mat image_rgb = imread("./fresque0.ppm", IMREAD_COLOR); // choissisez le bon chemin de votre image

    // afficher une erreur si le fichier de l'image n'existe pas
    if (image_rgb.empty()) {
        cout << "Image non trouvee" << endl;

        // attendre qu'un bouton soit appuy?
        cin.get();
        return -1;
    }
    /*********************Debut de coder***********************/
    Mat image_hsv, hue;
    cvtColor(image_rgb,image_hsv,COLOR_BGR2HSV);
    hue.create(image_hsv.size(),image_hsv.depth());
    int ch[] = { 0,0 };
    mixChannels(&image_hsv, 1, &hue, 1, ch, 1);

    /*********************Fin de coder*************************/

    // afficher l'image dans une fenetre avec un nom au choix
    imshow("nom de l'image", image_rgb);

    // attendre qu'un bouton soit appuy?pour arreter
    waitKey(0);
    return 0;
}
