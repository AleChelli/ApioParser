%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "APIO.h"
%}

%union {
	struct{
	char * sval;
    int    ival;  
	}pair;
}

/* declare syntax */
%type <pair> tag_o value

/* declare tokens */
%token TOK_ANGULAR_O TOK_ANGULAR_C TOK_ANGULAR_C_O
%token <pair> TOK_STRING TOK_INTEGER TOK_TAG_NAME TOK_VERBATIM_CODE

%%

lines:
	| lines line
	;

line: tag_o 
	| tag_o value
	| tag_o value tag_c
		{
			printf("sono una linea\n");
			printf("tag int: %d\n",$1.ival);
			if($1.ival==2)
			{
				//object name
				objectName=$2.sval;
			}
			else if($1.ival==3)
			{
				//object id
				objectId=$2.sval;
			} 
			else if($1.ival==8)
			{
				//method name
				methodsArray[methodsNumber-1].name=$2.sval;			
			}
			else if($1.ival==9)
			{
				//method code
				methodsArray[methodsNumber-1].code=$2.sval;			
			}
			else if($1.ival==10)
			{
				//pin number
				pinsArray[pinsNumber-1].number=$2.sval;
			}
			else if($1.ival==11)
			{
				//pin value
				pinsArray[pinsNumber-1].value=$2.sval;
			}
			else if($1.ival==12)
			{
				//pin mode
				pinsArray[pinsNumber-1].mode=$2.sval;
			}
			else if($1.ival==14)
			{
				//variable type
				variableArray[variableNumber-1].type=$2.sval;
			}
			else if($1.ival==15)
			{
				//variable name
				variableArray[variableNumber-1].name=$2.sval;
			}
			else if($1.ival==16)
			{
				//variable value
				variableArray[variableNumber-1].value=$2.sval;
			}
		}
	| value tag_c
	| tag_c
	;

tag_o: TOK_ANGULAR_O TOK_TAG_NAME TOK_ANGULAR_C
		{	
			$2.ival=switchTheTag($2.sval);
			printf("Opening tag founded\n");
			printf("\tstring: %s\n",$2.sval);
			printf("\tinteger: %d\n",$2.ival);
			printf("\n");
			$$.ival=$2.ival;
		}
	;
tag_c: TOK_ANGULAR_C_O TOK_TAG_NAME TOK_ANGULAR_C
		{
			//fprintf(headerArduino,"tag chiuso: %s\n",$2.sval);
			/*	$2.ival=1;
			printf("Closing tag founded\n");
			printf("\tstring: %s\n",$2.sval);
			printf("\tinteger: %d\n",$2.ival);
			printf("\n");
			*/
		}
value: TOK_STRING
		{
			printf("value string found: %s\n",$1.sval);
		}
	| TOK_INTEGER
		{
			printf("value integer found: %s\n",$1.sval);
		}
	| TOK_VERBATIM_CODE
		{
			printf("value verbatim found: %s\n",$1.sval);
		}
	| " "
	;

%%