import os
import subprocess

PATH="/home/gao/Documents/tiny-comparator/ast_comparator/"
num_student=63
num_prev=23
num_ref=39
num_scope=0
scope=[]    # string array
threshold=90

class student:
    # a student has num_scope scopes
    def __init__(self):
        self.scope = []     # append scopeFile objects to the scope field
    def overall(self):
        # calculate overall similarity of this student

        conformance = []
        # all index of references which is similar to 


class scopeFile:
    # one scope corresponds to one file
    def __init__(self, name, studentId, scopeid):
        self.filename=name      # file name
        self.similarity=[0 for y in range(num_ref+1)]
        self.id = studentId
        self.scope = scopeid
    # collect similarity from the references
    #       empty ref1   ref2    ref3    ... 
    # scpe1 

    # empty distance
    def empty_dis(self):
        p = subprocess.check_output(['java', '-jar', PATH+'apted.jar', '-f', self.filename, PATH+"brk_tree/empty.tree"])
        self.similarity[0] = p
        #print(self.similarity[index][0])

    # array distance from all references
    def add_distance(self):
            for y in range(1, num_ref+1):
                refer_name = PATH+"brk_tree/reference_"+str(y)+"_tsh.c.001t.tu."+scope[self.scope]+".tree"
                if not os.path.exists(refer_name):
                    self.similarity[y] = -1
                    continue
                elif os.stat(refer_name).st_size == 0:
                    self.similarity[y] = -1
                    continue

                #print("Comparing student {:d} with {:s}".format(self.id, "reference_"+str(y)+"_tsh.c.001t.tu."+scope[self.scope]+".tree"))
                p = subprocess.check_output(['java', '-jar', PATH+'apted.jar', '-f', self.filename, refer_name])
                self.similarity[y] = p
            print("Compare student {:d}, scope {:s} with all references".format(self.id, scope[self.scope]))

    def myprint(self):
            for y in range(num_ref+1):  # print to ref 39
                print("(x, y): (0 , "+str(y)+")")
                print((self.similarity[y]))


if __name__ == "__main__":

    with open(PATH+"test/scope.conf") as f:
        content = f.readlines()

    scope = [x.strip() for x in content]
    scope.remove(scope[0])
    num_scope = len(scope)
    students = []
    
    for x in range(0, num_student+1 - num_student):
        s = student()
        for y in range(0, num_scope):
            s_name = PATH+"brk_tree/"+str(x)+"_tsh.c.001t.tu."+scope[y]+".tree"
            scope1=scopeFile(s_name, x, y)
            # compute empty distance
            scope1.empty_dis()
            # compare student's this scope with each reference of this scope
            scope1.add_distance()
            # add student's this scope to object of this student
            s.scope.append(scope1)
        students.append(s)
    for x in range(0, len(students)):
    # calculate overall similarity---in terms of each reference
        students[x].overall()
    #subprocess.call(['java', '-jar', ])
    #subprocess.Popen()
    print("num of students"+str(len(students)))
