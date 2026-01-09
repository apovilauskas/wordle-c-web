const API_URL = 'http://localhost:8080/api'; // Change for production
let sessionId = null;

const ROWS = 6;
const COLS = 5;

let currentRow = 0;  // global variable for current row
let currentCol = 0;  // global variable for current column

let inputBlocked = false;

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
        inputBlocked = false; // unblock input
    } catch (error) {
        console.error('Error starting game:', error);
    }
}

// Submit guess
async function submitGuess(guess) {
    if (!sessionId) return { error: "Game not started" };

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
        return { error: data.error || "Word not found" };
    }

    return data.result;
}

// Update board with result
function updateBoard(result) {
    const board = document.getElementById("board");
    const row = board.querySelectorAll(".row")[currentRow];


    for (let i = 0; i < result.length; i++) {
        const tile = row.children[i];

        setTimeout(() => {
            if (result[i] === '2') {
                tile.classList.add('correct'); // Green
            } else if (result[i] === '1') {
                tile.classList.add('present'); // Yellow
            } else {
                tile.classList.add('absent'); // Grey
            }

            // Animation 
            tile.style.transition = 'transform 0.2s ease, background-color 0.5s ease';
            tile.style.transform = 'scale(1.2)';
            setTimeout(() => tile.style.transform = 'scale(1)', 150);
        }, i * 150); // Staggered delay for flip effect
    }

    // Check if won
    setTimeout(() => {
        if (result === '22222') showGameOver(true);
        else if (currentRow >= ROWS) showGameOver(false);
    }, result.length * 150 + 200);
}

function setupModal(modalId, triggerId) {
    const modal = document.getElementById(modalId);
    const trigger = document.getElementById(triggerId);

    // Open modal
    if (trigger) {
        trigger.addEventListener("click", () => {
            modal.classList.remove("hidden");
        });
    }

    // Close modal
    const closeBtn = modal.querySelector(".close");
    if (closeBtn) {
        closeBtn.addEventListener("click", () => {
            modal.classList.add("hidden");
        });
    }
}

// Initialize modals
setupModal("tutorial-modal", "tutorial-btn");
setupModal("settings-modal", "settings-btn");
setupModal("game-over-modal");

function showGameOver(won) {
    inputBlocked = true;
    const modal = document.getElementById("game-over-modal");
    const message = document.getElementById("game-over-message");
    message.textContent = won ? "You won!" : "You lost! Better luck next time.";
    modal.classList.remove("hidden"); // Show modal
}
// Button to start a new game
document.getElementById("new-game-btn").addEventListener("click", async () => {
    document.getElementById("game-over-modal").classList.add("hidden");
    await startNewGame();
});

function resetBoard() {
    const board = document.getElementById("board");
    const rows = board.querySelectorAll(".row");

    rows.forEach((row, rowIndex) => {
        row.classList.remove("active");

        const tiles = row.querySelectorAll(".tile");
        tiles.forEach(tile => {
            tile.textContent = "";
            tile.classList.remove("correct", "present", "absent");
        });
    });

    // Reset game state
    currentRow = 0;
    currentCol = 0;

    // Activate first row
    if (rows.length > 0) {
        rows[0].classList.add("active");
    }
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

function addLetter(letter) {
    if (currentCol >= 5) {                  // if column limit reached
        console.log("Column limit reached!");
        return;
    }
    const board = document.getElementById("board"); // search for the board element
    const rows = board.querySelectorAll(".row");  // get all rows
    const row = rows[currentRow];           // choose the current row
    const tile = row.children[currentCol]; // choose the current tile

    tile.textContent = letter;              // enter the letter into the tile
    ++currentCol;                           // move to the next column
}

function removeLetter() {
    if (currentCol === 0) return;           // if at the beginning of the row, do nothing

    --currentCol;                            // return to the previous column
    const board = document.getElementById("board");
    const rows = board.querySelectorAll(".row");
    const row = rows[currentRow];
    const tile = row.children[currentCol];

    tile.textContent = "";                   // clear the letter from the tile
}

function shakeRow() {
    const board = document.getElementById("board");
    const rows = board.querySelectorAll(".row");
    const row = rows[currentRow]; // current row

    if (!row) return;

    row.classList.add("shake");

    // Remove the class after animation ends
    row.addEventListener("animationend", () => {
        row.classList.remove("shake");
    }, { once: true });
}

async function handleEnter() {
    if (inputBlocked) return;

    inputBlocked = true;

    if (currentCol < COLS) {
        shakeRow();
        inputBlocked = false;
        return;
    }

    const word = getCurrentWord();
    const response = await submitGuess(word);

    if (response.error) {
        shakeRow();
        inputBlocked = false;
        return;
    }

    updateBoard(response);

    currentRow++;
    currentCol = 0;
    inputBlocked = false;
}

// Initialize game on page load
document.addEventListener('DOMContentLoaded', () => {
    startNewGame();
    const keyboard = document.getElementById("keyboard");

    keyboard.addEventListener("click", async (e) => {
        if (inputBlocked) return;

        const key = e.target.closest(".key");
        if (!key) return;

        // BACKSPACE
        if (key.id === "backspace") {
            removeLetter();
            return;
        }

        // ENTER
        if (key.id === "enter") {
            await handleEnter();
            return;
        }

        // LETTER
        const letter = key.textContent;
        if (/^[A-Z]$/.test(letter)) {
            addLetter(letter);
        }
    });

    document.addEventListener("keydown", async (e) => {
        if (inputBlocked) return; // ignore input if blocked

        // BACKSPACE
        if (e.key === "Backspace") {
            removeLetter();
            return;
        }

        // ENTER
        if (e.key === "Enter") {
            await handleEnter();
            return;
        }

        // LETTER
        if (/^[a-zA-Z]$/.test(e.key)) {
            addLetter(e.key.toUpperCase());
        }
    })

});

