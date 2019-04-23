def hello_func():
	#pass
	return 'hello function./!'
	#print ('hello function!')

print(hello_func())
print(hello_func())
print(hello_func())
print(hello_func().upper())

def hello_func2(greeting):
	return '{}, function'.format(greeting)

print (hello_func2('hi'))	

def hellofnc(greeting,name= 'naman'):
	return '{}, {}'.format(greeting,name)
	
print(hellofnc('hi'))

def student_info(*args,**kwargs):
	print(args)
	print(kwargs)
 #student_info('naman', 'boy', 'sr', age=18, year='first')
list = ('naman', 'boy', 'sr')#can be set[]
dict = {'age':18, 'year':'first'}
#student_info(list,dict)
student_info(*list,**dict)

def bdsjkjbjksd(greeting):
	return '{}, {} {}'.format(greeting,greeting,greeting)
print(bdsjkjbjksd('hi' ))