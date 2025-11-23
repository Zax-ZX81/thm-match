/* * * * * * * * * * * *
*                      *
*    six_sixfour.h     *
*                      *
*    Zax               *
*                      *
* * * * * * * * * * * */

struct rgb_accumulator get_nine_six (unsigned char *nine_byte_string);
char *twelve_six_bit (unsigned char *nine_byte_string);
int sixfour_to_dec (char sixfour);
unsigned char *twelveight_to_nine_six (unsigned char *twelve_byte_string);
unsigned char dec_to_sixfour (int dec_value);
