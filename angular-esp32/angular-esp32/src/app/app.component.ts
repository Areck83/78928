import { HttpClient } from '@angular/common/http';
import { Injectable, OnInit } from '@angular/core';
import { Component } from '@angular/core';

// @Injectable()
// export class DatosChunche {
//   constructor(private http: HttpClient) { }

//   private static CHUNCHE_URL = '/temperatura';

//   async fetchDatosTemp() {
//     try {
//       const data: any = await this.http.get(DatosChunche.CHUNCHE_URL).toPromise();
//       return data;
//     } catch (error) {
//       console.error('Ocurrio un error alv: ' + error);
//     }
//   }
// }

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent{
  title = 'angular-esp32';
  // datos: any[] = [];

  // constructor(private datosChunche: DatosChunche) { }

  // ngOnInit() {
  //   this.datosChunche.fetchDatosTemp().then(data => {
  //     this.datos = data;
  //   });
  // }
}
