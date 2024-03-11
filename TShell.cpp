// code by: Blake Cannoe
// TShell v0.1
// March 10th, 2024
// A small linux shell program I made to learn more about how the linux file system works, and to familiarize myself with C++ libraries.

// Libraries
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <bits/stdc++.h>
#include <vector>
#include <dirent.h>

using namespace std;

// Function Prototypes
void listProcess();
void killProcess(int);
void makeFile(string, string, string);
void removeFile(string, string);
void help();

int main() {
	// Important variables
	size_t pos = 0;
	string request;
	string cmd;
	string token;
	string path;
	string modifier;
	string curPath = "/home";
	string text = "";
	vector<string> history;
	string delim = " ";
	// Main loop everything happens in here.
	do{
		

		cout << curPath << " > ";
		getline(cin, request);
		history.push_back(request); 									// Adds each request to history to be viewed by user.
		int count = 0;
		stringstream ss(request);
		
		while (getline(ss, token, ' ')) {
			
			// First token will always be the command.
			if (count == 0){
				cmd = token;
			}

			// The second token could be a few different things.
			else if (count == 1){
				// If the second token starts with a "/" character it is going to be a directory.
				if (token[0] == '/') {
					
					// Check if the directory exists.
					if (filesystem::exists(token)) {
						path = token;
					} else {
						cout << "Invalid Filepath" << endl;
					}

				}
				// Otherwise the token will be handled when processing the command.
				else {
					modifier = token;
				}
			} 
			// The program will count a second time if there are more than 2 tokens so it can get picked
			// up by the other if statement after breaking the loop.
			else if (count > 1){
				count++;
				break;
			}
			count++;
		}
		// The input after this point is text to be placed into a file.
		if (count == 3){
			// Everything after the ":" is text to go into the file.
			char delimiter = ':';
			
			size_t delimiterPos = request.find(delimiter);

			if (delimiterPos != string::npos) {
				text = request.substr(delimiterPos + 1);
			} else {
				cerr << "Text not found" << endl;
			}
		}
		// Command to change the current path.
		if (cmd == "cpath") {
			curPath = path;
		}
		// Command to make a new file.
		else if (cmd == "mk") {
			makeFile(text, curPath, modifier);
		}
		// Command to remove a file.
		else if (cmd == "rm") {
			removeFile(curPath, modifier);
		}
		// Command to view a list of processes. (WIP)
		else if (cmd == "pl") {
			listProcess();
		}
		// Command to kill a process. (WIP) (unsafe)
		else if (cmd == "pk") { 
			int pid = stoi(modifier);
			killProcess(pid);
		}
		// Command to list out the current directory.
		else if (cmd == "lpath") {
			for (const auto & entry : filesystem::directory_iterator(path)){
				cout << entry.path() << endl;
			}
		}
		// Command to view list of previous commands.
		else if (cmd == "history") {
			for (int i = 0; i < history.size() - 1; i++) {
				cout << i << ". " << history[i] << endl;
			}
		}

		else if (cmd == "help") {
			help();
		}

		// TODO: Copy file
		// TODO: Move file
		// TODO: Concatinate two files
		// TODO: Append file
		// TODO: Make directory
		// TODO: Move directory

		else if (cmd == "quit");
		
		else {
			cout << "Invalid Command" << endl;
		}
	
	}while(cmd != "quit");

	return 0;
}

void listProcess() {
	DIR *dir;
	struct dirent *entry;
	// Unix systems will have a directory with the process Id's inside will not work on windows. It may be slightly different based
	// on Linux distribution, this was written on a machine running Ubuntu 22.04.4
	dir = opendir("/proc");
	if (dir == nullptr) {
		perror("opendir");
		
	}

	cout << "Information on running processes:" << endl;
	int count = 0;
	while ((entry = readdir(dir)) != nullptr) {
		if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
			string pid = entry->d_name;
			string cmdline_path = "/proc/" + pid + "/cmdline";		// Cmdline contains more information about the names of the processes which will make it easier to know what each process is doing.
			ifstream cmdline_file(cmdline_path);

			if (cmdline_file.is_open()) { 
				string process_name;
				getline(cmdline_file, process_name, '\0');
				if (count < 5) {
					cout << "PID: " << pid << ", Process Name: " << process_name << "\t\t";
					count++;
				} else {
					count = 0;
					cout << "PID: " << pid << ", Process Name: " << process_name << endl;
				}

				cmdline_file.close();
			}
		}
	}

	closedir(dir);
}

void killProcess(int pid) {
	pid_t process = pid;

	if (kill(process, SIGTERM) == 0) {						// If a process is killed it should still clean up the utilized resources, so it is more of "Terminate" process rather than "Kill".
		cout << "Process with ID " << process << " terminated successfully." << endl;
	} else {
		perror("Error Terminating process");
	}
}
// Create a file and place some text inside of it,
// if the user does not specify text it will create an empty file.
void makeFile(string text, string path, string title) {
	string output = path + "/" + title;
	ofstream outFile(output);
	if (outFile.is_open()) {
		outFile << text << endl;
		outFile.close();
		cout << "File created successfully" << endl;
	} else {
		cerr << "Unable to create file" << endl;
	}
}
// Removes a file from the file directory, this is potentially unsafe as well. It would be an interesting addition to
// this project to try and make this command more safe.
void removeFile(string path, string title) {
	string output = path + '/' + title;
        try {
		filesystem::remove(output);
        } catch (const filesystem::filesystem_error& ex) {
    		cerr << "Unable to delete the file: " << ex.what() << endl;
        }

}

void help() {
	cout << "List of commands and a brief description" << endl;
	cout << "(cpath /path/to/directory) will change your current directory relative paths are not supported yet.\n" << endl; // The '\n' was intentional
	cout << "(mk filename.extention :text) will create a new file everything after the ':' symbol will go in the file.\n" << endl;
       	cout << "(rm filename.extention) will delete the intended file.\n" << endl;
	cout << "(pl) will list all active process ID's currently the output looks bad and needs to be cleaned up.\n" << endl;
	cout << "(pk 12345) will terminate the process given the process ID in place of '12345' this is currently unsafe and may crash your system.\n" << endl;
	cout << "(lpath) will list all the folders and files in your current directory.\n" << endl;
	cout << "(history) will list previous commands that the user gave to the program,\n" << endl;
	cout << "(help) will take the user to this list.\n" << endl;
	cout << "(quit) will terminate the program.\n" << endl;	
}
