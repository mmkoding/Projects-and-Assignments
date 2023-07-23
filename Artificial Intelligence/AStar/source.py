# template PY file to get started with CENG462-HW1
# do not forget to rename the file before you submit it
import numpy as np
# you can import the related data structures from the package queue
# have fun!
# -mferhata
from queue import PriorityQueue
import copy

def get_height(problem, col):
    height = np.where(problem.grid == 1)
    indices = np.where(height[1] == col)
    return indices[0].size

def heuristic(problem, agent_col, gate_col):
    dist = 0
    for col in range(int(agent_col), int(gate_col)):
        height = get_height(problem, col)
        
        if col + 1 < problem.width:  # Check if the next column is within the grid boundaries
            next_height = get_height(problem, col + 1)
            
            if height > next_height - 2:
                dist += 1
            else:
                dist += next_height - height - 1
    return dist



def astar(problem):
    #print('A*')
    start_state = np.where(problem.grid == 2)
    gate_position = np.where(problem.grid == 4)
    agent_row, agent_col = start_state[0][0], start_state[1][0]
    gate_row, gate_col = gate_position[0][0], gate_position[1][0]

    open_queue = PriorityQueue()
    open_queue.put((heuristic(problem, agent_col, gate_col), 0, start_state, [], problem))
    x = 550000

    while not open_queue.empty() and x:
        x-=1
        f, g, (row, col), path, current_problem = open_queue.get()

        for move in ["R", "L", "CR", "CL"]:
            new_problem = copy.deepcopy(current_problem)
            new_problem.move_agent(move)
            new_agent_row, new_agent_col = np.where(new_problem.grid == 2)

            if (new_agent_row, new_agent_col) != (row, col):  # If the agent moved
                new_g = g + 1
                new_h = heuristic(new_problem, new_agent_col, gate_col)
                new_f = new_g + 2 * new_h
                
                if (new_agent_row, new_agent_col) == (gate_row, gate_col):
                    print("SUCCESS")
                    formatted_path = ' , '.join(path + [move])
                    print(formatted_path)
                    return path + [move]

                open_queue.put((new_f, new_g, (new_agent_row, new_agent_col), path + [move], new_problem))

    print("FAILURE")
    return []  # Return an empty list if there is no solution

    



def idastar(problem):
    #print("IDA*")

    def search(path, g, threshold, problem):
        node = path[-1][1]
        f = g + heuristic(problem, node[1], gate_col)

        if f > threshold:
            return f, None

        if (node[0], node[1]) == (gate_row, gate_col):
            return -1, [p[0] for p in path[1:]]

        min_cost = float('inf')
        for move in ["R", "L", "CR", "CL"]:
            new_problem = copy.deepcopy(problem)
            new_problem.move_agent(move)
            new_agent_row, new_agent_col = np.where(new_problem.grid == 2)

            if (new_agent_row, new_agent_col) != node:
                new_g = g + 1
                cost, result = search(path + [(move, (new_agent_row, new_agent_col))], new_g, threshold, new_problem)

                if cost == -1:
                    return -1, result

                min_cost = min(min_cost, cost)

        return min_cost, None

    start_state = np.where(problem.grid == 2)
    gate_position = np.where(problem.grid == 4)
    agent_row, agent_col = start_state[0][0], start_state[1][0]
    gate_row, gate_col = gate_position[0][0], gate_position[1][0]

    threshold = heuristic(problem, agent_col, gate_col)
    x = 200
    while x:
        x-=1
        result, path = search([("", (agent_row, agent_col))], 0, threshold, problem)
        if result == -1:
            print("SUCCESS")
            formatted_path = ' , '.join(path)
            print(formatted_path)
            return path
        threshold = result

    print("FAILURE")
    return []

