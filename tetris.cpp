#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <emscripten/bind.h>
#include <emscripten/val.h>

using namespace emscripten;

int * board;
bool gameOver;

int * activeBlock;
int * activeBoardIndices;
int * activeBlockIndices;
int activeIndex;
int activeRotation;
int blocks[7][16] = {{0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0}, {0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0}, {0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0}, {0, 3, 0, 0, 0, 3, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0}, {0, 0, 3, 0, 0, 3, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 1, 1, 0,	0, 1, 0, 0, 0, 0, 0, 0}};

int getIndexFromXAndY(int xIndex, int yIndex) {
	return ((yIndex * 4) + xIndex);
}

int getIndexFromXAndYAt90(int xIndex, int yIndex) {
	return (12 + yIndex - (4 * xIndex));
}

int getIndexFromXAndYAt180(int xIndex, int yIndex) {
	return (15 - (yIndex * 4) - xIndex);
}

int getIndexFromXAndYAt270(int xIndex, int yIndex) {
	return (3 - yIndex + (4 * xIndex));
}

int getXFromIndex(int index) {
	return (index % 4);
}

int getYFromIndex(int index) {
	if (index < 4) {
		return 0;
	}  else if (index < 8) {
		return 1;
	} else if (index < 12) {
		return 2;
	} else {
		return 3;
	}
}

std::vector<int> convertArrayToVector(int * array) {
	std::vector<int> vectorToReturn;
	
	for(int count = 0; count < 200; count++) {
		vectorToReturn.push_back(array[count]);
	}

	return vectorToReturn;
}

void addRandomBlock() {
	activeIndex = 3;
	activeRotation = 0;
	activeBoardIndices = new int[4];
	activeBlockIndices = new int[4];

	int choice = rand() % 7;
	int * block = blocks[choice];
	int boardIndex = activeIndex;
	int activeIndexCount = 0;
	int activeBlockIndexCount = 0;

	if (boardIndex < 7) {
		for (int count = 0; count < 16; count++) {
			if (block[count] > 0) {
				activeBlockIndices[activeBlockIndexCount] = count;
				activeBlockIndexCount++;
				if (board[boardIndex] > 0) {
					gameOver = true;
				}
				activeBoardIndices[activeIndexCount] = boardIndex;
				activeIndexCount++;
				board[boardIndex] = block[count];
			}
			if (((count + 1) % 4) != 0) {
				boardIndex++;
			} else {
				boardIndex += 7;
			}
		}
	}

	activeBlock = block;
}

std::vector<int> tetrisStart() {
	srand (time(NULL));

	board = new int[200];
	for (int count = 0; count < 200; count++) {
		board[count] = 0;
	}
	gameOver = false;

	addRandomBlock();

	return convertArrayToVector(board);
}

void removeCompletedRows() {
	bool rowComplete;
	int columnCount;
	int rowStartingIndex;
	for (int count = 0; count < 20; count++) {
		rowComplete = true;
		columnCount = 0;
		rowStartingIndex = (count * 10);
		while ((columnCount < 10) && (rowComplete == true)) {
			if (board[(rowStartingIndex + columnCount)] == 0) {
				rowComplete = false;
			}
			
			columnCount++;
		}
		if (rowComplete == true) {
			for (int moveDownCount = (rowStartingIndex + 9); moveDownCount >= 10; moveDownCount--) {
				board[moveDownCount] = board[moveDownCount - 10];
			}
			for (int firstRowCount = 0; firstRowCount < 10; firstRowCount++) {
				board[firstRowCount] = 0;
			}
		}
	}
}

bool gameLost() {
	return gameOver;
}

bool partOfActiveBlock(int index) {
	for (int count = 0; count < 4; count++) {
		if (index == activeBoardIndices[count]) {
			return true;
		}
	}
	return false;
} 

bool canBlockMoveDown() {
	if (activeBoardIndices[3] > 189) {
		return false;
	} else {
		for (int count = 0; count < 4; count++) {
			if ((board[activeBoardIndices[count] + 10] > 0) && (partOfActiveBlock(activeBoardIndices[count] + 10) == false)) {
				return false;
			}
		}
		return true;
	}
}

bool canBlockMoveLeft() {
	for (int count = 0; count < 4; count++) {
		if ((activeBoardIndices[count] % 10) == 0) {
			return false;
		} else if ((board[activeBoardIndices[count] - 1] > 0) && (partOfActiveBlock(activeBoardIndices[count] - 1)) == false) {
			return false;
		}
	}
	return true;
}

bool canBlockMoveRight() {
	for (int count = 0; count < 4; count++) {
		if (((activeBoardIndices[count] + 1) % 10) == 0) {
			return false;
		} else if ((board[activeBoardIndices[count] + 1] > 0) && (partOfActiveBlock(activeBoardIndices[count] + 1) == false)) {
			return false;
		}
	}
	return true;
}

