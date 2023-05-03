const ternometro = document.getElementById('ternometro-img');
const actual = document.getElementById('actual');
console.log(ternometro)
function actualizarTernometro(valorTemperatura) {
let temperatura = '';
    
    const imagen = new Image();
   
    
    if(valorTemperatura<0){
        imagen.src = '../ternometros/ternometro4.png';
    }else if(valorTemperatura<10){
        imagen.src = '../ternometros/ternometro3.png';
    }else if(valorTemperatura<20){
        imagen.src = '../ternometros/ternometro2.png';
    }
    else if(valorTemperatura<30){
        imagen.src = '../ternometros/ternometro1.png';
    }
    else {
        imagen.src = '../ternometros/ternometro0.png';
    }
    console.log(ternometro)
    ternometro.appendChild(imagen);
    actual.textContent = "La temperatura actual en celsius es de: "+valorTemperatura;

  }

  fetch('http://10.42.0.222/tempc')
  .then(res => res.json())
  .then(data =>{
    console.log(data);
    actualizarTernometro(data.temperatura);
  });
    
        
 
//   .catch(error => {
//     console.error(error);
//   });

//.then(response => element.innerHTML = response.data.total);

//   actualizarTernometro(20);


