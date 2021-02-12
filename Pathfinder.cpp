
#include "Pathfinder.h"
#include <sstream>
#include <fstream>
#include <cmath>
#include <random>

Pathfinder::Pathfinder(){
    for(int i = 0; i < 125; i++){
        maze[i%(MAZE_SIZE-1)][i/(MAZE_SIZE-1)][i%(int(std::pow(MAZE_SIZE-1, 2)))] = FREE;
    }
}

std::string Pathfinder::toString() const {
	std::stringstream ss;
	for (int x = 0; x < MAZE_SIZE; x++) {
		for(int y = 0; y < MAZE_SIZE; y++) {
			for(int z = 0; z < MAZE_SIZE; z++){
				ss << maze[x][y][z] << " ";
			}
			ss << std::endl;
		}
		ss << std::endl;
	}

	return ss.str();
}

bool Pathfinder::importMaze(std::string file_name) {
    int result[MAZE_SIZE][MAZE_SIZE][MAZE_SIZE];
    std::string buffer;
	std::ifstream in(file_name);

    int mazePos = 0;
	if(in.is_open()){
        while(std::getline(in, buffer)){
            if(mazePos > 124) // if there are more than 125 entries
                return false;
            istringstream intBuffer(buffer);
            /*
            x = mazePos%GRID_SIZE-1
            y = mazePos/GRID_SIZE-1 (int version)
            z = mazePos%(GRID_SIZE-1^2)
            */
            intBuffer >> result[mazePos%(MAZE_SIZE-1)][mazePos/(MAZE_SIZE-1)][mazePos%(int(std::pow(MAZE_SIZE-1, 2)))];
            mazePos++;
        }
	}else{
		std::cerr << "There was an error reading file, " << file_name << std::endl;
	}

    if(result[0][0][0] != 1 || result[MAZE_SIZE-1][MAZE_SIZE-1][MAZE_SIZE-1] != 1){
        return false;
    }

    // copy to actual maze
    for (int i = 0; i < 125; i++){
        for(int i = 0; i < 125; i++){
            maze[i%(MAZE_SIZE-1)][i/(MAZE_SIZE-1)][i%(int(std::pow(MAZE_SIZE-1, 2)))] = result[i%(MAZE_SIZE-1)][i/(MAZE_SIZE-1)][i%(int(std::pow(MAZE_SIZE-1, 2)))];
        }
    }

}

void Pathfinder::createRandomMaze() {
    for(int i = 0; i < 125; i++){
        maze[i%(MAZE_SIZE-1)][i/(MAZE_SIZE-1)][i%(int(std::pow(MAZE_SIZE-1, 2)))] = std::rand()%2;
    }
    maze[0][0][0] = maze[1][1][1] = FREE;
}

std::vector<std::string> Pathfinder::solveMaze() {

}

bool Pathfinder::findPath(int x, int y, int z){
    solutionPath.push_back(formatCoords(x, y, z));

    if(x > MAZE_SIZE || y > MAZE_SIZE || z > MAZE_SIZE || x < 0 || y < 0 || z < 0) {
        return false;
    }
    if(maze[x][y][z] == ON_PATH || maze[x][y][z] == OBSTACLE){
        return false;
    }
    if(x == MAZE_SIZE-1 && y == MAZE_SIZE-1 && z == MAZE_SIZE-1){
        return true;
    }

    if(findPath(x+1, y, z) || findPath(x-1, y, z) || findPath(x, y+1, z) || findPath(x, y-1, z) || findPath(x, y, z+1) || findPath(x, y, z-1)){
        return true; // meaning that we have not hit a dead end.
    }else{
        solutionPath.pop_back();
        return false;
    }
    

    maze[x][y][z] = ON_PATH; 
}

std::string formatCoords(int x, int y, int z){
    ostringstream os;
    os << "(" << x << ", " << y << ", " << z << ")";
    return os.str();
}
















