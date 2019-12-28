var mqtt = require('mqtt')
var mqttClient = mqtt.connect('mqtt://127.0.0.1')
mqttClient.subscribe('data');

const
    io = require("socket.io"),
    server = io.listen(8001);

let
    sequenceNumberByClient = new Map();

mqttClient.on('connect', function () {
  mqttClient.subscribe('presence', function (err) {
    if (!err) {
      mqttClient.publish('presence', 'Hello mqtt')
    }
  })
})

// event fired every time a new client connects:
server.on("connection", (socket) => {
    console.info(`Client connected [id=${socket.id}]`);
    // initialize this client's sequence number
    sequenceNumberByClient.set(socket, 1);

    // when socket disconnects, remove it from the list:
    socket.on("disconnect", () => {
        sequenceNumberByClient.delete(socket);
        console.info(`Client gone [id=${socket.id}]`);
    });
});

// event fired on received mqtt data
mqttClient.on('message', function (topic, message) {
  for (const [client, sequenceNumber] of sequenceNumberByClient.entries()) {
      console.info(message.toString());
      client.emit("data", parseInt(message));
  }
})