std::vector<int> moveRotate() {
	int boardIndex = activeIndex;
	int * blockIndices = new int [4];
	int blockIndicesCounter = 0;
	for (int count = 0; count < 16; count++) {
		if (partOfActiveBlock(boardIndex) == true) {
			blockIndices[blockIndicesCounter] = count;
			blockIndicesCounter++;
		}
		if (((count + 1) % 4) == 0) {
			boardIndex += 7;
		} else {
			boardIndex++;
		}
	}
	int * rotatedBlockIndices = new int [4];
	for(int count = 0; count < 4; count++) {
		if (activeRotation == 0) {
			rotatedBlockIndices[count] = getIndexFromXAndYAt90(getXFromIndex(activeBlockIndices[count]), getYFromIndex(activeBlockIndices[count]));
		} else if (activeRotation == 1) {
			rotatedBlockIndices[count] = getIndexFromXAndYAt180(getXFromIndex(activeBlockIndices[count]), getYFromIndex(activeBlockIndices[count]));
		} else if (activeRotation == 2) {
			rotatedBlockIndices[count] = getIndexFromXAndYAt270(getXFromIndex(activeBlockIndices[count]), getYFromIndex(activeBlockIndices[count]));
		} else {
			rotatedBlockIndices[count] = getIndexFromXAndY(getXFromIndex(activeBlockIndices[count]), getYFromIndex(activeBlockIndices[count]));
		}
	}

	if ((((activeIndex + 1) % 10) == 0) || (((activeIndex + 3) % 10) == 0)) {
		return convertArrayToVector(board);
	}
	for(int count = 0; count < 4; count++) {
		if((board[rotatedBlockIndices[count]] > 0) && (partOfActiveBlock(rotatedBlockIndices[count]) == false)) {
			return convertArrayToVector(board);
		}
	}

	int replacement = board[activeBoardIndices[0]];
	for (int count = 0; count < 4; count++) {
		board[activeBoardIndices[count]] = 0;
	}

	boardIndex = activeIndex;
	int activeBoardIndicesCounter = 0;
	for (int count = 0; count < 16; count++) {
		if ((count == rotatedBlockIndices[0]) || (count == rotatedBlockIndices[1]) || (count == rotatedBlockIndices[2]) || (count == rotatedBlockIndices[3])) {
			board[boardIndex] = replacement;
			activeBoardIndices[activeBoardIndicesCounter] = boardIndex;
			activeBoardIndicesCounter++;
		}
		if (((count + 1) % 4) == 0) {
			boardIndex += 7;
		} else {
			boardIndex++;
		}
	}

	if (activeRotation == 3) {
		activeRotation = 0;
	} else {
		activeRotation++;
	}

	return convertArrayToVector(board);
}

std::vector<int> moveLeft() {

	if (canBlockMoveLeft() == true) {
		int currentIndex = activeIndex;
		int activeIndexCount = 0;

		for (int count = 1; count < 16; count++) {
			if ((board[currentIndex] > 0) && (partOfActiveBlock(currentIndex) == true)) {
				board[currentIndex - 1] = board[currentIndex];
				board[currentIndex] = 0;
				activeBoardIndices[activeIndexCount] = activeBoardIndices[activeIndexCount] - 1;
				activeIndexCount++;
			}
			if ((count != 0) && ((count % 4) == 0)) {
				currentIndex += 7;
			} else {
				currentIndex++;
			}
		}

		activeIndex -= 1; 
	}

	return convertArrayToVector(board);
}

std::vector<int> moveRight() {

	if (canBlockMoveRight() == true) {
		int currentIndex = activeIndex + 33;
		int activeIndexCount = 3;

		for (int count = 15; count >= 0; count--) {
			if ((board[currentIndex] > 0) && (partOfActiveBlock(currentIndex) == true)) {
				board[currentIndex + 1] = board[currentIndex];
				board[currentIndex] = 0;
				activeBoardIndices[activeIndexCount] = activeBoardIndices[activeIndexCount] + 1;
				activeIndexCount--;
			}
			if ((count % 4) == 0) {
				currentIndex -= 7;
			} else {
				currentIndex--;
			}
		}

		activeIndex += 1; 
	}

	return convertArrayToVector(board);
}

std::vector<int> moveDown() {

	if (canBlockMoveDown() == true) {
		int currentIndex = activeIndex + 33;
		int activeIndexCount = 3;

		for (int count = 15; count >= 0; count--) {
			if ((board[currentIndex] > 0) && (partOfActiveBlock(currentIndex) == true)) {
				board[currentIndex + 10] = board[currentIndex];
				board[currentIndex] = 0;
				activeBoardIndices[activeIndexCount] = activeBoardIndices[activeIndexCount] + 10;
				activeIndexCount--;
			}
			if ((count % 4) == 0) {
				currentIndex -= 7;
			} else {
				currentIndex--;
			}
		}

		activeIndex += 10; 
	} else {
		removeCompletedRows();
		addRandomBlock();
	}

	for (int firstRowCount = 0; firstRowCount < 10; firstRowCount++) {
		if ((board[firstRowCount] > 0) && (partOfActiveBlock(firstRowCount) == false)) {
			gameOver = true;
		}
	}
	
	return convertArrayToVector(board);
}

EMSCRIPTEN_BINDINGS(Module) {
	function("tetrisStart", &tetrisStart);
	function("moveDown", &moveDown);
	function("moveLeft", &moveLeft);
	function("moveRight", &moveRight);
	function("moveRotate", &moveRotate);
	function("gameLost", &gameLost);

	register_vector<int>("vector<int>");
}