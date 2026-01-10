FROM gcc:latest

WORKDIR /app

# Copy everything
COPY . .

# Compile
WORKDIR /app/backend
RUN make

# Expose the port
EXPOSE 8080

# Run from the backend folder so it can find words.txt locally
# but can find ../frontend/ files
CMD ["./wordle_server"]