#ifndef BASE64_H__
#define BASE64_H__

// 对字符串进行base64加密
// str： 要进行加密的字符串
unsigned char* base64EncodeStr(unsigned char str[]);

// 对base64编码解密
// code: 要进行解密的字符串
unsigned char*base64DecodeStr(unsigned char code[]);

#endif