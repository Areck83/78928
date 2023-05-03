function actualizarGrafica(humedad) {
    let columna = document.getElementById("columna");
    const actual = document.getElementById('actual');
    let altura = 200 - (humedad * 2);
    columna.style.height = altura + "px";
    actual.textContent = "La humedad actual es de: "+humedad;
  }
  fetch('http://10.42.0.222/hum')
      .then(res => res.json())
      .then(data =>{
        console.log(data);
        actualizarGrafica(parseInt(data.humedad));
      });
