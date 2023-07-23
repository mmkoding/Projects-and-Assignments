#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/poll.h>
#include "message.h"
#include "logging.h"

struct Bomber{
    int x;
    int y;
	int arg_count;
	std::vector<std::string> args;
	bool active; 
};

struct Bomb{
    pid_t pid;
    coordinate location;
    long interval;
    unsigned int radius;
};

struct Obstacle{
    int x;
    int y;
    int durability;
};

void free_resources(Obstacle* obstacles, Bomber* bombers, int bomber_count, int **bomber_fds, int *pids, int map_height, int map_width, int ***bomb_fds, Bomb **bombs) {
    free(obstacles);
    free(bombers);
    
    for (int i = 0; i < bomber_count; i++) {
        free(bomber_fds[i]);
    }

    free(bomber_fds);
    free(pids);

    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            free(bomb_fds[i][j]);
        }
        free(bomb_fds[i]);
    }

    free(bomb_fds);

    for (int i = 0; i < map_height; i++) {
        free(bombs[i]);
    }
    free(bombs);
}



int main(){


    int map_width;
	int map_height;
	int obstacle_count;
	int bomber_count;
	int active_bomber_count;

    std::cin >> map_width >> map_height >> obstacle_count >> bomber_count;
	active_bomber_count = bomber_count;


    Obstacle* obstacles = (Obstacle*)malloc(obstacle_count * sizeof(Obstacle));
    for (int i = 0; i < obstacle_count; i++){
        std::cin >> obstacles[i].x >> obstacles[i].y >> obstacles[i].durability;
    }

	std::string arg;
	std::string executable_path;
    Bomber* bombers = (Bomber*)malloc(bomber_count * sizeof(Bomber));
    for (int i = 0; i < bomber_count; i++){
		
        std::cin >> bombers[i].x >> bombers[i].y >> bombers[i].arg_count >> executable_path;
		
		std::vector<std::string> args;
        for (int j = 1; j < bombers[i].arg_count; j++) {
            std::cin >> arg;
            bombers[i].args.push_back(arg);
        }
		
        bombers[i].active = true;
    }

	
	int*** bomb_fds = (int***)malloc(map_height * sizeof(int**));
	
    for (int i = 0; i < map_height; i++){
        bomb_fds[i] = (int**)malloc(map_width * sizeof(int*));
        for (int j = 0; j < map_width; j++){
            bomb_fds[i][j] = (int*)malloc(2 * sizeof(int));
        }
    }

    Bomb** bombs = (Bomb**)malloc(map_height * sizeof(Bomb*));
	
    for (int i = 0; i < map_height; i++){
        bombs[i] = (Bomb*)malloc(map_width * sizeof(Bomb));
        for (int j = 0; j < map_width; j++){
            bombs[i][j].pid = -1;
        }
    }
	

    int** bomber_fds = (int**)malloc(bomber_count * sizeof(int*));
	
    for (int i = 0; i < bomber_count; i++){
        bomber_fds[i] = (int*)malloc(2*sizeof(int));
        PIPE(bomber_fds[i]);
    }    

    pid_t* pids = (pid_t*)malloc(bomber_count * sizeof(pid_t));
	
    for (int i = 0; i < bomber_count; i++){
        pids[i] = fork();
        if(pids[i] == 0){
            dup2(bomber_fds[i][1],0);
            dup2(bomber_fds[i][1],1);
            close(bomber_fds[i][0]);
			
            std::vector<const char*> cargs;
            cargs.push_back(executable_path.c_str());
            for (auto& arg : bombers[i].args) {
                cargs.push_back(arg.c_str());
            }
            cargs.push_back(nullptr);
            execv(executable_path.c_str(), const_cast<char**>(&cargs[0]));
        }
        else if(pids[i] == -1){
            perror("fork");
            exit(1);
        }
        else
            close(bomber_fds[i][1]);
    }
   
    im message;
    om response;
    
    while (active_bomber_count > 1){
        		
        struct pollfd *poll_bomb_fds = (struct pollfd*)malloc(map_height * map_width *sizeof(struct pollfd));
        for (int i = 0; i < map_height; i++){
            for (int j = 0; j < map_width; j++){
		        poll_bomb_fds[i*map_width + j].revents = 0;
                poll_bomb_fds[i*map_width + j].fd = bomb_fds[i][j][1];
                poll_bomb_fds[i*map_width + j].events = POLLIN;
            }
         
        }
        int bomb_ret = poll(poll_bomb_fds,map_height*map_width,0);
        if (bomb_ret > 0){
			for (int i = 0; i < map_height; i++){
				for (int j = 0; j < map_width; j++){
					if(poll_bomb_fds[i*map_width + j].revents && bombs[i][j].pid > 0){
						read_data(bomb_fds[i][j][0],&message);
						
						if (message.type == BOMB_EXPLODE) {
							//printf("ARRIVED HERE\n");
							int radius = bombs[i][j].radius;
							coordinate bomb_location = bombs[i][j].location;

							for (int direction = 0; direction < 4; direction++) {
								int dx = (direction == 0) - (direction == 1);
								int dy = (direction == 2) - (direction == 3);

								for (int step = 1; step <= radius; step++) {
									int x = bomb_location.x + step * dx;
									int y = bomb_location.y + step * dy;

									if (x >= 0 && x < map_width && y >= 0 && y < map_height) {
										for (int o = 0; o < obstacle_count; o++) {
											if (obstacles[o].x == x && obstacles[o].y == y) {
												if (obstacles[o].durability > 0) {
													obstacles[o].durability--;
												}
												break;
											}
										}

										for (int b = 0; b < bomber_count; b++) {
											if (bombers[b].active && x == bombers[b].x && y == bombers[b].y) {
												bombers[b].active = false;
												active_bomber_count--;
											}
										}
									} else {
										break;
									}
								}
							}

							bombs[i][j].pid = -1;
						}
					
					}
					
				}
			}
        }
		
		
        struct pollfd *poll_bomber_fds = (struct pollfd*)malloc(bomber_count*sizeof(struct pollfd));
        for (int i = 0; i < bomber_count; i++){

            poll_bomber_fds[i].revents = 0;
            poll_bomber_fds[i].fd = bomber_fds[i][1];
            poll_bomber_fds[i].events = POLLIN;
        }
        int ret = poll(poll_bomber_fds,bomber_count,0);
        if (ret > 0){
        
            for (int i = 0; i < bomber_count; i++){
                if(poll_bomber_fds[i].revents && bombers[i].active){
                    read_data(bomber_fds[i][0],&message);
					imp in = {pids[i], &message};
                    print_output(&in,NULL,NULL,NULL);
					
					switch(message.type){
						case BOMBER_START: {
							response.type = BOMBER_LOCATION;
							response.data.new_position.x = bombers[i].x;
							response.data.new_position.y = bombers[i].y;
							send_message(bomber_fds[i][0],&response);
							omp out = {pids[i], &response};
							print_output(NULL,&out,NULL,NULL);
							break;
						}
						
						case BOMBER_SEE: {
							response.type = BOMBER_VISION;
							std::vector<od> visible_objects;

							for (int dx = -3; dx <= 3; dx++) {
								for (int dy = -3; dy <= 3; dy++) {
									if (abs(dx) + abs(dy) <= 3 && (dx != 0 || dy != 0)) {
										coordinate visible_coord;
										visible_coord.x = bombers[i].x + dx;
										visible_coord.y = bombers[i].y + dy;

										bool blocked = false;
										for (int o = 0; o < obstacle_count; o++) {
											if (visible_coord.x == obstacles[o].x && visible_coord.y == obstacles[o].y) {
												blocked = true;
												break;
											}
										}
										if (blocked) continue;

										for (int b = 0; b < bomber_count; b++) {
											if (bombers[b].active && visible_coord.x == bombers[b].x && visible_coord.y == bombers[b].y) {
												od obj = {visible_coord, BOMBER};
												visible_objects.push_back(obj);
												break;
											}
										}
										if ((visible_coord.x >= 0) && (visible_coord.x < map_width) && (visible_coord.y >= 0) && (visible_coord.y < map_height) && bombs[visible_coord.x][visible_coord.y].pid != -1) {
											od obj = {visible_coord, BOMB};
											visible_objects.push_back(obj);
										}
									}
								}
							}

							response.data.object_count = visible_objects.size();
							send_message(bomber_fds[i][0], &response);

							send_object_data(bomber_fds[i][0], visible_objects.size(), visible_objects.data());

							omp out = {pids[i], &response};
							print_output(NULL, &out, NULL, visible_objects.data());

							break;
						}

						
						case BOMBER_MOVE: {
							int target_x = message.data.target_position.x;
							int target_y = message.data.target_position.y;

							bool valid_move = false;
							int current_x = bombers[i].x;
							int current_y = bombers[i].y;

							if (((abs(target_x - current_x) == 1 && abs(target_y - current_y) == 0) ||
								 (abs(target_x - current_x) == 0 && abs(target_y - current_y) == 1)) &&
								target_x >= 0 && target_x < map_width &&
								target_y >= 0 && target_y < map_height) {

								bool blocked_by_obstacle = false;
								for (int j = 0; j < obstacle_count; j++) {
									if (target_x == obstacles[j].x && target_y == obstacles[j].y) {
										blocked_by_obstacle = true;
										break;
									}
								}

								bool blocked_by_bomber = false;
								if (!blocked_by_obstacle) {
									for (int j = 0; j < bomber_count; j++) {
										if (i != j && bombers[j].active && target_x == bombers[j].x && target_y == bombers[j].y) {
											blocked_by_bomber = true;
											break;
										}
									}
								}

								valid_move = !blocked_by_obstacle && !blocked_by_bomber;
							}

							if (valid_move) {
								bombers[i].x = target_x;
								bombers[i].y = target_y;
							}

							response.type = BOMBER_LOCATION;
							response.data.new_position.x = bombers[i].x;
							response.data.new_position.y = bombers[i].y;
							send_message(bomber_fds[i][0], &response);

							omp out = {pids[i], &response};
							print_output(NULL, &out, NULL, NULL);
							break;
						}
						case BOMBER_PLANT: {
							int explosion_interval = message.data.bomb_info.interval;
							int explosion_radius = message.data.bomb_info.radius;

							int current_x = bombers[i].x;
							int current_y = bombers[i].y;

							bool bomb_exists = (bombs[current_x][current_y].pid != -1);

							bool plant_success = false;
							if (!bomb_exists) {
								bombs[current_x][current_y].pid = fork();
								bombs[current_x][current_y].interval = explosion_interval;
								bombs[current_x][current_y].radius = explosion_radius;

								if (bombs[current_x][current_y].pid == 0) {
									PIPE(bomb_fds[current_x][current_y]);
									dup2(bomb_fds[current_x][current_y][1], fileno(stdin));
									dup2(bomb_fds[current_x][current_y][1], fileno(stdout));
									close(bomb_fds[current_x][current_y][0]);

									char interval_str[20];
									snprintf(interval_str, sizeof(interval_str), "%d", explosion_interval);
									execl("./bomb", "bomb", interval_str, (char *)NULL);
									perror("execl");
									exit(1);
								} else if (bombs[current_x][current_y].pid > 0) {
									// Parent process: add the new bomb to the game state
									close(bomb_fds[current_x][current_y][1]);
									plant_success = true;
								} else {
									perror("fork");
								}
							}

							// Send a BOMBER_PLANT_RESULT message with the plant result
							response.type = BOMBER_PLANT_RESULT;
							response.data.planted = plant_success;
							send_message(bomber_fds[i][0], &response);

							// Print outgoing message
							omp out = {pids[i], &response};
							print_output(NULL, &out, NULL, NULL);
							break;
						}

						default:{
							break;
						}
						
					
					}
                    
						
                }
            }
        }
		
		usleep(1000);
    }

    free_resources(obstacles, bombers, bomber_count, bomber_fds, pids, map_height, map_width, bomb_fds, bombs);


    return 0;
}