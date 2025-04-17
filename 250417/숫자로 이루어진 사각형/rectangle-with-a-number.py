N = int(input())

# Please write your code here.
num = 1
for i in range(N):
    for j in range(N):
        print(num, end=" ")
        if num == 9:
            num = 0
        num+=1
    print()