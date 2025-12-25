#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <list>
#include<direct.h>

#ifndef COMMIT_H
#define COMMIT_H

int commit(std::string pos);

// Shared helpers used by commit and branch
bool CommitPathWorks(const std::string pathstring);
void copyFiles(const std::list<std::string>& sourceFiles, const std::string& destinationDir, const std::string& root);


#endif


