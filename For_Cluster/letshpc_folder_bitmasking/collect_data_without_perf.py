import os


base = os.getcwd() + '/all_codes/'
all_codes = os.listdir(base)

name_of_combined_file = "combined_logs.txt"
combined_file = open(os.getcwd() + "/" + name_of_combined_file, "w")

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

combined_file.close()
