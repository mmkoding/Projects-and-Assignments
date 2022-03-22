import pygame
import math
from queue import PriorityQueue

SIZE = 750
WINDOW = pygame.display.set_mode((SIZE, SIZE))
pygame.display.set_caption("A* Path Finding Algorithm")

colorRed = (255, 0, 0)
colorGreen = (0, 255, 0)
colorBlue = (0, 0, 255)
colorWhite = (255, 255, 255)
colorBlack = (0, 0, 0)
colorGrey = (128, 128, 128)
colorYellow = (255, 255, 0)
colorPurple = (128, 0, 128)
colorOrange = (255, 140 ,0)
colorCyan = (0, 255, 255)


class Node:
    def __init__(self, row, column, size, total_rows):
        self.row = row
        self.column = column
        self.x = row * size
        self.y = column * size
        self.color = colorWhite
        self.neighbors = []
        self.size = size
        self.total_rows = total_rows
    
    def get_pos(self):
        return self.row, self.column

    def is_closed(self):
        return self.color == colorRed
    
    def is_open(self):
        return self.color == colorGreen
    
    def is_barrier(self):
        return self.color == colorBlack
        
    def is_start(self):
        return self.color == colorOrange
        
    def is_end(self):
        return self.color == colorCyan
        
    def reset(self):
        self.color =  colorWhite
        
    def make_start(self):
        self.color =  colorOrange
        
    def make_closed(self):
        self.color =  colorRed
        
    def make_open(self):
        self.color =  colorGreen
        
    def make_barrier(self):
        self.color = colorBlack
    
    def make_end(self):
        self.color = colorCyan
        
    def make_path(self):
        self.color = colorPurple
        
    def draw(self, win):
        pygame.draw.rect(win, self.color, (self.x, self.y, self.size, self.size))
        
    def update_neighbors(self, grid):
        self.neighbors = []
        if self.row < self.total_rows - 1 and not grid[self.row + 1][self.column].is_barrier(): # DOWN
            self.neighbors.append(grid[self.row + 1][self.column])

        if self.row > 0 and not grid[self.row - 1][self.column].is_barrier(): # UP
            self.neighbors.append(grid[self.row - 1][self.column])

        if self.column < self.total_rows - 1 and not grid[self.row][self.column + 1].is_barrier(): # RIGHT
            self.neighbors.append(grid[self.row][self.column + 1])

        if self.column > 0 and not grid[self.row][self.column - 1].is_barrier(): # LEFT
            self.neighbors.append(grid[self.row][self.column - 1])
    
    def __lt__(self, other):
        return False
        
    
def h(p1, p2):
    x1, y1 = p1
    x2, y2 = p2
    return abs(x1 - x2) + abs(y1 - y2)
    
def reconstruct_path(cameFrom, current, draw):
    while current in cameFrom:
        current = cameFrom[current]
        current.make_path()
        draw()
        
    
def aStar(draw, grid, start, end):
    count = 0
    open_set = PriorityQueue()
    open_set.put((0, count, start))
    cameFrom = {}
    g_score = {Node : float("inf") for row in grid for Node in row}
    g_score[start] = 0
    f_score = {Node : float("inf") for row in grid for Node in row}
    f_score[start] = h(start.get_pos(), end.get_pos())
    
    open_set_hash = {start}
    
    while not open_set.empty():
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                
        current = open_set.get()[2]
        open_set_hash.remove(current)
        
        if current == end:
            reconstruct_path(cameFrom, end, draw)
            start.make_start()
            end.make_end()
            return True
            
        for neighbor in current.neighbors:
            temp_g_score = g_score[current] + 1
            
            if temp_g_score < g_score[neighbor]:
                cameFrom[neighbor] = current
                g_score[neighbor] = temp_g_score
                f_score[neighbor] = temp_g_score + h(neighbor.get_pos(), end.get_pos())
                if neighbor not in open_set_hash:
                    count -=- 1
                    open_set.put((f_score[neighbor], count, neighbor))
                    open_set_hash.add(neighbor)
                    neighbor.make_open()
                
        
        draw()
        
        if current != start:
            current.make_closed()
    
    return False
            
    
def make_grid(rows, size):
    grid = []
    gap = size // rows
    for i in range(rows):
        grid.append([])
        for j in range(rows):
            node = Node(i,j, gap, rows)
            grid[i].append(node)
    
    return grid
        
def draw_grid(win, rows, size):
    gap = size // rows
    for i in range(rows):
        pygame.draw.line(win, colorGrey, (0, i* gap), (size, i * gap))
        for j in range(rows):
            pygame.draw.line(win, colorGrey, (j* gap, 0), (j* gap, size))
            
def draw(win, grid, rows, size):
    win.fill(colorWhite)
    
    for row in grid:
        for Node in row:
            Node.draw(win)
            
    draw_grid(win, rows, size)
    pygame.display.update()
    
def get_clicked_pos(pos, rows, size):
    gap = size // rows
    y, x = pos
    
    row = y // gap
    column = x // gap
    return row, column
    
def main(win, size):
    ROWS = 50
    grid = make_grid(ROWS, size)
    
    start = None 
    end = None
    
    run = True
    started = False
    
    while run:
        draw(win, grid, ROWS, size)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
            if started:
                continue
                
            if pygame.mouse.get_pressed()[0]: #left mouse button
                pos = pygame.mouse.get_pos()
                row, column = get_clicked_pos(pos, ROWS, size)
                Node = grid[row][column]
                if not start and Node != end:
                    start = Node
                    start.make_start()
                elif not end and Node != start:
                    end = Node
                    end.make_end()
                    
                elif Node != end and Node != start:
                    Node.make_barrier()
            
            elif pygame.mouse.get_pressed()[2]: #right mouse
                pos = pygame.mouse.get_pos()
                row, column = get_clicked_pos(pos, ROWS, size)
                Node = grid[row][column]
                Node.reset()
                if Node == start:
                    start = None
                elif Node == end:
                    end = None
                    
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE and start and end:
                    for row in grid:
                        for Node in row:
                            Node.update_neighbors(grid)
                    
                    aStar(lambda: draw(win, grid, ROWS, size), grid, start, end)
                    
                if event.key == pygame.K_c:
                    start = None
                    end = None
                    grid = make_grid(ROWS, size)
            
                
    pygame.quit()
        
main(WINDOW, SIZE)