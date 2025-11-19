/* * * * * * * * * * * * * *
 *                         *
 *       TMLib.h 0.35      *
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
#define DB_EXTN ".tpdb"

#define NULL_STRING ""
#define TAB_CHAR 9
#define CGE_RET 13
#define FILENAME_LENGTH 256
#define FILELINE_LENGTH 1024
#define FILEPATH_LENGTH 1024
#define MAG_SUM_LEN 12
#define THUMBNAIL_BYTES 9216
#define THUMBNAIL_PIXELS 4096
#define TRUE (1==1)
#define FALSE !TRUE
#define SW_ON TRUE
#define SW_OFF FALSE
#define SORT_DB TRUE
#define NO_SORT_DB FALSE
#define SQUARE 1
#define LANDSC 2
#define PORTRAIT 3

#define SLASH_TERM "/"
#define PATH_CURRENT "./"
#define DIR_CURRENT "."
#define DIR_PREV ".."
#define FILE_ENTRY 'f'
#define DIR_ENTRY 'd'
#define UNKNOWN_ENTRY 'x'
#define T_REJ 'r'
#define FOR_READ "r"
#define READ_BINARY "rb"
#define FOR_WRITE "w"
#define WRITE_BINARY "wb"
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

// Pixels and images
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

struct dimension_return
	{
	int width;
	int height;
	};

struct thumbprint_histogram
	{
	unsigned int histogram [64];
	float hscale;
	float grey_mean;
	float grey_tot;
	float vari_tot;
	float cont_mult;
	float std_dev;
	float grey_val [4096];
	};

struct file_name_return
	{
	char name [FILENAME_LENGTH];
	int width;
	int height;
	};

struct rgb_return
	{
	int red_val;
	int grn_val;
	int blu_val;
	};


// Database
struct tprint_database
	{
	char gry_print [5];			// grey print 4 character code
	char hue_print [5];			// hue print 4 character code
	char magnitude [1];			// magnitude 1 character code
	char filepath [FILEPATH_LENGTH];
	};

struct tfind_database
	{
	char gry_print [5];			// grey print 4 character code
	char hue_print [5];			// hue print 4 character code
	char magnitude [1];			// magnitude 1 character code
	char filepath [FILEPATH_LENGTH];
	unsigned long filesize;
	int index;
	};

struct tmatch_database
	{
	char gry_print [5];			// grey print 4 character code
	char hue_print [5];			// hue print 4 character code
	char magnitude [1];			// magnitude 1 character code
	char filepath [FILEPATH_LENGTH];
	unsigned long filesize;
	int timestamp;
	int fuzz_match;				// is fuzzy match?
	int dup_num;
	int thumb_pres;				// is thumbnail present?
	int index;
	};

struct find_list_entry
	{
	char object_type;
	char filepath [FILEPATH_LENGTH];
	char file_ext [6];
	unsigned long filesize;
	};

struct tprint_db_lookup
	{
	int start;
	int ents;
	};


// Flags
struct tprint_flags
	{
	char tprt;		// write thumbnail file
	char std_out;		// print output to stdout, supress file output
	char verbose;		// mirror everything to stdout
	};

struct tdup_flags
	{
	char fuzzy;		// fuzzy search
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


// Functions
void error_message (char *message_a, char *message_b);
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
char tpdb_verify (char *fileline);
int fuzz_search (char *srch_prnt, char *tgt_prnt);
int exact_search (char *srch_prnt, char *tgt_prnt);
struct rgb_return get_pixel (struct rgb_return *in_img_buff, int anch, int width, int y_max, int x_max);
