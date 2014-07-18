#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "APIO.h"

int main(int argc, char **argv)
{
	int i;


	//recover each file name passed by terminal which need the parser analisys
	for(i = 1; i < argc; i++) {
		if(newfile(argv[i]))
		{
			pinsNumber=0; //set the number of the pins to 0 at the beginning.
			listenerCounter=0;
			char *fileout;
			char *fileout_ext; //fileout with extension (ino)
			char *fileout2_ext; //fileout with extension (h)
			fileout=(char *) malloc(strlen(argv[i]));
			fileout_ext=(char *) malloc(strlen(argv[i])); 
			fileout2_ext=(char *) malloc(strlen(argv[i])); 
			strcpy(fileout,argv[i]);
			fileout[strlen(fileout)-4] = 0; //cut the extension of the input file
			//use fileout_ext variable to set the output file name into fileout.ino
			strcpy(fileout_ext,fileout);
			strcpy(fileout2_ext,fileout);
			fileout_ext=(char *) realloc(fileout_ext,4); //space reallocation for the ".ino" extension
			fileout2_ext=(char *) realloc(fileout2_ext,2); //space reallocation for the ".h" extension
			strcat(fileout_ext,".ino");//adding the ".ino" extension
			strcat(fileout2_ext,".h");//adding the ".h" extension


			yylineno = 0; //start the yylineno variable from the begin of the document (line zero)
			yyparse();
   			execute_vfgen(fileout_ext);
   			//printf("%s\n",fileout);
   			//printf("%s\n",fileout_ext );
   			//printf("%s\n",fileout2_ext );
			
			int k;
			char *command;
			char *pathIno;
			char *pathHeader;
			char *pathMake;
			char *objectNamePointer;
				command=(char *) malloc(strlen(fileout));
				strcpy(command,"mkdir ");
				command = (char *) realloc(command,strlen(fileout));
				strcat(command,fileout);
				printf("%s\n",command );
				system(command);

				pathIno=(char *) malloc(strlen(fileout)+strlen(fileout_ext)+1);
				strcpy(pathIno,fileout);
				strcat(pathIno,"/");
				strcat(pathIno,fileout_ext);

				pathHeader=(char *) malloc(strlen(fileout)+strlen(fileout2_ext)+1);
				strcpy(pathHeader,fileout);
				strcat(pathHeader,"/");
				strcat(pathHeader,fileout2_ext);

				pathMake=(char *) malloc(strlen(fileout)+9);
				strcpy(pathMake,fileout);
				strcat(pathMake,"/");
				strcat(pathMake,"Makefile");

			printf("objectName: %s\n",objectName);
			printf("objectID: %s\n",objectId);
			for(k=0;k<pinsNumber;k++){
				printf("pinsArray\n");
				printf("\tPin element %d:\n",k);
				printf("\t\tNumber: %s\n",pinsArray[k].number);
				printf("\t\tValue: %s\n",pinsArray[k].value);
			}
			for(k=0;k<variableNumber;k++){
				printf("variableArray\n");
				printf("\tVariable element %d:\n",k);
				printf("\t\tType: %s\n",variableArray[k].type);
				printf("\t\tName: %s\n",variableArray[k].name);
				printf("\t\tValue: %s\n",variableArray[k].value);
			}
			for(k=0;k<methodsNumber;k++){
				printf("methodsArray\n");
				printf("\tMethod element %d:\n",k);
				printf("\t\tName: %s\n",methodsArray[k].name);
				printf("\t\tCode: %s\n",methodsArray[k].code);
			}
			printf("Listener: %s\n",listenerCode);
			printf("Parsing procedure done.\n");
			printf("Composing the %s file...\n",fileout_ext);
				inoArduino = fopen(pathIno,"w");
					fprintf(inoArduino,"#include \"%s.h\"\nString oggetto;\nString proprieta;\nString valore;\nString content;\n\n",fileout);
					fprintf(inoArduino,"Output* %sPointer;\n",objectName);
					fprintf(inoArduino,"Output %s[%d];\n\n",objectName,pinsNumber);
					fprintf(inoArduino,"void serialEvent(){\n\tcontent=Serial.readStringUntil(\':\');\n\tif(content!=\"\"){\n\t\toggetto=content;\n\t\tif(oggetto==\"%s\"){\n\t\t\tcontent=Serial.readStringUntil(\':\');\n\t\t\tproprieta=content;\n\t\t\tcontent=Serial.readStringUntil(\':\');\n\t\t\tvalore=content;\n\t\t} else {\n\t\t\tSerial.read();\n\t\t}\n\t}\n}\n\n",objectId);
					for(k=0;k<variableNumber;k++){
						fprintf(inoArduino,"%s %s = %s;\n",variableArray[k].type, variableArray[k].name, variableArray[k].value);
					}
					//fprintf(inoArduino, "\nvoid listener(){%s\n}\n\n",listenerCode);
					fprintf(inoArduino,"void setup(){\n\tSerial.begin(9600);\n\t%sPointer=%s;\n\t",objectName,objectName);
					
					for(k=0;k<pinsNumber;k++){
						fprintf(inoArduino,"%s[%d].pin = %s;\n\t",objectName,k,pinsArray[k].number);
						fprintf(inoArduino,"%s[%d].valore = %s;\n\t",objectName,k,pinsArray[k].value);
						fprintf(inoArduino,"pinMode(%s[%d].pin, %s);\n\t",objectName,k,pinsArray[k].mode);
					}
					fprintf(inoArduino, "}\n\n void loop() {\n");
						for(k=0;k<methodsNumber;k++){	
						fprintf(inoArduino, "\tif (proprieta==\"%s\"){\n\t%sPointer=%s(%sPointer,valore);\n\t}\n\n",methodsArray[k].name,objectName,methodsArray[k].name,objectName);
					}
					//fprintf(inoArduino, "listener();\n}\n\n");
					fprintf(inoArduino, "}\n\n");

			fclose(inoArduino);

			printf("Composing the %s file...\n",fileout2_ext);
				headerArduino = fopen(pathHeader,"w");
					fprintf(headerArduino,"#ifndef apio_included\n#define apio_included\n#include <Arduino.h>\nstruct output{\n\tint pin;\n\tint valore;\n};\ntypedef struct output Output;\n\nstruct input{\n\tint pin;\n\tint valore;\n};\ntypedef struct input Input;\n");
					for(k=0;k<methodsNumber;k++){	
						fprintf(headerArduino, "Output * %s(Output * %s, String valore){\n\t%s\nreturn %s;\n}",methodsArray[k].name,objectName,methodsArray[k].code,objectName);
						fprintf(headerArduino, "\n\n");
					}
					fprintf(headerArduino, "\n#endif");
			fclose(headerArduino);

			printf("Composing the %s file...\n","Makefile");
				makeArduino = fopen(pathMake,"w");
					fprintf(makeArduino, "BOARD_TAG=uno\nARDUINO_PORT=/dev/ttyACM0\nARDUINO_LIBS=\nARDUINO_DIR=/usrshare/arduino\ninclude %s.mk",fileout);
				fclose(makeArduino);
			
		}
	}

}

