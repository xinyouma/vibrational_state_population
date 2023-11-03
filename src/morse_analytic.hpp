#include <iostream>
#include <iomanip>
#include <algorithm>    
#include <string>
#include <vector>
#include <math.h>
#include <iterator>
#include "read_parameter.hpp"
#include "constants.hpp"

class MORSE {
	public:
		bool init(input_par & morse_par);
		bool lmode0(void);
		bool inflate_diatomic_traj (unsigned int natom, 
			std::vector<std::vector<float>> & Q_molecule, std::vector<std::vector<float>> & P_molecule); 
		bool vib_pop_traj (void);
		float ebond_kcal (std::vector<float> & coords, std::vector<float> & momenta);
		float r_bond (std::vector<float> & coords);
		std::vector<std::vector<unsigned int>> get_N_pop_traj(){return N_pop_traj;}
		std::vector<unsigned int> get_N_pop_n_traj() {return N_pop_n_traj;}
		std::vector<float> get_E_traj() {return E_traj;}
		std::vector<float> get_ESQ_traj(){return ESQ_traj;}
	private:
		std::vector<unsigned int> morse_atom_serial = {0,0};
		float partial_mass_a, partial_mass_b;
		std::vector<float> morse_atomic_mass = {0.0,0.0};
		float r0_Ang, D0_kcal, beta_Angi, D0_ven; // Force constants
		float reduced_mass_amu, wavenum_cmi;
		float De, we, xe; // kcal/mol
		// Energy levels: (kcal/mol)
		std::vector<float> E_i, delta_E_i;
		float E_temp, E_n, E_n_plus_one, E_n_plus_two, E_n_minus_one;
		float delta_E_n, delta_E_n_plus_one, delta_E_n_minus_one;
		unsigned int init_level, max_level;
		// Trajectory: Q & P
		unsigned int maxstep;
		float time_per_step;
		std::vector<std::vector<float>> Q_traj, P_traj; 
		// number of states
		std::vector<std::vector<unsigned int>> N_pop_traj; // 1st dim: traj step, 2nd dim: E level
		std::vector<unsigned int> N_pop_i; // 1st dim: E level
		std::vector<unsigned int> N_pop_n_traj; // 1st dim: traj step, only for nth state
		std::vector<float> E_traj, ESQ_traj;
		
		float H_temp, V_temp, T_temp, tempvar, r_temp_Ang;
};


