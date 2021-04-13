#include "helpers.h"
#include <math.h>

void maxPixel(RGBTRIPLE *pixel)
{
    BYTE max = round((double)((double)pixel->rgbtBlue + (double)pixel->rgbtGreen + (double)pixel->rgbtRed) / (double)3);
    pixel->rgbtGreen = max;
    pixel->rgbtRed = max;
    pixel->rgbtBlue = max;
}
void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE placeHolder = *a;
    *a = *b;
    *b = placeHolder;
}
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            maxPixel(&image[i][j]);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        int right = width - 1;
        int left = 0;
        while (left < right)
        {
            swap(&image[i][left], &image[i][right]);
            left++;
            right--;
        }
    }
    return;
}
void addPixels(int *R, int *G, int *B, RGBTRIPLE *pixel)
{
    *G += (int)pixel->rgbtGreen;
    *R += (int)pixel->rgbtRed;
    *B += (int)pixel->rgbtBlue;
}
void blurPixel(int height, int width, RGBTRIPLE in[height][width], RGBTRIPLE out[height][width], int x, int y)
{
    //Up
    int R = in[x][y].rgbtRed;
    int G = in[x][y].rgbtGreen;
    int B = in[x][y].rgbtBlue;
    int add = 1;
    if (x - 1 >= 0)
    {
        addPixels(&R, &G, &B, &in[x - 1][y]);
        add++;
    }
    if (x + 1 < height)
    {
        addPixels(&R, &G, &B, &in[x + 1][y]);
        add++;
    }
    if (x - 1 >= 0 && y - 1 >= 0)
    {
        addPixels(&R, &G, &B, &in[x - 1][y - 1]);
        add++;
    }
    if (x - 1 >= 0 && y + 1 < width)
    {
        addPixels(&R, &G, &B, &in[x - 1][y + 1]);
        add++;
    }
    if (x + 1 < height && y - 1 >= 0)
    {
        addPixels(&R, &G, &B, &in[x + 1][y - 1]);
        add++;
    }
    if (x + 1 < height && y + 1 < width)
    {
        addPixels(&R, &G, &B, &in[x + 1][y + 1]);
        add++;
    }
    if (y - 1 >= 0)
    {
        addPixels(&R, &G, &B, &in[x][y - 1]);
        add++;
    }
    if (y + 1 < width)
    {
        addPixels(&R, &G, &B, &in[x][y + 1]);
        add++;
    }
    RGBTRIPLE new_pixel;
    new_pixel.rgbtGreen = (BYTE)round((double)G / (double)add);
    new_pixel.rgbtRed = (BYTE)round((double)R / (double)add);
    new_pixel.rgbtBlue = (BYTE)round((double)B / (double)add);
    out[x][y] = new_pixel;
}
// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE output[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            blurPixel(height, width, image, output, i, j);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = output[i][j];
        }
    }
    return;
}

void edgePixel(int height, int width, RGBTRIPLE in[height][width], RGBTRIPLE out[height][width], int x, int y)
{
    //Up
    int Rx = 0;
    int Gx = 0;
    int Bx = 0;
    int Ry = 0;
    int Gy = 0;
    int By = 0;
    if (x - 1 >= 0)
    {
        //X up
        Ry += (int)in[x - 1][y].rgbtRed * -2;
        Gy += (int)in[x - 1][y].rgbtGreen * -2;
        By += (int)in[x - 1][y].rgbtBlue * -2;
    }
    if (x + 1 < height)
    {
        Ry += (int)in[x + 1][y].rgbtRed * 2;
        Gy += (int)in[x + 1][y].rgbtGreen * 2;
        By += (int)in[x + 1][y].rgbtBlue * 2;
    }
    if (x - 1 >= 0 && y - 1 >= 0)
    {
        Rx += (int)in[x - 1][y - 1].rgbtRed * -1;
        Gx += (int)in[x - 1][y - 1].rgbtGreen * -1;
        Bx += (int)in[x - 1][y - 1].rgbtBlue * -1;
        //
        Ry += (int)in[x - 1][y - 1].rgbtRed * -1;
        Gy += (int)in[x - 1][y - 1].rgbtGreen * -1;
        By += (int)in[x - 1][y - 1].rgbtBlue * -1;
    }
    if (x - 1 >= 0 && y + 1 < width)
    {
        Rx += (int)in[x - 1][y + 1].rgbtRed * 1;
        Gx += (int)in[x - 1][y + 1].rgbtGreen * 1;
        Bx += (int)in[x - 1][y + 1].rgbtBlue * 1;
        
        Ry += (int)in[x - 1][y + 1].rgbtRed * -1;
        Gy += (int)in[x - 1][y + 1].rgbtGreen * -1;
        By += (int)in[x - 1][y + 1].rgbtBlue * -1;
    }
    if (x + 1 < height && y - 1 >= 0)
    {
        Rx += (int)in[x + 1][y - 1].rgbtRed * -1;
        Gx += (int)in[x + 1][y - 1].rgbtGreen * -1;
        Bx += (int)in[x + 1][y - 1].rgbtBlue * -1;
        
        Ry += (int)in[x + 1][y - 1].rgbtRed * 1;
        Gy += (int)in[x + 1][y - 1].rgbtGreen * 1;
        By += (int)in[x + 1][y - 1].rgbtBlue * 1;
    }
    if (x + 1 < height && y + 1 < width)
    {
        Rx += (int)in[x + 1][y + 1].rgbtRed * 1;
        Gx += (int)in[x + 1][y + 1].rgbtGreen * 1;
        Bx += (int)in[x + 1][y + 1].rgbtBlue * 1;
        
        Ry += (int)in[x + 1][y + 1].rgbtRed * 1;
        Gy += (int)in[x + 1][y + 1].rgbtGreen * 1;
        By += (int)in[x + 1][y + 1].rgbtBlue * 1;
    }
    if (y - 1 >= 0)
    {
        Rx += (int)in[x][y - 1].rgbtRed * -2;
        Gx += (int)in[x][y - 1].rgbtGreen * -2;
        Bx += (int)in[x][y - 1].rgbtBlue * -2;
    }
    if (y + 1 < width)
    {
        Rx += (int)in[x][y + 1].rgbtRed * 2;
        Gx += (int)in[x][y + 1].rgbtGreen * 2;
        Bx += (int)in[x][y + 1].rgbtBlue * 2;
    }
    RGBTRIPLE new_pixel;
    double green_val = sqrt(pow(Gx, 2) + pow(Gy, 2));
    if (green_val > 255)
    {
        green_val = 255;
    } 
    else if (green_val < 0)
    {
        green_val = 0;
    }
    double red_val = sqrt(pow(Rx, 2) + pow(Ry, 2));
    if (red_val > 255)
    {
        red_val = 255;
    } 
    else if (red_val < 0)
    {
        red_val = 0;
    }
    double blue_val = sqrt(pow(Bx, 2) + pow(By, 2));
    if (blue_val > 255)
    {
        blue_val = 255;
    } 
    else if (blue_val < 0)
    {
        blue_val = 0;
    }
    new_pixel.rgbtGreen = (BYTE)round(green_val);
    new_pixel.rgbtRed = (BYTE)round(red_val);
    new_pixel.rgbtBlue = (BYTE)round(blue_val);
    out[x][y] = new_pixel;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE output[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            edgePixel(height, width, image, output, i, j);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = output[i][j];
        }
    }
    return;
}