int newfile(char *fn)
{
	FILE *f = fopen(fn, "r");
	struct bufstack *bs = malloc(sizeof(struct bufstack));

	/*die if no file or no room */
	if(!f) { perror(fn); return 0; }
	if(!bs) { perror("malloc"); exit(1); }

	/* remember state */
	if(curbs)curbs->lineno = yylineno;
	bs->prev = curbs;

	/* set up current entry */
	bs->bs = yy_create_buffer(f, YY_BUF_SIZE);
	bs->f = f;
	bs->filename = fn;
	yy_switch_to_buffer(bs->bs);
	curbs = bs;
	yylineno = 1;
	curfilename = fn;
	return 1;
}


yyerror(char *s)
{
    fprintf(stderr, "error: %s\n",s);
    //insert code for the interrupt of the execution of the parsing
    //destroy the document already made
    //send a message to the user
    printf("PorcaMadonna\n");
}


int switchTheTag(char *sval)
{
	int ival;
		if(!strcmp(sval,"object"))
			{
				//if $2 it's equal to "..."
				ival=100;
			}
			else if(!strcmp(sval,"name"))
			{
				ival=2;
			}
			else if(!strcmp(sval,"id"))
			{
				ival=3;
			}
			else if(!strcmp(sval,"pins"))
			{
				ival=400;
			}
			else if(!strcmp(sval,"pin"))
			{
				ival=5;
				//founded a new pin tag so I extend the pinsArray structure
				pinsNumber++;
				pinsArray=(pin *)realloc(pinsArray, sizeof(pin)*(pinsNumber));
			}
			else if(!strcmp(sval,"number"))
			{
				//pin number
				ival=10;

			}
			else if(!strcmp(sval,"value"))
			{
				//pin value
				ival=11;
			}
			else if(!strcmp(sval,"mode"))
			{
				//pin mode
				ival=12;
			}
			else if(!strcmp(sval,"variables"))
			{
				ival=700; //excape not used
			}
			else if(!strcmp(sval,"variable"))
			{
				ival=13;
				//founded a new variable tag so I extend the variableArray structure
				variableNumber++;
				variableArray=(variable *)realloc(variableArray, sizeof(variable)*(variableNumber));
			}
			else if(!strcmp(sval,"variabletype"))
			{
				ival=14;

			}
			else if(!strcmp(sval,"variablename"))
			{
				ival=15;
			}
			else if(!strcmp(sval,"variablevalue"))
			{
				ival=16;
			}
			else if(!strcmp(sval,"methods"))
			{
				ival=600;
			}
			else if(!strcmp(sval,"method"))
			{
				ival=7;
				newLen=0;
				//founded a new pin tag so I extend the pinsArray structure
				methodsNumber++;
				methodsArray=(method *)realloc(methodsArray, sizeof(method)*(methodsNumber));
				methodsArray[methodsNumber-1].code=(char *) malloc(1);
			}
			else if(!strcmp(sval,"methodname"))
			{
				ival=8;
			}
			else if(!strcmp(sval,"code"))
			{
				ival=9;
			}
			else
			{
				ival=-1;
			}
		return ival;
}

void execute_vfgen(char *vfgen_filename)
{/*
	printf("Execution of VFGEN\nFile in analisys:%s\n",vfgen_filename);
	char *command;
	
	//./vfgen gsl:func=yes,demo=yes vfgen_filename
	//./vfgen gsl vfgen_filename
	//http://www.warrenweckesser.net/vfgen/menu_gsl.html
	command=(char *) malloc(strlen(vfgen_filename)+12);
	strcpy(command, "./vfgen gsl ");
	strcat(command,vfgen_filename);
	system(command);
*/
}