nums = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
my_list = []
# for n in nums:
#   my_list.append(n)
#my_list = [n for n in nums]
#my_list = []
# for n in nums:
#   my_list.append(n * n)
#my_list = [n * n for n in nums]
#my_list = map(lambda n: n * n, nums)
# for n in nums:
#   if n % 2 == 0:
#      my_list.append(n)
#my_list = [n for n in nums if n % 2 == 0]
my_list = filter(lambda n:  n % 2 == 0, nums)
print(my_list)
