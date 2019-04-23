f = """hjbhjbhjmkmfkl"""
f += """nkjnk
    ndksljdklsjkldjkldscjklds: """
x = input(f)
print(x)
age = input('age ')
age = int(age)
print(age >= 18)
print(age <= 50)
number = input("Enter a number, and I'll tell you if it's even or odd: ")
number = int(number)
if number % 2 == 0:
    print("\nThe number " + str(number) + " is even.")
else:
    print("\nThe number " + str(number) + " is odd.")

prompt = """I will repeat whatever you say
Quit by entering the 'quit': """

message = ""
while message[-4:] != 'quit':
    message = message + ' ' + input(prompt)
    if message[-4:] != 'quit':
        print(message)

prompt1 = """\ntell your best city
to exit say 'quit': """

while True:
    city = input(prompt1)

    if city == 'quit':
        break
    print('I would like to visit ' + city.title() + '!')
