// Git.cpp : This file contains the 'main' function. Program execution begins and ends there.
//



#include <iostream>
#include "CreateProject.h"
#include "Commit.h"
#include "Push.h"
#include "branch.h"

//VERSION
std::string VERSION = "v0.1.0";

//DEBUG
std::string command = "commit";
std::string path = "C:\\Users\\K754a\\Desktop\\test\\test";

//std::string command
int main(int argc, char** argv)
{

	////check if we have less than the needed input
	//if (argc < 2)
	//{
	//	std::cout << "Error! Please provide a command" << std::endl;
	//	return 1;
	//}
	//
	////get the command (array 1)
	//std::string command = argv[1];

	////path
	//std::string path = (argc >= 3) ? argv[2] : "";

	//std::transform(path.begin(), path.end(), path.begin(), [](char c) {
	//	return (c == '/') ? '\\' : c;
	//	});

	//std::cout << "PATH: " + path << std::endl;

	

	if (command == "commit")
	{
		commit(path);
	}
	else if(command == "push")
	{
		push(path);
	}
	else if (command == "createproj")
	{
		dir(path);
	}
	else if (command == "createbranch")
	{
		branch(path);
	}
	else if (command == "listbranch")
	{
		listbranch();
	}


	else if (command == "info")
	{
		std::cout << "List of commands: " << std::endl;
		std::cout << " commit (dir) \n push (dir) \n createproj (dir) \n createbranch (dir)" << std::endl;
		std::cout << "Version: " << VERSION << std::endl;
	}
	else {
		std::cout << "Error! Please provide a valid command" << std::endl;
	}

	return 0;
}






