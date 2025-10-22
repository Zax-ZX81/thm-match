/* * * * * * * * * * * * * *
 *                         *
 *       TMLib.h 0.10      *
 *                         *
 *       2025-10-01        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */

#define BASE_SIXTYFOUR "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-+\0"
#define GRAPHICS_EXTENSIONS " bmp BMP gif GIF jfif JFIF jpg jpeg JPG JPEG png PNG tif tiff TIF TIFF webp WEBP "
#define MAGICK_COMMAND "magick "
#define RGB_ARGS " -resize '64x64' -background 'gray(50%)' -gravity center -extent 64x64 -depth 6 -compress None rgb:-"
#define MAG_ARGS "identify -format '%G' "

#define RED_CHAN 0
#define GRN_CHAN 1
#define BLU_CHAN 2

#define FILE_ARG 2
#define FILE_EXTN ".rgb"

#define NULL_STRING ""
#define FILENAME_LENGTH 256
#define THUMBNAIL_BYTES 9216
#define TRUE (1==1)
#define FALSE !TRUE
#define SW_ON TRUE
#define SW_OFF FALSE

#define SLASH_TERM "/"
#define PATH_CURRENT "./"
#define DIR_CURRENT "."
#define DIR_PREV ".."
#define FILE_ENTRY 'f'
#define DIR_ENTRY 'd'
#define UNKNOWN_ENTRY 'x'
#define T_REJ 'r'
#define DATABASE_INITIAL_SIZE 4096
#define DATABASE_INCREMENT 4096


/*Calculation Values*/
#define SIXBIT_MULTIPLIER 10.5
#define DECIMAL_DIVIDER 256
#define QUADRANT_DIVIDER 256
#define EXPONENT 0.4082
#define DIVIDER 28.8
#define SUBTRACTOR 1.39

/*Text Colours*/
#define TEXT_RESET "\33[0m"
#define TEXT_YELLOW "\33[93m"
#define TEXT_BLUE "\33[94m"
#define TEXT_ORANGE "\33[33m"
#define TEXT_RED "\33[1m\33[91m"

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

struct rgb_six_bit
	{
	unsigned int red_val : 6;
	unsigned int grn_val : 6;
	unsigned int blu_val : 6;
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

struct dimension_return
	{
	int width;
	int height;
	};

struct tprint_database
	{
	char gry_print [5];		// grey print 4 character code
	char hue_print [5];		// hue print 4 character code
	char magnitude;			// magnitude 1 character code
	char filepath [FILENAME_LENGTH];
	};

struct tprint_flags
	{
	char tprt;		// write thumbnail file
	char std_out;		// print output to stdout, supress file output
	char verbose;		// mirror everything to stdout
};

struct tfind_flags
	{
	char recurse;		// go beyond the current directory
	char sort;		// sort or not
	char tprt;		// write thumbnail file
	char std_out;		// print output to stdout, supress file output
	char verbose;		// mirror everything to stdout
};

struct find_list_entry
{
	char object_type;
	char filepath [FILENAME_LENGTH];
	char file_ext [6];
};

void exit_error (char *message_a, char *message_b);
void error_mess (char *message_a, char *message_b);
struct rgb_accumulator get_nine_six (unsigned char *nine_byte_string);
struct maxmin_return find_limits (float red_value, float grn_value, float blu_value);
char *compose_filename (char *img_name, char *img_rename, char *gry_print, char *hue_print);
struct file_name_return separate_filename (char *thm_filename);
struct dimension_return separate_magnitude (char *mag_string);
char *get_gpx_ext (char *filepath);
struct tprint_database get_thumbprint (char *img_name);
char *enquote (char *filepath);
char *twelve_six_bit (unsigned char *nine_byte_string);
int sixfour_to_dec (char sixfour);
