#include <iostream>
#include <fstream>
#include <iomanip>
#include "Executive.h"
using namespace std;
Executive::Executive()
{
	m_size = 500;
	m_Keyword = "";
	m_Hfrequency = 0; //used to find highest frequency of factors
	m_Length = 0;
	m_Frequency = new float[26]{8.2,1.5,2.8,4.3,12.7,2.2,2.0,6.1,7.0,0.2,0.8,4.0,2.4,6.7,7.5,1.9,0.1,6.0,6.3,9.1,2.8,1.0,2.4,0.2,2.0,0.1};
	m_CharArray = new char[m_size];
	m_ShiftFrequency = new double[26];
	for(int i = 0; i<m_size;i++)
	{
		m_CharArray[i]=' ';
	}
	m_FactorArray = new int[m_size];
	for(int i = 0; i<m_size; i++)
	{
		m_FactorArray[i] = 0;
	}
	m_CipherFrequency = new double[26];
	for(int i = 0; i<26;i++)
	{
		m_CipherFrequency[i]=0;
	}
}
Executive::~Executive()
{
	delete [] m_CharArray;
	delete [] m_ShiftArray;
	delete [] m_ShiftFrequency;
	delete [] m_CipherFrequency;
	delete [] m_FactorArray;
	delete [] m_Frequency;
}
void Executive::Take(string Filename)
{
	int index = 0;
	char TempChar;
	ifstream InFile(Filename);
	while(InFile>>TempChar)
	{
		m_CharArray[index] = TempChar;
		index += 1;
	}
	InFile.close();
	Finalize();
	Search();
	
	for(int i = 0; i<m_size; i++)
	{
		if(m_FactorArray[i]>m_Hfrequency)
		{
			m_Hfrequency = m_FactorArray[i];
			m_Length = i;
		}
	}
	m_IntKey = new int[m_Length];	
	FrequencyAnalysis();
	cout<<"Keyword: "<<m_Keyword<<endl<<"Message: "<<endl;
	Decrypt();
	delete [] m_IntKey;
}
void Executive::Search()
{
	bool Found = false;
	int k = 0;
	int check1 = 0;
	int check2 = m_size/2;
	int NullNum = 0;
	while(check2-1!=check1)
	{
		//runs through entire array minus the size of the string it's checking
		for(int i = check2; check2-check1<=m_size-i;i++)
		{
			if(m_CharArray[check1]==m_CharArray[i])
			{
				//check rest of string
				while(m_CharArray[check1+k]==m_CharArray[i+k])
				{
					k++;
					if(k==check2-check1)
					{
//will only consider the factors of strings with lengths of three or greater
						//prints repeating string
						/*
						cout<<"String found: ";
						for(int l = 0; l<k;l++)
						{
							cout<<m_CharArray[check1+l];
						}
						cout<<endl;
						*/
						Factor(i-check1);
//length inbetween equals (i-check1-1) this means that we must factor the number (i-check1)
//insert the three lines of the if(found) statement here to cancel all comparisons of the same string or sub-strings
						k = 0;
						Found = true;
						break;
					}
					else if(m_CharArray[check1+k]!=m_CharArray[i+k])
					{
						k = 0;
						break;
					}
				}
			}
		}
//if the string is found it will skip all substring searches
		if(Found)
		{
			check1=check2-1;
			NullNum+=check2-check1-1;
			check2 = (m_size+NullNum)/2;
			Found = false;
		}
		//doesn't check length of 1 when -2
		else if(check2-2>check1)
		{
			check2--;
		}
		else
		{
			check1++;
			NullNum++;
			check2 = (m_size+NullNum)/2;
		}
	}
}
void Executive::Factor(int integer)
{
//does not consider or store 1 * itself
	int temp = 2;
	while(temp<=integer)
	{
		if(!(integer%temp))
		{
			m_FactorArray[temp]+=1;
		}
		temp++;
	}
}
void Executive::FrequencyAnalysis()
{
	int index = 0;
	int shift = 0;
	double temp = 0;
	double TotFreq = 0;
	double BestFreq = 0;
	if(m_size%m_Length!=0)
	{
		m_Shiftsize = (m_size/m_Length)+1;
	}
	else
	{
		m_Shiftsize = m_size/m_Length;
	}
	m_ShiftArray= new char[m_Shiftsize];
	for(int i = 0; i<m_Shiftsize; i++)
	{
		m_ShiftArray[i] = 0;
	}
	for(int i = 0; i<m_size;i++)
	{
		if(m_CharArray[i]<=90)
		{
			m_CipherFrequency[m_CharArray[i]-65]++;
		}
		else
		{
			m_CipherFrequency[m_CharArray[i]-97]++;
		}
	}
	for(int k = 0; k<m_Length;k++)
	{
		for(int i = 0; i<26; i++)
		{
			m_ShiftFrequency[i]=0;
		}
		int c = 0;	
		for(int i = k; i<m_size;i+=m_Length)
		{
			m_ShiftArray[c]=m_CharArray[i];
			c++;
		}
		for(int i = 0; i<m_Shiftsize;i++)
		{
			if((m_ShiftArray[i]<=90)&&(m_ShiftFrequency[m_ShiftArray[i]-65]==0))
			{
				m_ShiftFrequency[m_ShiftArray[i]-65]=m_CipherFrequency[m_ShiftArray[i]-65];
			}
			else if(m_ShiftFrequency[m_ShiftArray[i]-97]==0)
			{
				m_ShiftFrequency[m_ShiftArray[i]-97]=m_CipherFrequency[m_ShiftArray[i]-97];
			}
		}
		for(int i = 0; i<26;i++)
		{
			for(int j = 0; j<26;j++)
			{		
				TotFreq += (m_ShiftFrequency[j]/double(m_size))*m_Frequency[j];
			}
			if(TotFreq>BestFreq)
			{
				BestFreq=TotFreq;
				shift = i;
			}
			temp = m_ShiftFrequency[0];
			for(int i = 0; i<25;i++)
			{
				m_ShiftFrequency[i]=m_ShiftFrequency[i+1];
			}
			m_ShiftFrequency[25]=temp;
			TotFreq = 0;
		}
		m_Keyword+=(shift+97);
		m_IntKey[index]=shift;
		index++;
		shift = 0;
		BestFreq = 0;
	}
}
void Executive::Finalize()
{
	int i=m_size-1;
	int Counter=0;
	while(m_CharArray[i] == ' ')
	{
		Counter+=1;
		i--;
	}
	m_size-=Counter;
	char * temp = new char[m_size];
	for(int i = 0; i<m_size;i++)
	{
		temp[i]=m_CharArray[i];
	}
	m_CharArray = new char[m_size];
	for(int i = 0; i<m_size;i++)
	{
		m_CharArray[i]=temp[i];
	}
	delete [] temp;
}
void Executive::Decrypt()
{
	int i = 0;
	int j = 0;
	int temp;
	bool Decrypted = false;
	ofstream OutFile("Decrypted.txt");
	while(!Decrypted)
	{
		for(j = i; j<m_Length+i;j++)
		{
			if(j==m_size)
			{
				Decrypted=true;
				break;
			}
		
			if(m_CharArray[j]<=90)
			{
				temp = m_CharArray[j]-65-m_IntKey[j-i];
			}
			else
			{
				temp = m_CharArray[j]-97-m_IntKey[j-i];
			}
			if(temp<0)
			{
				temp = 26+temp;
			}
			cout<<char(temp+97);
			OutFile<<char(temp+97);
		}
		i+=m_Length;
	}
	cout<<endl;
	OutFile.close();
}
