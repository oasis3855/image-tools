
#pragma once

#define MAX_EXIF_ERR_MESSAGE_SIZE	256

// EXIF データ 格納用構造体
struct exif_data{
	UINT App1Size;
	unsigned short IfdDirCount;
	unsigned short SubIfdDirCount;
	// IFD
	char ImageDescription[256];		// 0x010e
	char Make[256];					// 0x010f
	char Model[256];				// 0x0110
	unsigned short Orientation;		// 0x0112
	char Software[256];				// 0x0131
	char DateTime[21];				// 0x0132
	char Copyright[256];			// 0x8298
	//Sub IFD
	double ExposureTime;			// 0x829a
	double FNumber;					// 0x829d
	unsigned short ISOSpeedRatings;	// 0x8827
	unsigned short ExposureProgram;	// 0x8822
	char ExifVersion[6];			// 0x9000
	char DateTimeOriginal[21];		// 0x9003
	char DateTimeDigitized[21];		// 0x9004
	unsigned long ExifImageWidth;	// 0xa002
	unsigned long ExifImageHeight;	// 0xa003
};

// ファイル名 データ 格納用構造体（ソート用）
struct _fstr{
	char str[_MAX_PATH];
};


void init_exif_data_struct(struct exif_data *dat);
BOOL read_exif_data(CString strPathOnly, CString strFilename, struct exif_data *dat, char *strMessage);
BOOL CheckBinaryData(char *data1, char *data2, long size);
void SortArray(struct _fstr *fstrPathArray, int nFileCount, int nMode);
unsigned short int ConvEndianWord(unsigned short int nWord);
unsigned int ConvEndianDWord(unsigned int nDWord);
