message = 'bobby\'s world'
message1 = "bobby's world"

message2 = 'bobby"s world'
message3 = """bobbnjnjs
njnjkdnkdknklmdksamkdmskmdkmksd
ndskmnkldsmckldmscklds"""

print(message)
print(message1)
print(message2)
print(message3)

msg = 'Hello World'
print(msg[6])
print(msg[:5])
print(msg[6:])
print(msg[6:9])
print(msg.count('l'))
print(msg.find('njkewnj'))
print(msg.replace('World', 'Universe'))
print(msg.lower())
print(msg.upper())

greeting = 'Hello'
name = 'NaMaN'

invitation = f'{greeting} {name}, Welcome!'
print(invitation)

inv = '{} {}, Welcome!'.format(greeting,name)
print(inv)

hard = greeting + ' ' + name + ', Welcome!'
print(hard)

print('Ya NaMaN, I am fine!')
name = 'NaMaN'
aur = 'Ya'

reply = aur + ' ' + name + ', I am fine!'
print (reply)  

rply = '{} {}, I am fine!'.format(aur,name)
print (rply)

ply = f'{aur} {name}, I am fine!'
print(ply)
print(dir(name))
print(help(str))




