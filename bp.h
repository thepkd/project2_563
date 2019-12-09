#include <iostream>
#include <stdlib.h>
using namespace std;

class Counter	
{
protected:

int ctr;

public:

Counter() {ctr=2;}  //Initialising
Counter(int i) {ctr = i;}
void incSat() {if(ctr!=3) ctr++;} 
void decSat() {if(ctr!=0) ctr--;}
char getPred() {if(ctr==0 || ctr==1) return 'n'; else return 't';}
void modCtr(char); 
void printCtr() { cout << ctr;}
};

class Predictor
{
protected:
ulong indexBits, assocBits, sets, assoc, indexMask, predCount, misPred, assocMask;
string name;
Counter **table;

public:
Predictor(int, int, char *);

Counter *findCtr(ulong index, ulong hist) { return &(table[index][hist]);}
void  incPred() {predCount++;}
void  incMisPred() {misPred++;}

void  printOutput();
void  printContents();
void  excelData();
ulong findIndex(ulong);
ulong findAssoc(ulong);
void  missRate();
};

class BHR
{
protected:
ulong shiftReg, mask, sizeBits;
public:
BHR(int n);
void updateBHR(char br);
ulong getHist() {return shiftReg;}
};

