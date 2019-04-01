"""
Author : Omkar Damle
11th September 2017

This script converts the text data file generated to a csv file of appropriate format 
which can to used to generate graphs by uploading on the letshpc graph generator
"""

import csv

txt_file_name = "combined_logs.txt"
csv_file_name = "combined_logs.csv"

field_names = ["n", "p", "run_id", "algS", "e2eS"]

csv_writer = csv.DictWriter(open(csv_file_name, 'wb'), field_names)
		
with open(txt_file_name, 'r') as txt_file:
	for line in txt_file:
		seq = line.split(',')
		del seq[6]
		dictionary = {}
		
		algS = float(seq[6])/1e9
		e2eS = float(seq[5])/1e9

		seq[5] = str(e2eS)
		seq[6] = str(algS)

		for i in range(len(field_names)):
			dictionary[field_names[i]] = seq[2+i] #remove the first two columns

		csv_writer.writerow(dictionary)	
