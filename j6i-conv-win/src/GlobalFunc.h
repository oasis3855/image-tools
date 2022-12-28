
typedef struct headertag
{

	unsigned char head;				/* 00 */
	unsigned char dummy1[7-1+1];
	unsigned char ver_h, ver_l;		/* 08,09 */
	char dummy2[0x0f-0x0a+1];
	char spec[0x2f-0x10+1];			/* 10 - 2F */
	unsigned char attr;				/* 30 */
	unsigned char gamma;			/* 31 */
	unsigned char endian;			/* 32 */
	unsigned char dummy3[0x37-0x33+1];
	unsigned long startaddr;		/* 38,39,3A,3B */
	unsigned long endaddr;			/* 3C,3D,3E,3F */
	unsigned char enabledate;		/* 40 */
	unsigned char dummy4;
	unsigned char tz;				/* 42 */
	char dummy5;
	unsigned char date[0x49-0x44+1];/* 44-49 */
	char dummy6[0xab-0x4a+1];
}header;

unsigned int BCDtoINT(unsigned char bcd);

BOOL ConvertJ6I(LPCSTR SrcFname, LPCSTR DstPath, LPSTR Msg, HWND HWnd, int year, BOOL ovwr);
CString GetJ6iName(LPCSTR SrcFname, LPCSTR DstPath);
