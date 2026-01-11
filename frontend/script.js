const API_URL = window.location.hostname === 'localhost'
    ? 'http://localhost:8080/api'
    : `https://${window.location.hostname}/api`;


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
        resetKeyboard();
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

function resetBoard() {
    const board = document.getElementById("board");
    const rows = board.querySelectorAll(".row");

    rows.forEach((row) => {
        row.classList.remove("active", "won");

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

function resetKeyboard() {
    const keys = document.querySelectorAll("#keyboard .key");

    keys.forEach(key => {
        key.classList.remove('correct', 'present', 'absent');
        key.style.backgroundColor = ''; // Reset to default
    });
}


// Update board with result
function updateBoard(result, guess) {
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
    console.log(result)

    // Update keyboard
    updateKeyboard(result, guess);

    // Check if won
    if (result === '22222') {
        inputBlocked = true; // block further input

        const board = document.getElementById("board");
        const row = board.querySelectorAll(".row")[currentRow];
        row.classList.add('won'); // Add a special class for winning row
        setTimeout(() => {
            showGameOver("You won! Congratulations!");
        }, 3000);

        return;
    } else if (currentRow >= ROWS - 1) {
        inputBlocked = true; // block further input
        setTimeout(() => {
            showGameOver("You lost! Better luck next time.");
        }, 2000);
    }
}

function updateKeyboard(result, guess) {
    const keys = document.querySelectorAll("#keyboard .key");
    guess = guess.toUpperCase();

    for (let i = 0; i < guess.length; i++) {
        const letter = guess[i];
        const key = Array.from(keys).find(k => k.textContent.toUpperCase() === letter);
        if (!key) continue;

        if (result[i] === '2') {
            key.classList.remove('present', 'absent');
            key.classList.add('correct');
        } else if (result[i] === '1') {
            if (!key.classList.contains('correct')) {
                key.classList.remove('absent');
                key.classList.add('present');
            }
        } else { // result[i] === '0'
            if (!key.classList.contains('correct') && !key.classList.contains('present')) {
                key.classList.add('absent');
            }
        }
        // Animation 
        key.style.transition = 'transform 0.5s ease, background-color 0.5s ease';
    }
}


////////// Modal windows //////////
function setupModal(modalId, triggerId) {
    const modal = document.getElementById(modalId);
    const trigger = triggerId && document.getElementById(triggerId);

    // Open
    trigger?.addEventListener("click", () =>
        modal.classList.remove("hidden")
    );

    // Close by X
    modal.querySelector(".close")?.addEventListener("click", () =>
        modal.classList.add("hidden")
    );

    // Close by overlay click
    modal.addEventListener("click", (e) => {
        if (e.target === modal) modal.classList.add("hidden");
    });
}

// Initialize modals
setupModal("tutorial-modal", "tutorial-btn");
setupModal("settings-modal", "settings-btn");
setupModal("game-over-modal");

// Game-over modal logic
const gameOverModal = document.getElementById("game-over-modal");
const gameOverMessage = document.getElementById("game-over-message");
const newGameBtn = document.getElementById("new-game-btn");

function showGameOver(msg) {
    gameOverMessage.textContent = msg;
    gameOverModal.classList.remove("hidden");
}

// "WORDLE" header click => ask for new game
document.querySelector('.title').addEventListener("click", () => {
    showGameOver("Try again?");
});

// New Game button (works both on game-over or header)
newGameBtn.addEventListener("click", async () => {
    gameOverModal.classList.add("hidden");
    inputBlocked = false;
    await startNewGame();
});

////////// Dark Mode Toggle //////////
const darkToggle = document.getElementById("dark-toggle");

darkToggle.addEventListener("change", () => {
    document.body.classList.toggle("dark", darkToggle.checked);
    localStorage.setItem("theme", darkToggle.checked ? "dark" : "light");
});

// Load saved theme on page load
document.addEventListener("DOMContentLoaded", () => {
    const saved = localStorage.getItem("theme");
    if (saved === "dark") {
        document.body.classList.add("dark");
        darkToggle.checked = true;
    }
});


////////// Game Logic //////////
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
        shakeRow();
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

async function handleEnter() {
    if (inputBlocked) return;

    if (currentCol !== COLS) {
        shakeRow();
        return;
    }

    inputBlocked = true; // block further input until processing is done

    const word = getCurrentWord();
    const response = await submitGuess(word);

    if (response.error) {
        shakeRow();
        inputBlocked = false; // unblock input
        return;
    }

    updateBoard(response, word);

    currentRow++;
    currentCol = 0;
    inputBlocked = false;
}


////////// Animations //////////
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

////////// Initialize game on page load //////////
document.addEventListener('DOMContentLoaded', () => {
    startNewGame();
    const keyboard = document.getElementById("keyboard");

    keyboard.addEventListener("click", async (e) => {
        if (inputBlocked) return;

        const key = e.target.closest(".key");
        if (!key) return;

        if (key.id === "backspace") {
            removeLetter();
            return;
        }

        if (key.id === "enter") {
            await handleEnter();
            return;
        }

        const letter = key.textContent;
        if (/^[A-Z]$/.test(letter)) {
            addLetter(letter);
        }
    });


    document.addEventListener("keydown", async (e) => {
        if (e.repeat) return;
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
