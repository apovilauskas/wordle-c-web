# 1. Use an image that has GCC (C compiler) pre-installed
FROM gcc:latest

# 2. Create a folder for your app inside the container
WORKDIR /usr/src/app

# 3. Copy all your project files (backend, frontend, etc.) into the container
COPY . .

# 4. Compile the C code
# We tell GCC to take the server.c file and output a program called "server"
RUN gcc backend/server.c -o server

# 5. Allow traffic on port 8080 (Matches your C code)
EXPOSE 8080

# 6. Run the compiled server
CMD ["./server"]