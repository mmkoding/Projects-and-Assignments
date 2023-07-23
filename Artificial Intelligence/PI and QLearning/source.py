import sys
import numpy as np
import random

ACTIONS = [(0, 1), (1, 0), (0, -1), (-1, 0)]  # Actions: Up, Right, Down, Left
ACTION_MAPPING = ['0', '1', '2', '3']  # Corresponding to indexes 0, 1, 2, 3

def parse_input(filepath):
    with open(filepath, 'r') as file:
        lines = file.readlines()

    data = {}

    # Parse method
    data['method'] = lines[0].strip()

    if data['method'] == 'P':
        data['iterations'] = int(lines[1])
        data['gamma'] = float(lines[2])
        data['epsilon'] = float(lines[3])
        start_line = 4
    else:  # Q-learning
        data['episodes'] = int(lines[1])
        data['alpha'] = float(lines[2])
        data['gamma'] = float(lines[3])
        data['epsilon'] = float(lines[4])
        start_line = 5

    # Parse grid dimensions
    data['N'], data['M'] = map(int, lines[start_line].split())

    # Parse obstacles
    data['obstacles'] = []
    num_obstacles = int(lines[start_line + 1])
    for i in range(start_line + 2, start_line + 2 + num_obstacles):
        data['obstacles'].append(tuple(map(int, lines[i].split())))

    # Parse puddles
    data['puddles'] = []
    num_puddles = int(lines[start_line + 2 + num_obstacles])
    start = start_line + 3 + num_obstacles
    for i in range(start, start + num_puddles):
        data['puddles'].append(tuple(map(int, lines[i].split())))

    # Parse goal
    data['goal'] = tuple(map(int, lines[start + num_puddles].split()))

    # Parse rewards
    data['r_d'], data['r_o'], data['r_p'], data['r_g'] = map(float, lines[start + num_puddles + 1].split())

    return data
    
def write_output(policy, goal, obstacles, output_filepath):
    M, N = policy.shape
    with open(output_filepath, 'w') as file:
        for i in range(1, M):
            for j in range(1, N):
                if (i, j) != goal and (i, j) not in obstacles:
                    action_str = ACTION_MAPPING[int(policy[i][j])]
                    file.write(f"{i} {j} {action_str}\n")



def is_valid(M, N, x, y, obstacles):
    return 1 <= x <= M and 1 <= y <= N and (x, y) not in obstacles

def transition_probability(current_action, actual_action):
    if current_action == actual_action:
        return 0.8
    elif (current_action - actual_action) % 4 == 1 or (current_action - actual_action) % 4 == 3:
        return 0.09
    else:
        return 0.02

def reward(x, y, r_d, r_o, r_p, r_g, goal, obstacles, puddles):
    if (x, y) == goal:
        return r_g
    elif (x, y) in obstacles:
        return r_o
    elif (x, y) in puddles:
        return r_p
    else:
        return r_d

def policy_iteration(data):
    M, N = data['M'], data['N']
    gamma = data['gamma']
    iterations = data['iterations']
    goal = data['goal']
    obstacles = data['obstacles']
    puddles = data['puddles']
    r_d, r_o, r_p, r_g = data['r_d'], data['r_o'], data['r_p'], data['r_g']

    V = np.zeros((M+1, N+1))  # State-value function
    pi = np.zeros((M+1, N+1))  # Policy

    for _ in range(iterations):
        # Policy Evaluation
        for i in range(1, M+1):
            for j in range(1, N+1):
                if (i, j) == goal:
                    continue
                V_prev = V[i][j]
                action_values = []
                for a in range(4):  # For each action
                    v = 0
                    for aa in range(4):  # For each actual action
                        dx, dy = ACTIONS[aa]
                        next_x, next_y = i + dx, j + dy
                        if not is_valid(M, N, next_x, next_y, obstacles):
                            next_x, next_y = i, j  # If the next state is not valid, stay in the current state
                        r = reward(next_x, next_y, r_d, r_o, r_p, r_g, goal, obstacles, puddles)
                        v += transition_probability(a, aa) * (r + gamma * V[next_x][next_y])
                    action_values.append(v)
                V[i][j] = max(action_values)  # Update state-value function

        # Policy Improvement
        for i in range(1, M+1):
            for j in range(1, N+1):
                if (i, j) == goal:
                    continue
                action_values = []
                for a in range(4):  # For each action
                    v = 0
                    for aa in range(4):  # For each actual action
                        dx, dy = ACTIONS[aa]
                        next_x, next_y = i + dx, j + dy
                        if not is_valid(M, N, next_x, next_y, obstacles):
                            next_x, next_y = i, j  # If the next state is not valid, stay in the current state
                        r = reward(next_x, next_y, r_d, r_o, r_p, r_g, goal, obstacles, puddles)
                        v += transition_probability(a, aa) * (r + gamma * V[next_x][next_y])
                    action_values.append(v)
                pi[i][j] = np.argmax(action_values)  # Update policy

    return pi

def is_terminal(state, goal):
    return state == goal


def get_start_state(M, N, goal, obstacles, puddles):
    while True:
        state = (random.randint(1, M), random.randint(1, N))
        if state not in obstacles and state not in puddles and state != goal:
            return state
    
def q_learning(data):
    M, N = data['M'], data['N']
    alpha = data['alpha']
    gamma = data['gamma']
    epsilon = data['epsilon']
    episodes = data['episodes']
    goal = data['goal']
    obstacles = data['obstacles']
    puddles = data['puddles']
    r_d, r_o, r_p, r_g = data['r_d'], data['r_o'], data['r_p'], data['r_g']

    Q = np.zeros((M+1, N+1, 4))  # Q function
    pi = np.zeros((M+1, N+1))  # Policy

    for _ in range(episodes):
        # Initialize start state for each episode
        state = get_start_state(M, N, goal, obstacles, puddles)

        while not is_terminal(state, goal):
            if np.random.uniform(0, 1) < epsilon:
                # Exploration: choose a random action
                action = np.random.choice(4)
            else:
                # Exploitation: choose the action with max Q-value
                action = np.argmax(Q[state[0]][state[1]])

            # Perform the action
            dx, dy = ACTIONS[action]
            next_state = (state[0] + dx, state[1] + dy)

            # If the next state is not valid, stay in the current state
            if not is_valid(M, N, *next_state, obstacles):
                next_state = state

            r = reward(*next_state, r_d, r_o, r_p, r_g, goal, obstacles, puddles)

            # Update Q-value
            Q[state[0]][state[1]][action] = (1 - alpha) * Q[state[0]][state[1]][action] \
                                            + alpha * (r + gamma * np.max(Q[next_state[0]][next_state[1]]))

            # Transition to the next state
            state = next_state

    # Derive the policy from Q
    for i in range(1, M+1):
        for j in range(1, N+1):
            if (i, j) == goal:
                continue
            pi[i][j] = np.argmax(Q[i][j])  # Update policy

    return pi





if __name__ == "__main__":
    input_filepath = sys.argv[1]
    output_filepath = sys.argv[2]

    data = parse_input(input_filepath)
    if data['method'] == 'P':
        optimal_policy = policy_iteration(data)
    elif data['method'] == 'Q':
        optimal_policy = q_learning(data)

    write_output(optimal_policy, data['goal'], data['obstacles'], output_filepath)

