poll_active = True
poll = {}
prompt = """f you could visit one place in the world,
where would you go?"""
while poll_active:
    city = input(prompt)
    name = input('what is your name?')
    poll[name] = city
    x = input('Would you like to continue poll? (yes/no)')
    if x == 'no':
        break

print(poll.items())
for nam, cit in poll.items():
    poll[nam] = cit
    print(nam + ' would like to visit ' + cit.title() + '.')
