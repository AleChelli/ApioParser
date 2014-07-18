%option noyywrap yylineno

%x VERBATIM_METHOD_CODE VERBATIM_LISTENER_CODE

/* patterns name */
ANGULAR_O 			"<"
ANGULAR_C 			">"
ANGULAR_C_O 		"</"
STRING				[a-zA-Z][_a-zA-Z0-9]*
DIGIT             	[0-9]
INTEGER            	{DIGIT}+
TAG_NAME  			"object"|"name"|"id"|"pins"|"pin"|"number"|"value"|"mode"|"methods"|"method"|"methodname"|"variables"|"variable"|"variabletype"|"variablename"|"variablevalue"
OPERATOR			"+"|"-"|"*"|"/"|"^"|"("|")"|"["|"]"|";"|","|"."|" "|"="|"=="|"_"|"|"|"==="|"!"|"{"|"}"|":"


VERBATIM_CODE		({STRING}|{INTEGER}|{OPERATOR})*

%{
	#include "APIO.h" 
	#include "APIO.tab.h"
%}

%%
"<code>"										{ printf("verbatim method iniziato a linea %d\n", yylineno); BEGIN(VERBATIM_METHOD_CODE); }
<VERBATIM_METHOD_CODE>"</code>"					{ printf("verbatim method finito a linea %d\n", yylineno); BEGIN(INITIAL); }
<VERBATIM_METHOD_CODE>{VERBATIM_CODE}|\n		{ 
													int len;
													int plusLen;
													plusLen=strlen(strdup(yytext));
													len=strlen(methodsArray[methodsNumber-1].code);
													newLen=newLen+plusLen+len;
													
													methodsArray[methodsNumber-1].code=(char *) realloc(methodsArray[methodsNumber-1].code,newLen);

													//printf(strdup(yytext));
													yylval.pair.sval=strdup(yytext);
													yylval.pair.ival=9;
													
													strcat(methodsArray[methodsNumber-1].code,strdup(yytext));
													//printf("\ncode: %s\n",methodsArray[methodsNumber-1].code);
													
												}
<VERBATIM_METHOD_CODE><<EOF>>					{ printf("%s:%d: Unterminated code\n", curfilename, yylineno); return 0; }

"<listener>"									{ printf("verbatim listener iniziato a linea %d\n", yylineno); BEGIN(VERBATIM_LISTENER_CODE); }
<VERBATIM_LISTENER_CODE>"</listener>"			{ printf("verbatim listener finito a linea %d\n", yylineno); BEGIN(INITIAL); }
<VERBATIM_LISTENER_CODE>{VERBATIM_CODE}|\n		{ 
													if(listenerCounter==0)
													{
														newLenListener=strlen(strdup(yytext));
														listenerCode=(char *) malloc(newLenListener);
														listenerCounter++;
														strcpy(listenerCode,strdup(yytext));
													} else {
														int len2;
														int plusLen2;
														plusLen2=strlen(strdup(yytext));
														len2=strlen(listenerCode);
														newLenListener=newLenListener+plusLen2+len2;

														listenerCode=(char *) realloc(listenerCode,newLenListener);
														strcat(listenerCode,strdup(yytext));
													}
													
													printf("guardami1: %s\n",strdup(yytext));
													printf("guardami2: %s\n",listenerCode);
													
												}
<VERBATIM_LISTENER_CODE><<EOF>>					{ printf("%s:%d: Unterminated code\n", curfilename, yylineno); return 0; }

{ANGULAR_O}		{return TOK_ANGULAR_O;}
{ANGULAR_C}		{return TOK_ANGULAR_C;}
{ANGULAR_C_O}	{return TOK_ANGULAR_C_O;}
{TAG_NAME}		{yylval.pair.sval=strdup(yytext);yylval.pair.ival=0;return TOK_TAG_NAME;}
{STRING}		{yylval.pair.sval=strdup(yytext);yylval.pair.ival=0;return TOK_STRING;}
{INTEGER}		{yylval.pair.sval=strdup(yytext);yylval.pair.ival=0;return TOK_INTEGER;}


%%