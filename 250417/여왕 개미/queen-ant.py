import sys
input = sys.stdin.readline
Q = int(input())

commands = [list(map(int,input().split())) for _ in range(Q)]
house_number = 1
def 실행():
    houses = {}
    answer = 0
    for command in commands:
        if command[0] == 100:
            build_town(command,houses)
        if command[0] == 200:
            build_house(command,houses)
        if command[0] == 300:
            crash_house(command,houses)
        if command[0] == 400:
            answer = recon(command[1],list(houses.values()))
            print(answer)
## 마을 건설
def build_town(command,houses):
    N = command[1]
    global house_number
    for index in range(2,2+N):
        houses[house_number] = command[index]
        house_number+=1
        
## 개미집 
def build_house(command,houses):
    p = command[1]
    global house_number
    houses[house_number] = p
    house_number+=1
## 철거
def crash_house(command,houses):
    q = command[1] 
    del houses[q]
    
## 정찰
def recon(r,houses):
    ## r개의 개미 
    
    
    houses.sort()
    lo = 0
    hi = houses[-1] - houses[0]
    def can_cover(max_time):
        count = 1 ## 개미의 수 
        start = houses[0] 
        for h in houses[1:]:
            if h - start > max_time: ## h(현재 탐색하는 집의 위치) - start(시작점)
                ## mid 보다 높다 -> count마리로는 시간내에 탐색이 불가능하다. -> 개미를 늘려야 한다
                count+=1
                start = h
        ## 현재 마리수 count가 R보다 작아야지 가능한 경우이다.
        return count <= r


    while lo <= hi:
        mid = (lo + hi) // 2
        # print("mid :" , mid)
        if can_cover(mid):  # 정찰 가능하면 시간 줄이기
            answer = mid
            hi = mid - 1
        else:  # 정찰 불가능하면 시간 늘리기
            lo = mid + 1
    return answer

실행()