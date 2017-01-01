import subprocess
import sys
import os

def writefile(file, output):
	fd = open(file, 'wb')
	fd.write(output)

def runone(choice):
	args=[program, "input"+str(choice)+".txt"]
	print("running line: ", args, "\n")
	output = subprocess.check_output(args)
	if(r):
		print (output.decode("ascii"))
	name="output/output"+str(choice)+".txt"
	writefile(name, output)

def runall():
	length=14
	for i in range(1,length):
		args=[program, "input"+str(i)+".txt"]
		print("running file: ", args, "\n")
		output = subprocess.check_output(args)
		if(r):
			print (output.decode("ascii"))
		name="output/output"+str(i+1)+".txt"
		writefile(name, output)

def main():
	choice = input("enter 1-16 for a specific file or else all will run: ")
	print("")
	if(choice==''):
		choice=14
	else:
		choice = int(choice)

	if(choice>0 and choice<13):
		runone(choice)
	else:
		runall()

r=1
program=""
name=os.name
size=len(sys.argv)
if(size>1):
	if(sys.argv[1]=='r' or sys.argv[1]=='R'):
		r=0
print("running on a: ", name, " system")
if(name=="nt"):
	program="alloc.exe"
else:
	program="./alloc"
main()