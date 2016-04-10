#include<stdio.h>

typedef struct
{
    unsigned int type;                 /* Identifier                  */
    unsigned long int size;                       /* File size in bytes          */
    unsigned int reserved1, reserved2;
    unsigned long int offset;                     /* Offset to image data, bytes */
}HEADER;

typedef struct
{
   unsigned long int size;               /* Header size in bytes      */
   long int width,height;                /* Width and height of image */
   unsigned int planes;       /* Number of colour planes   */
   unsigned int bits;         /* Bits per pixel            */
   unsigned long int compression;        /* Compression type          */
   unsigned long int imagesize;          /* Image size in bytes       */
   long int xresolution,yresolution;     /* Pixels per meter          */
   unsigned long int ncolours;           /* Number of colours         */
   unsigned long int importantcolours;   /* Important colours         */
}BMPHEADER;

void main()
{
	FILE *fp;
	HEADER fh;
	BMPHEADER bh;
	char t[15];
	char *s;
	printf("\n\n Enter the File Name : ");
	scanf("%s",t);
	fp = fopen(t,"r");
	if(fp == NULL) printf("\n\n problem\n\n");
	fread(&fh,sizeof(HEADER),1,fp);
	fread(&bh,sizeof(BMPHEADER),1,fp);
	printf("\n\n The information about the BMP file and the attributes of the image are : ");
	s = (char *)(&fh.type);
	printf("\n\n Type : %c%c",*s,*(s+1));
	printf("\n\n File Size : %lu",fh.size);
	printf("\n\n Image Data Offset : %lu",fh.offset);
	printf("\n\n Bitmap Header Size : %lu",bh.size);
	printf("\n\n Image Width : %ld",bh.width);
	printf("\n\n Image Height : %ld",bh.height);
	printf("\n\n Number of Colour Planes : %u",bh.planes);
	printf("\n\n Number of Bits per Pixel : %u",bh.bits);
	printf("\n\n Compression Type : %lu",bh.compression);
	printf("\n\n Image Size : %lu",bh.imagesize);
	printf("\n\n Xresolution : %ld",bh.xresolution);
	printf("\n\n Yresolution : %ld",bh.yresolution);
	printf("\n\n No. of Colours in ColourTable : %lu",bh.ncolours);
	printf("\n\n Important Colours in ColourTable : %lu\n\n",bh.importantcolours);
	fclose(fp);
}
