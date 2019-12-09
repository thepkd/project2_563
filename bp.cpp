#include <iostream>
#include <cstdio>
#include <sstream>
#include <stdlib.h>
#include "bp.h"
using namespace std;

Predictor::Predictor (int m, int n, char *type)
{
	name = std::string(type);	
	predCount =0;
	misPred = 0;
	int i,j;	
	indexBits = m;
	assocBits = n;	
	indexMask = 1<<(m+2); indexMask--; indexMask &= ~3;
	assocMask = 1<<(m+n+2); assocMask--; assocMask &= ~3;
	sets = 1<<m;
	assoc = 1<<n;
	table = new Counter*[sets];
	for (i=0;i<sets;i++)
	{
		table[i] = new  Counter[assoc];
	}
	if(type=="CHOOSER")
	{
		for(i=0;i<sets;i++)
		{
			for(j=0;j<assoc;j++)
			{
			table[i][j]= Counter(1);
			}
		}
	}
	
}

void Counter::modCtr(char event) {if(event == 'n') decSat(); else incSat();}
ulong Predictor::findIndex(ulong addr) {return (addr&indexMask)>>2;}
ulong Predictor::findAssoc(ulong addr) {return (addr&assocMask)>>(indexBits+2);}

void Predictor::printContents()
{
	ulong i,j;
	Counter *item;
	cout << "FINAL " << name << " CONTENTS" << endl;
	for(j=0;j<assoc;j++)
	{
		for(i=0;i<sets;i++)
		{
			item = findCtr(i,j);	
			cout << (j*sets + i) << " ";
			item->printCtr();
			cout << endl;
		}
	} 
}

void Predictor::printOutput()
{
	cout << "OUTPUT" << endl;
	cout << " number of predictions: " << predCount << endl;
	cout << " number of mispredictions: " << misPred << endl; 
	cout << " misprediction rate: "; missRate(); cout << "%" << endl;
}

void  Predictor::missRate() 
{
	double percentage = double(misPred)/double(predCount);
	printf("%.2f",percentage*100);
}

void  Predictor::excelData()
{
	cout << indexBits << " " << assocBits << " " << indexBits+assocBits  << " "; missRate(); cout << endl; 
}

void  BHR::updateBHR (char br)
{
	if(br == 'n')
	{
	shiftReg>>=1; 
	shiftReg &= mask;
	}
	else if(br == 't')
	{
	shiftReg>>=1;
	shiftReg |= 1<<(sizeBits-1);
	shiftReg &= mask;
	}
}

BHR::BHR (int n)
{
	sizeBits = n;
	shiftReg=0;
	mask = (1<<n)-1;
}
