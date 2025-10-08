/* * * * * * * * * * * * * *
 *                         *
 *       TMLib.h 0.10      *
 *                         *
 *       2025-10-01        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */

#define BASE_SIXTYFOUR "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-+"
#define RED_CHAN 0
#define GRN_CHAN 1
#define BLU_CHAN 2
#define FILE_ARG 2
#define FILE_EXTN ".fprt"
#define NULL_STRING ""
#define FILENAME_LENGTH 256
#define SIXBIT_MULTIPLIER 10.5
#define DECIMAL_DIVIDER 256
#define DECIMAL_DIVIDER 256
#define QUADRANT_DIVIDER 256


/*Text Colours*/
#define TEXT_RESET "\33[0m"
#define TEXT_YELLOW "\33[93m"
#define TEXT_BLUE "\33[94m"
#define TEXT_ORANGE "\33[33m"
#define TEXT_RED "\33[1m\33[91m"

struct four_six_bit_pixels	//test 6 bit integer array
        {
	unsigned int red_a : 6;
	unsigned int grn_a : 6;
	unsigned int blu_a : 6;
	unsigned int red_b : 6;
	unsigned int grn_b : 6;
	unsigned int blu_b : 6;
	unsigned int red_c : 6;
	unsigned int grn_c : 6;
	unsigned int blu_c : 6;
	unsigned int red_d : 6;
	unsigned int grn_d : 6;
	unsigned int blu_d : 6;
	};

struct rgb_accumulator
	{
	float red_val;
	float grn_val;
	float blu_val;
	};

struct colgry_accumulator
	{
	float red_val;
	float grn_val;
	float blu_val;
	float gry_val;
	};

struct maxmin_return
	{
	char channel;
	float max_val;
	float min_val;
	};

struct image_print
	{
	char gry;
	char hue;
	};


void exit_error (char *message_a, char *message_b);
struct rgb_accumulator get_nine_six (char *nine_byte_string);
struct maxmin_return find_limits (float red_value, float grn_value, float blu_value);
char *compose_filename (char *img_name, char *img_rename, char *gry_print, char *hue_print);
