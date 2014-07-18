/* 
*	header for APIO functions and variables
*/
char *curfilename;			/* name of current input file */

extern FILE *yyin;
extern FILE *yyout;
extern yylineno;
int prova;
char buf[256]; //memorize the buffer string for the assignement_line
typedef struct yy_buffer_state *YY_BUFFER_STATE;

/* Size of default input buffer. */
#ifndef YY_BUF_SIZE
#ifdef __ia64__
/* On IA-64, the buffer size is 16k, not 8k.
 * Moreover, YY_BUF_SIZE is 2*YY_READ_BUF_SIZE in the general case.
 * Ditto for the __ia64__ case accordingly.
 */
#define YY_BUF_SIZE 32768
#else
#define YY_BUF_SIZE 16384
#endif /* __ia64__ */
#endif

//typedef struct yy_buf_size *YY_BUF_SIZE;

/* include file stack */
struct bufstack {
	struct bufstack *prev; 			/* previous entry */
	YY_BUFFER_STATE bs;				/* saved buffer */
	int lineno;						/* saved line number in this file */
	char *filename;					/* name of this file */
	FILE *f;						/* current file */
} *curbs;


struct pin
    {
		char *number;				/* number of the pin in the Arduino */
		char *value;				/* initial value of the pin (number) of the Arduino */
		char *mode;					/* mode of the Arduino pin (INPUT/OUTPUT) */
    };
typedef struct pin pin;

struct variable
    {
		char *type;					/* type of the variable */
		char *name;					/* name of the variable */
		char *value;				/* initial value of the variable */
    };
typedef struct variable variable;

struct method
    {
		char *name;				/* number of the pin in the Arduino */
		char *code;				/* initial value of the pin (number) of the Arduino */
    };
typedef struct method method;

FILE *headerArduino;			//global file pointer for the header of arduino
FILE *inoArduino;				//global file pointer for the main code of arduino
FILE *makeArduino;				//global file pointer for the makefile for arduino
pin *pinsArray;					//global array that store the number and the value of the pins
int pinsNumber;					//global variable which stores the number of pins. That's for the realloc.
variable *variableArray;		//global array that store the number, name and value of a user variable
int variableNumber;				//global variable which stores the number of user variable. That's for the realloc.
method *methodsArray;			//global array that store the name and the code of a method
int methodsNumber;				//global variable which stores the number of methods. That's for the realloc.
char *objectName;				//global variable used for the object name
char *objectId;					//global variable used for the object identifier (xbee)
char *listenerCode;				//global variable used for the object name
int listenerCounter;			//global variable used for counting the listener lines

int newLen;
int newLenListener;

int newfile(char *fn);							/*file opener */
int switchTheTag(char *sval);					/*switch the tag giving the integer value a return*/

void execute_vfgen(char *);