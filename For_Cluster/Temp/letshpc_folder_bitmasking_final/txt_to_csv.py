"""
Author : Omkar Damle, Rajdeep Pinge
Date : September, 2017

This script converts the text data file generated to a csv file of appropriate format 
which can to used to generate graphs by uploading on the letshpc graph generator
"""

import csv
import maps

with open("codes_run_file") as f:
	prob = f.read().split("-")

numProblemSizes = maps.problem_size[prob[2]]

lineCount = 0
run_id = 0

txt_file_name = "combined_logs.txt"
csv_file_name = "combined_logs.csv"

field_names = ["n", "p", "run_id", "algS", "e2eS"]

csv_writer = csv.DictWriter(open(csv_file_name, 'wb'), field_names)
		
with open(txt_file_name, 'r') as txt_file:
	for line in txt_file:
		lineCount += 1

		seq = line.split(',')
		dictionary = {}
		
		algS = float(seq[6]) + float(seq[7])/1e9
		e2eS = float(seq[4]) + float(seq[5])/1e9

		seq[5] = str(algS)
		seq[6] = str(e2eS)

		for i in range(len(field_names)):
			dictionary[field_names[i]] = seq[2+i] #remove the first two columns
		
		dictionary["run_id"] = run_id

		csv_writer.writerow(dictionary)	
		
		if lineCount == len(numProblemSizes) * (max(maps.processor_range)+1):
			lineCount = 0
			run_id += 1