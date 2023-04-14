import { Component } from '@angular/core';
import { tap } from 'rxjs';
import { DatosChuncheService } from 'src/app/servicios/datos-chunche/datos-chunche.service';

@Component({
  selector: 'app-inicio',
  templateUrl: './inicio.component.html',
  styleUrls: ['./inicio.component.css']
})

export class InicioComponent{
  datos: any;
  datos$: any;

  constructor(private datosChunce : DatosChuncheService){}

  ngOnInit(): void{
     this.datos$ = this.datosChunce
     .getData()
     .pipe(tap((data) => (this.datos = data)));
  }
}
