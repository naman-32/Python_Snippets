dictionary = {'name' : 'naman','age' :  25,'stream' :'pcm'}
print(dictionary)
print(dictionary['name'])
print(dictionary['age'])
print(len(dictionary))
dictionary[12] = 'number'
print(dictionary) 
print(dictionary.get('stream'))
print(dictionary[12])
print(dictionary.get('phone','Not Found'))
dictionary.update({'clge':'bits','brnch':'cs'})

print(dictionary)
del dictionary[12]
stream =  dictionary.pop('stream')
print(stream)
print(dictionary)

print(dictionary.keys())
print(dictionary.values())
print(dictionary.items())

for key in dictionary:
	print(key)
	
for y,v in dictionary.items():
		print(y,v)