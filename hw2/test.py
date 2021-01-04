from eventManager import *

if __name__ == '__main__':
    file_name = "3.1.2.1_young_students_many_wrong_students_and_big_dataset_k_100000.txt"
    full_file_name = "saar_tests/inputs/" + file_name
    """ fileCorrect(full_file_name,"tests/our_out") """
    # num1 = correctAgeAvg(full_file_name, 1)
    printYoungestStudents(full_file_name, "tests/our_youngest_out", 20)
