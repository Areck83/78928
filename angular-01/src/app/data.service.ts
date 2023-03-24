import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { map } from 'rxjs';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class DataService {

  constructor(private http: HttpClient) { }
  //metodo salvar
  salvar(saludador:String[]){
	  this.http.post<any>('https://nuevo-3f4ed-default-rtdb.firebaseio.com/saludador.json', saludador)
    .subscribe(
      response => console.log('respuesta '+response.name ),
      error => console.log ('error' + error.error )
    );
  }

  mostrar(): Observable<any[]>{
    return this.http.get('https://nuevo-3f4ed-default-rtdb.firebaseio.com/saludador.json').pipe(
      map( (responseData:any) => { //traer los datos
        const items = []; //donde se van a guardar las respuestas
        for (const key in responseData) {
          if(responseData.hasOwnProperty(key)){ //preguntar si tiene la propiedad, precisamente para evitar traer cosas no deseadas
            items.push({id : key, ...responseData[key] });//Estructura de json, id es lo primero que haya extraido, y en ...responseData se coloca todo lo demas
          }
        }
        return items;
      }) )
  }

  //Mostrar en consola [Backup XD]
  // mostrar(){
  //   this.http.get('https://nuevo-3f4ed-default-rtdb.firebaseio.com/saludador.json').pipe(
  //     map( (responseData:any) => { //traer los datos
  //       const items = []; //donde se van a guardar las respuestas
  //       for (const key in responseData) {
  //         if(responseData.hasOwnProperty(key)){ //preguntar si tiene la propiedad, precisamente para evitar traer cosas no deseadas
  //           items.push({id : key, ...responseData[key] });//Estructura de json, id es lo primero que haya extraido, y en ...responseData se coloca todo lo demas
  //         }
  //       }
  //       return items;
  //     }) ).subscribe(items => {console.log(items); }); Esta escuchando lo que map, que es observable, esta respondiendo
  // }
	
}
