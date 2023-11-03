#include "read_parameter.hpp"
#include "linetoword.hpp"

#define PRINT_VAR(name) print_var(#name)
void print_var(char *name) { printf("name: %s\n", name);}

bool read_parameter(std::vector<std::string> &vecOfStr , input_par & morse_par)
{
	//input_par morse_par;
	std::vector<std::tuple<std::string, std::string>> stored_parameters;
	std::tuple<std::string, std::string> temp_parameters;
	size_t pos;
	std::vector<std::string> word;
	for(std::string & line : vecOfStr){
		word.clear();
		do { pos = line.find("//");  if (pos != std::string::npos)  line = line.substr(0, pos);
		} while (pos != std::string::npos);
		do { pos = line.find("#");  if (pos != std::string::npos)  line = line.substr(0, pos);
		} while (pos != std::string::npos);
		do { pos = line.find("!");  if (pos != std::string::npos)  line = line.substr(0, pos);
		} while (pos != std::string::npos);
		do { pos = line.find(',');  if (pos != std::string::npos)  line = line.replace(pos,1," ");
		} while (pos != std::string::npos);
		do { pos = line.find(':');  if (pos != std::string::npos)  line = line.replace(pos,1," ");
		} while (pos != std::string::npos);
		do { pos = line.find(';');  if (pos != std::string::npos)  line = line.replace(pos,1," ");
		} while (pos != std::string::npos);
		do { pos = line.find('=');  if (pos != std::string::npos)  line = line.replace(pos,1," ");
		} while (pos != std::string::npos);
		if (line.size() > 0 ) linetoword(line, word); 
  		while (word.size()>2) {
			temp_parameters = std::make_tuple(word[0], word[1]);
			stored_parameters.push_back(temp_parameters);
			word.erase(word.begin(),word.begin()+2);
		}
		
	}
	for (int it = 0; it < stored_parameters.size(); ++it)
	{
		auto temp_a = std::get<0>(stored_parameters[it]), temp_b = std::get<1>(stored_parameters[it]);
		//std::cout << "\t" << temp_a << "\t" << temp_b << std::endl;
		if (temp_a.compare("R0") == 0) morse_par.R0 = std::stof(temp_b);
		if (temp_a.compare("BETA") == 0) morse_par.BETA = std::stof(temp_b);
		if (temp_a.compare("D0") == 0) morse_par.D0 = std::stof(temp_b);
		if (temp_a.compare("atom_a_mass") == 0) morse_par.atom_a_mass = std::stof(temp_b);
		if (temp_a.compare("atom_b_mass") == 0) morse_par.atom_b_mass = std::stof(temp_b);
		if (temp_a.compare("natom") == 0) morse_par.natom = std::stoi(temp_b);
		if (temp_a.compare("morse_atom_a") == 0) morse_par.morse_atom_a = std::stoi(temp_b);
		if (temp_a.compare("morse_atom_b") == 0) morse_par.morse_atom_b = std::stoi(temp_b);
		if (temp_a.compare("init_exc_level") == 0) morse_par.init_exc_level = std::stoi(temp_b);
		if (temp_a.compare("maxstep") == 0) morse_par.maxstep = std::stoi(temp_b);
		if (temp_a.compare("time_per_step") == 0) morse_par.time_per_step = std::stof(temp_b);
	}
	std::cout << "\n" << " [ Reading Morse parameters ... ] \n" ;
	std::cout << "\t natom == " << morse_par.natom << std::endl;
	std::cout << "\t morse_atom_a == " << morse_par.morse_atom_a << std::endl;
	std::cout << "\t morse_atom_b == " << morse_par.morse_atom_b << std::endl;
	std::cout << "\t R0 == " << morse_par.R0 << std::endl;
	std::cout << "\t BETA == " << morse_par.BETA << std::endl;
	std::cout << "\t D0 == " << morse_par.D0 << std::endl;
	std::cout << "\t atom_a_mass == " << morse_par.atom_a_mass << std::endl;
	std::cout << "\t atom_b_mass == " << morse_par.atom_b_mass << std::endl;
	std::cout << "\t init_exc_level == " << morse_par.init_exc_level << std::endl;
	std::cout << "\t maxstep == " << morse_par.maxstep << std::endl;
	std::cout << "\t time_per_step == " << morse_par.time_per_step << std::endl;
	std::cout << "\n";
	return true;
}