class Problem:
    # 0: Empty cell
    # 1: Wall
    # 2: Agent
    # 3: Rock
    # 4: Gate
    def __init__(self):
        self.height = 6
        self.blocking_objs= [1, 3]
        self.falling_objs = [3, 2, 4]
        
    

    def move_agent(self, MOVE):
        # TODO model the agent movement
        # > refer to the assignment PDF when in doubt about a move
        # > if you still cannot decide on the outcome of a move in a certain scenario,
        # > ask it on the discussion board
        agent_row, agent_col = np.where(self.grid == 2)
        

        if agent_row.size == 0 or agent_col.size == 0:
            print("Error: Agent not found in the grid.")
            return

        agent_row, agent_col = agent_row[0], agent_col[0]

        def in_bounds(row, col):
            return 0 <= row < self.height and 0 <= col < self.width

        def handle_move(delta_col):
            new_col = agent_col + delta_col
            if not in_bounds(agent_row, new_col):
                return
            target_cell = self.grid[agent_row, new_col]
            if target_cell == 0 or target_cell == 4:#If the target grid is empty or gate, move agent
                if target_cell == 4: #REACHED TARGET
                    self.grid[agent_row, agent_col] = 0
                    self.grid[agent_row, new_col] = 2
                    #exit_status = True
                else:
                    self.grid[agent_row, agent_col] = 0
                    self.grid[agent_row, new_col] = 2
            elif target_cell == 3:#If target grid is wall or a rock
                if in_bounds(agent_row, new_col + delta_col):
                    if not(in_bounds(agent_row + 1, new_col)) or self.grid[agent_row + 1, new_col] == 0:
                        next_cell = self.grid[agent_row, new_col + delta_col]
                        if next_cell == 0:
                            self.grid[agent_row, agent_col] = 0
                            self.grid[agent_row, new_col] = 2
                            self.grid[agent_row, new_col + delta_col] = 3
                        elif next_cell in self.blocking_objs and self.grid[agent_row + 1, new_col + delta_col] == 0:
                            self.grid[agent_row, agent_col] = 0
                            self.grid[agent_row, new_col] = 2
                            self.grid[agent_row + 1, new_col + delta_col] = 3

        def handle_climb(delta_col):
            new_col = agent_col + delta_col
            if in_bounds(agent_row + 1, new_col):
                target_cell = self.grid[agent_row, new_col]
                above_target = self.grid[agent_row + 1, new_col]
                if target_cell in self.blocking_objs and (above_target == 0 or above_target == 4):
                    if above_target == 4: #REACHED TARGET
                        self.grid[agent_row, agent_col] = 0
                        self.grid[agent_row + 1, new_col] = 2
                        #exit_status = True
                    else:
                        self.grid[agent_row, agent_col] = 0
                        self.grid[agent_row + 1, new_col] = 2

                elif target_cell in self.blocking_objs and above_target == 3:
                    if not(in_bounds(agent_row + 2, new_col)):
                        if(in_bounds(agent_row + 1, new_col+delta_col) and self.grid[agent_row+1, new_col+delta_col] == 0):
                            self.grid[agent_row + 1, new_col + delta_col] = 3
                            self.grid[agent_row + 1, new_col] = 2
                            self.grid[agent_row, agent_col] = 0
                    elif(self.grid[agent_row + 2, new_col] == 0):
                        if(in_bounds(agent_row + 1, new_col+delta_col) and self.grid[agent_row+1, new_col+delta_col] == 0):
                            self.grid[agent_row + 2, new_col + delta_col] = 3
                            self.grid[agent_row + 1, new_col] = 2
                            self.grid[agent_row, agent_col] = 0
                            
        if MOVE == "R":
            handle_move(1)
        elif MOVE == "L":
            handle_move(-1)
        elif MOVE == "CR":
            handle_climb(1)
        elif MOVE == "CL":
            handle_climb(-1)
            
        # after each move you can call free_fall; efficiency is not our main concern
        self.free_fall()

    def free_fall(self):
        # again, efficiency is not our main concern
        for obj in self.falling_objs:
            rows,cols = np.where(self.grid==obj)
            # TODO implement free fall
            for row, col in zip(rows, cols):
                # Check if the object can fall
                if row > 0 and self.grid[row - 1, col] not in self.blocking_objs:
                    new_row = row - 1

                    # Find the top of the first blocking object underneath the falling object
                    while new_row > 0 and self.grid[new_row - 1, col] not in self.blocking_objs:
                        new_row -= 1

                    # Check if the object lands on the exit gate
                    if self.grid[new_row, col] == 4: #reached the exit gate.
                        self.grid[row, col] = 0  # Remove the object
                        if obj == 2:
                            self.grid[new_row, col] = 2
                        #exit_status = True
                    else:
                        # Move the object to its new position and clear its previous position
                        self.grid[new_row, col] = obj
                        self.grid[row, col] = 0

    def init_from_file(self, filename):
        with open(filename) as f:
            lines = f.readlines()

        if lines[0].split()[0] == 'A*':
            self.solver = astar
        else:
            self.solver = idastar

        state = ''
        for line in lines[1:]:
            strings = line.split()
            if strings[0] == 'W':
                # number of specified wall-heights decide the grid's width
                self.width  = len(strings) - 1
                self.grid   = np.zeros((self.height,self.width))
                for col,string in enumerate(strings[1:]):
                    # height is always a nonnegative number
                    height  = int(string)
                    self.grid[height::-1,col] = 1
            else:
                state += strings[0] + ','
                for string in strings[1:]:
                    state += string.zfill(2) + ','
        self.starting_state = state[:-1]
        self.set_from_state(state)

    # a STATE encoding looks like: "R,02,02,A,01,G,09"
    # STATE should start with 'R,'
    # ofc, you are free to change this convention
    def set_from_state(self, state):
        self.grid = np.where(self.grid<2, self.grid, np.zeros(self.grid.shape))

        reading = ''
        for string in state.split(','):
            if string in 'ARG':
                reading = string
            else:
                col = int(string)
                # place the object on top
                self.grid[self.height-1,col] = 'ARG'.index(reading) + 2
                # let it fall
                self.free_fall()
    
    def pretty_print(self):
        print('=='*(self.width+2))
        for row in self.grid[self.height-1::-1,:]:
            print('||', end='')
            for val in row:
                to_disp = ' WARG'[int(val)] + ' '
                print(to_disp, end='')
            print('||')
        print('=='*(self.width+2))


filename = input()
p = Problem()
p.init_from_file(filename)
#p.pretty_print()
p.solver(p)