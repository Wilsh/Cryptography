/*******************************************************************************
* Course:      CSC555
* Assignment:  #2
* Author:      Andrew Seligman
* Date:        February 27, 2014
* File:        simple-des.cpp
* Purpose:     This program simulates the encryption process of simple-DES. The
*               user is prompted for a 10-bit binary key and a string to 
*               encrypt. The program will output the two round keys and the
*               generated ciphertext.
*******************************************************************************/

#include <iostream>
#include <string>
#include <sstream>
#include <bitset>

using namespace std;

void generateKeys(string, bitset<8>&, bitset<8>&);
void encrypt(string, bitset<8>, bitset<8>);
bitset<4> f_k(bitset<8>, bitset<4>, bitset<4>);

int main()
{
    bitset<8> RoundKey1, RoundKey2;
    string enteredKey, plaintext;
   
    cout << "Enter 10-bit key:  ";
    cin >> enteredKey;
    
    cout << "Enter plaintext:   ";
    cin.get(); //eat newline in the stream
    getline(cin, plaintext);
    
    //generate the round keys
    generateKeys(enteredKey, RoundKey1, RoundKey2);
    
    //encrypt the plaintext one character at a time and print results
    encrypt(plaintext, RoundKey1, RoundKey2);

	return 0;
}

/*******************************************************************************
* Function name:  generateKeys
*                                                                             
* Description:    Generate two 8-bit round keys using the given 10-bit key
*                                                                             
* Parameters:     string enteredKey    - IMPORT - 10-bit key entered by user
*				  bitset<8>& RoundKey1 - EXPORT - the first round key used by 
*                   the encryption process
*                 bitset<8>& RoundKey2 - EXPORT - the second round key used by 
*                   the encryption process
*                                                                             
* Return Value:   none
*******************************************************************************/
void generateKeys(string enteredKey, bitset<8>& RoundKey1, bitset<8>& RoundKey2)
{
    bitset<10> Key, P10;
    int bitHolder1, bitHolder2;
    
    Key = bitset<10>(string(enteredKey));
    
    //permute 10-bit key
    P10[9] = Key[7];
    P10[8] = Key[5];
    P10[7] = Key[8];
    P10[6] = Key[3];
    P10[5] = Key[6];
    P10[4] = Key[0];
    P10[3] = Key[9];
    P10[2] = Key[1];
    P10[1] = Key[2];
    P10[0] = Key[4];
    
    //circular left shift (one position)
    bitHolder1 = P10[9];
    P10<<=1;
    P10[0] = P10[5];
    P10[5] = bitHolder1;

    //permute round key 1
    RoundKey1[7] = P10[4];
    RoundKey1[6] = P10[7];
    RoundKey1[5] = P10[3];
    RoundKey1[4] = P10[6];
    RoundKey1[3] = P10[2];
    RoundKey1[2] = P10[5];
    RoundKey1[1] = P10[0];
    RoundKey1[0] = P10[1];
    
    cout << "Round Key 1:  " << RoundKey1 << endl;
    
    //circular left shift again (two positions)
    bitHolder1 = P10[9];
    bitHolder2 = P10[8];
    P10<<=2;
    P10[0] = P10[5];
    P10[1] = P10[6];
    P10[6] = bitHolder1;
    P10[5] = bitHolder2;
    
    //permute round key 2
    RoundKey2[7] = P10[4];
    RoundKey2[6] = P10[7];
    RoundKey2[5] = P10[3];
    RoundKey2[4] = P10[6];
    RoundKey2[3] = P10[2];
    RoundKey2[2] = P10[5];
    RoundKey2[1] = P10[0];
    RoundKey2[0] = P10[1];
    
    cout << "Round Key 2:  " << RoundKey2 << endl;
}

