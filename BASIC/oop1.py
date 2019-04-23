class Employee:
    def __init__(self, first, last, pay):
        self.first = first
        self.last = last
        self.pay = first
        self.email = first + '.' + last + '@company.com'

    def fullname(self):
        return '{} {}'.format(self.first, self.last)


emp_1 = Employee('naman', 'goenka', 50000)
emp_2 = Employee('test', 'user', 45000)
# #emp_1.first = 'naman'
# emp_1.last = 'goenka'
# emp_1.pay = 50000

# emp_2.first = 'test'
# emp_2.last = 'user'
# # emp_2.pay = 45000
print(emp_1.email)
print(Employee.fullname(emp_1))
print(emp_1.fullname())
