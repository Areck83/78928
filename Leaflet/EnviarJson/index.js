// Create a client instance
client = new Paho.MQTT.Client("172.31.144.1", 9001, "Diegoooo");

// set callback handlers
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

// connect the client
client.connect({onSuccess:onConnect});

var markerPrevio = null;

var map = L.map('map').setView([19.529931601524314, -96.90632892452533], 13);
    L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
        maxZoom: 19,
        attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
    }).addTo(map);


    function onConnect() {
      // var mensaje = document.getElementById("mensaje").value;
      // Once a connection has been made, make a subscription and send a message.
      console.log("onConnect");
      client.subscribe("Testeo");
      // console.lo
      // message = new Paho.MQTT.Message(mensaje);
      // message.destinationName = "Testeo";
      // client.send(message);
    }

    // called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:"+responseObject.errorMessage);
  }
}
    

function myFunction(){

    var msj = document.getElementById("msj").value;
    var x = document.getElementById("primera").value;
    var y = document.getElementById("segunda").value;

    if(markerPrevio)
        map.removeLayer(markerPrevio);


    var marker = L.marker([x, y]).addTo(map);

    marker.bindPopup(msj).openPopup();


    markerPrevio = marker;

    

}


function onMessageArrived(message) {
  if(markerPrevio != null){
    map.removeLayer(marker);
  } 
  var msg = JSON.parse(message.payloadString); 
  console.log("onMessageArrived:" + message.destinationName + message.payloadString);
  console.log(msg);
  var latitud = msg.lat;
  var long = msg.long;
  var mensaje = msg.mensaje;
  marker = L.marker([latitud, long]).addTo(map);
  marker.bindPopup("<b>"+ mensaje + "</b><br> Desde mosquitto").openPopup();
  markerPrevio=marker; 
}