import random

def gen_random_list(n_elems, min_number=-0xFF, max_number=0xFF):
        list = []
        for x in range(n_elems):
                list.append(random.randint(min_number, max_number))

        return list