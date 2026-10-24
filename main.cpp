#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "third_party/stb_image_write.h"
#include "image.h"
#include <cstdlib>
#include <cstdio>


/*=========================== Helpers =========================================*/

void free_img(img *p)
{
    if (!p)
        return;
    free(p->pixels);
    free(p);
}


/*=========================== Color Inversion =========================================*/

void c_inversion(img *inverted, img *main_image)
{
    inverted->pixels = (unsigned char *)malloc((size_t)(main_image->width) * (main_image->height * 3));
    if (!inverted->pixels)
    {
        std::printf("Out of memory\n");
        return;
    }
    inverted->width = main_image->width;
    inverted->height = main_image->height;
    inverted->channels = main_image->channels;
    for (int i = 0; i < main_image->width * main_image->height * 3; i++)
    {
        inverted->pixels[i] = 255 - main_image->pixels[i];
    }
}



/*=========================== Gaussian Blur =========================================*/

int n_blurred_value(img *m, int i, int j, int k)
{
    int w = m->width;
    unsigned char *p = m->pixels;
    
    int stride = w * 3;               // bytes in one full row
    int center = 3 * (i * w + j) + k; // index of this pixel, this channel
    
    int sum = 1 * p[center - stride - 3] + 2 * p[center - stride] + 1 * p[center - stride + 3] + 2 * p[center - 3] + 4 * p[center] + 2 * p[center + 3] + 1 * p[center + stride - 3] + 2 * p[center + stride] + 1 * p[center + stride + 3];
    
    return sum / 16;
}

void n_blurring(img *blurred, img *main_image)
{
    blurred->pixels = (unsigned char *)malloc((size_t)(main_image->width) * (main_image->height * 3));
    if (!blurred->pixels)
    {
        std::printf("Out of memory\n");
        return;
    }
    blurred->width = main_image->width;
    blurred->height = main_image->height;
    blurred->channels = main_image->channels;
    for (int i = 1; i < blurred->height - 1; i++)
    {
        for (int j = 1; j < blurred->width - 1; j++)
        {
            for (int k = 0; k < blurred->channels; k++)
            {
                blurred->pixels[(blurred->channels) * (i * (blurred->width) + j) + k] = (unsigned char)n_blurred_value(main_image, i, j, k);
            }
        }
    }
}



/*=========================== Circular Blur =========================================*/

int c_blurred_value(img *m, int i, int j, int k)
{
    int w = m->width;
    unsigned char *p = m->pixels;
    
    int stride = w * 3;               // bytes in one full row
    int center = 3 * (i * w + j) + k; // index of this pixel, this channel
    
    int sum = 1 * p[center - stride - 3] + 1 * p[center - stride] + 1 * p[center - stride + 3] + 1 * p[center - 3] + 0 * p[center] + 1 * p[center + 3] + 1 * p[center + stride - 3] + 1 * p[center + stride] + 1 * p[center + stride + 3];

    return sum / 8;
}

void c_blurring(img *blurred, img *main_image)
{
    blurred->pixels = (unsigned char *)malloc((size_t)(main_image->width) * (main_image->height * 3));
    if (!blurred->pixels)
    {
        std::printf("Out of memory\n");
        return;
    }
    blurred->width = main_image->width;
    blurred->height = main_image->height;
    blurred->channels = main_image->channels;
    for (int i = 1; i < blurred->height - 1; i++)
    {
        for (int j = 1; j < blurred->width - 1; j++)
        {
            for (int k = 0; k < blurred->channels; k++)
            {
                blurred->pixels[(blurred->channels) * (i * (blurred->width) + j) + k] = (unsigned char)c_blurred_value(main_image, i, j, k);
            }
        }
    }
}



/*================================================================================================*/

int main()
{
    int width, height, channels;
    
    // Load test.png as RGB (3 bytes per pixel)
    unsigned char *pixels = stbi_load("test.png", &width, &height, &channels, 3);
    img *main_image = (img *)malloc(sizeof(img));
    main_image->width = width;
    main_image->height = height;
    main_image->channels = channels;
    main_image->pixels = pixels;
    if (!pixels)
    {
        std::printf("Failed to load: %s\n", stbi_failure_reason());
        return 1;
    }
    std::printf("Loaded %d x %d image\n", width, height);
    int choice;

    // Declaring color inverted image
    img *c_inverted = (img *)malloc(sizeof(img));
    if (!c_inverted)
    {
        printf("Failed to allocate memory to inverted image.\n");
        return 1;
    }
    c_inverted->width = 0;

    // Declaring of gaussian blurred img
    img *n_blurred = (img *)malloc(sizeof(img));
    if (!n_blurred)
    {
        printf("Failed to allocate memory to n_blurred image.\n");
        return 1;
    }
    n_blurred->width = 0;


    // Declaring of c blurred img
    img *c_blurred = (img *)malloc(sizeof(img));
    if (!c_blurred)
    {
        printf("Failed to allocate memory to c_blurred image.\n");
        return 1;
    }
    c_blurred->width = 0;

    bool running = true;
    while (running)
    {
        printf("1. To invert the color. \n2. Make image blur \n3. Make image circular blur \n4. Exit the program. \n ");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:

            if (c_inverted->width)
            {
                printf("Already implemented.\n");
                break;
            }
            c_inversion(c_inverted, main_image);
            stbi_write_png("output/c_invert_img.png", width, height, 3, c_inverted->pixels, width * 3);
            if (c_inverted)
                printf("Color Inverted image saved as c_invert_img.png\n");
            break;

        case 2:

            if (n_blurred->pixels)
            {
                printf("Already implemented.\n");
                break;
            }
            {
                img *tmp = (img *)calloc(1, sizeof(img));
                n_blurring(n_blurred, main_image);
                for (int i = 0; i < 9; i++)
                {
                    n_blurring(tmp, n_blurred);
                    free(n_blurred->pixels);
                    n_blurred->pixels = tmp->pixels;
                    tmp->pixels = nullptr;
                }
                free(tmp);
            }
            stbi_write_png("output/n_blurred_img.png", width, height, 3, n_blurred->pixels, width * 3);
            printf("Normal blurred image saved as n_blurred_img.png\n");
            break;

        case 3:

            if (c_blurred->width)
            {
                printf("Already implemented.\n");
                break;
            }
            n_blurring(c_blurred, main_image);
            stbi_write_png("output/c_blurred_img.png", width, height, 3, c_blurred->pixels, width * 3);
            if (c_blurred)
                printf("Circular Blurred image saved as c_blurred_img.png\n");
            break;

        case 4:

            running = false;
            std::printf("Ending program...\n");
            break;


        default:

            printf("Enter valid input...\n");
        }
    }
    free_img(c_inverted);
    free_img(n_blurred);
    free_img(c_blurred);
    free(main_image);
    stbi_image_free(pixels);
    printf("Ending program...\n");
    return 0;
}