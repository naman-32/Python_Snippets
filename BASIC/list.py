
courses = ['History' , 'Math', 'Physics', 'CompSci']
print(len(courses))
print(courses[1:3])
print(courses[0])
print(courses[1])
print(courses[:3])
print(courses[1:3])

courses_a = ['art','maths']
#courses.extend(courses_a)
#courses.append(courses_a)
courses.insert(1,courses_a)
print(courses)

courses.remove(courses_a)
print(courses)
courses.remove('History')
print(courses)

courses.sort()
print(courses)
num= [122123,51321,1312231,45454]
num.sort()
print(max(num))
print(min(num))
print(sum(num))
for item in courses:
	print(item)
	

for index,courses2 in enumerate(courses ,start=20):
	print(index,courses2)
		

for i,num2 in enumerate(num,start = 200):
		print(i,num2)

courses.reverse()
courses.sort(reverse = True)#T

dc = courses.pop()
print(dc)
print(courses)

num3= [122123,80000000,1312231,45454]

n = sorted(num3)
print(n)

print (courses.index('Math'))
print('Math' in courses)#truefalse

course_join = ' - '.join(courses)
print(course_join)
course_split = course_join.split('-')
print(course_split)

	
	
	
	

