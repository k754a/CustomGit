#include "Commit.h"
#include <Windows.h>
#include <list>
namespace fs = std::filesystem;
#include <sstream>
#include "branch.h"

// Helpers declared in Commit.h and defined in Commit.cpp

int branch(std::string path)
{
	std::ifstream file;
	//check for a project, cause we don't want to commit "aka save" without a project

	file.open(path + "\\proj");
	if (!file) //if a project is there already
	{
		std::cout << "ERROR, There is no project at " + path + " that exist. " << std::endl; return -1;
	}


    std::string name, message;

    std::cout << "What would you like to call this branch of project? " + path + "\n>";

    std::getline(std::cin >> std::ws, name); std::cout << std::endl;

    std::cout << "Making Branch, what should the info message be? \n>";

    std::getline(std::cin >> std::ws, message); std::cout << std::endl;


	
	//same thing as the commits, but with some extra stuff
	//we create a dir to store the branch, branches will not have any compression,
	//they should be like another version of the project, with everything else copied.


	std::cout << "Creating The Branch" << std::endl;







	std::cout << "Checking if VERSIONS Directory Exists." << std::endl;

	if (CommitPathWorks(path + "\\.VERSIONS"))
	{
		std::cout << "VERSIONS Directory Exists." << std::endl;

	}
	else
	{
		std::cout << "The VERSIONS Directory Does not exist." << std::endl;
		std::cout << "Creating it now in " + path << std::endl;

		std::string commitpos = (path + "\\" + ".VERSIONS");

		_mkdir(commitpos.c_str());


	}







	//set the list of files
	std::list<std::string> filesToCommit;





	//find all the files in the project.
	for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ path })
	{
		//we need everything in this project so we comment this out.
		//if (!((dir_entry.path().string().find(".COMMIT") != std::string::npos) || (dir_entry.path().string().find("proj") != std::string::npos)))
		//{
			std::cout << dir_entry << '\n';

			filesToCommit.push_front(dir_entry.path().string());
		//}

	}



	//now we check if the files are the same as the most recent branch version

	std::list<std::string> lastCommit;


	//find the latest folder to make sure we are checking the 2 files

	std::string oldest_name;

	bool first = true;
	for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ path + "\\" + ".VERSIONS" })
	{
		std::string foldername = dir_entry.path().filename().string();


		if (first || std::atoi(foldername.c_str()) > std::atoi(oldest_name.c_str()))
		{
			oldest_name = foldername;
			first = false;
		}



	}


	for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ path + "\\" + ".VERSIONS" + "\\" + oldest_name })
	{

		std::cout << dir_entry << '\n';

		lastCommit.push_front(dir_entry.path().string());
	}



	//Disabled cause user might want multiple branches of the same main.
	///////////
	/*
	//now we need to check the hash contents of that file vs the main commit stream and see if there is any changes, if there isnt, we stop.


	//we can do this though combining the file data, then hashing it


	//the commit hash

	std::string COMMITHASH;
	for (auto const& pathssaved : filesToCommit)
	{
		std::ifstream file(pathssaved);




		if (file.is_open())
		{
			std::stringstream buffer;

			buffer << file.rdbuf();

			COMMITHASH = COMMITHASH + buffer.str();


		}
	}

	std::hash<std::string> comH;

	size_t commitcompare = comH(COMMITHASH);




	std::cout << commitcompare << std::endl;


	std::string OLDCOMMITHASH;
	for (auto const& pathssaved : lastCommit)
	{
		std::ifstream file(pathssaved);

		std::cout << pathssaved << std::endl;


		if (file.is_open())
		{
			std::stringstream buffer;

			buffer << file.rdbuf();

			OLDCOMMITHASH = OLDCOMMITHASH + buffer.str();


		}
	}

	std::hash<std::string> oldcomH;

	size_t oldcommitcompare = oldcomH(OLDCOMMITHASH);






	std::cout << oldcommitcompare << std::endl;






	if (oldcommitcompare == commitcompare)
	{
		std::cout << std::endl; std::cout << std::endl;
		std::cout << "Nothing has changed since last commit, ending." << std::endl;
		return 0;
	}








	*/


	

	//std::cout << "Commiting: " << std::endl;


	//final confirm.
	bool loop = true;

	while (loop)
	{

		std::string answer;

		std::cout << "Are you sure you want to Create This Branch? Y/N" << std::endl;
		std::getline(std::cin >> std::ws, answer); std::cout << std::endl;

		if (answer == "N" || answer == "n")
		{
			std::cout << "Stopping" << std::endl;
			return 0;
		}
		if (answer == "Y" || answer == "y")
		{
			loop = false;
		}
	}

	//create the commit file in the .COMMIT and upload data to it. 
	//lets use the date as its special signature, cause its never gonna happen again.


	//first clear old commits


	/*for (const auto& entry : std::filesystem::directory_iterator(path + "\\" + ".COMMIT"))
		std::filesystem::remove_all(entry.path());*/





	

	_mkdir((path + "\\" + ".VERSIONS" + "\\" + name).c_str());

	std::cout << "Creating Branch." << std::endl;



	copyFiles(filesToCommit, (path + "\\" + ".VERSIONS" + "\\" + name), path);



	std::cout << "Branch Made." << std::endl;






	//Make description
	std::ofstream Discript(path + "\\" + ".VERSIONS" + "\\" + name + "\\" + "Discript.branch");
	if (Discript.is_open()) {
		Discript << message << std::endl;
	}


	Discript.close();


	//WRITE TO MAIN ABOUT THE BRANCH
	


	
	//Create a buffer to hold the Path
	char exePathBuf[MAX_PATH] = { 0 };

	//Get the path of the executable
	DWORD len = GetModuleFileNameA(NULL, exePathBuf, MAX_PATH);

	//Convert to std::filesystem::path and remove the filename to get the directory 
	std::filesystem::path exeDir = std::filesystem::path(std::string(exePathBuf)).remove_filename();

	//add the branches.list file type
	std::filesystem::path branchesPath = exeDir / "branches.list";

	
	std::ofstream pathfile(branchesPath.string(), std::ios::app);
	
	if (pathfile.is_open())
	{
	
		pathfile << path << "\\" << ".VERSIONS" << "\\" << name << " | ";
	

		pathfile.close();
	}
	system("cls");

	std::cout << "Branch " + name + " Created Successfully at " + path + "\\" + ".VERSIONS" + "\\" + name << std::endl;

	std::cout << "Branch Info: " + message << std::endl;

	std::cout << std::endl;
	std::cout << "If you want to see all branches check - " + exeDir.string() + "branches.list" << std::endl;
	std::cout << "Or you can check with:  git branch list" << std::endl;


	//std::cout << "THE PATH IS: " + system("echo $PWD") << std::endl;
















	return 0;
}////////////////////


int listbranch()
{
	//Create a buffer to hold the Path
	char exePathBuf[MAX_PATH] = { 0 };
	//Get the path of the running program (so our git)
	DWORD len = GetModuleFileNameA(NULL, exePathBuf, MAX_PATH);
	//Convert to std::filesystem::path and remove the filename to get the directory 
	std::filesystem::path exeDir = std::filesystem::path(std::string(exePathBuf)).remove_filename();
	//add the branches.list file type
	std::filesystem::path branchesPath = exeDir / "branches.list";


	std::cout << "BRANCH LIST: " << std::endl;

	std::ifstream branch;
	branch.open(branchesPath);

	std::string line;
	while (std::getline(branch, line)) {
		std::cout << ">> " << line << std::endl;
		std::cout << "" << std::endl;
	}

	return 0;
}

