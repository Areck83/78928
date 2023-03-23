import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

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
}
