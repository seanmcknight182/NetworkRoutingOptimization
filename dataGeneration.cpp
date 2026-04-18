#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

// generates 300 points from (0,0) to (99,104)
// ensures that all points are reachable by having 3 points for every x and every y
// decides which verticies have edges by checking if 2 points have the same x or the same y, they have an edge
// because this is in manhatten distance, the shortest path will always be 198 units, rather than sqrt(2) * 99
// code written by Sean McKnight(AND IM NOT PROUD OF IT) for CSC 2400 design of algorithms



// this class has 2 main functions, generatePoints() which makes the points and outputPoints() which dumps the points in .points.dat
// probabbly shouldve made the range adjustable, but thats an unneeded thing for this project and would add too much unnescisary extra work 
class thePoints{
private:

struct pointStruc{		
	int xValue = -1;
	int yValue = -1;	//coordinates
	int touches1 = -1;	// the index of the other points it touches
	int touches2 = -1;	
	int touches3 = -1;		
	int touches4 = -1;	
};

void addEdge(int i, int i2){	//i is the index we are adding the edge to, i2 is the one we are adding
	if(theArray[i].touches1 == -1){
		theArray[i].touches1 =i2;
		return;
	}
	if(theArray[i].touches2 == -1){
		theArray[i].touches2 =i2;
		return;
	}
	if(theArray[i].touches3 == -1){
		theArray[i].touches3=i2;
		return;
	}
	if(theArray[i].touches4 == -1){
		theArray[i].touches4=i2;
		return;
	}
}
int middleOfThree(int a, int b, int c) {
	if ((a <= b && b <= c) || (c <= b && b <= a))
        	return 2;
    	else if ((b <= a && a <= c) || (c <= a && a <= b))
        	return 1;
	else
        	return 3;
}
		

	pointStruc theArray[300];	// 3 points for every x, and for (most) y, added the extra space up top to prevent issue caused by all y values being taken by points with same
					// x value, yes this is a cop out of a solution, and I am rather ashamed of it
	int writable[105][5];		// writable[i][0] is the original y value
					// writable[i][1] is how many points have that y value
					// writable[i][2-4] is the index in theArray[] that have this y value
					// wouldve been better to make this a structure, but idk what I would name it
					// this is for when randomly figuring out what y to put a point at
					// having all the points with certain y values will make adding edges SOOOO much easier
	
public: 
	thePoints(){
		for(int i = 0; i < 105; i++){
			writable[i][0] = i;	// as previously defined this is the y value
			writable[i][1] = 0;	// how many points have this y value
			writable[i][2] = -1;	// index in theArray for point 1 with this
			writable[i][3] = -1;	// for point 2
			writable[i][4] = -1;	// for point 3
			cout << writable[i][0] << "\n";

		}
		theArray[0].xValue = 0;
		theArray[0].yValue = 0;
		theArray[299].xValue = 99;
		theArray[299].yValue = 99;
		writable[0][1] = 1;
		writable[0][2] = 0;
		writable[99][1] = 1;
		writable[99][2] = 299;
		//cout << "init done \n";
		
		
	}
	void generatePoints(){
		srand(time(0));
		cout << "started to gen points";
		int acceptableWritePos = 104;	// for use with writable
		int writableIndex = writableIndex = rand() % 100;
		int placeholder = 0; // for use in the ugly part's swapping
		for(int i = 1; i < 299; i++){	// theArray[0] and [299] are preset to always be 0,0 and 99,99 respectively
			theArray[i].xValue=i/3;
			//cout << i <<"wrote value \n";
			do{
				writableIndex = rand() % acceptableWritePos;
			} while(theArray[i/3].yValue == writable[writableIndex][0] || theArray[(i/3)+1].yValue == writable[writableIndex][0]);
			//cout << writable[writableIndex][0] << "\n";
			theArray[i].yValue = writable[writableIndex][0];
			
			//cout << theArray[i].yValue <<"\n";
			// and now for the ugly part
			writable[writableIndex][1]++;	// updates number of points with this y value
			writable[writableIndex][1+(writable[writableIndex][1])] = i;	// this is very ugly, but it puts the current points index in theArray into the correct spot in
											// list of points with that y value 
											// arguably better than if this array had been an array of
											// a struc rather than as it is. this would have a switch statement. not nearly as cool as this.
			if(writable[writableIndex][1]>=3){
				placeholder = writable[acceptableWritePos][0];
				writable[acceptableWritePos][0] = writable[writableIndex][0];
				writable[writableIndex][0] = placeholder;
				placeholder = writable[acceptableWritePos][1];
				writable[acceptableWritePos][1] = writable[writableIndex][1];
				writable[writableIndex][1] = placeholder;
				placeholder = writable[acceptableWritePos][2];
				writable[acceptableWritePos][2] = writable[writableIndex][2];
				writable[writableIndex][2] = placeholder;
				placeholder = writable[acceptableWritePos][3];
				writable[acceptableWritePos][3] = writable[writableIndex][3];
				writable[writableIndex][3] = placeholder;
				placeholder = writable[acceptableWritePos][4];
				writable[acceptableWritePos][4] = writable[writableIndex][4];
				writable[writableIndex][4] = placeholder;
				acceptableWritePos--;
				//cout << i3 <<"updated the writable Index \n";
			}
		}		// ugly part over
		int i1, i2, i3, a, b, c;
		// add the edges
		for(int i = 0; i < 100; i++){
			int a, b, c;
			i1 = i*3;
			i2 = i1+1;
			i3 = i2+1;
			a = theArray[i1].yValue;
			b = theArray[i2].yValue;
			c = theArray[i3].yValue;
			switch(middleOfThree(a, b, c)){
 				case 1:
					addEdge(i1, i2);
					addEdge(i1, i3);
					addEdge(i2, i1);
					addEdge(i3, i1);
				case 2:
					addEdge(i2, i1);
					addEdge(i2, i3);
					addEdge(i1, i2);
					addEdge(i3, i2);
				case 3:
					addEdge(i3, i1);
					addEdge(i3, i2);
					addEdge(i1, i3);
					addEdge(i2, i3);
			}
		}
		for(int i = 0; i < 105; i++){
			int a, b, c;
			i1 = writable[i][2];
			i2 = writable[i][3];
			i3 = writable[i][4];
			a = theArray[i1].xValue;
			b = theArray[i2].xValue;
			c = theArray[i3].xValue;
			if(i3 == -1){
				if (!(i2 == -1)){
					addEdge(a,b);
					addEdge(b,a);
				}
			}
			else {
				switch(middleOfThree(a, b, c)){
 					case 1:
						addEdge(i1, i2);
						addEdge(i1, i3);
						addEdge(i2, i1);
						addEdge(i3, i1);
					case 2:
						addEdge(i2, i1);
						addEdge(i2, i3);
						addEdge(i1, i2);
						addEdge(i3, i2);
					case 3:
						addEdge(i3, i1);
						addEdge(i3, i2);
						addEdge(i1, i3);
						addEdge(i2, i3);
					}
			}
		}
}
	void outputPoints(){
		ofstream theFile("thePoints.dat");
		for(int i=0; i < 300; i++){
			theFile << theArray[i].xValue << " " << theArray[i].yValue << " "; 
			theFile << theArray[i].touches1 << " " << theArray[i].touches2 << " " << theArray[i].touches3 << " " << theArray[i].touches4 << "\n";
		}
	}
};

int main() {
	srand(time(0));

	thePoints varName; // couldnt think of anything better to name it :(	
	varName.generatePoints();
	varName.outputPoints();
	return 0;
}
