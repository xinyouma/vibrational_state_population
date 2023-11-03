----------------
#    README    #
----------------

Author: Xinyou Ma (xinyou.ma@ttu.edu)

## 0. Presumptions:

Presumming: VENUS-NWCHEM or VENUS simulation program is used.

Presumming: One VENUS output file contains only one trajectory.

Presumming: VENUS inputfile parameters are well inhereted in this program.

## 1. Usage:

./bin/vib_state_pop.x  <parameter_file> <traj_list_file>

where <parameter_file> has all the necessary parameters inherited from VENUS input file, and <traj_list_file> contains a list of VENUS trajectory output files.

For example, trying out the demo file by:

 ./bin/vib_state_pop.x ./data/morse.par ./data/traj.metadata

## 2. Parameter_file

Must define the following parameters:

| Variable Name  | Data Type |                         Description                          |
| :------------: | :-------: | :----------------------------------------------------------: |
|     natom      |    int    |                    total number of atoms                     |
| time_per_step  |   float   |                  time per step (unit: 10fs)                  |
|  morse_atom_a  |    int    |      Serial number of first atom in bond (start from 0)      |
|  morse_atom_b  |    int    | Serial number of second atom in bond (cannot be the same as morse_atom_a) |
|       R0       |   float   |         Morse equilibrium distance (unit: Angstrom)          |
|      BETA      |   float   |           Morse force constant (unit: Angstrom^-1)           |
|       D0       |   float   |            Morse force constant (unit: kcal/mol)             |
|  atom_a_mass   |   float   |         Atomic mass of 1st atom in bond (unit: amu)          |
|  atom_b_mass   |   float   |         Atomic mass of 2nd atom in bond (unit: amu)          |
| init_exc_level |    int    | Morse oscillator initial excitation level (n = nth vibrational level, n >= 0) |
|    maxstep     |    int    | Vibrational state population is analyzed for the first "maxstep" steps. maxstep should be smaller than the step number of the shortest trajectory. |

## 3. Trajectory Metadata File

The trajectory metadata file should contain a list of paths of VENUS output files, so that the users do not need to move/copy their output files around. The path should be either absolute path or relative path w.r.t. the user's current path. Do not put comments or blank lines in this file.



## 4. Output Quantities

"PE": this is the average bond energy in the initially excited state over time, in unit of kcal/mol.

"PESQ": this is the standard deviation (with Bessel's correction) of average bond energy in the initially excited state over time, in unit of kcal/mol.

"PNT": this is the percentage of trajectory population remaining in the initially excited state.

"PLOG": this is ln(PNT), unless PNT is 0.0.

"ith state pop count": this is an array of total counts of populations in each state, ranging from ground state to n+1 state, where n is the initially excited state.

## 5. Other Fancy Usage

Q: How do I monitor any other state? How do I monitor ground state population?

A: This is easy, just change the "init_exc_level" in the parameter file as the nth state, e.g. init_exc_level=3. The total count of trajectory is then showing in the range of ground state to the n+1'th state. Set "init_exc_level" as 0 to monitor ground state population.

Q: How do I monitor other local modes?

A: This is easy, just change the definition of bond, mostly almost all parameters in the parameter file. R0, BETA, D0 could be obtained from VENUS input file force constants for VENUS trajectories or by fitting 1-D potential energy curve for VENUS-NWCHEM trajectories.

Q: Other functions?

A: Contact author.




