import os
import subprocess

PATH="/home/gao/Documents/tiny-comparator/ast_comparator/"
SIGMOID=1
num_student=63
num_prev=23
num_ref=39
num_scope=0
scope=[]    # string list
threshold=0.90
sigma = 2


def sigmoid100(argu):
    return 1/(1 + exp(- sigma * argu*100))

class student:
    # a student has num_scope scopes
    def __init__(self):
        self.scope = []     # append scopeFile objects to the scope field
        self.conformance=[[-100 for x in range(num_ref+1)] for y in range(num_scope+1)]
        self.sim_ref =[[]]
    def overall(self):
        # calculate overall similarity of this student based on different scopes
        # for each scope
        #       if  
        #           avg(empty_dis, ref_dis) - empty_dis/10 < conformance < avg(empty_dis, ref_dis) - empty_dis/10 
        #           safe
        #
        #   conformance matrix
        #           ref1    ref2    ref3
        #   scp1
        #   scp2
        #   scp3
        for x in range(0, num_scope):
            scope_obj = self.scope[x] 
            for y in range(1, num_ref+1):
                if scope_obj.similarity[y] == -1:
                    continue
                print(scope_obj.similarity)
                avg = (scope_obj.similarity[0] + scope_obj.similarity[y])/2
                bias = scope_obj.similarity[0]/5
                if scope_obj.similarity[y] > (avg - bias) and scope_obj.similarity[y] < avg + 2*bias:
                    # safe
                    self.conformance[x][y]=0
                else :
                    # similar
                    if SIGMOID==1:
                        similarity = 1 - sigmoid100(scope_obj.similarity[y] / avg )
                    else:
                         similarity = 1 - (scope_obj.similarity[y] / avg )
                    self.conformance[x][y] = (similarity)
    def listSimilar(self):
        # return a list of int which are ids of similar references
        guilty_ref = []
        for y in range(1, num_ref+1):
            count=0
            for x in range(0, num_scope):
                if self.conformance[x][y] <= -1:
                    continue
                print(self.conformance[x][y])
                if self.conformance[x][y] > threshold :
                    count = count+1
            if count >= 1:
                # calculate weighted avg
                up = 0   
                down = 0
                sim=0
                for c in range(0, num_scope):
                    down = down+self.scope[c].similarity[0]
                for c in range(0, num_scope):
                    up = up+self.conformance[c][y]*self.scope[c].similarity[0]

                sim = up/down
                self.sim_ref[len(self.sim_ref)-1].append(y)
                self.sim_ref[len(self.sim_ref)-1].append(sim)
    def showConformance(self):
            print(self.sim_ref)

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
        self.similarity[0] = int.from_bytes(p, byteorder='little')
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
                self.similarity[y] = int.from_bytes(p, byteorder='little')                
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
        students[x].listsimilar()
        students[x].showConformance()
    #subprocess.call(['java', '-jar', ])
    #subprocess.Popen()
    print("num of students"+str(len(students)))
