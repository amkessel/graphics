/************************************
 *	Andrew Kessel
 *	CSCI 5229
 *
 *	ArgumentParser.h
 *
 *	Definition & implementation of a class that will parse and determine the values of command
 * 	arguments passed into a program (all in one file for easiest portability).
 *
 *************************************/

#ifndef ARGUMENTPARSER_H_
#define ARGUMENTPARSER_H_

#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

/*
 *	Parses and gets values from an argument list.
 */
class ArgumentParser
{
	private:
		// The number of arguments
		int argc;
		
		// The array of arguments
		char **argv;
		
		// Finds the index of an argument, -1 if it doesn't exist
		int FindArgument(const char *argument)
		{
			int index = -1;
	
			for(int i = 0; i < this->argc; i++)
			{
				if( strcmp(argument, this->argv[i]) == 0 )
				{
					index = i;
					break;
				}
			}
	
			return index;
		}
		
	public:
		// Constructor
		ArgumentParser(int argc, char **argv)
		{
			this->argc = argc;
			this->argv = argv;
		}
		
		// Determines whether an argument is present
		bool IsArgumentPresent(const char *argument)
				{
			int index = this->FindArgument(argument);

			bool found = (index >= 0);
	
			return found;
		}
		
		// Gets the value of an argument. Returns whether the argument
		// is present while the value is passed out through the pointer.
		bool GetArgumentValue(const char *argument, int *val)
		{
			int index = this->FindArgument(argument);
	
			bool found = false;
	
			// ensure that there is at least one more argument after this one
			if(index >= 0 && this->argc > index + 1)
			{
				*val = atoi(this->argv[index+1]);
				found = true;
			}
	
			return found;
		}
		
		// Gets the value of an argument. Returns whether the argument
		// is present while the value is passed out through the pointer.
		bool GetArgumentValue(const char *argument, double *val)
		{
			int index = this->FindArgument(argument);
	
			bool found = false;
	
			// ensure that there is at least one more argument after this one
			if(index >= 0 && this->argc > index + 1)
			{
				*val = atof(this->argv[index+1]);
				found = true;
			}
	
			return found;
		}
};
#endif
