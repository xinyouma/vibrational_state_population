#include "readVENUSout.hpp"
#include "linetoword.hpp"

bool readVENUSout(std::vector<std::string> &vecOfStr, unsigned int natom, std::vector<std::vector<float>> &Q, std::vector<std::vector<float>> &P)
{

/* Only print out trajectory parts
read until find XXXX
*/
		//unsigned int natom=28;
		int k=-1;
		std::vector<std::string> word;
		//std::vector<std::vector<float>> Q,P;
		std::vector<float> tempQ,tempP;
		Q.clear(); P.clear();
/*
		std::istringstream iss(sentence);
    		copy(istream_iterator<string>(iss),
         		istream_iterator<string>(),
         		ostream_iterator<string>(cout, "\n"));
*/
                // Print the vector contents
                //for(std::string & line : vecOfStr){
                for(std::string & line : vecOfStr){
			word.clear();
                        auto pos = line.find( " XXXXX" ) ;
                        if(pos == 0) {
				k=0;
				tempQ.clear();
				tempP.clear();
			  //std::cout << line << std::endl;
			  //linetoword(line,word);
//			  line >> kk;
//			  std::cout << kk << std::endl;
			}
			if (k>=0) {
				switch (k)
				{
				case 0:
					break;
				case 1:
			  		//std::cout << line << std::endl;
			  		//linetoword(line,word);
					//std::cout << "Cycle = " << word[4] << std::endl;
					//std::cout << "Time  = " << word[6] << std::endl;
					break;
				case 2:
					break;
				case 3:
			  		//std::cout << line << std::endl;
			  		//linetoword(line,word);
					//std::cout << "T = " << word[2] << std::endl;
					//std::cout << "V  = " << word[5] << std::endl;
					break;
				case 4:
			  		//std::cout << line << std::endl;
			  		//linetoword(line,word);
					//std::cout << "H = " << word[2] << std::endl;
					break;
				case 5:
					break;
				default:
					// used for Q and P read in
			  		linetoword(line,word);
					tempQ.push_back(std::stod(word[0]));
					tempQ.push_back(std::stod(word[1]));
					tempQ.push_back(std::stod(word[2]));
					tempP.push_back(std::stod(word[3]));
					tempP.push_back(std::stod(word[4]));
					tempP.push_back(std::stod(word[5]));
					// std::cout << word[0] <<" "<< word[1] <<" "<< word[2] << std::endl;
					for (int i=0; i< tempQ.size(); i++)
					{
						//std::cout << tempQ[i];
					}
						// std::cout<<std::endl;
					break;
				}
				++k;
			}
			if (k==5+natom) 
			{
				k=-1;
				Q.push_back(tempQ);
				P.push_back(tempP);
			}
                }

	// Check if Q & P are correct
	for(unsigned int i=0; i < Q.size(); i++)
	{
		for(unsigned int j=0; j < Q[i].size(); j++)
		{
			//std::cout << Q[i][j] << std::endl;
		}
	}
	return true;
}
