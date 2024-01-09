#include "helpers.h"
#include "math.h"
#include "stdio.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float grey = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed)/3.0);
            image[i][j].rgbtBlue = (int)grey;
            image[i][j].rgbtGreen = (int)grey;
            image[i][j].rgbtRed = (int)grey;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE reflect;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            //store the swaping pixel in to a temp variable.
            reflect.rgbtBlue = image[i][j].rgbtBlue;
            reflect.rgbtGreen = image[i][j].rgbtGreen;
            reflect.rgbtRed = image[i][j].rgbtRed;

            //swap the current pixel of [i][width-j]
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;

            //swap pixel [i][width-j] withtemp
            image[i][width - j - 1].rgbtBlue = reflect.rgbtBlue;
            image[i][width - j - 1].rgbtGreen = reflect.rgbtGreen;
            image[i][width - j - 1].rgbtRed = reflect.rgbtRed;

        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    //create a temporary picture with all original pixels values.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
            copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copy[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }
    // cycle between the 4 corners.
    for (int i = 0; i <= height - 2; i += (height - 2))
    {
        for (int j = 0; j <= width - 2; j += (width - 2))
        {
            int k, l;
            //index for top left corner
            if (i == 0 && j == 0)
            {
                k = i;
                l = j;
            }

            // index for top right corner
            else if (i == 0 && j == width - 2)
            {
                k = 0;
                l = j + 1;
            }

            //index for bottom left corner
            else if (i == width - 2 && j == 0)
            {
                k = i + 1;
                l = j;
            }
            // index for bottom right corner
            else
            {
                k = i + 1;
                l = j + 1;
            }

            image[k][l].rgbtBlue = (int)round((copy[i][j].rgbtBlue + copy[i + 1][j].rgbtBlue + copy[i][j + 1].rgbtBlue + copy[i + 1][j +
                                               1].rgbtBlue) / 4.0);
            image[k][l].rgbtGreen = (int)round((copy[i][j].rgbtGreen + copy[i + 1][j].rgbtGreen + copy[i][j + 1].rgbtGreen + copy[i + 1][j +
                                                1].rgbtGreen) / 4.0);
            image[k][l].rgbtRed = (int)round((copy[i][j].rgbtRed + copy[i + 1][j].rgbtRed + copy[i][j + 1].rgbtRed + copy[i + 1][j +
                                              1].rgbtRed) / 4.0);
        }
    }
    //blur top and bottom edge
    for (int i = 0; i <= height - 2 ; i += height - 2)
    {
        for (int j = 0; j <= width - 3; j++)
        {
            int sum_green = 0;
            int sum_blue = 0;
            int sum_red = 0;

            //add the surrounding pixels.
            for (int x = i; x < i + 2; x++)
            {
                for (int y = j; y < j + 3; y++)
                {
                    sum_green += copy[x][y].rgbtGreen;
                    sum_blue += copy[x][y].rgbtBlue;
                    sum_red += copy[x][y].rgbtRed;
                }
            }

            //correct index for current pixel
            int k, l;
            //current width = j+1
            l = j + 1;
            //correct index for bottom row
            if (i == height - 2)
            {
                k = i + 1;
            }
            //correct index for top row
            else
            {
                k = i;
            }

            image[k][l].rgbtGreen = (int)round(sum_green / 6.0);
            image[k][l].rgbtBlue = (int)round(sum_blue / 6.0);
            image[k][l].rgbtRed = (int)round(sum_red / 6.0);
        }
    }
    //blur side edge
    for (int i = 0; i <= height - 3 ; i++)
    {
        for (int j = 0; j <= width - 2; j += width - 2)
        {
            int sum_green = 0;
            int sum_blue = 0;
            int sum_red = 0;

            //add the surrounding pixels.
            for (int x = i; x < i + 3; x++)
            {
                for (int y = j; y < j + 2; y++)
                {
                    sum_green += copy[x][y].rgbtGreen;
                    sum_blue += copy[x][y].rgbtBlue;
                    sum_red += copy[x][y].rgbtRed;
                }
            }

            //correct index for current pixel
            int k, l;
            //current height = i+1
            k = i + 1;
            //correct index for sides
            if (j == width - 2)
            {
                l = j + 1;
            }
            //correct index for top row
            else
            {
                l = j;
            }

            image[k][l].rgbtGreen = (int)round(sum_green / 6.0);
            image[k][l].rgbtBlue = (int)round(sum_blue / 6.0);
            image[k][l].rgbtRed = (int)round(sum_red / 6.0);
        }
    }
    //center
    for (int i = 0; i <= height - 3 ; i++)
    {
        for (int j = 0; j <= width - 3; j++)
        {
            int sum_green = 0;
            int sum_blue = 0;
            int sum_red = 0;

            //add the surrounding pixels.
            for (int x = i; x < i + 3; x++)
            {
                for (int y = j; y < j + 3; y++)
                {
                    sum_green += copy[x][y].rgbtGreen;
                    sum_blue += copy[x][y].rgbtBlue;
                    sum_red += copy[x][y].rgbtRed;
                }
            }

            //correct index for current pixel
            int k, l;
            k = i + 1;
            l = j + 1;

            image[k][l].rgbtGreen = (int)round(sum_green / 9.0);
            image[k][l].rgbtBlue = (int)round(sum_blue / 9.0);
            image[k][l].rgbtRed = (int)round(sum_red / 9.0);
        }
    }
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height + 2][width + 2];
    //create a temporary picture with all original pixels values.
    for (int i = 0; i < height + 1; i++)
    {
        for (int j = 0; j < width + 1; j++)
        {
            if (i == 0 || i == width + 1 || j == 0 || j == width + 1)
            {
                copy[i][j].rgbtBlue = 0;
                copy[i][j].rgbtGreen = 0;
                copy[i][j].rgbtRed = 0;
            }

            else
            {
                copy[i][j].rgbtBlue = image[i - 1][j - 1].rgbtBlue;
                copy[i][j].rgbtGreen = image[i - 1][j - 1].rgbtGreen;
                copy[i][j].rgbtRed = image[i - 1][j - 1].rgbtRed;

            }
        }
    }

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    //invert the center pixels
    for (int i = 0; i < height ; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int Gx_green = 0;
            int Gx_blue = 0;
            int Gx_red = 0;
            int Gy_green = 0;
            int Gy_blue = 0;
            int Gy_red = 0;

            //add the surrounding pixels.
            for (int x = i; x < i + 3; x++)
            {
                for (int y = j; y < j + 3; y++)
                {
                    Gx_green += copy[x][y].rgbtGreen * Gx[x - i][y - j];
                    Gx_blue += copy[x][y].rgbtBlue * Gx[x - i][y - j];
                    Gx_red += copy[x][y].rgbtRed * Gx[x - i][y - j];
                    Gy_green += copy[x][y].rgbtGreen * Gy[x - i][y - j];
                    Gy_blue += copy[x][y].rgbtBlue * Gy[x - i][y - j];
                    Gy_red += copy[x][y].rgbtRed * Gy[x - i][y - j];
                }
            }
            //correct index for current pixel

            double f_green = sqrt((Gx_green * Gx_green) + (Gy_green * Gy_green));
            double f_blue = sqrt((Gx_blue * Gx_blue) + (Gy_blue * Gy_blue));
            double f_red = sqrt((Gx_red * Gx_red) + (Gy_red * Gy_red));

            if (f_green > 255)
            {
                f_green = 255;
            }
            if (f_blue > 255)
            {
                f_blue = 255;
            }
            if (f_red > 255)
            {
                f_red = 255;
            }

            image[i][j].rgbtGreen = (int)round(f_green);
            image[i][j].rgbtBlue = (int)round(f_blue);
            image[i][j].rgbtRed = (int)round(f_red);
        }
    }
    return;
}
