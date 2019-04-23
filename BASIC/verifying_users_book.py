unconfirmed_users = ['nain', 'josh', 'faltu']
confirmed_users = []
while unconfirmed_users:
    confirming_user = unconfirmed_users.pop()
    print('verification of ' + confirming_user + ' is being done.')
    confirmed_users.append(confirming_user)
print('Verification of following users have been confirmed')
for i in confirmed_users:
    print(i.title())
