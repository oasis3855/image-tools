#include "StdAfx.h"
#include "GlobalFunc.h"
#include "CopyTimeExif.h"


// exif_data �̏�����
void init_exif_data_struct(struct exif_data *dat)
{
	dat->App1Size = 0;
	dat->IfdDirCount = 0;
	dat->SubIfdDirCount = 0;
	// IFD
	strcpy(dat->ImageDescription, "");
	strcpy(dat->Make, "");
	strcpy(dat->Model, "");
	dat->Orientation = 0;
	strcpy(dat->Software, "");
	strcpy(dat->DateTime, "");
	strcpy(dat->Copyright, "");
	//Sub IFD
	dat->ExposureTime = 0;
	dat->FNumber = 0;
	dat->ISOSpeedRatings = 0;
	dat->ExposureProgram = 0;
	strcpy(dat->ExifVersion, "");
	strcpy(dat->DateTimeOriginal, "");
	strcpy(dat->DateTimeDigitized, "");
	dat->ExifImageWidth = 0;
	dat->ExifImageHeight = 0;
}

// �t�@�C������ EXIF �f�[�^��ǂݍ���
BOOL read_exif_data(CString strPathOnly, CString strFilename, struct exif_data *dat, char *strExifErrMessage)
{
	CString strFullPath;
	CString strTmp1;
	char datData[2048], datData2[2048], datTiffHeadIntel[10], datTiffHeadMotorola[10];
	unsigned short int wordData;
	unsigned int dwordData, dwordData2;
	FILE *in;
	int i;

	strcpy(strExifErrMessage, "");

	strFullPath = strPathOnly + strFilename;

	if((in = fopen((LPCSTR)strFullPath, "rb")) == NULL)
	{	// �t�@�C�����J���Ȃ��ꍇ
		strTmp1.LoadString(STR_FILE_CANNOT_OPEN_RB);
		strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
		return FALSE;
	}

	// CHECK JPEG MARKER
	if(fread(datData, 1, 2, in) < 2)
	{
		strTmp1.LoadString(STR_FILE_ABNORMAL_END);
		strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
		fclose(in);
		return FALSE;
	}
	datData2[0] = (char)0xff;
	datData2[1] = (char)0xd8;
	if(!CheckBinaryData(datData, datData2, 2))
	{
		strTmp1.LoadString(STR_EXIF_DATA_ERR_1);
		strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
		fclose(in);
		return FALSE;
	}


	// CHECK APP1, APP0 MARKER
	for(;;)
	{
		// CHECK EXIF HEADER
		if(fread(datData, 1, 2, in) < 2)
		{
			strTmp1.LoadString(STR_FILE_ABNORMAL_END);
			strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
			fclose(in);
			return FALSE;
		}
		datData2[0] = (char)0xff;
		datData2[1] = (char)0xe1;
		if(CheckBinaryData(datData, datData2, 2)) break; // EXIF APP1 �w�b�_�̏ꍇ
		else
		{	// EXIF APP1 �łȂ��ꍇ
			datData2[0] = (char)0xff;
			datData2[1] = (char)0xe0;
			if(CheckBinaryData(datData, datData2, 2))
			{
				// JFIF APP0 �w�b�_�̏ꍇ
				// APP0 �w�b�_�T�C�Y��ǂݍ���
				if(fread(&wordData, 2, 1, in) < 1)
				{	// �ǂݍ��ݕs�\�̂Ƃ�
					strTmp1.LoadString(STR_FILE_ABNORMAL_END);
					strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
					fclose(in);
					return FALSE;
				}
				// APP0 �w�b�_��ǂݔ�΂�
				fseek(in, ConvEndianWord(wordData)-2, SEEK_CUR);
				continue;
			}
			// APP1, APP0 �̂ǂ���ł��Ȃ��ꍇ�́A���ߕs�\
			strTmp1.LoadString(STR_EXIF_DATA_ERR_1);
			strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
			fclose(in);
			return FALSE;
		}
	}

	// APP1 SIZE
	if(fread(&wordData, 2, 1, in) < 1)
	{	// �ǂݍ��ݕs�\�̂Ƃ�
		strTmp1.LoadString(STR_FILE_ABNORMAL_END);
		strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
		fclose(in);
		return FALSE;
	}
	dat->App1Size = ConvEndianWord(wordData);

	// CHECK EXIF HEADER
	if(fread(datData, 1, 6, in) < 6)
	{
		strTmp1.LoadString(STR_FILE_ABNORMAL_END);
		strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
		fclose(in);
		return FALSE;
	}
	datData2[0] = (char)0x45;	// E
	datData2[1] = (char)0x78;	// x
	datData2[2] = (char)0x69;	// i
	datData2[3] = (char)0x66;	// f
	datData2[4] = (char)0;
	datData2[5] = (char)0;
	if(!CheckBinaryData(datData, datData2, 6))
	{	// EXIF �}�[�J�[�l���ُ�
		strTmp1.LoadString(STR_EXIF_DATA_ERR_2);
		strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
		fclose(in);
		return FALSE;
	}

	long tiff_filepoint;
	tiff_filepoint = ftell(in);

	// CHECK TIFF HEADER
	if(fread(datData, 1, 4, in) < 4)
	{
		strTmp1.LoadString(STR_FILE_ABNORMAL_END);
		strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
		fclose(in);
		return FALSE;
	}
	datTiffHeadIntel[0] = (char)0x49;	// I (Intel) , (if Motorola 'M')
	datTiffHeadIntel[1] = (char)0x49;	// I (Intel) , (if Motorola 'M')
	datTiffHeadIntel[2] = (char)0x2a;	// (if Motorola 0x00)
	datTiffHeadIntel[3] = (char)0x00;	// (if Motorola 0x2a)
	datTiffHeadIntel[4] = (char)0x08;	// size of Header (8) �i8�o�C�g�ȊO�̏ꍇ������j
	datTiffHeadIntel[5] = (char)0;
	datTiffHeadIntel[6] = (char)0;
	datTiffHeadIntel[7] = (char)0;

	datTiffHeadMotorola[0] = (char)0x4d;	// I (Intel) , (if Motorola 'M')
	datTiffHeadMotorola[1] = (char)0x4d;	// I (Intel) , (if Motorola 'M')
	datTiffHeadMotorola[2] = (char)0x00;	// (if Intel 0x2a)
	datTiffHeadMotorola[3] = (char)0x2a;	// (if Intel 0x00)
	datTiffHeadMotorola[4] = (char)0;		// size of Header (8)
	datTiffHeadMotorola[5] = (char)0;
	datTiffHeadMotorola[6] = (char)0;
	datTiffHeadMotorola[7] = (char)0x08;

	// Pharse IFD Data
	unsigned short int ifd_TagID, ifd_Format;
	unsigned int ifd_DataCount, ifd_Data;
	long cur_filepoint, ifd_SubPtr = 0;

	if(CheckBinaryData(datData, datTiffHeadIntel, 4))
	{
		/************************
		 INTEL �`���i���g���G���f�B�A���j�̏ꍇ��IFD�ǂݍ���
		************************/
		// TIFF�w�b�_�̑傫���𓾂�
		if(fread(&dwordData, 4, 1, in) < 1)
		{
			strTmp1.LoadString(STR_FILE_ABNORMAL_END);
			strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
			fclose(in);
			return FALSE;
		}
		// IFD�f�[�^�f�[�^�܂ŃV�[�N����
		fseek(in, dwordData-8, SEEK_CUR);

		// Exif IFD Directry Entries Count
		if(fread(&wordData, 2, 1, in) < 1)
		{
			strTmp1.LoadString(STR_FILE_ABNORMAL_END);
			strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
			fclose(in);
			return FALSE;
		}
		dat->IfdDirCount = wordData;



		cur_filepoint = ftell(in);

		for(i=0; i<dat->IfdDirCount; i++)
		{
			if(fseek(in, cur_filepoint, SEEK_SET))
			{
				strTmp1.LoadString(STR_FILE_ABNORMAL_END);
				strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
				fclose(in);
				return FALSE;
			}

			fread(&wordData, 2, 1, in);
			ifd_TagID = wordData;
			fread(&wordData, 2, 1, in);
			ifd_Format = wordData;
			fread(&dwordData, 4, 1, in);
			ifd_DataCount = dwordData;
			if(fread(&dwordData, 4, 1, in) < 1)
			{	// �ǂݍ��݃G���[
				strTmp1.LoadString(STR_FILE_ABNORMAL_END);
				strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
				fclose(in);
				return FALSE;
			}
			ifd_Data = dwordData;

			cur_filepoint = ftell(in);		// �t�@�C���|�C���^�̈ړ��ɔ����ĕۑ�

			// TagID�ɂ��������āA�f�[�^��ǂݏo���B
			// �f�[�^�I�t�Z�b�g�� "II" �̈ʒu����B(tiff_filepoint�ŕۑ��ς�)
			switch(ifd_TagID)
			{
			case 0x010e :
				if(ifd_Format != 2) break;
				fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
				fread(datData, 1, ifd_DataCount, in);
				datData[ifd_DataCount] = (char)0;
				strncpy(dat->ImageDescription, datData, 254);
				break;
			case 0x010f :
				if(ifd_Format != 2) break;
				fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
				fread(datData, 1, ifd_DataCount, in);
				datData[ifd_DataCount] = (char)0;
				strncpy(dat->Make, datData, 254);
				break;
			case 0x0110 :
				if(ifd_Format != 2) break;
				fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
				fread(datData, 1, ifd_DataCount, in);
				datData[ifd_DataCount] = (char)0;
				strncpy(dat->Model, datData, 254);
				break;
			case 0x0112 :
				if(ifd_Format != 3) break;
				dat->Orientation = ifd_Data;
				break;
			case 0x0131 :
				if(ifd_Format != 2) break;
				fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
				fread(datData, 1, ifd_DataCount, in);
				datData[ifd_DataCount] = (char)0;
				strncpy(dat->Software, datData, 254);
				break;
			case 0x0132 :
				if(ifd_Format != 2) break;
				fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
				fread(datData, 1, ifd_DataCount, in);
				datData[ifd_DataCount] = (char)0;
				strncpy(dat->DateTime, datData, 20);
				break;
			case 0x8298 :
				if(ifd_Format != 2) break;
				fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
				fread(datData, 1, ifd_DataCount, in);
				datData[ifd_DataCount] = (char)0;
				strncpy(dat->Copyright, datData, 20);
				break;
			case 0x8769 :		// Exif SUB IFD �����݂���ꍇ�A�|�C���^�𓾂�
				if(ifd_Format != 4) break;
				ifd_SubPtr = ifd_Data;
				break;
			}
		}

		// Sub IFD �̓ǂݍ���
		if(ifd_SubPtr)
		{
			if(fseek(in, tiff_filepoint + ifd_SubPtr, SEEK_SET))
			{
				strTmp1.LoadString(STR_FILE_ABNORMAL_END);
				strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
				fclose(in);
				return FALSE;
			}

			// Exif IFD Directry Entries Count
			fread(&wordData, 2, 1, in);
			dat->SubIfdDirCount = wordData;

			cur_filepoint = ftell(in);

			for(i=0; i<dat->SubIfdDirCount; i++)
			{
				fseek(in, cur_filepoint, SEEK_SET);

				fread(&wordData, 2, 1, in);
				ifd_TagID = wordData;
				fread(&wordData, 2, 1, in);
				ifd_Format = wordData;
				fread(&dwordData, 4, 1, in);
				ifd_DataCount = dwordData;
				if(fread(&dwordData, 4, 1, in) < 1)
				{	// �ǂݍ��݃G���[
					strTmp1.LoadString(STR_FILE_ABNORMAL_END);
					strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
					fclose(in);
					return FALSE;
				}
				ifd_Data = dwordData;

				cur_filepoint = ftell(in);		// �t�@�C���|�C���^�̈ړ��ɔ����ĕۑ�

				// TagID�ɂ��������āA�f�[�^��ǂݏo���B
				// �f�[�^�I�t�Z�b�g�� "II" �̈ʒu����B(tiff_filepoint�ŕۑ��ς�)

				switch(ifd_TagID)
				{
				case 0x829a :
					if(ifd_Format != 5) break;
					fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
					fread(&dwordData, 4, 1, in);	// ���q
					fread(&dwordData2, 4, 1, in);	// ����
					if(dwordData == 0) break;	// �[�����Z���
					dat->ExposureTime = (double)dwordData2 / (double)dwordData; // 1/n �� n ���i�[
					break;
				case 0x829d :
					if(ifd_Format != 5) break;
					fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
					fread(&dwordData, 4, 1, in);	// ���q
					fread(&dwordData2, 4, 1, in);	// ����
					if(dwordData2 == 0) break;	// �[�����Z���
					dat->FNumber = (double)dwordData / (double)dwordData2;
					break;
				case 0x8827 :
					if(ifd_Format != 3) break;
					dat->ISOSpeedRatings = ifd_Data;
					break;
				case 0x8822 :
					if(ifd_Format != 3) break;
					dat->ExposureProgram = ifd_Data;
					break;
				case 0x9000 :
					if(ifd_Format != 7) break;
					fseek(in, -4, SEEK_CUR);
					fread(datData, 1, 4, in);
					datData[4] = (char)0;
					strncpy(dat->ExifVersion, datData, 5);
					break;
				case 0x9003 :
					if(ifd_Format != 2) break;
					fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
					fread(datData, 1, ifd_DataCount, in);
					datData[ifd_DataCount] = (char)0;
					strncpy(dat->DateTimeOriginal, datData, 20);
					break;
				case 0x9004 :
					if(ifd_Format != 2) break;
					fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
					fread(datData, 1, ifd_DataCount, in);
					datData[ifd_DataCount] = (char)0;
					strncpy(dat->DateTimeDigitized, datData, 20);
					break;
				case 0xa002 :
					if(ifd_Format == 3) 
						dat->ExifImageWidth = (unsigned short)ifd_Data;
					if(ifd_Format == 4) 
						dat->ExifImageWidth = ifd_Data;
					break;
				case 0xa003 :
					if(ifd_Format == 3) 
						dat->ExifImageHeight = (unsigned short)ifd_Data;
					if(ifd_Format == 4) 
						dat->ExifImageHeight = ifd_Data;
					break;
				}
			}

		}

	}
	else if(CheckBinaryData(datData, datTiffHeadMotorola, 4))
	{
		/************************
		 Motorola �`���̏ꍇ��IFD�ǂݍ���
		************************/
		// TIFF�w�b�_�̑傫���𓾂�
		if(fread(&dwordData, 4, 1, in) < 1)
		{
			strTmp1.LoadString(STR_FILE_ABNORMAL_END);
			strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
			fclose(in);
			return FALSE;
		}
		// IFD�f�[�^�f�[�^�܂ŃV�[�N����
		fseek(in, ConvEndianDWord(dwordData)-8, SEEK_CUR);

		// Exif IFD Directry Entries Count
		if(fread(&wordData, 2, 1, in) < 1)
		{
			strTmp1.LoadString(STR_FILE_ABNORMAL_END);
			strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
			fclose(in);
			return FALSE;
		}
		dat->IfdDirCount = ConvEndianWord(wordData);



		cur_filepoint = ftell(in);

		for(i=0; i<dat->IfdDirCount; i++)
		{
			if(fseek(in, cur_filepoint, SEEK_SET))
			{
				strTmp1.LoadString(STR_FILE_ABNORMAL_END);
				strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
				fclose(in);
				return FALSE;
			}

			fread(&wordData, 2, 1, in);
			ifd_TagID = ConvEndianWord(wordData);
			fread(&wordData, 2, 1, in);
			ifd_Format = ConvEndianWord(wordData);
			fread(&dwordData, 4, 1, in);
			ifd_DataCount = ConvEndianDWord(dwordData);
			if(fread(&dwordData, 4, 1, in) < 1)
			{	// �ǂݍ��݃G���[
				strTmp1.LoadString(STR_FILE_ABNORMAL_END);
				strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
				fclose(in);
				return FALSE;
			}
			ifd_Data = ConvEndianDWord(dwordData);

			cur_filepoint = ftell(in);		// �t�@�C���|�C���^�̈ړ��ɔ����ĕۑ�

			// TagID�ɂ��������āA�f�[�^��ǂݏo���B
			// �f�[�^�I�t�Z�b�g�� "MM" �̈ʒu����B(tiff_filepoint�ŕۑ��ς�)
			switch(ifd_TagID)
			{
			case 0x010e :
				if(ifd_Format != 2) break;
				fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
				fread(datData, 1, ifd_DataCount, in);
				datData[ifd_DataCount] = (char)0;
				strncpy(dat->ImageDescription, datData, 254);
				break;
			case 0x010f :
				if(ifd_Format != 2) break;
				fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
				fread(datData, 1, ifd_DataCount, in);
				datData[ifd_DataCount] = (char)0;
				strncpy(dat->Make, datData, 254);
				break;
			case 0x0110 :
				if(ifd_Format != 2) break;
				fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
				fread(datData, 1, ifd_DataCount, in);
				datData[ifd_DataCount] = (char)0;
				strncpy(dat->Model, datData, 254);
				break;
			case 0x0112 :
				if(ifd_Format != 3) break;
				dat->Orientation = ConvEndianDWord(ifd_Data);
				break;
			case 0x0131 :
				if(ifd_Format != 2) break;
				fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
				fread(datData, 1, ifd_DataCount, in);
				datData[ifd_DataCount] = (char)0;
				strncpy(dat->Software, datData, 254);
				break;
			case 0x0132 :
				if(ifd_Format != 2) break;
				fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
				fread(datData, 1, ifd_DataCount, in);
				datData[ifd_DataCount] = (char)0;
				strncpy(dat->DateTime, datData, 20);
				break;
			case 0x8298 :
				if(ifd_Format != 2) break;
				fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
				fread(datData, 1, ifd_DataCount, in);
				datData[ifd_DataCount] = (char)0;
				strncpy(dat->Copyright, datData, 20);
				break;
			case 0x8769 :		// Exif SUB IFD �����݂���ꍇ�A�|�C���^�𓾂�
				if(ifd_Format != 4) break;
				ifd_SubPtr = ifd_Data;
				break;
			}
		}

		// Sub IFD �̓ǂݍ���
		if(ifd_SubPtr)
		{
			if(fseek(in, tiff_filepoint + ifd_SubPtr, SEEK_SET))
			{
				strTmp1.LoadString(STR_FILE_ABNORMAL_END);
				strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
				fclose(in);
				return FALSE;
			}

			// Exif IFD Directry Entries Count
			fread(&wordData, 2, 1, in);
			dat->SubIfdDirCount = ConvEndianWord(wordData);

			cur_filepoint = ftell(in);

			for(i=0; i<dat->SubIfdDirCount; i++)
			{
				fseek(in, cur_filepoint, SEEK_SET);

				fread(&wordData, 2, 1, in);
				ifd_TagID = ConvEndianWord(wordData);
				fread(&wordData, 2, 1, in);
				ifd_Format = ConvEndianWord(wordData);
				fread(&dwordData, 4, 1, in);
				ifd_DataCount = ConvEndianDWord(dwordData);
				if(fread(&dwordData, 4, 1, in) < 1)
				{	// �ǂݍ��݃G���[
					strTmp1.LoadString(STR_FILE_ABNORMAL_END);
					strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
					fclose(in);
					return FALSE;
				}
				ifd_Data = ConvEndianDWord(dwordData);

				cur_filepoint = ftell(in);		// �t�@�C���|�C���^�̈ړ��ɔ����ĕۑ�

				// TagID�ɂ��������āA�f�[�^��ǂݏo���B
				// �f�[�^�I�t�Z�b�g�� "MM" �̈ʒu����B(tiff_filepoint�ŕۑ��ς�)

				switch(ifd_TagID)
				{
				case 0x829a :
					if(ifd_Format != 5) break;
					fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
					fread(&dwordData, 4, 1, in);	// ���q
					fread(&dwordData2, 4, 1, in);	// ����
					if(dwordData == 0) break;	// �[�����Z���
					dat->ExposureTime = (double)ConvEndianDWord(dwordData2) / (double)ConvEndianDWord(dwordData); // 1/n �� n ���i�[
					break;
				case 0x829d :
					if(ifd_Format != 5) break;
					fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
					fread(&dwordData, 4, 1, in);	// ���q
					fread(&dwordData2, 4, 1, in);	// ����
					if(dwordData2 == 0) break;	// �[�����Z���
					dat->FNumber = (double)ConvEndianDWord(dwordData) / (double)ConvEndianDWord(dwordData2);
					break;
				case 0x8827 :
					if(ifd_Format != 3) break;
					dat->ISOSpeedRatings = ifd_Data;
					break;
				case 0x8822 :
					if(ifd_Format != 3) break;
					dat->ExposureProgram = ifd_Data;
					break;
				case 0x9000 :
					if(ifd_Format != 7) break;
					fseek(in, -4, SEEK_CUR);
					fread(datData, 1, 4, in);
					datData[4] = (char)0;
					strncpy(dat->ExifVersion, datData, 5);
					break;
				case 0x9003 :
					if(ifd_Format != 2) break;
					fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
					fread(datData, 1, ifd_DataCount, in);
					datData[ifd_DataCount] = (char)0;
					strncpy(dat->DateTimeOriginal, datData, 20);
					break;
				case 0x9004 :
					if(ifd_Format != 2) break;
					fseek(in, tiff_filepoint + ifd_Data, SEEK_SET);
					fread(datData, 1, ifd_DataCount, in);
					datData[ifd_DataCount] = (char)0;
					strncpy(dat->DateTimeDigitized, datData, 20);
					break;
				case 0xa002 :
					if(ifd_Format == 3) 
						dat->ExifImageWidth = (unsigned short)ifd_Data;
					if(ifd_Format == 4) 
						dat->ExifImageWidth = ifd_Data;
					break;
				case 0xa003 :
					if(ifd_Format == 3) 
						dat->ExifImageHeight = (unsigned short)ifd_Data;
					if(ifd_Format == 4) 
						dat->ExifImageHeight = ifd_Data;
					break;
				}
			}
		}

	}
	else
	{	// TIFF �}�[�J�[�l���ُ�
		strTmp1.LoadString(STR_EXIF_DATA_ERR_3);
		strncpy(strExifErrMessage, strTmp1, MAX_EXIF_ERR_MESSAGE_SIZE);
		fclose(in);
		return FALSE;
	}


	fclose(in);

	return TRUE;
}

