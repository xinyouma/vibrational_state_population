#include <iostream> 
#include <algorithm> 
#include <functional> 
#include <iomanip>
#include "getFileContent.hpp"
#include "readVENUSout.hpp"
#include "read_parameter.hpp"
#include "constants.hpp"
#include "morse_analytic.hpp"

#define OPRINT 0 
using namespace std;

int usage_n_exit(string message)
{
    cout << " Aborted unexpectedly! At/Beacuse : " << message << endl <<endl;
    cout << " Usage 1: $(directory)/bin/readuntil.x " << '\n' << "\t\t (reading example.txt by default)" << endl;
    cout << " Usage 2: $(directory)/bin/readuntil.x <inputfile>" << '\n' << 
		"\t\t (inputfile must indicate a file with Morse parameters at its 1st line)" << '\n' <<
		"\t\t (inputfile must indicate a series of VENUS outputfile from its 2nd line)" << endl;
    cout << " Usage 3: $(directory)/bin/readuntil.x <inputfile1> <inputfile2>" << '\n' <<
		"\t\t (inputfile1 must contains Morse parameters)" << '\n' << 
		"\t\t (inputfile2 must indicate a series of VENUS outputfile from its 1st line)" << endl;
	exit(-1);
	return 0;
}

int emergency_exit(string message)
{
    cout << " Aborted unexpectedly! At/Beacuse : " << message << endl;
	exit(-2);
	return 0;
}

