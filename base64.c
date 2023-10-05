/*
    Copyright (c) 2023 JiayuTu

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
#include<stdio.h>
#include<stdlib.h>		// for malloc
#include<string.h>		// strcmp()
#include<errno.h>
#include"base64.h"


extern int errno;


unsigned char* base64EncodeStr(unsigned char str[])
{
	// str_len: the length of string before encode
	// encode_len: the length of string after encode
	unsigned long str_len, encode_len;
	
	str_len = strlen(str);
	encode_len = (str_len + 2) / 3 * 4;

	unsigned char* encode;
	encode = (unsigned char*)malloc(sizeof(unsigned char) * encode_len);
	encode[encode_len] = '\0';

	int i, j;

	for (i = 0, j = 0; i < encode_len - 2; i += 4, j += 3) {
		// get the first byte after encode
		encode[i] = encodeTable[str[j] >> 2];
		// get the second byte after encode
		encode[i + 1] = encodeTable[(str[j] & 0x3) << 4 | str[j + 1] >> 4];	
		// get the third byte after encode
		encode[i + 2] = encodeTable[(str[j + 1] & 0xf) << 2 | str[j + 2] >> 6];
		// get the fourth byte after encode
		encode[i + 3] = encodeTable[str[j + 2] & 0x3f];
	}

	// replenish '='
	if (str_len % 3 == 1) {
		encode[encode_len - 2] = '=';
		encode[encode_len - 1] = '=';
	}
	else if (str_len % 3 == 2) {
		encode[encode_len - 1] = '=';
	}


	return encode;
}


// get index from table
// code： the code want to get 
// index： the index of code in string,for error 
// return: index in table，if none return '*' and '=' is 64
char getIndexfromTable(char code,int index)
{
	if(code >= 65 && code<=90){
		code -= 65;
	}else if(code >= 97 && code <= 122){
		code -= 71;
	}else if(code >= 48 && code <= 57){
		code += 4;
	}else if(code == '+'){
		code += 19;
	}else if(code == '/'){
		code += 16;
	}else if(code == '='){
		code = 64;
	}
	else{
		// if code not in table,return '*' and echo error
		fprintf(stderr,"Characters that cannot be processed: index:%d content:%c\n",index+1,code);
		return '*';
	}
	return code;
}

unsigned char*base64DecodeStr(unsigned char code[])
{
	// str_len： the length of string before decode 
	// code_len: the length of string after decode
    unsigned long str_len,code_len;
    code_len = strlen(code);
	
	// check if it a multiple of 4(string after encode must be multiple of 4)
	// first two bits is '00' if it a multiple of 4,if not,return error
	if((code_len & 0x03) != 0){
		fprintf(stderr,"Length Error!");
		return "";
	}

    str_len = code_len / 4 * 3;
    
    unsigned char* decode;
    decode = (unsigned char*)malloc(sizeof(unsigned char) * code_len);
    decode[code_len] = '\0';
    
	// get the number of '='
	int EqualsignNum = 0;

	if(code[code_len -1] == '='){
		EqualsignNum += 1;
	}
	if (code[code_len - 2] == '='){
		EqualsignNum += 1;
	}

    int i,j;

    for(i = 0,j = 0;i < code_len - 3;i += 3,j += 4){	
		for(int t=j;t<=j+3;t++){
			if(getIndexfromTable(code[t],t) != '*'){
				code[t] = getIndexfromTable(code[t],t);
			}else{				
				return "";
			}
		}
		
		decode[i] = code[j] << 2 | code[j+1] >> 4;
		if(i+1 == str_len -2 && EqualsignNum == 2){
			decode[i+1] = ' ';
		}else{
			decode[i+1] = code[j+1] << 4 | (code[j+2]& 0x3c) >> 2;
		}			

		if(i+2 == str_len - 1 && EqualsignNum != 0 ){
			decode[i+2] = ' ';
		}else{
			decode[i+2] = (code[j+2] & 0x3) << 6 | code[j+3];
		}	
    }

    return decode;
}