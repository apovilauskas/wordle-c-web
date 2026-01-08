const API_URL = 'http://localhost:8080/api'; // Change for production
let sessionId = null;

const ROWS = 6;
const COLS = 5;

let currentRow = 0;  // global variable for current row
let currentCol = 0;  // global variable for current column

// Start new game
async function startNewGame() {
    try {
        const response = await fetch(`${API_URL}/new-game`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        });
        
        const data = await response.json();
        sessionId = data.sessionId;
        console.log('New game started:', sessionId);
        
        // Reset your board UI here
        resetBoard();
    } catch (error) {
        console.error('Error starting game:', error);
    }
}

// Submit guess
async function submitGuess(guess) {
    if (!sessionId) throw new Error("Game not started");

    const response = await fetch(`${API_URL}/guess`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
            guess: guess.toUpperCase(),
            sessionId
        })
    });

    const data = await response.json();

    if (!response.ok) {
        throw new Error(data.error || "Invalid guess");
    }

    data.result = "01220"; // Mocked result for testing

    return data.result; // "01220"
}

// Update board with result
function updateBoard(result) {
    const board = document.getElementById("board");
    const row = board.querySelectorAll(".row")[currentRow];

    for (let i = 0; i < result.length; i++) {
        const tile = row.children[i];
        if (result[i] === '2') {
            tile.classList.add('correct'); // Green
        } else if (result[i] === '1') {
            tile.classList.add('present'); // Yellow
        } else {
            tile.classList.add('absent'); // Grey
        }
    }
    
    // Check if won
    if (result === '22222') {
        setTimeout(() => alert('You won!'), 500);
    }
}

function resetBoard() {
    // Clear all tiles and reset to first row
    // Your existing UI reset code
}

function getCurrentWord() {
  const board = document.getElementById("board");
  const rows = board.querySelectorAll(".row");
  const row = rows[currentRow]; // current row

  let word = "";
  for (let i = 0; i < row.children.length; i++) {
    const tile = row.children[i];
    word += tile.textContent; // letters collection
  }

  return word;
}


// Initialize game on page load
document.addEventListener('DOMContentLoaded', () => {
    startNewGame();

    document.addEventListener("keydown", async (e) => {
        if (e.key === "Backspace") {
            removeLetter();
            return;
        }

        if (e.key === "Enter") {
            console.log("Enter pressed");

            if (currentCol < COLS) {
            console.log("Not enough letters");
            return;
            }

            const word = getCurrentWord();

            try {
            const result = await submitGuess(word);
            console.log(result);
            updateBoard(result);

            ++currentRow;     // next row
            currentCol = 0;   // reset column

            if (currentRow >= ROWS) {
                console.log("Game over!");
            }
            } catch (err) {
            console.error("Error in sending word", err);
            }

            return;
        }

        // letter input
        if (/^[a-zA-Z]$/.test(e.key)) {
            addLetter(e.key.toUpperCase());
        }
    }) 
    // Your existing keyboard/input handling here
    // When user presses Enter, call submitGuess(currentGuess)
});