/*******************************************************************************
* Function name:  encrypt
*                                                                             
* Description:    Use the generated round keys to encrypt the given plaintext
*                   one character at a time
*                                                                             
* Parameters:     string plaintext    - IMPORT - plaintext entered by user
*                 bitset<8> RoundKey1 - IMPORT - the first round key used by 
*                   the encryption process
*                 bitset<8> RoundKey2 - IMPORT - the second round key used by 
*                   the encryption process
*                                                                             
* Return Value:   none
*******************************************************************************/
void encrypt(string plaintext, bitset<8> RoundKey1, bitset<8> RoundKey2)
{	
    bitset<8> character, IP, IP_1;
    bitset<4> R, L;
    
    cout << "Ciphertext:   ";
    
    for(int idx = 0; idx < plaintext.length(); idx++)
    {
        character = bitset<8>((int)plaintext[idx]);

        //IP
        IP[7] = character[6];
        IP[6] = character[2];
        IP[5] = character[5];
        IP[4] = character[7];
        IP[3] = character[4];
        IP[2] = character[0];
        IP[1] = character[3];
        IP[0] = character[1];

        //separate IP into L and R
        L[0] = IP[4];
        L[1] = IP[5];
        L[2] = IP[6];
        L[3] = IP[7];
        
        R[0] = IP[0];
        R[1] = IP[1];
        R[2] = IP[2];
        R[3] = IP[3];
        
        //first round of f_k
        L = f_k(RoundKey1, R, L);
        
        //swap and second round of f_k
        R = f_k(RoundKey2, L, R);
        
        //IP^(-1)
        IP_1[7] = R[0];
        IP_1[6] = R[3];
        IP_1[5] = R[1];
        IP_1[4] = L[3];
        IP_1[3] = L[1];
        IP_1[2] = R[2];
        IP_1[1] = L[0];
        IP_1[0] = L[2];
        
        cout << IP_1 << " ";
    }
    
    cout << endl;
}

/*******************************************************************************
* Function name:  f_k
*                                                                             
* Description:    Perform permutation and substitution operations with a round 
*                   key
*                                                                             
* Parameters:     bitset<8> RoundKey - IMPORT - the round key used by the 
*                   encryption function
*                 bitset<4> R        - IMPORT - the four bits on the right used
*                   by the function
*                 bitset<4> L        - IMPORT - the four bits on the left used
*                   by the function
*                                                                             
* Return Value:   4-bit result of the operations
*******************************************************************************/
bitset<4> f_k(bitset<8> RoundKey, bitset<4> R, bitset<4> L)
{
    bitset<8> EP;
    bitset<4> S0, S1, SBoxOut, P4;
    
    string SBox0[4][4] = {
        {"01", "00", "11", "10"},
        {"11", "10", "01", "00"},
        {"00", "10", "01", "11"},
        {"11", "01", "11", "10"}};
        
    string SBox1[4][4] = {
        {"00", "01", "10", "11"},
        {"10", "00", "01", "11"},
        {"11", "00", "01", "00"},
        {"10", "01", "00", "11"}};
    
    // E/P
    EP[7] = R[0];
    EP[6] = R[3];
    EP[5] = R[2];
    EP[4] = R[1];
    EP[3] = R[2];
    EP[2] = R[1];
    EP[1] = R[0];
    EP[0] = R[3];
    
    // E/P XOR Round Key
    EP ^= RoundKey;
    
    //separate EP into S0 and S1
    S0[0] = EP[4];
    S0[1] = EP[5];
    S0[2] = EP[6];
    S0[3] = EP[7];
    
    S1[0] = EP[0];
    S1[1] = EP[1];
    S1[2] = EP[2];
    S1[3] = EP[3];
    
    //S-Box lookups
    //S-Box 0
    int row1 = 0;
    if(S0[3] == 1)
        row1 = 2;
    if(S0[0] == 1)
        row1 += 1;
        
    int col1 = 0;
    if(S0[2] == 1)
        col1 = 2;
    if(S0[1] == 1)
        col1 += 1;
        
    //S-Box 1
    int row2 = 0;
    if(S1[3] == 1)
        row2 = 2;
    if(S1[0] == 1)
        row2 += 1;
        
    int col2 = 0;
    if(S1[2] == 1)
        col2 = 2;
    if(S1[1] == 1)
        col2 += 1;

    SBoxOut = bitset<4>(string(SBox0[row1][col1] + SBox1[row2][col2]));
    
    //P4
    P4[3] = SBoxOut[2];
    P4[2] = SBoxOut[0];
    P4[1] = SBoxOut[1];
    P4[0] = SBoxOut[3];
    
    //P4 XOR L
    P4 ^= L;
    
    return P4;
}
