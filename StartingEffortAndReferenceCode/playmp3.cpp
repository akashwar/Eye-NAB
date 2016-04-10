#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cv.h>
#include <highgui.h>
#include <windows.h>
#include <mmsystem.h>
void playSoundNumber(int number)
{

	char a[2];
		int digit[5];
	int cnt=0;
	while(number>0)
	{
		digit[cnt]=number%10;
		number=number/10;
		cnt++;



	}
	char address[500]="c:\\tv\\vlc\\vlc.exe ";

	for(int i=cnt-1;i>=0;i--)
	{

		char temp[50]="c:\\tv\\sounds\\";
		a[0]=digit[i]+48;
		a[1]='\0';
		strcat(temp,a);
		strcat(address,temp);
		strcat(address,".mp3 ");
		
		//system("exit");
		//system("taskkill /IM wmplayer.exe");

		//PlaySound(TEXT("c:\\tv\\love.mp3"),NULL,SND_FILENAME);
		printf("%s",address);
		cvWaitKey(5000);
		
	}
system(address);
	system("taskkill /IM vlc.exe");


}

int main()
{

playSoundNumber(570);

return 0;
}
