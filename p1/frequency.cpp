/*******************************************************************************
* Course:      CSC555
* Assignment:  #1
* Author:      Andrew Seligman
* Date:        February 6, 2014
* File:        frequency.cpp
* Purpose:     This program performs a frequency analysis on the text
*				contained within a file. The user is prompted for the name of
*				the file containing the text, and the frequency of the
*				occurence of each letter is then displayed. The text is
*				assumed to be all lowercase letters.
*******************************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <iomanip>

using namespace std;

bool openFile(string&, ifstream&);
void initializeMap(map<char, int>&);
int populateMap(ifstream&, map<char, int>&);
void printMap(string, map<char, int>&, int);

int main()
{
	string file;
	ifstream inf;
	//map of letters and the number of times they occur
	map<char, int> LetterCount;
	int totalLetters = 0;
	
	if(openFile(file, inf))
	{
		//set up map
		initializeMap(LetterCount);
		//parse text
		totalLetters = populateMap(inf, LetterCount);
		//output letter frequencies
		printMap(file, LetterCount, totalLetters);
	}

	return 0;
}

/*******************************************************************************
* Function name:  openFile
*                                                                             
* Description:    Ask for a file name and attempt to open it
*                                                                             
* Parameters:     string& file  - EXPORT - file containing ciphertext
*				  ifstream& inf - EXPORT - stream of file contents
*                                                                             
* Return Value:   true or false
*******************************************************************************/
bool openFile(string& file, ifstream& inf)
{
	cout << "Enter file name > ";
	cin >> file;
	
	inf.close();
	inf.clear();
	inf.open(file.c_str());
	
	if(!inf)
	{
		cout << "Error opening the file " << file << endl
			<< "The program will now quit." << endl;
	
		return false;
	}
	
	return true;
}

/*******************************************************************************
* Function name:  initializeMap
*                                                                             
* Description:    Add characters a-z to the map and initialize count to zero
*                                                                             
* Parameters:     map<char, int>& Letters - EXPORT - map of letters
*                                                                             
* Return Value:   none
*******************************************************************************/
void initializeMap(map<char, int>& Letters)
{	
	for(char alpha = 'a'; alpha <= 'z'; alpha++)
	{
		Letters[alpha] = 0;
	}
}

/*******************************************************************************
* Function name:  populateMap
*                                                                             
* Description:    Examine each character in the ciphertext and determine how
*					many times a letter occurs
*                                                                             
* Parameters:     ifstream& inf           - IMPORT - stream of file contents
*				  map<char, int>& Letters - IMPORT/EXPORT - map of letters
*                                                                             
* Return Value:   total count of letters in the ciphertext
*******************************************************************************/
int populateMap(ifstream& inf, map<char, int>& Letters)
{
	char character;
	int totalLetters = 0;
	
	character = inf.get();
	while(!inf.eof())
	{
		//check if character is a lowercase letter
		if(Letters.find(character) != Letters.end())
		{
			//increment count for that letter and the total letter count
			Letters[character]++;
			totalLetters++;
		}
		character = inf.get();
	}
	
	return totalLetters;
}

/*******************************************************************************
* Function name:  printMap
*                                                                             
* Description:    print a list of characters and their relative freqencies
*                                                                             
* Parameters:     string& file            - IMPORT - file containing ciphertext
*				  map<char, int>& Letters - IMPORT - map of letters
*				  int totalLetters        - IMPORT - total count of letters
*                                                                             
* Return Value:   none
*******************************************************************************/
void printMap(string file, map<char, int>& Letters, int totalLetters)
{
	map<char, int>::iterator mapItr;
	
	cout << "Frequency analysis for " << file << endl
		<< "===============================" << endl;
	
	for (mapItr = Letters.begin(); mapItr != Letters.end(); mapItr++) 
	{
		cout << "(" << mapItr->first << ", "  << setprecision(4) << fixed
			<< mapItr->second/double(totalLetters) << ")" << endl;
	}
}
