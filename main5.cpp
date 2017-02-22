#include <bits/stdc++.h>

using namespace std;

const int maxN = 444;
string data[maxN];
int dataL;
int currentLine;

void readAllLines() {
	string s;
	while (getline(cin, s))
		data[dataL++] = s;
}

string action[] = {"ADD", "MUL", "SUB", "DIV", "MOV", "CMP", "JE", "JNE", "JGE", "JLE", "PRN", "JMP", "JR", "exit"};

string delSpaces(string v) {
	while (v.size() > 0 && v[v.size() - 1] == ' ') 
		v.erase(v.size() - 1);
	while (v.size() > 0 && v[0] == ' ') 
		v.erase(0, 1);		
	return v;
}

bool isAction(string x) {
	for (int i = 0; i < 14; ++i)
		if (action[i] == x) return true;
	return false;
}

bool actB[maxN];
string first[maxN];

void getFirst() {
	for (int i = 0; i < dataL; ++i) {
		int len = data[i].size();
		string act = "";
		for (int f = 0; f < len; ++f)
		{
			if (data[i][f] == ' ') break;
			act += data[i][f];
		}		
		actB[i] = isAction(act);
		first[i] = act;
	}
}
int startLine, cntMain;
map <string, bool> Exist;
map <string, int> getLine;

void compilationErrors() {
	bool hasError = false;
	for (int i = 0; i < dataL; ++i) {
		int len = data[i].size();
		if (len == 0) continue;
		if (actB[i]) continue;
		string func = "";
		bool was = false;
		for (int f = 0; f < len; ++f)
		{
			char x = data[i][f];
			if (!isalpha(x) && !isdigit(x) && x != ':' && x != '_' && x != ' ') 
				was = true;
			func += data[i][f];
		}	
		while (func.size() > 0 && func[func.size() - 1] == ' ') 
			func.erase(func.size() - 1);
		int cnt = 0;
		len = func.size();
		for (int f = 0; f < len; ++f) {
			char x = func[f];
			if (!isalpha(x) && !isdigit(x) && x != ':' && x != '_') 
				was = true;
			if (x == ':') cnt++;
		}
		if (func.size() > 0) {	
			if (func[func.size() - 1] != ':' || cnt != 1)
				was = true;
		}
		if (was) {
			printf ("Error in line %d\n", i + 1);
			hasError = true;
		}
		if (!was && func == "main:") startLine = i, cntMain++;
		if (!was) {
			Exist[func] = true; 
			getLine[func] = i;
		}
	}
	if (hasError) exit(0);
}

void wrong(int x) {
	printf ("Wrong action in line %d\n", x + 1);
	exit(0);
}

string T[4];
int mem[256], reg[8];

bool bad(string v) {
	if (v.size() < 2) return true;
	if (v[0] == 'R' || v[0] == 'A' || v[0] == '#') {
		for (int i = 1; i < (int) v.size(); ++i)
			if (!isdigit(v[i])) return true;
		return false;	
	}
	else return true; 
}

int lenAct;

void parseLine(int x) {
	for (int i = 0; i < 3; ++i) 
		T[i] = "";
	lenAct = 0;
	for (int i = 4; i < (int) data[x].size(); ++i) {
		char u = data[x][i];
		if (u == ',') { ++lenAct; continue; }
		if (lenAct > 2) wrong(x);
		T[lenAct] += u;
	}
	for (int i = 0; i <= lenAct; ++i) {
		T[i] = delSpaces(T[i]);
		if (T[i].size() == 0) wrong(x);
	}
	for (int i = 0; i <= lenAct; ++i)
		if (bad(T[i])) wrong(x);
}
int getID(string x) {
	if (x[0] == '#') return -1;
	int res = 0;
	for (int i = 1; i < (int) x.size(); ++i)
		res = res * 10 + (x[i] - '0');
	return res + ((x[0] == 'R') ? 256 : 0);
}

int getNum(string x) {
	int res = 0;
	for (int i = 1; i < (int) x.size(); ++i)
		res = res * 10 + (x[i] - '0');
	if (x[0] == '#') return res;
	if (x[0] == 'R') return reg[res];
	if (x[0] == 'A') return mem[res];
}

int memID, num1, num2;

void getIDandNUM(int x, int len) {
	memID = getID(T[0]);
	if (len > 0) num1 = getNum(T[1]);
	if (len > 1) num2 = getNum(T[2]);
	if (memID > 256 + 7) memID = -1;
	if (memID == -1) wrong(x);
}

