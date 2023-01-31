#include "HGS_harness_program.h"


bool printRunningScenario = false; //enable this to print which index is getting executed
unsigned int p1_freq;

void(*classFuncPointers[7])(long) {
		& runClass0,
		& runClass1,
		& runVNNI, 
		& runClass3,
		& runUserMWait,
		& runINT8,
		& harnessSleep
};

/////////////////// private declerations
typedef struct indexedOperations* IndexedOperations;
IndexedOperations ClassifiedOperationsCreate(Index,long);
void executeOperations(IndexedOperations,long);
int findInString(char*, char);
void runClassX_wrapper(Index,long);
void setupTimer();
unsigned int getProccessorP1Freq();


//the main idea of the algorithm: for each scenario we need, a linked list of indexed operations is created (circular)
//for example, if we have a:   6 seconds class0, 
//							   3 seconds class3,
//							   9 seconds class2
//we will have 3 items in the linkedlist: class0,6sec -> class3,3sec -> class2,9sec -> ptr to firstnode

struct indexedOperations {
	Index index;
	long duration;
	//other proprties- for future use
	IndexedOperations next; //next operation in the given scenario
};
//struct "constructor"
IndexedOperations ClassifiedOperationsCreate(Index ind, long timeOut)
{
	IndexedOperations tmp = (IndexedOperations)malloc(sizeof(*tmp));
	if (tmp) {//initialize parameters
		tmp->index = ind;
		tmp->duration = timeOut;
		tmp->next = NULL;
	}
	return tmp;
}
//Free mallocs
void releaseIndexedOperationsList(IndexedOperations op) { 
	IndexedOperations itr = op;
	while (op) {
		itr = op;
		op = op->next;
		free(itr);
	}
}


//the heart of the program, argv is analysed here
void initProgram(int argc, char** argv) {
	
	setupTimer();

	IndexedOperations head =NULL,
				      itr  =NULL;
	long timeOut = 0;
	
												//   Parse string
	for (int i = 1;i < argc;i++) {
		if (strcmp(argv[i], "-c") == 0) {
			for (int j = i + 1;j < argc;j++) {

				char* str = argv[j];
				Index ind;
				if (strcmp(str, "-t") == 0) {
					i = j - 1;
					break;
				}
				switch (str[0]) {//only use first letter to know which function to invoke
				case 'V': ind = VNNI; break;
				case 'v': ind = VNNI; break;
				case 'I': ind = INT8_FUNC;break;
				case 'i': ind = INT8_FUNC;break;
				case 'U': ind = USERMWAIT; break;
				case 'u': ind = USERMWAIT; break;
				case 'P': ind = PAUSE; break;
				case 'p': ind = PAUSE; break;
				case 's': ind = HARNESSSLEEP; break;
				case 'S': ind = HARNESSSLEEP; break;
				default: // in case number of class is provided
					ind = (Index)(str[0] - '0'); break;
				}
				int commaIndex = findInString(str, ',');
				if (ind > CLASS_MAX || ind < CLASS_MIN || commaIndex == -1 || 
					commaIndex >= 90 /*in case comma was too far (="USERMWAIT".len)*/)
				{
					printf("Invalid Input\nUsage: HGSharnessapp.exe -c <class,duration_ms> <class,duration_ms> ... [-t <timeout_ms>]");
					releaseIndexedOperationsList(head);
					exit(1);
				}
				long classTime = atoi(&str[commaIndex+1]);
				if (classTime < 0)
				{
					printf("Invalid Input\nUsage: HGSharnessapp.exe -c <class,duration_ms> <class,duration_ms> ... [-t <timeout_ms>]");
					releaseIndexedOperationsList(head);
					exit(1);
				}
				else if (classTime == 0) 
					classTime--;
				IndexedOperations op = ClassifiedOperationsCreate(ind, classTime);
				if (!op) {
					//malloc failed, quit
					releaseIndexedOperationsList(head);
					exit(1);
				}
				if (itr == NULL)
					itr = head = op;
				else {
					itr->next = op;
					itr = op;
				}
			}
		}
		else if (strcmp(argv[i], "-t") == 0) {
			if (i+1 >= argc)
			{
				printf("Invalid Input\nUsage: HGSharnessapp.exe -c <class,duration_ms> <class,duration_ms> ... [-t <timeout_ms>]");
				releaseIndexedOperationsList(head);
				exit(1);
			}
			timeOut = (long)atoi(argv[++i]);
		}
	}

	if (!itr)
	{
		printf("Invalid Input\nUsage: HGSharnessapp.exe -c <class,duration_ms> <class,duration_ms> ... [-t <timeout_ms>]");
		exit(1);
	}

	itr->next = head;							// set the scencrio circular last.next = first

	executeOperations(head, timeOut);
}

