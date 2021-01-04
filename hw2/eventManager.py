#### PART 1 ####
# Filters a file of students' subscription to specific event:
#   orig_file_path: The path to the unfiltered subscription file
#   filtered_file_path: The path to the new filtered file
def fileCorrect(orig_file_path: str, filtered_file_path: str):
    origin_file = open(orig_file_path, "r")
    filtered_file = open(filtered_file_path, "w")
    not_fixed_line_list = []
    id_list = []
    line_list = []
    for line in origin_file:
        not_fixed_line_list.append(line)
    
    for line in reversed(not_fixed_line_list):
        id, name, age, year,semester = line.split(',')
        if checkId(int(id)) and checkYear(int(year), int(age)) \
            and checkName(name) and checkSemester(int(semester)) and checkAge(int(age)):
            if int(id) in id_list:
                continue
            id_list.append(int(id))
            line_list.append(getFixedLine(id, name , age, year, semester))
    id_list.sort()
    writeLines(id_list, line_list, filtered_file)

    origin_file.close()
    filtered_file.close()

def writeLines(id_list, line_list, file):
    for id in id_list:
        id = str(id)
        for line in line_list:
            if id == line[:8]:
                file.write(line)
    

def getFixedLine(id: str, name: str, age: str, year: str, semester: str):
    id = str(int(id))
    age = str(int(age))
    year = str(int(year))
    semester = str(int(semester))
    line = ", ".join([id, fixName(name), age, year, semester])
    line += '\n' 
    return line
   
    
def fixName(name: str):
    split_string = name.split(" ")
    
    fixed_name_list = []
    for temp_string in split_string:
        if temp_string != "":
            fixed_name_list.append(temp_string.strip())
    return " ".join(fixed_name_list)
            
def checkId(id: int):
    return id >= 10000000 and id < 100000000

def checkYear(year: int, age: int):
    return 2020 - age == year

def checkName(name: str):
    for letter in name:
        if not letter.isalpha() and not letter == ' ':
            return False
    return True

def checkAge(age: int):
    return 16 <= age <= 120

def checkSemester(semester: int):
    return semester >= 1    

# Writes the names of the K youngest students which subscribed 
# to the event correctly.
#   in_file_path: The path to the unfiltered subscription file
#   out_file_path: file path of the output file
def printYoungestStudents(in_file_path: str, out_file_path: str, k: int) -> int:
    if k < 1:
        return -1

    fileCorrect(in_file_path, "corrected_file")
    corrected_file = open("corrected_file", "r")
    output_file = open(out_file_path, "w")
    id_list, name_list, age_list = [], [], []
   
    for line in corrected_file:
        id, name, age, year, semester = line.split(', ')
        id_list.append(int(id))
        age_list.append(int(age))
        name_list.append(name)

    data_dict = []
    index = 0
    for name in name_list:
        current_dict = {"name": name, "age": age_list[index], "id": id_list[index]}
        data_dict.append(current_dict)
        index += 1

    data_dict = sorted(data_dict, key=lambda i: (i["age"], i["id"]))
    
    count = 0
    for i in range(k):
        if i < len(name_list):
            count += 1
            output_file.write(data_dict[i]['name'] + '\n')
    
    corrected_file.close()
    output_file.close()

    return count
    
    
# Calculates the avg age for a given semester
#   in_file_path: The path to the unfiltered subscription file
#   retuns the avg, else error codes defined.
def correctAgeAvg(in_file_path: str, semester: int) -> float:
    if semester < 1:
        return -1

    fileCorrect(in_file_path, "corrected_file")
    corrected_file = open("corrected_file", "r")
    
    age_total = 0
    student_count = 0
    for line in corrected_file:
        id, name, age, year, current_semester = line.split(', ')
        if semester == int(current_semester):
            age_total += int(age)
            student_count += 1
    
    if student_count == 0:
        return 0
    
    return age_total / student_count
    

#### PART 2 ####
# Use SWIG :)
import event_manager as EM
#
# # print the events in the list "events" using the functions from hw1
# #   events: list of dictionaries
# #   file_path: file path of the output file
def printEventsList(events :list,file_path :str): #em, event_names: list, event_id_list: list, day: int, month: int, year: int):
    date_list =[]
    for event in events:
        date_list.append(event["date"])

    event_manager = EM.createEventManager(findEarliestDate(date_list))
    for event in events:
        EM.emAddEventByDate(event_manager, event["name"], event["date"], event["id"])

    EM.emPrintAllEvents(event_manager, file_path)

    return event_manager


def findEarliestDate(date_list):
    if len(date_list) < 1:
        return -1
    min_date = date_list[0]
    for date in date_list:
        if EM.dateCompare(min_date, date) > 0:
            min_date = date
    return min_date


def testPrintEventsList(file_path :str):
    events_lists=[{"name":"New Year's Eve","id":1,"date": EM.dateCreate(30, 12, 2020)},\
                    {"name" : "annual Rock & Metal party","id":2,"date":  EM.dateCreate(21, 4, 2021)}, \
                                 {"name" : "Improv","id":3,"date": EM.dateCreate(13, 3, 2021)}, \
                                     {"name" : "Student Festival","id":4,"date": EM.dateCreate(13, 5, 2021)},    ]
    em = printEventsList(events_lists,file_path)
    for event in events_lists:
        EM.dateDestroy(event["date"])
    EM.destroyEventManager(em)
#### Main #### 
# feel free to add more tests and change that section. 
# sys.argv - list of the arguments passed to the python script
if __name__ == "__main__":
   import sys
   if len(sys.argv)>1:
        testPrintEventsList(sys.argv[1]) 
