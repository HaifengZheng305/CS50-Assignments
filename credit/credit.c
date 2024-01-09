#include <cs50.h>
#include <stdio.h>
#include <math.h>

long get_cc(void);
int validation(long cc);
void card_type(long cc);

int main(void)
{
    //gets credit card
    long cc = get_cc();

    //check the type of card
    card_type(cc);
}







long get_cc(void)
{
    long cc_number = 0;

    do
    {
        cc_number = get_long("Number: ");
    }
    while (cc_number < 0);

    return cc_number;

}

//check if it is a valid credit card
int validation(long cc)
{
    int valid = 0;
    //if less than 13 digits it returns 0
    if (cc <= 999999999999)
    {
        return valid;
    }
    //////////////////////////////////////


    //else run the rest of the program
    else

    {
        //Calculate the sumprodcut. Sumproduct is the sum of every other digit*2
        long sumproduct, sum, i, j;

        sumproduct = 0;

        i = 10;

        while ((cc % i) < cc)
        {
            long EO_digit = 2 * ((cc % (i * 10)) - (cc % i)) / i;
            long sum_digit = (EO_digit % 100 - EO_digit % 10) / 10 + EO_digit % 10;
            sumproduct += sum_digit;
            i = i * 100;
        }
        ///////////////////////////////////////////////////////////////////////

        //Calculate the sum the other digits starts adding from the sumproduct;
        sum = sumproduct;

        j = 1;

        while ((cc % j) < cc)
        {
            long EO_digit = ((cc % (j * 10)) - (cc % j)) / j;
            sum += EO_digit;
            j = j * 100;
        }
        /////////////////////////////////////////////////////////////////////////////////

        //if final sum = 20 then valid = 1
        if (sum % 10 == 0)
        {
            valid = 1;
        }
    }
    return valid;
}




void card_type(long cc)
{

    int valid = validation(cc);

    //if validation == 0 return Invalid
    if (valid == 0)
    {
        printf("INVALID\n");
        return;
    }

    //else run the rest of the program to find the card type.

    else
    {
        //find first 2 digits
        long i = 10000000000000;
        while ((cc % i) < cc)
        {
            i *= 10;
        }
        long f2_digits = cc / (i / 100);
        /////////////////////////////////////////////////////////////////

        //determine tha card
        if ((f2_digits == 34) || (f2_digits == 37))
        {
            printf("AMEX\n");
        }
        else if ((f2_digits == 51) || (f2_digits == 52) || (f2_digits == 53) || (f2_digits == 54) || (f2_digits == 55))
        {
            printf("MASTERCARD\n");
        }
        else if ((f2_digits) / 10 == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
        ///////////////////////////////////////////////////////////////////////
    }


}