// **********************************
// �o�C�i���f�[�^�ɕs��v������ꍇ�AFALSE ��Ԃ�
// **********************************
BOOL CheckBinaryData(char *data1, char *data2, long size)
{
	long i;

	for(i=0; i<size; i++)
	{
		if(data1[i] != data2[i]) return FALSE;
	}
	return TRUE;
}


// **********************************
// �\�[�e�B���O
// **********************************
void SortArray(_fstr *fstrPathArray, int nFileCount, int nMode)
{
	int i, j;
	CString strTmp;

	////////////////////////
	// �\�[�g���[�h��؂�ւ���
	////////////////////////
	if(nMode == 0)
	{	// �t�@�C�����Ń\�[�g�i�����j
		for(i=0; i<nFileCount; i++)
		{
			for(j=i; j<nFileCount; j++)
			{
				if(stricmp((const char*)fstrPathArray[i].str, (const char*)fstrPathArray[j].str) > 0)
				{
					// �t�@�C�����̌���
					strTmp = fstrPathArray[i].str;
					strcpy(fstrPathArray[i].str, fstrPathArray[j].str);
					strcpy(fstrPathArray[j].str, strTmp);
				}
			}
		}
	}
	else if(nMode == 1)
	{	// �t�@�C�����Ń\�[�g�i�~���j
		for(i=0; i<nFileCount; i++)
		{
			for(j=i; j<nFileCount; j++)
			{
				if(stricmp((const char*)fstrPathArray[i].str, (const char*)fstrPathArray[j].str) < 0)
				{
					// �t�@�C�����̌���
					strTmp = fstrPathArray[i].str;
					strcpy(fstrPathArray[i].str, fstrPathArray[j].str);
					strcpy(fstrPathArray[j].str, strTmp);
				}
			}
		}
	}
}




// **********************************
// WORD�ϐ��̃G���f�B�A���ϊ�
// **********************************
unsigned short int ConvEndianWord(unsigned short int nWord)
{
	unsigned short int nTempWord;

	nTempWord = ((nWord & 0xff00) >> 8) | ((nWord & 0xff) << 8);

	return nTempWord;
}

// **********************************
// DWORD�ϐ��̃G���f�B�A���ϊ�
// **********************************
unsigned int ConvEndianDWord(unsigned int nDWord)
{
	unsigned int nTempDWord;

	nTempDWord = ((nDWord & 0xff000000) >> 24) | ((nDWord & 0xff0000) >> 8) | 
			((nDWord & 0xff00) << 8) | ((nDWord & 0xff) << 24);

	return nTempDWord;
}
