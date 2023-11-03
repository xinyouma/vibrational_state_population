#include "morse_analytic.hpp"

float MORSE::ebond_kcal (std::vector<float> & coords, std::vector<float> & momenta) 
{	
	H_temp=0.0, V_temp=0.0, T_temp=0.0, tempvar=0.0; 
	r_temp_Ang = MORSE::r_bond(coords);
	//std::cout << " r_temp_Ang = " << r_temp_Ang << std::endl;
	tempvar = 1 - exp(-beta_Angi*(r_temp_Ang - r0_Ang));
	V_temp = D0_kcal * tempvar * tempvar;
	unsigned int it;
	for (it = 0; it <3; it++)
		T_temp += pow(partial_mass_b * momenta[0+it] - partial_mass_a * momenta[3+it],2);
	T_temp = 0.5* T_temp /reduced_mass_amu;
	T_temp /= C1;
	H_temp = V_temp + T_temp;
	//std::cout << " bond energy = " << H_temp << " kcal/mol, V_temp = " << V_temp << " kcal/mol, T_temp = " << T_temp << " kcal/mol" << std::endl;;
	return H_temp; 
}


float MORSE::r_bond (std::vector<float> & coords)
{
	return sqrt( pow(coords[3]-coords[0],2) + pow(coords[4]-coords[1],2) + pow(coords[5]-coords[2],2));
}

bool MORSE::init(input_par & morse_par)
{
	r0_Ang = morse_par.R0;
	beta_Angi = morse_par.BETA;
	D0_kcal = morse_par.D0;
	D0_ven = D0_kcal*C1;
	morse_atom_serial = {morse_par.morse_atom_a, morse_par.morse_atom_b};
	morse_atomic_mass = {morse_par.atom_a_mass, morse_par.atom_b_mass};
	reduced_mass_amu = (morse_par.atom_a_mass * morse_par.atom_b_mass)/(morse_par.atom_a_mass + morse_par.atom_b_mass); 
	init_level = morse_par.init_exc_level; // initial excitation level
	time_per_step = morse_par.time_per_step;
	maxstep = morse_par.maxstep; // maxprintstep
	std::cout << " Morse oscillator : Atom1 = " << morse_atom_serial[0] << " ( " << morse_atomic_mass[0] << " amu )";
	std::cout <<                  ", Atom2 = " << morse_atom_serial[1] << " ( " << morse_atomic_mass[1] << " amu )" << std::endl;
	std::cout << " Morse r_0 = " << r0_Ang << " Angstrom" << "\t" << "Morse beta = " << beta_Angi << " 1/Angstrom" << std::endl;	
	std::cout << " Morse D_0 = " << D0_kcal << " kcal/mol" << "\t" << "Morse mu = " << reduced_mass_amu << " amu" << std::endl;	
	partial_mass_a = morse_atomic_mass[0]/(morse_atomic_mass[0] + morse_atomic_mass[1]);
	partial_mass_b = morse_atomic_mass[1]/(morse_atomic_mass[0] + morse_atomic_mass[1]);
	return true;
}
	
bool MORSE::lmode0(void)
{
	E_i.clear();
	delta_E_i.clear();
	De = D0_kcal;
	we = 1.0 / twopi / c_ven * sqrt(2.0 * D0_ven * pow(beta_Angi,2)/reduced_mass_amu); // Ang-1
	// 1 kcal/mol = 349.757 cm-1 = 3.49757*10^4 m-1 = 3.49757*10^-6 Ang-1;
	wavenum_cmi = we * 1.0e8; // cm-1 
	we = we / 3.49757e-6; // kcal/mol

	std::cout << " Morse nu = " << wavenum_cmi << " cm-1" << std::endl;	
	std::cout << " Morse W_e = " << we << " kcal/mol" << std::endl;	

	xe = we * we / D0_ven /4.0; // (kcal/mol)^2/ven
	xe = xe * C1; // kcal/mol

	std::cout << " Morse X_e = " << xe << " kcal/mol" << std::endl;	
	std::cout << " Morse D_e = " << De << " kcal/mol" << std::endl;	

//	ground state population can now be counted.	
//	if (init_level == 0) return false;
	max_level = init_level + 2; // highest level being monitored
	for (unsigned int i=0; i<max_level+1; i++)
	{
		E_temp = (float(i)+0.5) * we - pow((float(i)+0.5),2) * xe;
		E_i.push_back(E_temp);		
	}
	for (unsigned int i=0; i<max_level; i++)
	{
		delta_E_i.push_back( 0.5*(E_i[i+1]-E_i[i]) );		
	}
	std::cout << std::endl << "E(n) =  n :     E    \t+-    delta_E" << std::endl;
	//for (std::vector<float>::iterator it = E_i.begin() ; it != E_i.end(); ++it)
	//std::cout <<'\t' << *it;
	for (unsigned int it = 0; it < E_i.size()-1; ++it)
	std::cout <<'\t' << it << " : " << E_i[it] << "\t+- " << delta_E_i[it] <<" kcal/mol" << std::endl;

	// VENUS methods are deprecated since replaced by vectors of E_i and delta_E_i
	E_n = E_n_plus_one = E_n_plus_two = E_n_minus_one = delta_E_n = delta_E_n_plus_one = delta_E_n_minus_one = 0.0;
//	E_n = E_i[init_level];
//	E_n_plus_one = E_i[init_level + 1];
//	E_n_plus_two = E_i[init_level + 2];
//	E_n_minus_one = E_i[init_level - 1 ];
	
//	delta_E_n = 0.5*(E_n_plus_one - E_n);
//	delta_E_n_plus_one = 0.5*(E_n_plus_two - E_n_plus_one);		
//	delta_E_n_minus_one = 0.5*(E_n_plus_two - E_n_plus_one);		


	return true;
}	

