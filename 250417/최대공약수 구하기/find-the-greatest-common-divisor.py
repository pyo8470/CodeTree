n, m = map(int, input().split())

import math
# Please write your code here.

def gcd(m,n):
    if m<n :
        ## m이 더 틈
        m,n = n,m
    if m%n==0:
        return n
    else:
        return gcd(n,m%n)
print(gcd(n,m))