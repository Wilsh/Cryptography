/*******************************************************************************
* Course:      CSC555
* Assignment:  #1
* Author:      Andrew Seligman
* Date:        February 6, 2014
* File:        decrypt.cpp
* Purpose:     This program uses a partial key to decrypt the ciphertext 
*				contained within a file (encrypted using a monoalphabetic
*				cipher). After the program reads in the ciphertext and 
*				capitalizes all of the letters, the user is asked to enter a key
*				which will be used to decrypt the ciphertext. A lowercase letter
*				should be entered below a known mapped letter, and any unknown
*				mappings should be entered with '*'. The program will then 
*				display the decoded cyphertext after making the appropriate
*				substitutions.
*******************************************************************************/

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

bool openFile(ifstream&);
void capitalize(ifstream&, string&);
void decrypt(string&, string);

int main()
{
	string message, key;
	ifstream inf;

	int totalLetters = 0;
	
	if(openFile(inf))
	{
		//store file data in string and capitalize letters
		capitalize(inf, message);
	
		//get key from user
		cout << "Enter key:\nABCDEFGHIJKLMNOPQRSTUVWXYZ" << endl;
		cin >> key;
		
		//use key to decrypt cyphertext
		decrypt(message, key);
		
		//print (partially) decoded cyphertext
		cout << message << endl;
	}

	return 0;
}

/*******************************************************************************
* Function name:  openFile
*                                                                             
* Description:    Ask for a file name and attempt to open it
*                                                                             
* Parameters:     ifstream& inf - EXPORT - stream of file contents
*                                                                             
* Return Value:   true or false
*******************************************************************************/
bool openFile(ifstream& inf)
{
	string file;
	
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
* Function name:  capitalize
*                                                                             
* Description:    Convert file stream to string and capitalize letters
*                                                                             
* Parameters:     ifstream& inf   - IMPORT - stream of file contents
*				  string& message - EXPORT - cyphertext to decrypt
*                                                                             
* Return Value:   none
*******************************************************************************/
void capitalize(ifstream& inf, string& message)
{
	char character;
	
	character = inf.get();
	while(!inf.eof())
	{
		message += toupper(character);
		character = inf.get();
	}
}

/*******************************************************************************
* Function name:  decrypt
*                                                                             
* Description:    Use the given key to decrypt ciphertext
*                                                                             
* Parameters:     string& message - IMPORT - cyphertext to be decrypted
*				  string key      - IMPORT - partial key to decrypt cyphertext
*                                                                             
* Return Value:   none
*******************************************************************************/
void decrypt(string& message, string key)
{
	string::iterator msgItr;
	char alpha = 'A';
	
	//loop through each letter
	for(int idx = 0; idx < 26; idx++)
	{
		//check if a letter is mapped
		if(isalpha(key[idx]))
		{
			//replace mapped letter using the key
			for (msgItr = message.begin(); msgItr != message.end(); msgItr++) 
			{
				if(*msgItr == alpha)
				{
					*msgItr = key[idx];
				}
			}
		}
		
		alpha++;
	}	
}
