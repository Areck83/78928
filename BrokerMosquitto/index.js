// Create a client instance
client = new Paho.MQTT.Client("ip172-19-0-44-ch1alm81k7jg00bbt4mg-80.direct.labs.play-with-docker.com", 80, "D.I.");

// set callback handlers
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

// connect the client
client.connect({onSuccess:onConnect});


// called when the client connects
function onConnect() {
  var mensaje = document.getElementById("mensaje").value;
  // Once a connection has been made, make a subscription and send a message.
  console.log("onConnect");
  client.subscribe("World");
  message = new Paho.MQTT.Message(mensaje);
  message.destinationName = "World";
  client.send(message);
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:"+responseObject.errorMessage);
  }
}

// called when a message arrives
function onMessageArrived(message) {
  console.log("onMessageArrived:"+message.payloadString);
  document.getElementById("recibo").innerText=message.payloadString;
}