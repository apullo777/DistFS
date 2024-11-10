# DistFS - Distributed File System
DistFS is a distributed, multithreaded file storage and retrieval system, built to efficiently and safely manage file operations across multiple servers. It utilizes gRPC for remote communication, POSIX IPC for inter-process coordination, and Address Sanitizer for memory safety. The project includes multiple stages, progressing from a basic echo server to a fully multithreaded GETFILE server capable of handling concurrent file requests.

## Features
- Distributed File Storage: Allows clients to store and retrieve files across multiple servers.
- gRPC Communication: Manages client-server interactions and file transfer requests.
- Multithreaded Server Architecture: Supports concurrent access and file requests.
- POSIX IPC: Enables inter-process communication within each server for shared resource management.
- Address Sanitizer Integration: Ensures memory safety by detecting leaks and buffer overflows during development.

## Project Components
1. Echo Server/Client: A basic server-client model that echoes client messages to verify connectivity.
2. File Transfer Server/Client: Extends the echo functionality to allow file transfers from client to server.
3. GETFILE Server/Client: Implements a custom protocol for clients to request specific files from the server.
4. Multithreaded GETFILE Server: Final version that supports concurrent file requests with multithreading.

## Installation
### Prerequisites
- gRPC: Ensure gRPC and Protobuf are installed. gRPC Installation Guide
- Address Sanitizer: Available on systems with GCC or Clang.
- POSIX-compliant System: Required for POSIX IPC functions (Linux, macOS).

### Build Instructions
1. Clone the Repository:

```bash
git clone https://github.com/apullo777/DistFS.git
cd DistFS
```

2. Install Dependencies: Ensure gRPC and any POSIX IPC dependencies are installed.

3. Compile the Project:

```bash
cd src
make
```

This make command will compile each component of the project, producing executables for the echo server/client, file transfer, GETFILE, and multithreaded GETFILE servers.


### Usag

### Running Each Part Individually

1. Echo Server/Client:

- Start the server:
```bash
./echo_server
```

- Run the client in a separate terminal:
```bash
./echo_client
```

2. File Transfer Server/Client:

- Start the server:
```bash
./file_transfer_server
```

- Run the client with the file to transfer:

```bash
./file_transfer_client
```

3. GETFILE Server/Client:

- Start the server:

```bash
./getfile_server
```

- Run the client with the file to request:

```bash
./getfile_client
```

4. Multithreaded GETFILE Server:

- Start the server:

```bash
./multithreaded_getfile_server
```

- Run multiple clients simultaneously to request files. Use the provided script:

```bash
./run_multiple_clients.sh
```

## Project Highlights
- Scalable Multithreaded Design: Each client request is handled by a separate thread in the multithreaded GETFILE server, enabling high concurrency.
- Custom GETFILE Protocol: A unique protocol designed for file retrieval requests, including custom status codes and error messages.
- Memory Safety: Integrated with Address Sanitizer, which ensures detection of memory leaks, buffer overflows, and other memory errors.
- POSIX IPC: Utilizes shared memory and semaphores within each server for efficient inter-process coordination.

## Example Use Cases
- Testing Network Communication: Start with the echo server to verify basic client-server interaction.
- File Transfer Simulation: Use the file transfer server to simulate uploading files to a server in a controlled environment.
- Distributed File Retrieval: With the multithreaded GETFILE server, demonstrate a distributed file retrieval system where multiple clients can request and download files concurrently.

## Contributing
We welcome contributions to enhance DistFS! To contribute:

1. Fork the repository.
2. Create a new branch with your feature or fix.
3. Submit a pull request with a detailed description of the changes.

## License
This project is licensed under the MIT License. See LICENSE for more details.

## Acknowledgments
Thanks to the open-source community for providing tools like gRPC, POSIX IPC, and Address Sanitizer, which made this project possible.
