#!/bin/python

import subprocess
import os
import sys
import maps
import time
import logging




def line(n):
    print('-'*n)


logging.basicConfig(filename = "LetsHPC_Team_CodeRunner.log", level = logging.INFO)
logger = logging.getLogger(__name__)

########################################################################################################




USAGE = """
Usage:

run.py problem_name approach_name serial_executable parallel_executable runs log_directory output_directory input_directory base_directory

'problem_name' is the name of the problem assigned to you.
'approach_name' is the name of the appraoch assigned to you.
'serial_executable' must be the name of the compiled executable file for the serial code.
'parallel_executable' must be the name of the compiled executable file for the parallel code.
'runs' is the number of times to run the codes. Run at least thrice and ideally 10 times.
'log_directory' is the directory where you want to store the log files
'output_directory' is the directory where you want to store the output files
'input_directory' is the directory where you take the input from
"""


def foobar(l):
    if len(l) < 10:
        print USAGE
        return

    problem_name = l[1]
    approach_name = l[2]
    serial_executable = l[3]
    parallel_executable = l[4]
    runs = int(l[5])
    compiler_to_use = l[-1]

    logger.info("-"*80)
    logger.info("Problem Name : %s" % (problem_name))
    logger.info("Approach Name : %s" % (approach_name))
    logger.info("Serial Executable : %s" % (serial_executable))
    logger.info("Parallel Executable : %s" % (parallel_executable))
    logger.info("Number of runs : %s" % (str(runs)))


    if problem_name not in maps.problem_list:
        print problem_name, 'not in', maps.problem_list
        logger.error("%s not in problem list" % (problem_name))
        exit(0)

    if approach_name not in maps.approaches[problem_name]:
        print approach_name, 'not a valid approach for', problem_name
        print 'Choose from:' 
        print maps.approaches[problem_name]
        logger.error("%s is not a valid approach" % (approach_name))
        exit(0)


    log_directory = l[6]
    output_directory = l[7]
    input_directory = l[8]
    line(80)
    logger.info("Log Directory : %s" %(log_directory))
    logger.info("Output Directory : %s" % (output_directory))
    logger.info("Input Directory : %s" % (input_directory))


    print 'Assuming that input has been created for:', problem_name
    subprocess.call('lscpu > '
            + log_directory
            + "lscpu.txt", shell=True)
    subprocess.call('cat /proc/cpuinfo > '
            + log_directory
            + "cpuinfo.txt", shell=True)
    


    for run in range(runs):
        os.chdir(l[9])
        print 'Run:', str(run+1) 
        print('Running Serial')
        logger.info("Started running the serial code for run_id = %d" %(run))
        for n in maps.problem_size[problem_name]:
            print('Problem Size:', n)
            input_file = input_directory+problem_name+'_'+str(n)+'_input.txt'

            if compiler_to_use == 'openmp':
                logger.info("Running the Command : " + serial_executable
                                + " " + str(n)
                                + " " + str(0)  # p=0 for serial code.                           
    							+ " " + input_file
                                + " >> " + log_directory
                                + problem_name + "_" + approach_name
                                + ".logs") 

                subprocess.call(serial_executable
                                + " " + str(n)
                                + " " + str(0)  # p=0 for serial code.                          
    							+ " " + input_file
                                + " >> " + log_directory
                                + problem_name + "_" + approach_name
                                + ".logs",
                                shell=True)

            elif compiler_to_use == 'mpi':
                logger.info("Running the Command : mpirun -np 1 " + serial_executable
                                + " " + str(n)
                                + " " + str(0)  # p=0 for serial code.                           
                                + " " + input_file
                                + " >> " + log_directory
                                + problem_name + "_" + approach_name
                                + ".logs") 

                subprocess.call("mpirun -np 1 " + serial_executable
                                + " " + str(n)
                                + " " + str(0)  # p=0 for serial code.                          
                                + " " + input_file
                                + " >> " + log_directory
                                + problem_name + "_" + approach_name
                                + ".logs",
                                shell=True)
     
            
            
        line(80)

        print('Running Parallel')
        for p in maps.processor_range:
            print('Number of Processors:', p)
            logger.info("Running the parallel code with %d processors" % (p))
            for n in maps.problem_size[problem_name]:
                os.chdir(l[9])
                input_file = input_directory+problem_name+'_'+str(n)+'_input.txt'
                print('Problem Size:', n)

                if compiler_to_use == 'openmp':
                    logger.info("Running the Command : " + parallel_executable
                                    + " " + str(n)
                                    + " " + str(p)
                                    + " " + input_file
                                    + " >> " + log_directory
                                    + problem_name + "_" + approach_name
                                    + ".logs") 

                    subprocess.call(parallel_executable
                                    + " " + str(n)
                                    + " " + str(p)
                                    + " " + input_file
                                    + " >> " + log_directory
                                    + problem_name + "_" + approach_name
                                    + ".logs",
                                    shell=True)

                elif compiler_to_use == 'mpi':
                    logger.info("Running the Command : mpirun -np " + str(p) + " " + parallel_executable
                                    + " " + str(n)
                                    + " " + str(p)
                                    + " " + input_file
                                    + " >> " + log_directory
                                    + problem_name + "_" + approach_name
                                    + ".logs") 

                    subprocess.call("mpirun -np " + str(p) + " " + parallel_executable
                                    + " " + str(n)
                                    + " " + str(p)
                                    + " " + input_file
                                    + " >> " + log_directory
                                    + problem_name + "_" + approach_name
                                    + ".logs",
                                    shell=True)


        line(80)

    print(os.getcwd())




