#include "SrcMain.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "sha1.h"
#include "HashTable.hpp"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

void myShaFunc(std::string word){
    
}
void ProcessCommandArgs(int argc, const char* argv[])
{
	// TODO: Implement
    if(strcmp(argv[1], "dictionary") == 0){
        std::string inputFile = argv[3];
        std::ifstream file(inputFile);
        std::string line;
        std::vector<std::string> originalPass;
        while(true){
            std::getline(file,line);
            if(file.fail()){
                break;
            }
            originalPass.emplace_back(line);
        }
        HashTable<std::string> table(originalPass.size());
        for(int i = 0; i < originalPass.size(); i++){
            unsigned char hash[20];
            sha1::Calc(originalPass[i].c_str(), originalPass[i].length(), hash);
            std::string hexStr;
            hexStr.resize(40);
            sha1::ToHexString(hash, &hexStr[0]);
            table.Insert(hexStr, originalPass[i]);
            //std::cout << originalPass[i] << " " << hexStr << std::endl;
        }
        std::string passFile = argv[4];
        std::ifstream ofile(passFile);
        std::string passLine;
        std::vector<std::string> outVec;
        while(true){
            std::getline(ofile, passLine);
            if(ofile.fail()){
                break;
            }
            outVec.emplace_back(passLine);
        }
        std::ofstream outputFile("solved.txt");
        for(int i = 0; i < outVec.size(); i++){
            //std::cout << table.Find(outVec[i]) << std::endl;
            outputFile << outVec[i] << "," << *table.Find(outVec[i]) << std::endl;
            
        }
    }
    
    else if (strcmp(argv[1],"phrases") == 0){
        std::string inputFile = argv[3];
        std::ifstream file(inputFile);
        std::string line;
        std::vector<std::string> originalPass;
        while(true){
            std::getline(file,line);
            if(file.fail()){
                break;
            }
            originalPass.emplace_back(line);
        }
        std::string passFile = argv[4];
        std::ifstream ofile(passFile);
        std::string passLine;
        std::vector<std::string> outVec;
        while(true){
            std::getline(ofile, passLine);
            if(ofile.fail()){
                break;
            }
            outVec.emplace_back(passLine);
        }
        HashTable<std::string> table(std::stoi(argv[2]));
        for(int i = 0; i < outVec.size(); i++){
            table.Insert(outVec[i], "1");
        }
    
        
        tbb::parallel_for(tbb::blocked_range<size_t>(0,originalPass.size()),
                          [&](const tbb::blocked_range<size_t>& r){
            
            unsigned char hash[20];
            std::string hexStr;
            hexStr.resize(40);
            std::string permu;
            for(size_t i = r.begin(); i != r.end(); ++i){
                    for(int j = 0; j < originalPass.size(); j++){
                        for(int k = 0; k < originalPass.size(); k++){
                            for(int m = 0; m < originalPass.size(); m++){
                                //std::cout << originalPass[i] + originalPass[j] + originalPass[k] + originalPass[m] << std::endl;
                                permu = (originalPass[i] + originalPass[j] + originalPass[k] + originalPass[m]);
                                sha1::Calc(permu.c_str(), permu.length(), hash);
                                sha1::ToHexString(hash, &hexStr[0]);
                                std::string* found = table.Find(hexStr);
                                if(found != nullptr){
                                    *found = permu;
                                }
                            }
                        }
                    }
            }
        });
            
        std::ofstream outputFile("solved.txt");
        for(int i = 0; i < outVec.size(); i++){
            outputFile << outVec[i] << "," << *table.Find(outVec[i]) << std::endl;
            
        }
    }
    
    
    
    
    
}
