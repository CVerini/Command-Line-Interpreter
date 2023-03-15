/*
Programmed by Cameron Verini

This program performs basic Linux command line interpretations and executions.
It also has the building blocks for a Virtual Memory Manager.
*/

#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cstring>
#include <alloca.h>
#include <thread>
#include <cstdlib>
#include <time.h>
#include <chrono>

#define PAGE_SIZE 4906						//bytes in each page
#define TABLE_SIZE 5						//pages per table
#define VM_AVAIL (PAGE_SIZE * TABLE_SIZE)	//Total Virtual Memory Available

struct process
{
	int id;
	int uses;
	int memory;
	int state = 0;
};

int curVM = 0;		//current amount of Virtual Memory used
int numProc = 0;	//amount of processes created
process pageTable[TABLE_SIZE];

using namespace std;

string commands;

void run(int procID)
{
	//adds one use to all pages that contain part of the process
	int p;
	cout << "\nRunning process " << procID << "..." << endl;
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (procID == pageTable[i].id)
		{
			pageTable[i].uses += 1;
			pageTable[i].state = 0;
			p = i;
		}
	}
	cout << "\nProcess " << procID << " has now been run " << pageTable[p].uses << " time(s)." << endl;
}

void firstComeFirstServe(int procs[], double sec)
{
	int temp;
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (procs[0] == pageTable[i].id)
		{
			pageTable[i].state += 1;
		}
	}
	for (int i = 0; i < TABLE_SIZE; i++)
	{		
		if (procs[i] != 0)
		{
			for (int j = 0; j < TABLE_SIZE; j++)
			{
				if (procs[i] == pageTable[j].id)
				{
					pageTable[j].state += 1;
					temp = j;
				}
			}
			auto start = std::chrono::system_clock::now();
			while(pageTable[temp].state < 2)
			{
				sleep(sec);
				auto end = std::chrono::system_clock::now();
				std::chrono::duration<double> diff = end - start;
				if (diff.count() > 1)
				{
					cout << "ERROR: STARVATION! CLOSING PROGRAM...\n";
					exit(1);
				}
			}
			std::thread runProc(run, procs[i]);
			runProc.detach();
			sleep(sec);
		}
		if (i < TABLE_SIZE)
		{
			for (int j = 0; j < TABLE_SIZE; j++)
			{
				if (procs[i + 1] == pageTable[j].id)
				{
					pageTable[j].state += 1;
				}
			}
		}
		
		
	}
}

void addProcess()
{
	numProc++;
	int mem = rand() % 9000 + 1000;	//randomly determines memory of new process

	cout << "\nProcess " << numProc << " added (process required " << mem <<" total bytes).\n\n";

	while (mem > 0)
	{
		int use = pageTable[0].uses;
		int place = 0;
		//determines which pages the new process will occupy
		for (int i = 1; i < TABLE_SIZE; i++)
		{
			if (pageTable[i].uses < use && pageTable[i].id != numProc)
			{
				use = pageTable[i].uses;
				place = i;
			}
		}
		//deletes processes that are being overwritten to avoid segmentation
		int idDel = pageTable[place].id;
		for (int j = 0; j < TABLE_SIZE; j++)
		{
			if (pageTable[j].id == idDel) pageTable[j] = { -1, -1, 0 };
		}
		
		if (mem > 4906)
		{
			pageTable[place] = { numProc, 0, 4906 };
		}
		else
		{
			pageTable[place] = { numProc, 0, mem };
		}
		mem -= 4906;
	}
}

void showTable()
{
	curVM = 0;
	cout << "\n\n\t\t\tPAGE TABLE\n########################################################################";
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		//shows page if it is not empty
		if (pageTable[i].id > -1)
		{
			cout << "\n\tPAGE " << i+1 << "|\tPROCESS ID: " << pageTable[i].id << "|\tUSES: "
				<< pageTable[i].uses << "|\tMEMORY (BYTES): " << pageTable[i].memory;
			curVM += pageTable[i].memory;
		}
		else
		{
			//prints if page is empty
			cout << "\n\tPAGE " << i + 1 << ":\t EMPTY";
		}
	}
	cout << "\n\nTOTAL MEMORY USED: " << curVM << "/" << VM_AVAIL
		<< "\n########################################################################\n\n";
}

