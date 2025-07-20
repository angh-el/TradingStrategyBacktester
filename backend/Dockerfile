# backend/Dockerfile

FROM python:3.11

WORKDIR /app

# Install make and gcc if needed to build core
RUN apt-get update && apt-get install -y build-essential make

# Copy backend and core code
COPY backend/ /app
COPY core/ /app/core

# Build core executable
WORKDIR /app/core
RUN make  # must produce "main" executable here

# Back to backend directory
WORKDIR /app

# Install backend deps
RUN pip install --no-cache-dir -r requirements.txt

# Start Flask
CMD ["flask", "run", "--host=0.0.0.0", "--port=5000"]
