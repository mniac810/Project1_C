#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb_image_write.h"

//accuracy parameter
#define accuracy 55


//Compare function
int comp_pixle(unsigned char* background, unsigned char* foreground,int index_height, int index_width, int height, int width, int channel){
    double delta = 0;
    for (int k = 0; k < channel; k++){
        double coef = 1;
        if (k == 1){
            coef = 2;
        }
        delta += coef * abs(background[index_height*width*channel + index_width*channel+k]-foreground[index_height*width*channel + index_width*channel+k]);
    }
    delta /= 4;
    return delta;

}


//Masking function
void mask_image(unsigned char* background, unsigned char* foreground, unsigned char* res_image, int width, int height, int channel){
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if(comp_pixle(background, foreground, i, j, height, width, channel) > accuracy){
                for (int k = 0; k < channel; k++){
                    res_image[i*width*channel + j*channel+k] = foreground[i*width*channel + j*channel+k];
                }
            }
        }
    }
}


int main(){
    int width, height, channel;
    char path_background[] = "./images/background.jpg";
    char path_foreground[] = "./images/foreground.jpg";
    char path_new_background[] = "./images/new_background.jpg";
    char path_output[] = "./images/output.jpg";


    //Load background
    unsigned char *background = stbi_load(path_background,&width,&height,&channel,0);
    if (background == NULL){
        printf("Background not found!");
        exit(1);
    }

    //Load foreground
    unsigned char *foreground = stbi_load(path_foreground,&width,&height,&channel,0);
    if (foreground == NULL){
        printf("Foreground not found!");
        exit(1);
    }

    //Load new background
    unsigned char *new_background = stbi_load(path_new_background,&width,&height,&channel,0);
    if (new_background == NULL){
        printf("New background not found!");
        exit(1);
    }

    //Masking the difference from foreground and background to new background
    mask_image(background,foreground,new_background,width,height,channel);

    // Save image to directory
    stbi_write_png(path_output,width,height,channel,new_background,width*channel);
    printf("New image save to %s", path_output);
}
