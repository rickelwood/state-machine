/*/////////////////////////// Added 11-28-2015 ////////////////////////////
Rick Elwood
CISP 440
HW #14
Fall 2015
sm.cpp

Implemented function: generate_StateTable()
Edited functions: print_StateTable(), process()

takes a string of a's and b's and creates a state machine for them then 
a file for those strings using the state machine.
//////////////////////////////////////////////////////////////////////////
A state machine to find all occurrences of the string "abab"

input file "abab.txt":
aaaaaababbaaaababababaabaabbbbaabbabbabababbbbbbbabbababaaabaabbab

output:
aaaaaabab8baaaabab16ab18ab20aabaabbbbaabbabbabab40ab42bbbbbbabbabab55aaabaabbab

Dan Ross
April 2013
//////////////////////////////////////////////////////////////////////////*/
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
#define STATES 20    

typedef char state_machine[STATES][2];

char state = 0;
int c_count = 0;

state_machine nextState;

// print_StateTable() prints the state table, now accepts a string and state_machine
void print_StateTable(char * instr, state_machine nextState)
{
	//char instr[80] = "abab";
	std::cout << "s\ta\tb\n\n";

	int l = strlen(instr) + 1;
	for (int s = 0; s < l; s++) {
		std::cout << s << "\t";
		for (int ch = 0; ch < 2; ch++)
			std::cout << (int)nextState[s][ch] << "\t";
		std::cout << endl;
	}
}


/*
Compares first n characters of str1,
with last n characters of str2.

Returns 0 (false) if they match

Copyright (c) 1981 Dan Ross
*/
int strncmp_olap(char * str1, char * str2, int n)
{
	int i, j;
	int l1 = strlen(str1);
	int l2 = strlen(str2);

	for (i = 0, j = l2 - n; i < n; i++, j++)
		if (str1[i] != str2[j])
			return true;

	return false;
}
/*
Creates statemachines for strings of a's and b's up to length 20
*/
void generate_StateTable(char * search_string, state_machine & stateM)
{
	char got[80]; //running tally of the characters so far
	char tmp_got[80]; //temp string for comparing overlap
	got[0] = 0, tmp_got[0] = 0; // null terminator
	int ss_length = strlen(search_string); //get the length of the search string
	for (int i = 0; i <= ss_length; i++) {
		cout << "state " << i << " ";
		if (search_string[i] == 'a') { 
			strcpy_s(tmp_got, got); //copy the characters so far into the temp
			strcat_s(tmp_got, "b"); //since we know it's a add a b to temp to check overlap
			stateM[i][1] = 0; //initialize b at this state to zero before checking overlap
			for (int k = i; k >= 0; k--) {
				if (!strncmp_olap(search_string, tmp_got, k+1)) { //if it's a match set "go to" state to the overlap
					stateM[i][1] = k+1; //set where to go for the 'b' case
					break; // if we got here we found the largest overlap and we can leave
				} //end if
			}//end for
			strcat_s(got, "a"); //add an 'a' to our current characters
			stateM[i][0] = i + 1; //set the 'a' case to proceed to the next state
			cout <<  got << " goto " << (int)stateM[i][0] << "  " << tmp_got << " goto " << (int)stateM[i][1] << endl; //print it
		}//end if
		else if (search_string[i] == 'b') {
			strcpy_s(tmp_got, got);
			strcat_s(tmp_got, "a");
			stateM[i][0] = 0; //initialize b at this state to zero before checking overlap
			for (int k = i; k >= 0; k--) {
				if (!strncmp_olap(search_string, tmp_got, k + 1)) { //if it's a match set "go to" state to the overlap
					stateM[i][0] = k+1; //set where to go for the 'a' case
					break; // if we got here we found the largest overlap and we can leave
				}
			}
			strcat_s(got, "b"); //add an 'b' to our current characters
			stateM[i][1] = i + 1; //set the 'b' case to proceed to the next state
			cout << tmp_got << " goto " << (int)stateM[i][0] << "  " << got << " goto " << (int)stateM[i][1] << endl; //print it
		}
		if (i == ss_length) {  //are we on the final state? if so find out where to go for 'a' and 'b' cases
			strcpy_s(tmp_got, got);
			strcat_s(tmp_got, "a");
			stateM[i][0] = 0; //initialize b at this state to zero before checking overlap

			for (int k = i; k >= 0; k--) {
				if (!strncmp_olap(search_string, tmp_got, k + 1)) {
					stateM[i][0] = k + 1; // set the 'a' case for the final state
					break;
				}
			}
			cout << tmp_got << " goto " << (int)stateM[i][0] << "  "; //print it
			strcpy_s(tmp_got, got);
			strcat_s(tmp_got, "b");
			stateM[i][1] = 0; //initialize b at this state to zero before checking overlap
			for (int k = i; k >= 0; k--) {
				if (!strncmp_olap(search_string, tmp_got, k + 1)) {
					stateM[i][1] = k + 1; //set the 'b' case for the final state
					break;
				}
			}
			cout << tmp_got << " goto " << (int)stateM[i][1] << endl; //print it
		}
	}
}

// A State Machine
bool process(char ch, int string_length)
{
	// move to next state
	state = nextState[state][ch - 'a'];

	// output c_count if we are in the accept state
	if (state == string_length) {
		std::cout << c_count + 2 << ", ";  // +2 for notepad
		c_count++; //another ch bites the dust!
		return true;
	}	
	else {
		c_count++; //another ch bites the dust!
		return false;//
	} 
}

int main()
{
	int string_length = 0;
	char search_string[6][30] = { "abab", "abbbbbbaaaaaba", "abbbbaabbbbaaa", "aaaaaaabaaaaaaa", "aabbbababaaababbaaaa", "aaaabababaaaaabbaaaa" };
	char file_name[6][20] = { "abab.txt", "monkeydata.txt", "monkeydata.txt", "monkeydata.txt", "monkeydata.txt", "monkeydata.txt" };
	char ch;
	int search_count=0,l=0;
	c_count = 0;
	for (l = 0; l < 6; l++) { //iterate through all the search strings and files
		c_count = 0;
		search_count = 0;
		string_length = strlen(search_string[l]);
		cout << "Generating state table for " << search_string[l] << "..." << endl;
		generate_StateTable(search_string[l], nextState);  //generate the table
		cout << endl << "Complete state table for " << search_string[l] << "..." << endl;
		print_StateTable(search_string[l], nextState);	//print the table

		// open a file
		ifstream in(file_name[l]);
		if (!in) {
			std::cout << "Error opening file";
			return 1;
		}

		// read characters from file and process them
		cout << "The string " << search_string[l] << " was found at locations: ";
		while (in) {
			in.get(ch);
			if (in)
				if (process(ch, string_length))
					search_count++;
					//std::cout << c_count + 2 << ", ";  // +2 for notepad
		}
		cout << " in " << file_name[l] << " a total of " << search_count << " times" << endl << endl;
	} //end for loop
	getchar();
	return 0;
}
