sandwich_orders = ['paneer', 'cheese', 'veg','pastrami','pastrami','pastrami']
while 'pastrami' in sandwich_orders:
    sandwich_orders.remove('pastrami')

print(sandwich_orders)
print('restrnt has run out of pastrami')
finished_sandwiches = []
while sandwich_orders:
    sndwch = sandwich_orders.pop()
    print('I have made ' + sndwch + ' sandwich')
    finished_sandwiches.append(sndwch)

print('the following sandwiches were made: ')
for i in finished_sandwiches:
    print(i)
