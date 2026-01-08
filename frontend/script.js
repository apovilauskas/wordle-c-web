const API_URL = 'http://localhost:8080/api'; // Change for production
let sessionId = null;

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
    if (!sessionId) {
        alert('Please start a new game first');
        return;
    }
    
    try {
        const response = await fetch(`${API_URL}/guess`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ 
                guess: guess.toUpperCase(), 
                sessionId: sessionId 
            })
        });
        
        const data = await response.json();
        
        if (response.ok) {
            // data.result is like "01202"
            // 0 = grey (not in word)
            // 1 = yellow (wrong position)
            // 2 = green (correct position)
            updateBoard(data.result, guess);
        } else {
            alert(data.error);
        }
    } catch (error) {
        console.error('Error submitting guess:', error);
    }
}

// Update board with result
function updateBoard(result, guess) {
    const currentRow = document.querySelector('.row.active'); // Your current row
    const tiles = currentRow.querySelectorAll('.tile');
    
    for (let i = 0; i < 5; i++) {
        tiles[i].textContent = guess[i];
        
        if (result[i] === '2') {
            tiles[i].classList.add('correct'); // Green
        } else if (result[i] === '1') {
            tiles[i].classList.add('present'); // Yellow
        } else {
            tiles[i].classList.add('absent'); // Grey
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

// Initialize game on page load
document.addEventListener('DOMContentLoaded', () => {
    startNewGame();
    
    // Your existing keyboard/input handling here
    // When user presses Enter, call submitGuess(currentGuess)
});