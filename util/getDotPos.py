TARGET_CHAR = "3"
OUTPUT_FILE = "powerDots.txt"


f = open('pacman.txt', 'r')
grid = []
for line in f.readlines():
    outputLine = (line.strip() + " " + line[::-1].strip()).split()
    grid.append(outputLine)
f.close()

dotPos = []
def main():
    for i in range(len(grid)):
        for j in range(len(grid[0])):
            if grid[i][j] == TARGET_CHAR:
                dotPos.append((i,j))

    writeToFile(OUTPUT_FILE)
    print(dotPos)


def writeToFile(outputPath):
    f = open(outputPath, 'w')

    for pos in dotPos:
        tileX, tileY = pos
        out = f"{tileX} {tileY}\n"
        f.write(out)
    f.close()
main()
