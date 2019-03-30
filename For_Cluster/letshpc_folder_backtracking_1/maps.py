'''
Author: Akshar Varma
Date: 19 November, 2016.
'''
import os
import subprocess
import random

base = os.getcwd()+'/'
log_directory = base + 'logs/'
input_directory = base + 'input/'
output_validation_directory = base + 'output-validation/'
output_directory = base + 'output/'


problem_list = ['trapezoidal', 'vector', 'matrix_multiplication',
                'pi_using_series', 'image_warping', 'median_filtering',
                'monte_carlo', 'prefix_sum', 'reduction', 'filter', 'divide_and_conquer','nqueen']

approaches = {'trapezoidal': ['reduction', 'critical', 'private'],
              'vector': ['static', 'dynamic', 'side_by_side'],
              'matrix_multiplication': ['outermost', 'middle',
                                        'transpose', 'block'],
              'pi_using_series': ['critical', 'reduction',
                                  'pow_function'],
              'image_warping': ['data_division',
                                'collapsed_directive'],
              'divide_and_conquer' : ['pi_reduction'],
              'median_filtering': ['qsort', 'diff_mem_alloc'],
              'monte_carlo': ['rand', 'randr_reduction', 'rand_r_reduction',
                              'randr_critical', 'rand_r_critical', 'own_prng', 'pi_using_critical'],
              'prefix_sum': ['double_tree', 'data-segmenting'],
              'reduction': ['tree', 'data_segmenting',
                            'segment_tree'],
              'filter': ['double_tree_prefix',
                         'data_segmenting_prefix',
                         'linked_list'],
               'nqueen':['design1']}

problem_size = {'trapezoidal': [10**x for x in range(1, 9)],
                'vector': [10**x for x in range(1, 9)],
                'matrix_multiplication': [2**x for x in range(4, 11)],
                'pi_using_series': [10**x for x in range(1, 9)],
                'divide_and_conquer': [10**x for x in range(1, 9)] + [int(0.5*10**x) for x in range(5,9)],
                'image_warping': [2**x for x in range(3, 11)],
                'median_filtering': [2**x for x in range(3, 11)],
                'monte_carlo': [10**x for x in range(1, 9)] + [int(0.5*10**x) for x in range(5,9)] ,
                'prefix_sum': [10**x for x in range(1, 9)],
                'reduction': [10**x for x in range(1, 9)] + [int(0.5*10**x) for x in range(5,9)] ,
                'filter': [10**x for x in range(1, 9)],
                'nqueen':[x for x in range(4,14)]}

processor_range = range(1, 9)

"""
problem_size = {'trapezoidal': [10**x for x in range(1, 4)],
                'vector': [10**x for x in range(1, 4)],
                'matrix_multiplication': [2**x for x in range(3, 4)],
                'pi_using_series': [10**x for x in range(1, 4)],
                'divide_and_conquer': [10**x for x in range(1, 4)] + [int(0.5*10**x) for x in range(5,6)],
                'image_warping': [2**x for x in range(3, 5)],
                'median_filtering': [2**x for x in range(3, 5)],
                'monte_carlo': [10**x for x in range(1, 4)] + [int(0.5*10**x) for x in range(5,6)],
                'prefix_sum': [10**x for x in range(1, 4)],
                'reduction': [10**x for x in range(1, 4)] + [int(0.5*10**x) for x in range(5,6)],
                'filter': [10**x for x in range(1, 4)]}
processor_range = range(1, 2)

"""

def create_rename_directory(dir_name):
    if not os.path.exists(dir_name):
        subprocess.call("mkdir "+dir_name, shell=True)
    else:
        foo = str(random.randint(0, 10**9))
        print('WARNING!! '+dir_name+' already exists.')
        print('Renaming using ' + foo)
        os.rename(dir_name, dir_name[:-1]+'-renamed-' + foo)
        subprocess.call("mkdir -p "+dir_name, shell=True)

if __name__ == "__main__":
    print "Printing various problem sizes:"
    for each in problem_size:
        print each,problem_size[each]
    print "*"*80
    print "Processor Range is ", processor_range
