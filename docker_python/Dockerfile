# Use the official Ubuntu 22.04 base image
FROM ubuntu:22.04

# Update the package list and install Python 3
RUN apt-get update && apt-get install -y python3 && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
WORKDIR /usr/src/app

# Create a simple "Hello World" Python script
RUN echo 'print("Hello World")' > hello.py

# Set the Python script as the entry point
ENTRYPOINT ["python3", "hello.py"]
