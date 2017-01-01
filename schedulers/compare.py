
def comp(str1, str2):
	tot=0
	for letter in str1:
		if(letter in str2):
			tot+=1
	print (str1)
	print (str2)
	return tot

def compare(list1, list2):
	size=len(list1)
	size2=len(list2)
	tot=0
	rel=0
	str1=0
	str2=0
	if(size>size2):
		size=size2
	for x in range(size):
		str1=len(list1[x])
		str2=len(list2[x])
		if(str1<str2):
			tot+=str1
			rel+=comp(list1[x], list2[x])
		else:
			tot+=str2
			rel+=comp(list2[x], list1[x])
	print(tot) 
	print(rel)
	return (rel/tot)

def isvalid(letter):
	if(letter.isalpha()):
		return 1
	else:
		return 0

def process(lists, i):
	string=lists[i]
	new_string=""
	for letter in string:
		if(isvalid(letter)):
			new_string+=letter
	lists[i]=new_string

def normalize(lists):
	size=len(lists)
	for x in range(size):
		if(lists[x]!=""):
			process(lists, x)
			lists[x]=lists[x].lower()

def read(listread, fileread):
	with open(fileread, "r") as f:
		for line in f:
			if line!="":
				listread.append(line)

def new(list1, temp):
	size=len(list1)
	for x in range(size):
		if(list1[x]!=""):
			if(list1[x]!="theoriginalinputwas"):
				temp.append(list1[x])

def main():
	x=input("Enter in a number for a file comparison for example Input1 = 1\n>")
	file1="output/original/output"+x+".txt"
	file2="output/output"+x+".txt"
	list1=[]
	list2=[]
	read(list1, file1)
	read(list2, file2)
	normalize(list1)
	normalize(list2)
	temp=[]
	new(list1, temp)
	list1=temp
	temp=[]
	list2=new(list2, temp)
	list2=temp
	tot=compare(list1, list2)
	print(tot)

main()