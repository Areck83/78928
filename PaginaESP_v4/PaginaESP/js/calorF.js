document.addEventListener('DOMContentLoaded', function() {


    const ternometro = document.getElementById('ternometro-img');
    const actual = document.getElementById('actual');
    console.log(ternometro)
    function actualizarCalor(valorCalor) {
        
        
        const imagen = new Image();
       
        
        if(valorCalor<50){
            imagen.src = '../calor/calor-bajo.jpg';
        }else if(valorCalor<77){
            imagen.src = '../calor/calor-medio.jpg';
        }else{
            imagen.src = '../calor/calor-alto.jpg';
        }

        imagen.style.height = '400px';
        imagen.style.width= '500px';

        console.log(ternometro)
        ternometro.appendChild(imagen);
        actual.textContent = "La calor actual en fahrenheit es de: "+valorCalor;
    
      }
    
    
    
      fetch('http://10.42.0.222/calorf')
      .then(res => res.json())
      .then(data =>{
        console.log(data);
        actualizarCalor(data.calor);
      });
    
    
    
    });