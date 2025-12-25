#include "Commit.h"
#include <Windows.h>
#include <list>
namespace fs = std::filesystem;
#include <sstream>

bool CommitPathWorks(const std::string pathstring)
{
	fs::path check(pathstring);
	return fs::exists(check);
}

void copyFiles(const std::list<std::string>& sourceFiles, const std::string& destinationDir, const std::string& root) {

	//we need to scroll through each file, the copy it

	//TODO - In the future, this is the best spot to git ignore.


	for (const fs::path& eachfile : sourceFiles)
	{
		std::cout << "Committing >> " << root << "\\" << eachfile << std::endl;
		
		//basic error handling
		try
		{
			//copy goes, file, dest, params
			fs::copy_file(eachfile, destinationDir / eachfile.filename());
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}



}

int commit(std::string path)
{
	std::string commitpos = (path + "\\" + ".COMMIT"); //this is so we don't need to calculate this every bit

	//now we need to find all the files in the project, and check we have the dir for commits
	//compress to format, then save it with an id tag maybe at the top, lets do time
	//then we want to stick this save state into a commits folder that saves everything.
	//and compare files for changes and set this in the state thing.



	std::ifstream file;
	//check for a project, cause we don't want to commit "aka save" without a project

	file.open(path + "\\proj");
	if (!file) //if a project is there already - if not, we cannot commit, so we end.
	{
		std::cout << "ERROR, There is no project at " + path + " that exist. " << std::endl; std::cout << "Run git createproj" << path << std::endl; return -1;
	}
	else {

		//if we can find the project, give some user telem.

		std::cout << "Running Commit." << std::endl;


		std::cout << "Checking if Commit Directory Exists." << std::endl;
	}

	









	if (CommitPathWorks(path + "\\.COMMIT"))
	{
		std::cout << "Commit Directory Exists." << std::endl;

	}
	else
	{
		//our dir doesn't exist. lets make it
		std::cout << "The Commit Directory Does not exist." << std::endl;
		std::cout << "Creating it now in: " + path << std::endl;



		//grab the commitpos
		

		_mkdir(commitpos.c_str() );

		//make it hidden, just for developer cleaness

		DWORD attributes = GetFileAttributesA(commitpos.c_str());

		if (attributes != INVALID_FILE_ATTRIBUTES)
		{
			SetFileAttributesA(commitpos.c_str(), attributes | FILE_ATTRIBUTE_HIDDEN);
			std::cout << "Made .Git a hidden file." << std::endl;
		}
		else {
			std::cout << "--ERROR-- Could not make .COMMIT a hidden file, continuing." << std::endl;
		}
	}







	//set the list of files
	std::list<std::string> filesToCommit;





	//find all the files in the project.
	for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ path })
	{

		if (!((dir_entry.path().string().find(".COMMIT") != std::string::npos) || (dir_entry.path().string().find("proj") != std::string::npos)))
		{
			std::cout << dir_entry << '\n';

			filesToCommit.push_front(dir_entry.path().string());
		}

	}



	//now we check if the files are the same as the most recent commit dir

	std::list<std::string> lastCommit;


	//find the latest folder to make sure we are checking the 2 files

	std::string oldest_name;
   
	bool first = true;
	for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ commitpos })
	{
		std::string foldername = dir_entry.path().filename().string();

		
		if (first || std::atoi(foldername.c_str()) > std::atoi(oldest_name.c_str())) 
		{
			oldest_name = foldername;
			first = false;
		}
		


	}

	
	for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ commitpos + "\\" + oldest_name})
	{

			std::cout << dir_entry << '\n';

			lastCommit.push_front(dir_entry.path().string());
	}








	//now we need to check the hash contents of that file vs the main commit stream and see if there is any changes, if there isn't, we stop.


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









	//std::cout << "Committing: " << std::endl;


	//final confirm.
	bool loop = true;
	
	while (loop)
	{

		std::string answer;

		std::cout << "Are you sure you want to Commit these files? Y/N" << std::endl;
		std::getline(std::cin >> std::ws, answer);std::cout << std::endl;

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


	/*for (const auto& entry : std::filesystem::directory_iterator(commitpos))
		std::filesystem::remove_all(entry.path());*/





	time_t timestamp;

	std::string committime = std::to_string(time(&timestamp));

	//we make the dir before
	_mkdir((commitpos + "\\" + committime).c_str());

	std::cout << "Creating Commit." << std::endl;



	copyFiles(filesToCommit, (commitpos + "\\" + committime), path);



	std::cout << "Files committed." << std::endl;








	












	return 0;
}////