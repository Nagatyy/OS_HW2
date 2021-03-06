#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>

double calculateDistance(double, double);
double calculatePI(int, int);

int main(int argc, char** argv){
	pid_t child;
	double x;
	double y;
	int numOfChildren;
	int numOfPoints;


	if (argc != 3){
		std::cout << "Incorrect Number of Arguments!" <<std::endl;
		return 0; //exit
	}

	numOfChildren = atoi(argv[1]);
	numOfPoints = atoi(argv[2]);

	if(numOfChildren <= 0){
		std::cout << "number of children cannot be 0!" <<std::endl;
		return 0;
	}

	int numberOfPointsPerChild = numOfPoints / numOfChildren; // force num of points to be an integer in case 
	int pHandle[numOfChildren][2];
	int numberOfPointsInSquare = 0;
	int totalNumberOfPoints = 0;

	for(int i = 0; i < numOfChildren; i++){
		int pipeStatus = pipe(pHandle[i]);
		if(pipeStatus < 0){
			std::cout << "Could not pipe!" << std::endl;
			return 0;
		}

		child = fork();
		srand(time(NULL)^getpid());

		if (child < 0) { // failure
			std::cout << "Could Not Fork!" << std::endl;
			return 1;
		}
		else if (child == 0) { // child
			for(int j = 0; j < numberOfPointsPerChild; j++){

				x = (double)rand()/RAND_MAX*2.0-1.0;
				y = (double)rand()/RAND_MAX*2.0-1.0;

				if(calculateDistance(x,y) < 0.5)
					numberOfPointsInSquare++;
			}
			close(pHandle[i][0]); 
			write(pHandle[i][1], &numberOfPointsInSquare, sizeof(int));
			close(pHandle[i][1]);
			exit(0);
		}
	}

			wait(NULL);
			for(int z = 0; z < numOfChildren; z++){
			close(pHandle[z][1]);
			int n=0;
			read(pHandle[z][0],&n, sizeof(int));
			close(pHandle[z][0]);
			totalNumberOfPoints += n;
			//std::cout << n << std::endl;
			}
		//}

	std::cout << totalNumberOfPoints << std::endl;


	return 0;
}

double calculateDistance(double x, double y){

	return sqrt( x*x + y*y);
}


double calculatePI(int totalNumOfPoints, int actualNumOfPoints){

	return 4.00 *((double) totalNumOfPoints / actualNumOfPoints);
}

