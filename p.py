import sys
current,alarm = sys.argv[1:3]
current = int(current)
alarm = int(alarm)

def p(n):
    x = bin(int(n))[2:]
    l = 20
    x = ("0"*(l-len(x)))+x
    return x[0:l-12] + " " + x[l-12:]
def tod(n):
    x = n&0xFFF
    h = x//60
    m = x%60
    return "TOD: " + str(h) + ":" + str(m) 
print(p(current), tod(current))
print(p(alarm), tod(alarm))
print(" DOW", p(((current & alarm) >> 12)))
timemask = (1<<12) -1
print(" TOD", p(current & timemask))
print("MTOD", p(alarm&timemask))
