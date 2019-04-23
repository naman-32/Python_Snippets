prompt = """What is your age?
enter 'quit' to exit: """
#active = True
age = ''
while age != 'quit':
    age = input(prompt)
    if age == 'quit':
        break  # active = False
    if age != 'quit':
        age = int(age)
        if age < 3:
            print('your ticket is free')
        elif 3 <= age <= 12:
            print('your ticket is $10')
        else:
            print('your ticket is $15')
