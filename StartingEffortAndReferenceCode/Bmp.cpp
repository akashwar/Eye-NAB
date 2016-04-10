#include<stdio.h>

/* The use of "char" arrays instead of "int" and "short" has been done on purpose. 
The use of "int" and "short" leads to padding of extra bytes by the compiler to 
align the struct size as a multiple of 4. This results into incorrect addressing 
of the various fields in the file.
char a[2] stands for unsigned short int a.
char a[4] stands for unsigned int a.     */

typedef struct
{
    char type[2];                       // File Type Identifier
    char size[4];                       // File size in bytes
    char reserved1[2], reserved2[2];
    char offset[4];                     // Offset to image data, bytes
}HEADER;

typedef struct
{
   char size[4];                        // Header size in bytes
   char width[4],height[4];             // Width and height of image
   char planes[2];                      // Number of colour planes
   char bits[2];                        // Bits per pixel
   char compression[4];                 // Compression type
   char imagesize[4];                   // Image size in bytes
   char xresolution[4],yresolution[4];  // Pixels per meter
   char ncolours[4];                    // Number of colours
   char importantcolours[4];            // Important colours
}BMPHEADER;

char *s;
unsigned short int *q;
unsigned int *r;

void show(int i)
{
	if(i==2)
	{
		q = (unsigned short int *)s;
		printf("%u",*q);
	}
	else if(i==4)
	{
		r = (unsigned int *)s;
		printf("%u",*r);
	}
	s = s + i;
}

void main()
{
	FILE *fp;
	HEADER fh;
	BMPHEADER bh;
	char t[15];
	printf("\n\n Enter the File Name : ");
	scanf("%s",t);
	fp = fopen(t,"r");
	fread(&fh,sizeof(HEADER),1,fp);
	fread(&bh,sizeof(BMPHEADER),1,fp);
	printf("\n\n The information about the BMP file and the attributes of the image are : ");
	s = fh.type;    // making s point to the first byte of file header
	printf("\n\n Type : %c%c",*s,*(s+1));  s+=2;
	printf("\n\n File Size : ");  show(4);
	s = s + 4;      // to step over the 4 reserved bytes in the file header
	printf("\n\n Image Data Offset : ");  show(4);
	s = bh.size;    // making s point to the first byte of bmp header
	printf("\n\n Bitmap Header Size : ");  show(4);
	printf("\n\n Image Width : ");  show(4);
	printf("\n\n Image Height : ");  show(4);
	printf("\n\n Number of Colour Planes : ");  show(2);
	printf("\n\n Number of Bits per Pixel : ");  show(2);
	printf("\n\n Compression Type : ");  show(4);
	printf("\n\n Image Size : ");  show(4);
	printf("\n\n Xresolution : ");  show(4);
	printf("\n\n Yresolution : ");  show(4);
	printf("\n\n No. of Colours in ColourTable : ");  show(4);
	printf("\n\n Important Colours in ColourTable : ");	show(4);
	printf("\n\n");
	fclose(fp);
}