void ADD(int x) {
	parseLine(x);
	if (lenAct != 2) wrong(x);
	getIDandNUM(x, lenAct);
	int res = num1 + num2;
	(memID > 255) ? reg[memID - 256] = res: mem[memID] = res;
}

void SUB(int x) {
	parseLine(x);
	if (lenAct != 2) wrong(x);
	getIDandNUM(x, lenAct);
	int res = num1 - num2;
	(memID > 255) ? reg[memID - 256] = res: mem[memID] = res;
}

void MUL(int x) {
	parseLine(x);
	if (lenAct != 2) wrong(x);
	getIDandNUM(x, lenAct);
	int res = num1 * num2;
	(memID > 255) ? reg[memID - 256] = res: mem[memID] = res;
}

void DIV(int x) {
	parseLine(x);
	if (lenAct != 2) wrong(x);
	getIDandNUM(x, lenAct);	
	if (num2 == 0) {
		printf ("Impossible divide to zero (0).");
		wrong(x);
	}
	int res = num1 / num2;
	(memID > 255) ? reg[memID - 256] = res: mem[memID] = res;
}
void MOV(int x) {
	parseLine(x);
	if (lenAct != 1) wrong(x);
	getIDandNUM(x, lenAct);
	int res = num1;
	(memID > 255) ? reg[memID - 256] = res: mem[memID] = res;	
}
void PRN(int x) {
	parseLine(x);
	if (lenAct != 0) wrong(x);
	int res;
	if(T[0][0] == '#') res = getNum(T[0]);
	else res = (memID > 255) ? reg[memID - 256]: mem[memID];
	printf ("PRN -> %d\n", res);	
} 

int num[3];
bool isJE, isJNE, isJLE, isJGE;
void CMP(int x) {
	parseLine(x);
	if (lenAct != 1) wrong(x);
	for (int i = 0; i < 2; ++i) {
		if (T[i][0] == '#')
			num[i] = getNum(T[i]);
		else {
			memID = getID(T[i]);
			num[i] = (memID > 255) ? reg[memID - 256]: mem[memID];
		}
	}
	isJE = isJNE = isJLE = isJGE = false;
	if (num[0] == num[1]) isJE = true;
	if (num[0] != num[1]) isJNE = true;
	if (num[0] <= num[1]) isJLE = true;
	if (num[0] >= num[1]) isJGE = true;	
}

int pos[maxN], posL;

void JMP(int x, bool dobry, int len) {
	string v = data[x];
	v.erase(0, len);
	v = delSpaces(v) + ":";
	if (Exist[v] == false) wrong(x);
	if (dobry) {
		pos[++posL] = currentLine;
		currentLine = getLine[v];
	}
}

void sejda(int cur) {
	if (first[cur] == "ADD") ADD(cur);
	if (first[cur] == "SUB") SUB(cur);
	if (first[cur] == "MUL") MUL(cur);
	if (first[cur] == "DIV") DIV(cur);
	if (first[cur] == "MOV") MOV(cur);
	if (first[cur] == "PRN") PRN(cur);
	if (first[cur] == "CMP") CMP(cur);
	if (first[cur] == "JE") JMP(cur, isJE, 2);
	if (first[cur] == "JMP") JMP(cur, true, 3);
	if (first[cur] == "JNE") JMP(cur, isJNE, 3);
	if (first[cur] == "JGE") JMP(cur, isJGE, 3);
	if (first[cur] == "JLE") JMP(cur, isJLE, 3);	
}

void printAll() {
	printf ("Registers\n");
	for (int i = 0; i < 8; ++i)
		printf ("%d\n", reg[i]);
	printf ("Memory\n");
	for (int i = 0; i < 256; ++i) 
		printf ("%d\n", mem[i]);
	printf ("\n");
}

void startTheProgram() {
	printf ("Program has started!\n");
	currentLine = startLine;
	currentLine++;
	while (true) {
		printf ("in line %d\n", currentLine + 1);
		
		if (currentLine >= dataL || first[currentLine] == "exit") {
			printAll();
			exit(0);
		}
		if (first[currentLine] == "JR") 
			currentLine = pos[posL], posL--;
		else if (actB[currentLine]) sejda(currentLine);
		currentLine++;
	}
}

int main() {
	readAllLines();
	getFirst();
	compilationErrors();
	if (cntMain != 1) {
		printf ("You don't have OR have more than 1 main functions!"); 
		exit(0);
	}
	startTheProgram();
	return 0;
}