void runProcesses()
{
	int procID = -1;
	int options[TABLE_SIZE];
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		options[i] = -1;
	}

	bool skip = false;
	//checks if table is empty
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (pageTable[i].id > procID) procID = pageTable[i].id;
	}
	if (procID == -1)
	{
		cout << "\n*****ERROR: NO PROCESSES IN PAGE TABLE!*****\n\n";
	}
	else 
	{
		int selections[TABLE_SIZE];
		int selection;
		int processes = 0;
		int p;
		//checks what processes are available based on process ID in pages and puts available processes in array
		for (int i = 0; i < TABLE_SIZE; i++)
		{
			skip = false;
			for (int j = 0; j < TABLE_SIZE; j++)
			{
				if (pageTable[i].id == options[j])
				{
					skip = true;
					break;
				}
			}
			if (skip != true)
			{
				options[processes] = pageTable[i].id;
				processes++;
			}
		}

		cout << "\nSelect which processes to run (enter 0 to start running processes):\n";
		//displays available processes and sets all selections to 0
		for (int i = 0; i < TABLE_SIZE; i++)
		{
			selections[i] = 0;
			if (options[i] > 0)
			{
				cout << "Process " << options[i] << endl;
			}
		}
		int numRun = 0;
		while (selection != 0)
		{
			int valid = 0;
			cin >> selection;
			//if selection is invalid, error occurs
			for (int i = 0; i < TABLE_SIZE; i++)
			{
				if (selection == options[i]) valid++;
			}
			if (selection != 0 && valid == 0)
			{
				cout << "\n*****ERROR: INVALID OPTION*****\n";
			}
			else
			{
				int invalid = 0;
				for (int i = 0; i < TABLE_SIZE; i++)
				{
					if (selection == selections[i] && selection != 0) invalid++;
				}
				if (invalid != 0)
				{
					cout << "\n***** ERROR: PROCESS ALREADY SET TO RUN***** \n";
				}
				else 
				{
					if (selection != 0)
					{
						selections[numRun] = selection;
						cout << "\nProcess " << selections[numRun] << " is set to run. What other processes do you wish to run? (enter 0 to start running): \n";
						numRun++;
					}
				}
			}
		}
		auto start = std::chrono::system_clock::now();
		cout << "\nEstimating process time..." << endl;
		auto end = std::chrono::system_clock::now();

		std::chrono::duration<double> diff = end - start;

		firstComeFirstServe(selections, diff.count());
	}
	
}

//exit function
void exit()
{
	exit(0);
}

//funciton to perform commands
string interpreter(string com)
{
	char commands[com.size()+1];
	string end = "";

	//helps check for "quit"
	if(com.size() > 3)
	{
		end = com.substr(com.size()-4, com.size());
	}
	
	strcpy(commands,com.c_str());
	
	//sends commands to system to perform commands
    if (com == "quit")
	{
       return end;
    }
	if (com == "table")
	{
		showTable();
		return end;
	}
	if (com == "new")
	{
		addProcess();
		return end;
	}
	if (com == "run")
	{
		runProcesses();
		return end;
	}
	system(commands);
	return end;
}

int main()
{
	srand(time(0));
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		pageTable[i] = {-1, -1, 0};
	}
	while(1)
	{
		//input
		cout << "Enter a command (\"new\" for new process, \"run\" to run processes, \"table\" to display page table, \"quit\" to exit): " << endl;
		getline(cin, commands);
		
		//sends commands to line interpreter and checks for "quit command" 
		string command = interpreter(commands);
		
		//cout << "quit the application" << endl;
		if(command == "quit") 
		{
            cout << "Application closed" << endl;
		    exit();
		}
	}
	
return 0;
}