#######################################################################



base = os.getcwd()
all_files = os.listdir(base)

inp = None
while True:
    if 'codes_run_file' in all_files:
        inp = raw_input("Do you want to reuse the results of previous run? (y/n): ").lower()
        if inp == 'y':
            break
        elif inp == 'n':
            os.remove(base + '/codes_run_file')
            break
        else:
            print "Invalid input. Try again."

    else:
        break

while True:
    compiler_to_use = raw_input("Which parallel framework would you be using? (openmp/mpi): ").lower()

    if compiler_to_use == 'mpi' or compiler_to_use == 'openmp':
        break
    else:
        print("Incorrect input. Try again.")


while True:
    try:
        runs = int(raw_input("Enter the number of times you want the code to run (recommended: at least 10 runs): "))
        if runs <= 0:  # if not a positive int print message and ask for input again
            print("Input must be a positive integer, try again!")
            continue
    except ValueError as ve:
        print("That's not an int! Try again!")
        continue
    else:
        print('the number of runs is ' + str(runs))    
        break 

all_inputs = os.getcwd() + '/all_input/'
base = os.getcwd() + '/all_codes/'
starting_point = os.getcwd()
all_codes = os.listdir(base)

count = 0
try:
    os.remove(base + "progress.txt")
except Exception as e:
    print "File already deleted"

print(all_codes)
code_to_run = None
codes_already_run = None

try:
    uber = open(os.getcwd() + "/codes_run_file", "r") 
    codes_already_run = uber.readlines()
    uber.close()

except Exception as e:
    command = "touch %s" % (starting_point + "/codes_run_file")
    subprocess.call(command, shell = True)

if codes_already_run is None:
    code_to_run = all_codes[0]
    
else:
    for each in all_codes:
        if each+"\n" not in codes_already_run:
            code_to_run = each
            break            
        
print "The following code will be run now", code_to_run

if code_to_run is None:
    print "All the codes have already been executed."# + " You can run the collect data script now"
    sys.exit(1)

for each_code in [code_to_run]:
    if each_code == "progress.txt" or "log" in each_code:
        continue
    subprocess.call("rm -rf "
                + base + each_code + "/output"
                , shell=True)
    subprocess.call("rm -rf "
                + base + each_code + "/logs"
                , shell=True)
    division = each_code.split("-")
    problem = division[2]
    approach = division[3]
    print "-"*80
    print problem, approach
    all_files = os.listdir(base+each_code+"/")
    serial = None
    parallel = None
    for each_file in all_files:
        if 'clean' not in each_file.lower() and 'logs'!=each_file.lower() and 'output'!=each_file.lower():
            if 'par' not in each_file.lower() and each_file!="ser":
                serial = each_file
            elif 'parallel' in each_file.lower():
                parallel = each_file
    
    if compiler_to_use == 'mpi':
        compiler = "mpicc "
    elif compiler_to_use == 'openmp':
        compiler = "gcc "

    if ".cpp" in parallel:
        if compiler_to_use == "mpi":
            compiler = "mpiCC "
        elif compiler_to_use == "openmp":
            compiler = "g++ "

    print serial, parallel

    if 'logs' not in all_files:
        os.mkdir(base + each_code + "/logs")
        os.mkdir(base + each_code + "/output")    


    if compiler_to_use == 'openmp':
        subprocess.call(compiler
                + base + each_code + "/" + parallel
                + " -fopenmp -lm -w -o "
                +  base + each_code + "/parr", shell=True)
        subprocess.call(compiler
                + base + each_code + "/" + serial
                + " -fopenmp -lm -w -o "
                +  base + each_code + "/ser", shell=True)

    elif compiler_to_use == 'mpi':
        subprocess.call(compiler
                + base + each_code + "/" + parallel
                + " -lm -w -o "
                +  base + each_code + "/parr", shell=True)
        subprocess.call(compiler
                + base + each_code + "/" + serial
                + " -lm -w -o "
                +  base + each_code + "/ser", shell=True)        

    print serial,parallel
    #raw_input()
    foobar(['run.py', problem, approach, base + each_code + "/ser", base + each_code + "/parr", int(runs), base + each_code + "/logs/", \
        base + each_code + "/output/",  all_inputs, base + each_code + "/", compiler_to_use])

    f = open(base + "progress.txt", "a")
    f.write(str(time.time()) + " " + str(count) + " " + str(each_code)+"\n")
    f.close() 
    count +=1
    print "Reached Here:", code_to_run, type(code_to_run)
    w2f = open(starting_point + "/codes_run_file", "a")
    string_to_write = code_to_run + "\n"
    w2f.write(string_to_write)
    w2f.close()
    print "Written To file"