void setupTimer() {
	p1_freq = getProccessorP1Freq();
}
unsigned int getProccessorP1Freq() {
	//Conversions
	std::string str1 = std::string("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0");
	std::wstring locTmpStr = std::wstring(str1.begin(), str1.end());
	LPCWSTR locTmp = locTmpStr.c_str();
	std::string str2 = std::string("~MHz");
	std::wstring HKEYTmpStr = std::wstring(str2.begin(), str2.end());
	LPCWSTR HKEYTmp = HKEYTmpStr.c_str();
    ////// end conversions
	char Buffer[_MAX_PATH];
	DWORD BufSize = _MAX_PATH;
	DWORD dwMHz = _MAX_PATH;
	HKEY hKey;
	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, locTmp,0,KEY_READ,&hKey);
	if (lError != ERROR_SUCCESS)
	{
		printf("Couldn't get p1_freq, Error num %ld\n",lError);
		exit(1);
	}
	// query the key:
	RegQueryValueEx(hKey, HKEYTmp, NULL, NULL, (LPBYTE)&dwMHz, &BufSize);
	return dwMHz*1000;
}


int findInString(char* str, char toFind) {
	if (!str) return -1;
	int res = 0;
	while (str[res] != '\0')
	{
		if (str[res] == toFind)
			return res;
		res++;
	}
	return -1;
}
void printStart(enum Index ind,long duration) {
	if (printRunningScenario) {
		switch (ind) {
		case VNNI:printf("Running Class 2");break;
		case INT8_FUNC:printf("Running INT8");break;
		case USERMWAIT:printf("Running UserMWait");break;
		case PAUSE:printf("Running Class 3-PAUSE");break;
		case HARNESSSLEEP:printf("Sleeping start");break;
		default:printf("Running Class %d", (int)ind);break;
		}
		if (duration != -1) printf(" for %d[msec]", duration);
		printf("...\n");
	}
}
void printFinished(enum Index ind)
{
	if (printRunningScenario) {
		switch (ind) {
		case VNNI:printf("Class 2 finished\n");break;
		case INT8_FUNC:printf("INT8 finished\n");break;
		case USERMWAIT:printf("UserMWait finished\n");break;
		case PAUSE:printf("Class 3-PAUSE finished\n");break;
		case HARNESSSLEEP:printf("Sleeping end\n");break;
		default:printf("Class %d finished\n", (int)ind);break;
		}
	}
}



void executeOperations(IndexedOperations op,long timeOut) {
	if (!op)
		return;
	if (printRunningScenario) {
		if (timeOut != 0) printf("Test Duration %llu [msec]\n", timeOut);
		else  printf("Test will run forever\n");
	}
	bool endlessScenario = (timeOut == 0);
	IndexedOperations itr = op;
	while ((endlessScenario || itr)) {
		if ((!endlessScenario && timeOut <= itr->duration) 
			|| (!endlessScenario && itr->duration==-1)) itr->duration = timeOut;

		if (!endlessScenario)
			timeOut -= itr->duration;

		//__int64 st = __rdtsc(), end;

		runClassX_wrapper(itr->index, itr->duration);

		//end = __rdtsc();
		//if (itr->index == HARNESSSLEEP) printf("len %f\n", (double)(end - st) / p1_freq);

		if (!endlessScenario && timeOut <= 0)
			break;
		itr = itr->next;
	}
	
	//free memory if we get here //critical // cut circle
	if (op->next) {//no need for if, but keeping it for future
		auto tmp = op;
		op = op->next;
		tmp->next = nullptr;
	}

	releaseIndexedOperationsList(op);
}

void runClassX_wrapper(Index ind,long duration) {//this function might get modified on upgrades
	printStart(ind, duration);

	classFuncPointers[(int)ind](duration);

	printFinished(ind);
}










