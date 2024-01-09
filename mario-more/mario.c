#include <cs50.h>
#include <stdio.h>
#include <string.h>

int get_size(void);
void print_block(int height);

int main(void)
{
    //Get size of grind
    int height = get_size();
    //Print grind
    print_block(height);
}

int get_size(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    return height;
}

void print_block(int height)
{
    char block[] = "#";
    char output[9];

    for (int i = 0; i < height ; i++)
    {
        output[i] = '#';

        for (int j = 0 ; j < height - i - 1 ; j++)
        {
            printf(" ");
        }
        printf("%s  %s\n", output, output);
    }

}

void card_type(long cc){

    int validation = validation(cc);

    //if validation == 0 return Invalid
    if (validation == 0)
    {
        printf("INVALID\n");
        return;
    }

    //else run the rest of the program to find the card type.

    else
    {
        //find how many digit the credit card it
        long i = 10000000000000;
        while ((cc%i)<cc)
        {
            i *= 10;
        }
        long f2_digits = ((cc%i)-(cc%(i/100)))/100;
        printf("%li",f2_digits);
    }


}