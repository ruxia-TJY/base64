#include<stdio.h>
#include<stdlib.h>		// �ڴ�
#include<string.h>		// strcmp()

// base64�����
unsigned char* encodeTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// ���ַ�������base64����
// str�� Ҫ���м��ܵ��ַ���
unsigned char* base64EncodeStr(unsigned char str[])
{
	// str_len,Ҫ������ַ�������
	// encode_len,���ܺ���ַ�������
	unsigned long str_len, encode_len;
	
	str_len = strlen(str);
	// ������ܺ���ַ�������
	// ���ַ������� + 2�� / 3������ȡ��һ��������ȡ����
	encode_len = (str_len + 2) / 3 * 4;

	//�����ڴ�
	unsigned char* encode;
	encode = (unsigned char*)malloc(sizeof(unsigned char) * encode_len);
	encode[encode_len] = '\0';

	// ���б���
	int i, j;
	// ѭ��ÿ�����ֽ�ִ�в���
	for (i = 0, j = 0; i < encode_len - 2; i += 4, j += 3) {
		// �˴�����λ����
		// �ַ�����һ���ֽ�������λ����λ����
		// ��ȡ���ܺ�ĵ�һ���ֽ�        
		encode[i] = encodeTable[str[j] >> 2];		
		// ���ַ�����һ���ֽ���0x3(0000 0011)ִ�����������ȡ��һ���ֽڵĵ���λ
		// �ڶ����ֽ�������λ��ִ�л����
		// ��ȡ���ܺ�ڶ����ֽ�
		encode[i + 1] = encodeTable[(str[j] & 0x3) << 4 | str[j + 1] >> 4];		
		// ���ַ����ڶ����ֽ���0xf(0000 1111)ִ�����������ȡ�ڶ����ֽڵĵ���λ
		// �������ֽ�������λ��ִ�л����
		// ��ȡ���ܺ�ĵ������ֽ�
		encode[i + 2] = encodeTable[(str[j + 1] & 0xf) << 2 | str[j + 2] >> 6];
		// ���ַ����������ֽ���0x3f(0011 1111)ִ�����������ȡ�������ֽڵĵ���λ
		// ��ȡ���ܺ���ĸ��ֽ�
		encode[i + 3] = encodeTable[str[j + 2] & 0x3f];
	}

	// ��=
	if (str_len % 3 == 1) {
		encode[encode_len - 2] = '=';
		encode[encode_len - 1] = '=';
	}
	else if (str_len % 3 == 2) {
		encode[encode_len - 1] = '=';
	}


	return encode;
}


// ���ַ�ת�ɱ�������λ�ò���������ֵ�����ڷ���*��=�ŷ���64
// code�� Ҫת�����ַ�
// index�� �ڱ�������λ��
// return: ����ֵ�����ڷ���*��=�ŷ���64
char changeToArr(char code,int index)
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
	}else{
		printf("�������ַ�!%dλ,%c\n",index+1,code);
		return '*';
	}
	return code;
}

// ��base64�������
// code: Ҫ���н��ܵ��ַ���
unsigned char*base64DecodeStr(unsigned char code[])
{
	// str_len�� ������ַ����ĳ���
	// code_len: ����ĳ���
    unsigned long str_len,code_len;
    code_len = strlen(code);
    str_len = code_len / 4 * 3;
    
    // �����ڴ�
    unsigned char* decode;
    decode = (unsigned char*)malloc(sizeof(unsigned char) * code_len);
    decode[code_len] = '\0';
    
	// ���ںŵĸ���
	int EqualsignNum = 0;

	if(code[code_len -1] == '='){
		EqualsignNum += 1;
	}
	if (code[code_len - 2] == '='){
		EqualsignNum += 1;
	}

    // ���н���
    int i,j;

    for(i = 0,j = 0;i < code_len - 3;i += 3,j += 4){
		// ����		
		for(int t=j;t<=j+3;t++){
			if(changeToArr(code[t],t) != '*'){
				code[t] = changeToArr(code[t],t);
			}else{				
				return "������";
			}
		}
		
		decode[i] = code[j] << 2 | code[j+1] >> 6;			
		
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