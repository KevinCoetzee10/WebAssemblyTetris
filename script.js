let TetrisColumns = 10;
let TetrisRows = 20;
let EmptyGridValue = '';
let RedGridValue = 'r';
let GreenGridValue = 'g';
let BlueGridValue = 'b';
let Grid = [];

let alreadyStarted = false;
let gameLoop;

var Module = {
	onRuntimeInitialized: function () {}
};

function resetTetrisGrid() {
	if (alreadyStarted == true) {
		Grid = [];
		for (let count = 0; count < 200; count++) {
			Grid.push('');
		}
		updateTetrisGrid(Grid);
		let lostLabel = document.getElementById("lostLabel");
		lostLabel.style.display = "block";
		clearInterval(gameLoop);
		alreadyStarted = false;
	}
}

function updateTetrisGrid(grid) {
	for (let count = 0; count < grid.length; count++) {
		if (grid[count] == 'r') {
			document.getElementById('cell-' + (count + 1)).className = "cell red";
		} else if (grid[count] == 'g') {
			document.getElementById('cell-' + (count + 1)).className = "cell green";
		} else if (grid[count] == 'b') {
			document.getElementById('cell-' + (count + 1)).className = "cell blue";
		} else if (grid[count] == '') {
			document.getElementById('cell-' + (count + 1)).className = "cell";
		}
	}
};

function convertVectorToGridArray(vector) {
	let gridToReturn = [];
	let valueToAdd;
	let intToInterpret;
	for (let count = 0; count < vector.size(); count++) {
		intToInterpret = vector.get(count);
		if (intToInterpret == 1) {
			valueToAdd = 'r';
		} else if (intToInterpret == 2) {
			valueToAdd = 'g';
		} else if (intToInterpret == 3) {
			valueToAdd = 'b';
		} else {
			valueToAdd = '';
		}
		gridToReturn.push(valueToAdd);
	}
	return gridToReturn;
}

function moveDown() {
	let vector = Module.moveDown();
	Grid = convertVectorToGridArray(vector);
	updateTetrisGrid(Grid);
	let lost = Module.gameLost();
	if (lost) {
		resetTetrisGrid();
	}
}

function moveLeft() {
	let vector = Module.moveLeft();
	Grid = convertVectorToGridArray(vector);
	updateTetrisGrid(Grid);
}

function moveRight() {
	let vector = Module.moveRight();
	Grid = convertVectorToGridArray(vector);
	updateTetrisGrid(Grid);
}

function moveRotate() {
	let vector = Module.moveRotate();
	Grid = convertVectorToGridArray(vector);
	updateTetrisGrid(Grid);
}

function tetrisStart() {
	if (alreadyStarted == false) {
		let vector = Module.tetrisStart();
		Grid = convertVectorToGridArray(vector);
		updateTetrisGrid(Grid);

		let lostLabel = document.getElementById("lostLabel");
		lostLabel.style.display = "none";

		gameLoop = setInterval(moveDown, 500);
		alreadyStarted = true;
	}
}