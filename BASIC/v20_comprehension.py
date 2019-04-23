nums = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
my_dict = {}
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
#my_list = filter(lambda: n % 2 == 0, nums)
# for num in range(4):
#   for letter in 'abcd':
#     my_list.append((letter, num))
player = ['dh', 'sn', 'sc']
game = ['cricket', 'badminton', 'football']
# for player in player:
#   for game in game:
#      my_list.append((player, game))
# for pl, gm in zip(player, game):
#   my_dict[pl] = gm
#my_dict = {(pl, gm) for pl, gm in zip(player, game)}
#my_dict = {pl: gm for pl, gm in zip(player, game) if pl != 'sn'}
# print(my_dict)
my_set = set()
# for n in nums:
#   my_set.add(n)
#my_set = {n for n in nums}


def gen_func(nums):
    for n in nums:
        yield n * n


njeknjekl = gen_func(nums)

for i in njeknjekl:
    print(i)
