#include <stdio.h>
#include <opencv2/opencv.hpp>

/* CONSTANTES *****************************************************************/

#define IMG_W 400
#define IMG_H 400 //768
#define MAX_ITER 300    // 200
#define MAX_NORM 4        // 2

#define LIMIT_LEFT -1
#define LIMIT_RIGHT 1
#define LIMIT_TOP -1
#define LIMIT_BOTTOM 1

/* MANIPULER LES NOMBRES COMPLEXES ********************************************/

typedef struct {
    long double real;
    long double imag;
} complex;
complex c; // GLOBALE

complex new_complex(long double real, long double imag) {
    complex c;
    c.real = real;
    c.imag = imag;
    return c;
}

complex add_complex(complex a, complex b) {
    a.real += b.real;
    a.imag += b.imag;
    return a;
}

complex mult_complex(complex a, complex b) {
    complex m;
    m.real = a.real * b.real - a.imag * b.imag;
    m.imag = a.real * b.imag + a.imag * b.real;
    return m;
}

long double module_complex(complex c) {
    return c.real * c.real + c.imag * c.imag;
}

/* FRACTALE DE JULIA *****************************************************/

complex convert(int x, int y) {
    return new_complex(
            ((long double) x / IMG_W * (LIMIT_RIGHT - LIMIT_LEFT)) + LIMIT_LEFT,
            ((long double) y / IMG_H * (LIMIT_BOTTOM - LIMIT_TOP)) + LIMIT_TOP );
}

int juliaDot(complex z, int iter) {
    int i;
    for (i = 0; i < iter; i++) {
        z = add_complex(mult_complex(z, z), c);
        long double norm = module_complex(z);
        if (norm > MAX_NORM) {
            break;
        }
    }
    return i * 255 / iter; // on met i dans l'intervalle 0 à 255
}

void julia(cv::Mat& img) {
    for (int x = 0; x < IMG_W; x++) {
        for (int y = 0; y < IMG_H; y++) {
            int j = juliaDot(convert(x, y), MAX_ITER);
            cv::Vec3b color(j, j, j);
            img.at<cv::Vec3b>(cv::Point(x, y)) = color;
        }
    }
}

/* MAIN ***********************************************************************/

int main(int argc, char * argv[]) {
    // creation de l'image
    cv::Mat newImg(IMG_H, IMG_W, CV_8UC3);

    const int nb_values = 4;
    long double c_values[nb_values][2] = {{-1.41702285618, 0},
                                          {0.285, 0.013},
                                          {0.285, 0.01},
                                          {0.3, 0.5},};

        // interaction avec l'utilisateur
        int key; // -1 indique qu'aucune touche est enfoncée
        int f_choice = 0;
        double r =  c_values[f_choice][0];
        double i = c_values[f_choice][1];

        // on attend 30ms une saisie clavier, key prend la valeur de la touche
        // si aucune touche est enfoncée, au bout de 30ms on exécute quand même
        // la boucle avec key = -1, l'image est mise à jour
        while( (key =  (cvWaitKey(30) & 0xFF)) ) {
            printf("key : %d", key);
            switch (key){
                case 82:
                    r +=  0.001;
                    i += 0.001;
                    break;
                case 84:
                    r -= 0.001;
                    i -= 0.001;
                    break;
                case 177:
                    f_choice = 0;
                    r = c_values[f_choice][0];
                    i = c_values[f_choice][1];
                    break;
                case 178:
                    f_choice = 1;
                    r = c_values[f_choice][0];
                    i = c_values[f_choice][1];
                    break;
                case 179:
                    f_choice = 2;
                    r = c_values[f_choice][0];
                    i = c_values[f_choice][1];
                    break;
                case 180:
                    f_choice = 3;
                    r = c_values[f_choice][0];
                    i = c_values[f_choice][1];
                    break;
            }

            if (key == 'q'){
                break;
            }

            c = new_complex(r, i);
            julia(newImg);
            imshow("image", newImg); // met à jour l'image
        }

        char name[15];
        sprintf(name, "image_bw%d.bmp", 0);
        imwrite(name, newImg);
        cvDestroyWindow("image"); // ferme la fenêtre
        return 0;
    }


