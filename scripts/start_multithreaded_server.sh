#!/bin/bash

# Default port and server executable
PORT=8080
SERVER_EXEC="./multithreaded_getfile_server"

# Check if the server executable exists
if [ ! -f "$SERVER_EXEC" ]; then
    echo "Error: $SERVER_EXEC not found! Please build the server first."
    exit 1
fi

# Start the server
echo "Starting multithreaded GETFILE server on port $PORT..."
$SERVER_EXEC --port $PORT &

# Save the server PID to terminate it later if needed
SERVER_PID=$!
echo "Server started with PID: $SERVER_PID"

# Wait for the server process to keep it running in the background
wait $SERVER_PID