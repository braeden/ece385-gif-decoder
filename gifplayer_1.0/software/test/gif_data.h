/*Hold static array of gif data for testing to avoid reflashing*/
/*
xxd -i -a sample1.gif
*/

//unsigned char _gif[] = {
//  0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x20, 0x00, 0x34, 0x00, 0xf0, 0xff,
//  0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x21, 0xf9, 0x04, 0x05, 0x00,
//  0x00, 0x02, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x34, 0x00,
//  0x00, 0x02, 0x78, 0x8c, 0x8f, 0xa9, 0xcb, 0x0b, 0x0f, 0xa3, 0x94, 0xed,
//  0xcc, 0x7b, 0xab, 0xc1, 0x1c, 0xea, 0xd0, 0x75, 0x5f, 0xc8, 0x8d, 0x64,
//  0xa6, 0x9d, 0x68, 0xa5, 0x4e, 0x66, 0xeb, 0xa5, 0x70, 0x2c, 0x36, 0x75,
//  0xcd, 0xdc, 0xa5, 0xbd, 0xe3, 0x4e, 0xbf, 0xcb, 0x01, 0x31, 0xac, 0xe1,
//  0xea, 0x47, 0x04, 0x05, 0x91, 0x28, 0x9f, 0x42, 0x97, 0x14, 0x26, 0x67,
//  0xa7, 0x0d, 0x35, 0x64, 0xbd, 0x1a, 0xb5, 0x2e, 0x25, 0xb7, 0xf9, 0x05,
//  0x87, 0x29, 0xde, 0x31, 0xcd, 0x1c, 0xc9, 0xa2, 0x01, 0x6a, 0x74, 0xdb,
//  0xfc, 0x1e, 0xc7, 0xc3, 0xf3, 0x6f, 0x9d, 0x7b, 0xd7, 0xe6, 0xaf, 0x7b,
//  0x6a, 0x7f, 0xf6, 0x07, 0x13, 0xd8, 0x32, 0xa8, 0x52, 0x58, 0x55, 0xe6,
//  0x96, 0x08, 0xb7, 0x28, 0xd7, 0x48, 0xf7, 0x68, 0x17, 0x89, 0xf7, 0x51,
//  0xb9, 0x50, 0x00, 0x00, 0x3b
//};
//unsigned int _gif_len = 161;

unsigned char _gif[] = {
  0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x0a, 0x00, 0x0a, 0x00, 0x91, 0x00,
  0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
  0x00, 0x21, 0xf9, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00,
  0x00, 0x00, 0x0a, 0x00, 0x0a, 0x00, 0x00, 0x02, 0x16, 0x8c, 0x2d, 0x99,
  0x87, 0x2a, 0x1c, 0xdc, 0x33, 0xa0, 0x02, 0x75, 0xec, 0x95, 0xfa, 0xa8,
  0xde, 0x60, 0x8c, 0x04, 0x91, 0x4c, 0x01, 0x00, 0x3b
};
unsigned int _gif_len = 69;
