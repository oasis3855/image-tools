/* J6ItoJPG.C */
/* �f�W�^���J������ J6I �t�@�C������ JPEG �t�@�C���𒊏o����B */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>

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

int sw_verbose;

void main(int argc, char *argv[])
{
	FILE *in, *out;
	void *buf;
	header hd;
	char filename[125], tmpname[125];
	int i;
	unsigned long j;
	unsigned int rd,rt;	/* ���W�X�^�Ɋi�[���鎞�ԂƓ��t */

	if(argc < 2 || argc > 3)
	{
		printf("RICOH DC-2L �摜�ް����ް� by INOUE. Hirokazu\n�g�p�@ : J6ItoJPG [-v] ̧�ٖ�\n\n-v �����ŏڍ׏���\��\n");
		exit(1);
	}

	sw_verbose = 0;
	if(argc == 3)
	{
		if(!strcmp(argv[1], "-v")) sw_verbose = 1;
		else
		{
			printf("�g�p�@ : J6ItoJPG -v ̧�ٖ�\n");
			exit(1);
		}
	}

	if((buf = malloc(1024*10+1)) == NULL)
	{
		printf("��߰�p�ޯ̧�̈� 10 kBytes ���m�ۂł��Ȃ�\n");
		exit(1);
	}

	if(sw_verbose) strcpy(tmpname, argv[2]);
	else strcpy(tmpname, argv[1]);
	strtok(tmpname, ".");
	strcpy(filename, tmpname);
	strcat(filename, ".j6i");
	if((in = fopen(filename, "rb")) == NULL)
	{
		printf("J6I ̧�� %s �����݂��Ȃ�\n", filename);
		free(buf);
		exit(1);
	}
	
	strcpy(filename, tmpname);
	strcat(filename, ".jpg");
	if((out = fopen(filename, "wb")) == NULL)
	{
		printf("JPG ̧�� %s �쐬�װ\n", filename);
		fclose(in);
		free(buf);
		exit(1);
	}

	if(sw_verbose) printf("%s.j6i --> %s.jpg\n", strlwr(tmpname), strlwr(tmpname)); 
	
	if(fread(&hd, sizeof(header), 1, in) != 1)
	{
		printf("J6I ̧�� ͯ�ޗ̈�װ\n");
		fclose(out);
		fclose(in);
		free(buf);
		exit(0);
	}
	if(!hd.endian)
	{
		j = hd.startaddr;
		hd.startaddr = ((j & 0xff000000) >> 24) | ((j & 0x00ff0000) >> 8) | 
			((j & 0x0000ff00) << 8) | ((j & 0x000000ff) << 24);
		j = hd.endaddr;
		hd.endaddr = ((j & 0xff000000) >> 24) | ((j & 0x00ff0000) >> 8) | 
			((j & 0x0000ff00) << 8) | ((j & 0x000000ff) << 24);
	}

	if(fseek(in, hd.startaddr, SEEK_SET))
	{
		printf("J6I ̧�� ͯ�ޗ̈�װ\n");
		fclose(out);
		fclose(in);
		free(buf);
		exit(1);
	}
	if(hd.head != (unsigned char)0x80 || hd.dummy1[0] != (unsigned char)0x3e ||
		hd.enabledate != (unsigned char)0x81)
	{
		printf("J6I ̧��ͯ�ނ� RICOH DC-1/2 �̂��̂łȂ�\n");
		fclose(out);
		fclose(in);
		free(buf);
		exit(1);
	}

	if(sw_verbose)
	{
		printf("   %02u/%02u/%02u %02u:%02u:%02u %Lu Bytes (head %Lu Bytes) ver %02d.%02d attr %02X\n",
			BCDtoINT(hd.date[0]),
			BCDtoINT(hd.date[1]),
			BCDtoINT(hd.date[2]),
			BCDtoINT(hd.date[3]),
			BCDtoINT(hd.date[4]),
			BCDtoINT(hd.date[5]),
			hd.endaddr - hd.startaddr + 1,
			hd.startaddr,
			hd.ver_h, hd.ver_l, hd.attr);
	}

	j = hd.endaddr - hd.startaddr + 1;
	do{
		i = fread(buf, 1, 1024*10 < j ? 1024*10 : j , in);
		if(ferror(in))
		{
			printf("J6I �Ұ���ް��ǂݍ��ݴװ\n");
			break;
		}
		if(fwrite(buf, 1, i, out)<(unsigned)i)
		{
			printf("JPG �Ұ���ް��̏������ݴװ\n");
			break;
		}
		j -= i;
	}while(!feof(in) && i && j>0);

	i = fileno(out);
	if(BCDtoINT(hd.date[0]) < 80)
		rd = (BCDtoINT(hd.date[0])+20)*0x200 + BCDtoINT(hd.date[1])*0x20 + BCDtoINT(hd.date[2]);
	else
		rd = (BCDtoINT(hd.date[0])-80)*0x200 + BCDtoINT(hd.date[1])*0x20 + BCDtoINT(hd.date[2]);
	rt = BCDtoINT(hd.date[3])*0x800 + BCDtoINT(hd.date[1])*0x20 + BCDtoINT(hd.date[2]);
/*
	_DX = rd;
	_CX = rt;
	_BX = i;
	_AL = 0x01;
	geninterrupt(0x21);
*/

	fclose(out);
	fclose(in);
	free(buf);


	exit(0);

}

unsigned int BCDtoINT(unsigned char bcd)
{
	unsigned int i,j;
	i = bcd;
	j = ((i & 0xf0) >> 4) * 10;
	j += bcd & 0x0f;
	return j;
}