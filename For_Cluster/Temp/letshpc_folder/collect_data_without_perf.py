import subprocess
import os
import sys
import time
import glob
import re

def process_perf_file(file_name, csv_list, l, n = "0", p = "0"):
    problem_name = l[0]
    problem_approach = l[1]
    q = open(file_name, "r")
    all_lines = q.readlines()
    run_id = 1
    not_supported = []
    csv_list_2 = []
    l = [problem_name,problem_approach,n,p,str(run_id)]
    for each in all_lines[1:]:
        if each[0] == "#":
            l = [problem_name,problem_approach,n,p,str(run_id)]
            for each in csv_list_2:
                l.append(each[1])
            csv_list.append(l)
            run_id+=1
            csv_list_2 = []
        else:
            s = re.findall(r"[0-9\.\,]+      [a-zA-Z0-9\-]+", each)
            if len(s)!=0:
                value, key = s[0].split("      ")
                value = value.replace(",", "")
                value = float(value)
                csv_list_2.append([key, str(value)])
    l = [problem_name,problem_approach,n,p,str(run_id)]    
    for each in csv_list_2:
        l.append(each[1])
    csv_list.append(l)
    
    headers = ["problem_name","problem_approach","n","p","run_id"]
    for each in csv_list_2:
        headers.append(each[0])

    return csv_list, headers    

def process_all_perfs(perf_directory, problem_name, problem_approach):
    headers = []
    dic_headers = {}

    dic_csv = {}
    
    all_files = glob.glob(perf_directory + "serial*")
    for each in all_files:
        name_of_file = each.split("/")[-1]
        log_no = name_of_file.split(".")[-1]
        csv_list = []
        print name_of_file
        n = name_of_file.split("_")[1]
        p = name_of_file.split("_")[2].split(".")[0]
        temp_list, temp_headers = process_perf_file(each, csv_list, [problem_name, problem_approach], n, p)
        for each in temp_list:
            if (n,p) not in dic_csv:
                dic_csv[(n,p)] = {}
            if each[4] not in dic_csv[(n,p)]:
                dic_csv[(n,p)][each[4]] = {}
            dic_csv[(n,p)][each[4]][log_no] = each
        
        
        
        if log_no not in dic_headers:
            dic_headers[log_no] = temp_headers


    csv_list_serial = []
    
    headers = dic_headers['1']
    for i in range(2,5):
        headers = headers + dic_headers[str(i)][5:]
        
    for each in dic_csv:
        for each_run in dic_csv[each]:
            new_row = dic_csv[each][each_run]['1']
            for i in range(2,5):
                new_row = new_row + dic_csv[each][each_run][str(i)][5:]
            csv_list_serial.append(new_row)


    csv_list_parallel = []
    all_files = glob.glob(perf_directory + "parallel*")
    dic_csv = {}

    for each in all_files:
        name_of_file = each.split("/")[-1]
        log_no = name_of_file.split(".")[-1]
        csv_list = []
        print name_of_file
        n = name_of_file.split("_")[1]
        p = name_of_file.split("_")[2].split(".")[0]
        temp_list, temp_headers = process_perf_file(each, csv_list, [problem_name, problem_approach], n, p)
        for each in temp_list:
            if (n,p) not in dic_csv:
                dic_csv[(n,p)] = {}
            if each[4] not in dic_csv[(n,p)]:
                dic_csv[(n,p)][each[4]] = {}
            dic_csv[(n,p)][each[4]][log_no] = each

    for each in dic_csv:
        for each_run in dic_csv[each]:
            new_row = dic_csv[each][each_run]['1']
            for i in range(2,5):
                new_row = new_row + dic_csv[each][each_run][str(i)][5:]
            csv_list_parallel.append(new_row)

    return csv_list_serial+csv_list_parallel, headers


base = os.getcwd() + '/all_codes/'
all_codes = os.listdir(base)

name_of_combined_file = "combined_logs.txt"
combined_file = open(os.getcwd() + "/" + name_of_combined_file, "w")

#Making Perf Log directory
#os.mkdir(os.getcwd() + "/perf_log")

for each_code in all_codes:
    if each_code == "progress.txt":
        continue
    files = os.listdir(base + each_code + "/logs/")
    main_file = None
    for each in files:
        if 'logs' in each and 'perf' not in each:
            main_file = each
            break
    file_lines = open(base + each_code + "/logs/" + main_file, "r").readlines()
    for each in file_lines:
        if len(each.split(","))!=8:
            continue
        else:
            combined_file.write(each)
    #f = each_code.split("-")
    #csv_list, headers = process_all_perfs(base+each_code+"/logs/perf_logs/", f[2], f[3])
    #f = open(os.getcwd() + "/perf_log/" + each_code + ".log", "w")
    #f.write(",".join(headers) + "\n")
    #for each in csv_list:
    #    f.write(",".join(each) + "\n")
    #f.close()

combined_file.close()
