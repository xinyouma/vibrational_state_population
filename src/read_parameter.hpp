#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#ifndef INPUT_PAR
#define INPUT_PAR
struct input_par{
	float R0, BETA, D0;
	float atom_a_mass, atom_b_mass;
	float time_per_step;
	unsigned int natom;
	unsigned int morse_atom_a, morse_atom_b;
	unsigned int init_exc_level;
	unsigned int maxstep;
	};
#endif
bool read_parameter(std::vector<std::string> & vecOfStr, input_par & morse_par);
