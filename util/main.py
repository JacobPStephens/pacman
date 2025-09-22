
f = open('pacman.txt', 'r')
grid = []
for line in f.readlines():
    outputLine = (line.strip().split())# + " " + line[::-1].strip()).split()
    grid.append(outputLine)
f.close()
boxes = []

def main():
    for i in range(len(grid)):
        for j in range(len(grid[0])):
            if grid[i][j] == '0' and not isVisited(i,j):
                getBox(i,j)

    outputToFile()

# function gets width of a wall starting at coordinate (i,j) and adds walls to visited
def getWidthOfWall(i, j):
    width = 1
    while bounded(i, j+width) and grid[i][j+width] == '0':
        width += 1
    return width

def getBox(startRow, startCol):
    totalWidth = getWidthOfWall(startRow, startCol)

    height = 1
    rowWidth = totalWidth
    while (startRow + height) < len(grid) and grid[startRow+height][startCol] == '0':
        
        rowWidth = getWidthOfWall(startRow + height, startCol)
        totalWidth = min(rowWidth, totalWidth)
        height += 1

    print(f"BOX FOUND:: {startRow=}, {startCol=}, {totalWidth=}, {height=}")

    boxes.append((startRow, startCol, totalWidth, height))



# returns if a point belongs to already found box
def isVisited(i, j):
    for box in boxes:
        y1, x1, w, h = box
        x2 = x1 + w-1
        y2 = y1 + h-1
        if (x1 <= j <= x2 and y1 <= i <= y2):
            return True
    return False
    
def bounded(i, j):
    return (0 <= i < len(grid) and 0 <= j < len(grid[0]))

def outputToFile():
    out = open('wallsOut.txt', 'w')

    for box in boxes:
        y1, x1, w, h = box
        out.write(f"{x1} {y1} {w} {h}\n")
    out.close()
main()

