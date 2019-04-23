daysofmonth = [0,31,28,31,30,31,30,31,31,30,31,30,31]

year = int(input("Enter the year"))#imp
month = int(input("Enter the month"))



print(year)
print(month)

def isleap():
	if year %4==0 and (year %100 != 0 or year %400 == 0):
		return True
	
def noofdays():
	if not 1<=month<=12:
		print('invalid month')
		
	if isleap() and month==2:
		return 29
	
	
	return daysofmonth[month]
	
if isleap():
	print(year,'is a leap year')
else:
	print(year,'is not a leap year')
	
print ('number of days in this month is', noofdays())	