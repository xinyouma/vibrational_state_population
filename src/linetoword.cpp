#include "linetoword.hpp"

void linetoword(std::string &line, std::vector<std::string> &word){
    // Used to split string around spaces. 
    std::istringstream ss(line); 
    
    // Traverse through all words 
    do { 
        // Read a word 
        std::string tempword; 
        ss >> tempword;
   	word.push_back(tempword);
    
        // Print the read word 
	// std::cout << tempword << std::endl; 
    
        // While there is more to read 
    } while (ss); 	

}

