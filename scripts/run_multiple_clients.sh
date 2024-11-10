#!/bin/bash

# Default number of clients and file name
NUM_CLIENTS=5
FILE_NAME="testfile.txt"
CLIENT_EXEC="./getfile_client"
SERVER_ADDRESS="127.0.0.1"
SERVER_PORT=8080

# Check if the client executable exists
if [ ! -f "$CLIENT_EXEC" ]; then
    echo "Error: $CLIENT_EXEC not found! Please build the client first."
    exit 1
fi

# Function to run a single client
run_client() {
    echo "Starting client to request file '$FILE_NAME' from $SERVER_ADDRESS:$SERVER_PORT"
    $CLIENT_EXEC --operation retrieve --file "$FILE_NAME" --server "$SERVER_ADDRESS:$SERVER_PORT"
}

# Run multiple clients in parallel
echo "Running $NUM_CLIENTS clients concurrently..."
for (( i=1; i<=NUM_CLIENTS; i++ ))
do
    run_client &
done

# Wait for all clients to complete
wait
echo "All client requests completed."