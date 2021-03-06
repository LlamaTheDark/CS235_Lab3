
#include "Pathfinder.h"
#include <sstream>
#include <fstream>
#include <cmath>
#include <random>

#define X(i, m) i%m
#define Y(i, m) (i/m)%m
#define Z(i, m) ((i/int(std::pow(m, 2)))%m)

Pathfinder::Pathfinder(){ // WORKING
    srand( time(NULL) );

    for(int i = 0; i < std::pow(MAZE_SIZE, 3); i++){
        // maze[i%MAZE_SIZE][(i/MAZE_SIZE)%MAZE_SIZE][(i/int(std::pow(MAZE_SIZE, 2)))%MAZE_SIZE] = FREE;
        maze[X(i, MAZE_SIZE)][Y(i, MAZE_SIZE)][Z(i, MAZE_SIZE)] = FREE;
    
    }
}

std::string Pathfinder::toString() const { // WORKING
	std::stringstream ss;
	for (int z = 0; z < MAZE_SIZE; z++) {
		for(int y = 0; y < MAZE_SIZE; y++) {
			for(int x = 0; x < MAZE_SIZE; x++){
				ss << maze[x][y][z];
                if(x != MAZE_SIZE-1) ss << " ";
			}
			if(z != MAZE_SIZE-1 || y != MAZE_SIZE-1) ss << std::endl; // this and the other if statment are so that there are no trailing newlines
		}
		ss << std::endl;
	}
	return ss.str();
}

bool Pathfinder::importMaze(std::string file_name) { // WORKING
    // std::cout << "CURRENTLY READING FILE: " << file_name << std::endl;

    int result[MAZE_SIZE][MAZE_SIZE][MAZE_SIZE];
    short int count; // counts number of inputs per line
    int index = 0; // refers to the index of the element being added
    int max_index = std::pow(MAZE_SIZE, 3);

    std::string buffer;
	std::ifstream in(file_name);

	if(in.is_open()){
        while(std::getline(in, buffer)){
            std::istringstream iss(buffer);
            count = 0;
            while(iss >> buffer){
                if(count > MAZE_SIZE) return false;
                if(index > max_index) return false;
                std::string::const_iterator it = buffer.begin();
                while(it != buffer.end() && std::isdigit(*it)) it++;
                if(it!=buffer.end()){
                    return false;
                }

                result[X(index, MAZE_SIZE)][Y(index, MAZE_SIZE)][Z(index, MAZE_SIZE)] = std::stoi(buffer);
                count++;
                index++;
            }
        }
	}else{
        return false; // error reading file
    }

    if(result[0][0][0] != 1 || result[MAZE_SIZE-1][MAZE_SIZE-1][MAZE_SIZE-1] != 1){
        return false;
    }

    // copy to actual maze
    for(int i = 0; i < std::pow(MAZE_SIZE, 3); i++){
        // maze[i%MAZE_SIZE][(i/MAZE_SIZE)%MAZE_SIZE][(i/int(std::pow(MAZE_SIZE, 2)))%MAZE_SIZE] = FREE;
        maze[X(i, MAZE_SIZE)][Y(i, MAZE_SIZE)][Z(i, MAZE_SIZE)] = result[X(i, MAZE_SIZE)][Y(i, MAZE_SIZE)][Z(i, MAZE_SIZE)];
    
    }
    return true;
}

void Pathfinder::createRandomMaze() { // WORKING
    for(int i = 0; i < 125; i++){
        maze[i%MAZE_SIZE][(i/MAZE_SIZE)%MAZE_SIZE][(i/int(std::pow(MAZE_SIZE, 2)))%MAZE_SIZE] = std::rand()%2;
    }
    maze[0][0][0] = maze[MAZE_SIZE][MAZE_SIZE][MAZE_SIZE] = FREE;
}

std::vector<std::string> Pathfinder::solveMaze() {
    int m[MAZE_SIZE][MAZE_SIZE][MAZE_SIZE];
    for(int i = 0; i < std::pow(MAZE_SIZE, 3); i++){
        m[X(i, MAZE_SIZE)][Y(i, MAZE_SIZE)][Z(i, MAZE_SIZE)] = maze[X(i, MAZE_SIZE)][Y(i, MAZE_SIZE)][Z(i, MAZE_SIZE)];
    }

    solutionPath = *(new std::vector<std::string>());

    if(!isValidMaze() || !findPath(0, 0, 0, m)){
        // std::vector<std::string> noSolution;
        // noSolution.push_back("no solution found.");
        // return noSolution;
        return *(new std::vector<std::string>());
    }else{
        // std::cout << "found a path!" << std::endl;
        // std::cout << "\tsolution path: " << std::endl;
        // for(int i = 0; i < solutionPath.size(); i++){
        //     std::cout << solutionPath.at(i) << ": " << solutionPathNums.at(i) << std::endl;
        // }
        // std::cout << std::endl;
        return solutionPath;
    }


    
}

bool Pathfinder::findPath(int x, int y, int z, int m[MAZE_SIZE][MAZE_SIZE][MAZE_SIZE]){
    // std::cout << "testing coordinate: (" << x << ", " << y << ", " << z << ")" << std::endl;
    // std::cout << "\tvalue at coord: " << m[x][y][z] << std::endl;

    solutionPath.push_back(formatCoords(x, y, z));
    solutionPathNums.push_back(maze[x][y][z]);

    // std::cout << "\tcurrent solution path: ";
    // for(auto& e : solutionPath){
    //     std::cout << e << " ";
    // }
    // std::cout << std::endl;

    if(x >= MAZE_SIZE || y >= MAZE_SIZE || z >= MAZE_SIZE || x < 0 || y < 0 || z < 0) {
        // std::cout << "\tBOUNDS TEST FAILED" << std::endl;

        solutionPath.pop_back();
        solutionPathNums.pop_back();
        return false;
    }
    if(m[x][y][z] == ON_PATH || m[x][y][z] == OBSTACLE){
        // std::cout << "\tPATH/OBST TEST FAILED" << std::endl;

        solutionPath.pop_back();
        solutionPathNums.pop_back();
        return false;
    }

    m[x][y][z] = ON_PATH;

    if(x == MAZE_SIZE-1 && y == MAZE_SIZE-1 && z == MAZE_SIZE-1){
        // std::cout << "\tREACHED END OF MAZE" << std::endl;

        return true;
    }

    if(findPath(x+1, y, z, m) || findPath(x-1, y, z, m) || findPath(x, y+1, z, m) || findPath(x, y-1, z, m) || findPath(x, y, z+1, m) || findPath(x, y, z-1, m)){
        return true; // meaning that we have not hit a dead end.
    }else{
        solutionPath.pop_back();
        solutionPathNums.pop_back();
        return false;
    }
}

std::string Pathfinder::formatCoords(int x, int y, int z){
    ostringstream os;
    os << "(" << x << ", " << y << ", " << z << ")";
    return os.str();
}

bool Pathfinder::isValidMaze(){
    if(maze[0][0][0] == FREE && maze[MAZE_SIZE-1][MAZE_SIZE-1][MAZE_SIZE-1] == FREE){
        return true;
    }else{
        return false;
    }
    // return maze[0][0][0] == FREE && maze[MAZE_SIZE-1][MAZE_SIZE-1][MAZE_SIZE-1] == FREE;
}