bool MORSE::inflate_diatomic_traj (unsigned int natom, 
	std::vector<std::vector<float>> & Q_molecule, std::vector<std::vector<float>> & P_molecule)
{
	Q_traj.clear(); P_traj.clear();
	std::vector<float> tempQ, tempP;

	if (Q_molecule.size() == P_molecule.size())
	{
		//std::cout << " In total " << Q_molecule.size() << " steps " << std::endl;
	} else {
		return false;
	}

	int stepsize;
	stepsize = (Q_molecule.size()<maxstep) ? Q_molecule.size() : maxstep;	
	for(std::vector< std::vector<float>>::iterator it = Q_molecule.begin() ; it != Q_molecule.begin()+stepsize; ++it)
	{
		for(std::vector<float>::iterator jt = it->begin()+3*morse_atom_serial[0] ; jt != it->begin()+3*(morse_atom_serial[0]+1); ++jt)	
			tempQ.push_back(*jt);
		for(std::vector<float>::iterator jt = it->begin()+3*morse_atom_serial[1] ; jt != it->begin()+3*(morse_atom_serial[1]+1); ++jt)	
			tempQ.push_back(*jt);
		Q_traj.push_back(tempQ);
		tempQ.clear();
	}	
	stepsize = (P_molecule.size()<maxstep) ? P_molecule.size() : maxstep;	
	for(std::vector< std::vector<float>>::iterator it = P_molecule.begin() ; it != P_molecule.begin()+stepsize; ++it)
	{
		for(std::vector<float>::iterator jt = it->begin()+3*morse_atom_serial[0] ; jt != it->begin()+3*(morse_atom_serial[0]+1); ++jt)	
			tempP.push_back(*jt);
		for(std::vector<float>::iterator jt = it->begin()+3*morse_atom_serial[1] ; jt != it->begin()+3*(morse_atom_serial[1]+1); ++jt)	
			tempP.push_back(*jt);
		P_traj.push_back(tempP);
		tempP.clear();
	}	
/*
	for (auto it = Q_traj.begin(); it != Q_traj.end(); ++it)
	{
		for (auto jt = it->begin(); jt != it->end(); ++jt)
		std::cout << '\t' << *jt;
		std::cout << std::endl;
	}

	for (auto it = P_traj.begin(); it != P_traj.end(); ++it)
	{
		for (auto jt = it->begin(); jt != it->end(); ++jt)
		std::cout << '\t' << *jt;
		std::cout << std::endl;
	}
*/
	return true;
}

bool MORSE::vib_pop_traj(void)
{
	E_traj.clear();
	ESQ_traj.clear();
	N_pop_traj.clear();
	N_pop_n_traj.clear();
	for(unsigned int it = 0; it < P_traj.size(); ++it)
	{
		N_pop_i.clear();
		E_temp = ebond_kcal(Q_traj[it], P_traj[it]);
		E_traj.push_back(E_temp);
		for(unsigned int jt = 0; jt < delta_E_i.size(); ++jt)
		{
			N_pop_i.push_back(abs(E_temp - E_i[jt]) <= delta_E_i[jt]); 	
			if (jt == init_level) N_pop_n_traj.push_back(abs(E_temp - E_i[jt]) <= delta_E_i[jt]);
		}
		N_pop_traj.push_back(N_pop_i);
		E_temp = pow(E_temp,2);
		ESQ_traj.push_back(E_temp);
	}
	std::cout << "\n";
	std::cout << "  Step\ttime(ps)\tE_vib(kcal/mol)\tith state pop count" << std::endl << std::endl;
	for(unsigned int it = 0; it < N_pop_traj.size(); ++it)
	{
		std::cout << " " << std::setw(5) << it;
		std::cout << "\t" << E_traj[it]; 
		std::cout << "\t" << float(it)*time_per_step/100.0 << "\t";
		for(auto jt = N_pop_traj[it].begin(); jt != N_pop_traj[it].end(); ++jt)
		{
			std::cout << *jt << "   ";
		}
		std::cout << "\n";
	}
	return true;
}
