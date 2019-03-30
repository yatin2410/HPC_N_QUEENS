Regarding code modification and running using script

1. See codes on github repo "https://github.com/letshpcorg/letshpcsample" and modify your codes accordingly so that they can be run automatically using script.
2. E2E is the total time for which the code runs while ALG is the time of the parallel part in the code. Put these two in your code properly before running the script.
3. Put the codes in a folder named 'all_codes'
4. Download the scripts on repo. (OR use the scripts sent to you)
5. Adjust number of processors and problem size in maps.py for the given problem. Make sure that it does not take too much time.
6. Do ./run_script_without_perf.sh <number of runs> to start data collection and generate csv file. For example, "./run_script_without_perf.sh 10" if you want to run 10 iterations
7. Make sure you run the code for at least 10 iterations. Run the script on lab machines only. Don't run the script on cluster. You can observe the results by running the code for 1 iteration on cluster.



Generating graphs

1. Use Google Chrome for this part.
2. Go to letshpc.herokuapp.com
3. In Analysis Tools, select "Analyse Custom Data"
4. Upload CSV file generated after running script using 'Normal' option.
5. Ignore the error in red. Select all the cores and 'Update'
6. Adjust scale and metric. Choose 'Mean' as statistic. Make sure the scale of x and y-axis is proper and both the axes are labelled.
7. Export all the necessary graphs.


Writing report

1. Use chrome for this part.
2. Select Report Generator Tool in letshpc.herokuapp.com
3. Fill the relevant fields
4. Download the latex report
5. Download llncs.cls for latex report. (OR use the file that is sent to you)
6. Open the file in latex editor. Add necessary details. Add generated graphs at proper places.
7. Add any extra sections if necessary.
