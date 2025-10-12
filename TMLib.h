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
#define TRUE (1==1)
#define EXPONENT 0.408163265306
#define DIVIDER 28.8
#define SUBTRACTOR 0.83

/*Text Colours*/
#define TEXT_RESET "\33[0m"
#define TEXT_YELLOW "\33[93m"
#define TEXT_BLUE "\33[94m"
#define TEXT_ORANGE "\33[33m"
#define TEXT_RED "\33[1m\33[91m"

/*struct four_six_bit_pixels
        {
	int red_a;
	int grn_a;
	int blu_a;
	int red_b;
	int grn_b;
	int blu_b;
	int red_c;
	int grn_c;
	int blu_c;
	int red_d;
	int grn_d;
	int blu_d;
	};*/

struct four_six_bit_pixels
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

struct file_name_return
	{
	char name [FILENAME_LENGTH];
	int width;
	int height;
	};

void exit_error (char *message_a, char *message_b);
struct rgb_accumulator get_nine_six (char *nine_byte_string);
struct maxmin_return find_limits (float red_value, float grn_value, float blu_value);
char *compose_filename (char *img_name, char *img_rename, char *gry_print, char *hue_print);
struct file_name_return separate_filename (char *thm_filename);
