#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include "bp.h"
using namespace std;

int strInt(char* type, int args)
{
	if(strcmp(type,"bimodal")==0 && args==6)
	return 0;
	else if(strcmp(type,"gshare")==0 && args==7)
	return 1;
	else if(strcmp(type,"hybrid")==0 && args==9)
	return 2;
	else 
	return 3;
}

ulong getAddr (string line)
{
	string addr;
	ulong addrInt;
	addr = line.substr(0,6);
	stringstream ss;
	ss << addr;
	ss >> std::hex >> addrInt;
	return addrInt;
}

char getTN(string line) 
{
	char c;
	stringstream cc;
	cc << line.substr(7,1);
	cc >> c;
	return c;
}

void predBimodal(int m, char* fileName, char* type)
{
	ulong i;
	char pred;
	Counter *item;
	Predictor biMod(m,0, "BIMODAL");
	fstream file;
	file.open(fileName);
	string line;
	while(getline(file, line))
	{
		biMod.incPred();
	 	i = biMod.findIndex(getAddr(line));
		item = biMod.findCtr(i,0);
		pred = item->getPred();
		item->modCtr(getTN(line));
		if(pred != getTN(line))
		{
		biMod.incMisPred();
		}
	}
	biMod.printOutput();
	biMod.printContents();
//	biMod.excelData();
}

void predGshare(int m, int n, char* fileName, char* type)
{
	ulong i,j;
	char pred;
	Counter *item;
	Predictor gShare(m-n,n, "GSHARE");
	BHR hist(n);
	fstream file;
	file.open(fileName);
	string line;
	while(getline(file, line))
	{
		gShare.incPred();
		i = gShare.findIndex(getAddr(line));
		j = gShare.findAssoc(getAddr(line));
		j ^= hist.getHist(); 
		item = gShare.findCtr(i,j);	
		pred = item->getPred();	
		item->modCtr(getTN(line));
		if(pred != getTN(line))
		{
		gShare.incMisPred();
		}
		hist.updateBHR(getTN(line));	
	}
	gShare.printOutput();
	gShare.printContents();
//	gShare.excelData();
}

void predHybrid(int k, int m1, int n, int m2, char* fileName, char* type)
{
	ulong i,j,x,y;
	char pred1, pred2;
	Counter *itemBi, *itemGshare, *itemChooser;
	Predictor biMod(m2, 0, "BIMODAL");	
	Predictor gShare(m1-n, n, "GSHARE");
	Predictor chooser(k,0, "CHOOSER");
	BHR hist(n);
	fstream file;
	file.open(fileName);
	string line;
	while(getline(file, line))
	{
		chooser.incPred();
		i = biMod.findIndex(getAddr(line));
		itemBi = biMod.findCtr(i,0);
		pred1 = itemBi->getPred();
		x = gShare.findIndex(getAddr(line));
		y = gShare.findAssoc(getAddr(line));
		y ^= hist.getHist();
		itemGshare = gShare.findCtr(x,y);
		pred2 = itemGshare->getPred();
		j = chooser.findIndex(getAddr(line));
		itemChooser = chooser.findCtr(j,0);
		hist.updateBHR(getTN(line));
		if(itemChooser->getPred()=='n')
		{
			itemBi->modCtr(getTN(line));
			if(pred1 != getTN(line))
			chooser.incMisPred();		
		}
		else 
		{
			itemGshare->modCtr(getTN(line));
			if(pred2 != getTN(line))
			chooser.incMisPred();
		}
//Modifying Chooser		
		if(pred1==getTN(line) && pred2!=getTN(line))
		itemChooser->modCtr('n');
		else if(pred1!=getTN(line) && pred2==getTN(line))	
		itemChooser->modCtr('t');	
	}
	chooser.printOutput();
	chooser.printContents();
	gShare.printContents();
	biMod.printContents();
}

int main(int argc, char** argv)
{
	int i;	
	cout << "COMMAND" << endl;
	for(i=0;i<argc;i++)
	cout << argv[i] << " ";
	cout << endl;
	switch(strInt(argv[1], argc))
	{
		case 0: predBimodal(atoi(argv[2]), argv[5], argv[1]); break;
				
		case 1: predGshare(atoi(argv[2]),atoi(argv[3]), argv[6], argv[1]); break;

		case 2: predHybrid(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]), argv[8], argv[1]); break;

		case 3: cerr << "Not Valid Type Input" << endl; break; 
	}	
}
