f = open('pacman.txt', 'r')
outF = open('map.txt', 'w')
grid = []
for line in f.readlines():
    outputLine = (line.strip() + " " + line[::-1].strip()) + '\n'
    outF.write(outputLine)
f.close()
outF.close()