# Wordle-C-Web

## Project Overview

We, @apovilauskas (backend) and @VlaSarnovskij (frontend), created a Wordle clone with a C backend, HTML/CSS/JS frontend, and a healthy dose of AI assistance.

This project was a fun challenge: building a full web game in **C**, a language not commonly used for web servers. It combines traditional web technologies with low-level socket programming to make Wordle run online.

---

## Why We Did This

- **Wordle game:** simple, popular, and universally playable.  
- **C for the backend:** to push our skills and tackle a challenge — most web games use Python, Node.js, or Java.  
- **AI assistance:** helped us debug Docker, file paths, and HTTP handling, which is tricky in C.

---

## How It Works

1. **Backend (C):**  
   - Runs a server on a specified port (usually 8080).  
   - Waits for requests from a browser.  
   - Reads files like `index.html`, `style.css`, and `script.js` from disk and sends them to the browser.  
   - Handles API requests for starting a new game or submitting a guess.  
   - Tracks sessions and game state (which word is secret, guesses so far).

2. **Frontend (HTML/CSS/JS):**  
   - Browser receives HTML, CSS, and JS from backend.  
   - JS interacts with the backend via HTTP POST requests (`/api/new-game`, `/api/guess`).  
   - Updates the board UI dynamically based on responses.  
   - Handles user input (typing letters, pressing Enter, keyboard clicks) and animations.

3. **How Files Interact:**  
   - `server.c` — sets up sockets, listens for connections, and calls `handle_http_request()` for each request.  
   - `http_handler.c` — decides what to do with each request: serve static files or process API calls.  
   - `words.c` — loads the word dictionary, picks a secret word, and validates guesses.  
   - `game_state.c` — manages active game sessions and stores current words and guesses.  
   - `logic.c` — checks guesses against the secret word and generates results (correct/present/absent).  
   - `frontend/` — contains HTML, CSS, JS, and icon files for the user interface.  

4. **User Flow:**  
   - Browser requests `/index.html`.  
   - Backend serves the HTML, CSS, JS, and icons.  
   - JS starts a new game via `/api/new-game`.  
   - Player types guesses; JS sends them to `/api/guess`.  
   - Backend checks the guess, responds with result.  
   - JS updates the UI based on the response (colors, keyboard hints, win/loss messages).

---

## Play It Now

[Click here to play Wordle-C-Web!](https://wordle-c-web.onrender.com/)
