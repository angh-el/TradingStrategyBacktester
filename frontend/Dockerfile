FROM node:20

# Set working directory
WORKDIR /app

# Copy only package.json and lock file first (for caching)
COPY package*.json ./

# Install dependencies
RUN npm install

# Copy the rest of the frontend app
COPY . .

# Start with development server
EXPOSE 3000
CMD ["npm", "start"]
