import subprocess
import argparse
import time
import random
import os
from subprocess import Popen, PIPE, STDOUT
from pathlib import Path

from my_utils import gen_random_list

class TERMINAL_COLORS:
        PURPLE    = '\033[95m'
        OKBLUE    = '\033[94m'
        OKCYAN    = '\033[96m'
        OKGREEN   = '\033[92m'
        WARNING   = '\033[93m'
        ERROR     = '\033[91m'
        DEFAULT   = '\033[0m'
        BOLD      = '\033[1m'
        UNDERLINE = '\033[4m'

class ERRORS:
        ERROR = -1

GEN_DATA_MAX = 0xFFFFFFF

data_files_names = []
log_file = open("tests_log.txt", "w")

def get_data_files_names():
        dir_path = str(Path.cwd()) + "/../tests/e2e/data/"
        data_files_counter = 0

        for path in os.listdir(dir_path):
                if os.path.isfile(os.path.join(dir_path, path)):
                        data_files_counter += 1
                        data_files_names.append(os.path.join(dir_path, path))


def check_output_data(n_test, stdout_data, correct_output, exec_time):
        try:
                if stdout_data == correct_output:
                        print(TERMINAL_COLORS.OKGREEN                    + \
                                f"Test {n_test:3} Passed. "              + \
                                f"File {data_files_names[n_test]} "      + \
                                f"Execution time: {exec_time:.03f} sec"  + \
                        TERMINAL_COLORS.DEFAULT
                        )
                else:
                        print(TERMINAL_COLORS.ERROR                       + \
                                f"Test {n_test} NOT Passed. "             + \
                                f"File {data_files_names[n_test]}\n"      + \
                                f"Execution time: {exec_time:.03f} sec\n" + \
                                f"Output={stdout_data}\n"                 + \
                                f"\nRight={correct_output}\n"             + \
                        TERMINAL_COLORS.DEFAULT
                        )
        except:
                print(TERMINAL_COLORS.WARNING                                 + \
                        f"Test {n_test} has fallen. Output of program is:\n'" + \
                        stdout_data + "'"                                     + \
                      TERMINAL_COLORS.DEFAULT
                      )


def get_correct_output (n_triangles, data):
        correct_output = data[n_triangles*3*3:]
        return correct_output


def parse_data_file (file_name):
        with open(file_name) as file:
                try:
                        n_nodes = int(next(file))
                        return n_nodes
                except:
                        print(TERMINAL_COLORS.WARNING            + \
                                f"File {file_name} is broken.\n" + \
                                TERMINAL_COLORS.DEFAULT
                             )                        


def gen_data(file_name):
        n_elems = parse_data_file(file_name)

        random_data = gen_random_list(n_elems)
        sorted_rand_data = random_data.copy()
        sorted_rand_data.sort()

        data = [n_elems]
        sorted_data = []

        for x in random_data:
                data.append(str(x))

        for x in sorted_rand_data:
                sorted_data.append(str(x) + ' ')       

        return data, sorted_data
        

def run_e2e_test(app2run, input_data):
        print(TERMINAL_COLORS.OKCYAN                    + \
                        f"Run {app2run}."               + \
                TERMINAL_COLORS.DEFAULT
                )
        pipe = Popen([app2run], stdout=PIPE, stdin=PIPE)
        
        data_str = str()
        for data in input_data:
                data_str += str(data) + " "

        start_time = time.time()
        stdout_data = pipe.communicate(input=bytes(data_str, "UTF-8"))
        exec_time = time.time() - start_time

        return stdout_data[0].decode(), exec_time
        

def check_app_output(n_test, data, correct_output, output_data, exec_time):
        correct_str = ''.join(str(elem) for elem in correct_output)
        check_output_data(n_test, output_data, correct_str, exec_time)

        return correct_output


def run_e2e_tests(app_name, app2_name=str()):
        for (n_test, file_name) in zip(range(len(data_files_names)), data_files_names):
                # data, correct_output = parse_data_file(file_name)
                data, correct_output = gen_data(file_name)

                output_data, exec_time = run_e2e_test(app_name, data)
                correct_str = check_app_output(n_test, data, correct_output, output_data, exec_time)
                
                log_file.write(f"Input data:\n ")
                for dat in data:
                        log_file.write(f"{dat} ")
                log_file.write(f"\n Correct output:\n {correct_str} \n Output: \n {output_data[:-2]}\n\n")
                
                if (app2_name):
                        output_data, exec_time = run_e2e_test(app2_name, data)
                        check_app_output(n_test, data, correct_output, output_data, exec_time)
                

if __name__ == "__main__":
        get_data_files_names()
        run_e2e_tests("./bitonic_sort")


log_file.close()