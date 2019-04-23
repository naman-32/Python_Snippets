daysofmonth = [0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]


def isleap(year):
    if year % 4 == 0 and (year % 100 != 0 or year % 400 == 0):
        return True


def noofdays(year, month):
    if not 1 <= month <= 12:
        print('invalid month')

    if isleap(year) and month == 2:
        return 29

    return daysofmonth[month]


print(noofdays(2016, 5))
