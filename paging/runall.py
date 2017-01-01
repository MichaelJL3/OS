import subprocess
import sys
import os

def writefile(file, output):
	fd = open(file, 'wb')
	fd.write(output)

def runone(args, choice):
	if(v):
		args[0]=vprogram
	print("running line: ", args, "\n")
	output = subprocess.check_output(args)
	if(r):
		print (output.decode("ascii"))
	name="output/output"+str(choice)+".txt"
	writefile(name, output)

def runall(arglist):
	length=16
	for i in range(length):
		args=arglist[i]
		if(v):
			args[0]=vprogram
		print("running line: ", args, "\n")
		output = subprocess.check_output(args)
		if(r):
			print (output.decode("ascii"))
		name="output/output"+str(i+1)+".txt"
		writefile(name, output)

def main():
	arglist=[[] for _ in range(16)]
	arglist[0]= [program,"10","10","20","1","10","lru"]
	arglist[1]= [program,"10","10","10","1","100","lru"]
	arglist[2]= [program,"10","10","10","2","10","lru"]
	arglist[3]= [program,"20","10","10","2","10","lru"]
	arglist[4]= [program,"20","10","10","2","10","random"]
	arglist[5]= [program,"20","10","10","2","10","fifo"]
	arglist[6]= [program,"20","10","10","3","10","lru"]
	arglist[7]= [program,"20","10","10","3","10","fifo"]
	arglist[8]= [program,"20","10","10","4","10","lru"]
	arglist[9]= [program,"20","10","10","4","10","random"]
	arglist[10]=[program,"90","10","40","4","100","lru"]
	arglist[11]=[program,"40","10","90","1","100","lru"]
	arglist[12]=[program,"40","10","90","1","100","fifo"]
	arglist[13]=[program,"800","40","400","4","5000","lru"]
	arglist[14]=[program,"10","5","30","4","3","random"]
	arglist[15]=[program,"1000","40","400","4","5000","fifo"]

	choice = input("enter 1-16 for a specific file or else all will run: ")
	print("")
	if(choice==''):
		choice=17
	else:
		choice = int(choice)

	if(choice>0 and choice<17):
		runone(arglist[choice-1], choice)
	else:
		runall(arglist)

v=0
r=1
program=""
vprogram=""
name=os.name
size=len(sys.argv)
if(size>1):
	if(sys.argv[1]=='v' or sys.argv[1]=='V'):
		v=1
	elif(sys.argv[1]=='r' or sys.argv[1]=='R'):
		r=0
if(size>2):
	if(sys.argv[2]=='r' or sys.argv[2]=='R'):
		r=0
print("running on a: ", name, " system")
if(name=="nt"):
	program="paging.exe"
	vprogram="vpaging.exe"
else:
	program="./paging"
	vprogram="./vpaging"
main()