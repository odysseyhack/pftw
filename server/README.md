# Server

## Description
This server runs on a raspberry pi and provides a MQTT server (port: 1337). If the server fetched some sensor data on the "sensor" channel, it publish it into the IOTA tangle via MAM stream.

## Setup and run

```bash
# Install dependencies
npm install

# Run the server at localhost:3443
npm run dev
```