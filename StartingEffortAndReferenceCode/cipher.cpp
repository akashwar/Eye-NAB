#include<stdio.h>
#include<conio.h>
#include<string.h>
int main()
{
	char monarray[5][5]={0},message[50],repmessage[50],finalmessage[50],decrypted[50];
	int flag=0,find=0,temp;
	int i=0,j=0,m,n,k,p,q,sp;
	char ch=65;
	int msgpointer=0;
	printf("Enter the key\n");
	gets(message);
	while(i<5)
	{
		j=0;
		while(j<5)
		{
			flag=0;
			for(m=0;m<=i;m++)
			{
			
				if(m<i)
				temp=5;
				else
				temp=j;
				for(n=0;n<temp;n++)
				{
					
					{
						if(message[msgpointer]!='\0')
						{
							if(message[msgpointer]==monarray[m][n])
							{
								flag=1;
								msgpointer++;
								break;
							
							
							}
						}
						else
						{
							if(ch==monarray[m][n] || ch==74)
							{
								
								flag=1;
								ch++;
								break;
						
						
							}
					
						}	
					}
				}
				if(flag==1)
				break;
				
			
			}
			if(flag==0)
			{
				if(message[msgpointer]!='\0')
				{
					monarray[i][j++]=message[msgpointer++];
				}
				else
				{
					monarray[i][j++]=ch++;
					
				}
				
				
			}
			
			
		
				
		
		}
		i++;
	}
	
	for(i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
			printf("%c  ",monarray[i][j]);
		}
		printf("\n");
	}
	printf("\nEnter the message to encrypt\n");
	gets(message);

	m=0;
	k=0;
	for(i=0;message[i]!='\0';i++)
	{	
		if(message[i]==32)
		{
			repmessage[k++]=message[i];
			continue;
		}
		repmessage[k++]=message[i];
		sp=i;
		if(message[sp+1]!='\0')
		{	
			
			while(message[sp+1]==32){ repmessage[k++]=message[++sp]; }
			if(message[sp+1]==message[i])
			{	
				repmessage[k++]='X';
			}
			else
			{	
				i=sp;		
				i++;
				repmessage[k++]=message[i];

			}
		}
		else
			repmessage[k++]='X';

		
		
	}
	repmessage[k]='\0';
	printf("Altered message\n");
		puts(repmessage);
	k=0;
	sp=0;
	while(repmessage[k]!='\0')
	{
		if(repmessage[k]==32){  finalmessage[k]=repmessage[k];k++;continue; }
		sp=k;
				while(repmessage[sp+1]==32){ sp++; finalmessage[sp]=repmessage[sp]; }
		for(i=0;i<5;i++)
		{

			for(j=0;j<5;j++)
			{
				
				if(monarray[i][j]==repmessage[k] || monarray[i][j]==repmessage[sp+1] )
				{
					if(monarray[i][j]==repmessage[sp+1])
					{
						p=i;
						q=j;

					}
					else
					{
						m=i;
						n=j;


					}

				}

			}
			
		}
		if(m==p)
			{
				finalmessage[k]=monarray[m][(n+1)%5];
				finalmessage[sp+1]=monarray[p][(q+1)%5];
				k=sp+2	;			
			}
			else
			if(n==q)
			{
				finalmessage[k]=monarray[(m+1)%5][n];
				finalmessage[sp+1]=monarray[(p+1)%5][q];
				k=sp+2;
			}
			else
				if(m!=p && n!=q)
				{
					finalmessage[k]=monarray[m][q];
					finalmessage[sp+1]=monarray[p][n];
					k=sp+2;

					
				}
		
		
	
	}
	finalmessage[k]='\0';
	printf("\nEncrypted Message\n");
	puts(finalmessage);
	//-----------------------------------------------------Decrypted----------------------------------
	k=0;
	while(finalmessage[k]!='\0')
	{
		if(finalmessage[k]==32){  decrypted[k]=finalmessage[k];k++;continue; }
		sp=k;
				while(finalmessage[sp+1]==32){ sp++; decrypted[sp]=finalmessage[sp]; }
		for(i=0;i<5;i++)
		{

			for(j=0;j<5;j++)
			{
				if(monarray[i][j]==finalmessage[k] || monarray[i][j]==finalmessage[sp+1] )
				{
					if(monarray[i][j]==finalmessage[sp+1])
					{
						p=i;
						q=j;

					}
					else
					{
						m=i;
						n=j;


					}

				}

			}
			
		}
		if(m==p)
			{
				
				
				decrypted[k]=monarray[m][(n-1+5)%5];
				decrypted[sp+1]=monarray[p][(q-1+5)%5];
				k=sp+2;
			}
			else
			if(n==q)
			{
				decrypted[k]=monarray[(m-1+5)%5][n];
				decrypted[sp+1]=monarray[(p-1+5)%5][q];
				k=sp+2;
			}
			else
				if(m!=p && n!=q)
				{
					decrypted[k]=monarray[m][q];
					decrypted[sp+1]=monarray[p][n];
					k=sp+2;
					
				}


	}
	decrypted[k++]='\0';
	temp=k;
	k=0;
	while(decrypted[k]!='\0')
	{
		if(decrypted[k]=='X')
		{
			for(i=k;i<temp-1;i++)
			{	
				decrypted[i]=decrypted[i+1];

			}
			temp--;
		}
		k++;
		

	}
	printf("\n Decrypted message\n");
	puts(decrypted);

	//-----------------------------------------------------------------------------------------------
	getch();
	return 0;
}