int main(int argc, char **argv)
{
        std::vector<std::string> vecOfStr, vecOfFile; 
	int skip=0;

	cout << fixed;
	cout << setprecision(6);

	cout << " ----------------------------------  Local Mode Excited State Population  -----------------------------------" << endl;
	cout << " [ Starting ... ]"                                << endl;
	cout << " [ Checking command line input options ...]: " ;
	cout <<  " "   <<   argc-1   <<    " input arguments "     << endl;
	for (unsigned int i = 1; i < argc; ++i)
	       cout << " [ Input argum " << i << " ]: " << argv[i] << endl;

	input_par morse_par;

	std::string trajfile="", parameterfile=""; //load default options
	switch (argc-1){
		case 0:
			usage_n_exit("does not take 0 argument");
			break;
		case 1:
			trajfile=argv[argc-1];
			if(!getFileContent(trajfile, vecOfFile)) 
				usage_n_exit("could not read traj file list");
			parameterfile=vecOfFile[0];
			skip=1;
			break;
		case 2:
			parameterfile=argv[1];	
			trajfile=argv[argc-1];
			if(!getFileContent(trajfile, vecOfFile)) 
				usage_n_exit("could not read traj file list");
			break;
		default:
			break;
	}

	if(!getFileContent(parameterfile, vecOfStr)) emergency_exit("could not read Morse parameter file"); 
	read_parameter(vecOfStr, morse_par);

	int ntraj = vecOfFile.size()-skip;
	if (ntraj<2) emergency_exit(" Need at least 2 trajectories for state population analysis. "); 

	MORSE morse;
	if(!morse.init(morse_par)) emergency_exit("morse init");
	if(OPRINT) cout << " [ main : Morse initialized ! ] " << endl; 
	if(!morse.lmode0()) emergency_exit("morse lmode0");
	if(OPRINT) cout << " [ main : Morse vib level calculated ! ] " << endl; 

	
	std::vector<std::vector<float>> Q, P;// 1st dim : traj step, 2nd dim : 1->natom
	std::vector<std::vector<unsigned int>> N_t_i, N_t_itraj; // 1st dim : traj step, 2nd dim: ith state population count
	
	// sumation of E and E^2 over all trajectories
	std::vector<float> ESAV_t, ESQ_t; //1st dim: traj step
	std::vector<float> ESAV_temp, ESQ_temp;
	
	// 4 quantities evaluating the states population 
	std::vector<float> PN_t, PESQ_t, PE_t, PLOG_t; // 1st dim : traj step

	for (int i = 0+skip; i < vecOfFile.size(); ++i)
	{

         	// Get the contents of file in a vector
		cout << endl <<" Reading file ["<<i<<"] = " << vecOfFile[i] << endl;
		if(!getFileContent(vecOfFile[i], vecOfStr)) emergency_exit("could not read trajectory file "+to_string(i)); 

		// Deprecated since no longer using one input file.
		// if(!getFileContent(trajfile, vecOfStr)) emergency_exit("could not read trajectory file"); 
	
		readVENUSout(vecOfStr,morse_par.natom,Q,P);
		if(OPRINT) cout << " [ main : finished reading VENUS output files ! ] " << endl; 

		if(!morse.inflate_diatomic_traj(morse_par.natom, Q, P)) emergency_exit("morse.inflate_diatomic_traj");
		if(OPRINT) cout << " [ main : Morse vib traj inflated ! ] " << endl; 
		Q.clear(); P.clear();
   
		if(!morse.vib_pop_traj()) emergency_exit("morse.vib_pop_traj"); 
		if(OPRINT) cout << " [ main : Morse vib traj population analysis finished. ] " << endl;

		if (i == 0 + skip)
		{
			ESAV_t = morse.get_E_traj();
			ESQ_t = morse.get_ESQ_traj();
			N_t_i = morse.get_N_pop_traj();
		} else {
			ESAV_temp = morse.get_E_traj();
			ESQ_temp = morse.get_ESQ_traj();
			N_t_itraj = morse.get_N_pop_traj();

			std::transform(ESAV_t.begin(), ESAV_t.end(), ESAV_temp.begin(), ESAV_t.begin(), std::plus<float>());	
			std::transform(ESQ_t.begin(), ESQ_t.end(), ESQ_temp.begin(), ESQ_t.begin(), std::plus<float>());	
			for(unsigned int it=0; it<N_t_i.size();++it)
			{
				std::transform(N_t_i[it].begin(),N_t_i[it].end(), N_t_itraj[it].begin(), N_t_i[it].begin(),std::plus<unsigned int>());
			}
		}
		ESAV_temp.clear(); ESQ_temp.clear();
		Q.clear(); P.clear();
		vecOfStr.clear();
	}
	cout << endl << "   ------------------------------------- Final Analysis -------------------------------------" << endl;
	cout << "  Total # trajectoy == " << ntraj << endl << endl;
	cout << "  Step\ttime(ps)\tPE (kcal/mol)\tPESQ (kcal/mol)\tPNT     \tPLOG    \tith state pop count" << endl << endl;
	for(unsigned int it = 0; it < N_t_i.size(); ++it)
	{
		PE_t.push_back(ESAV_t[it]/float(ntraj));
		PESQ_t.push_back(sqrt((ESQ_t[it] - ESAV_t[it]*ESAV_t[it]/float(ntraj))/float(ntraj-1)));
		PN_t.push_back(float(N_t_i[it][morse_par.init_exc_level])/float(ntraj)); 
		PLOG_t.push_back((PN_t[it]==0) ? 0.0 : log(PN_t[it]));

		std::cout << " " << setw(5)<< it;
		std::cout << "\t" << float(it)*morse_par.time_per_step/100.0;
		std::cout << "\t" << PE_t[it];
		std::cout << "\t" << PESQ_t[it];
		std::cout << "\t" << PN_t[it];
		std::cout << "\t" << PLOG_t[it];
		std::cout << "\t";
		for (auto jt = N_t_i[it].begin(); jt != N_t_i[it].end(); ++jt)
		{
			std::cout << *jt << "    ";
		}
		std::cout << "\n";
	}

	N_t_i.clear(); N_t_i.shrink_to_fit();
	ESAV_t.clear(); ESAV_t.shrink_to_fit();
	Q.clear(); Q.shrink_to_fit();
	P.clear(); P.shrink_to_fit();
   	if(OPRINT) cout << " [ main : completed. exiting program. ]" << endl;
	//std::cout << "Size of float : " << sizeof(float) << std::endl;
	//std::cout << "Size of String : " << sizeof(std::string) << std::endl;
	//std::cout << "Size of vecOfStr : " << vecOfStr.size()*sizeof(std::string) << std::endl;
    	return 0;
	exit(0);
}
	
