import { Component } from '@angular/core';
import { DataService } from '../data.service';

@Component({
  selector: 'app-formulario',
  templateUrl: './formulario.component.html',
  styleUrls: ['./formulario.component.css']
})
export class FormularioComponent {
  nombre:String = '';

  constructor(private ds:DataService){}
  
  onSubmit(){
    this.ds.salvar([this.nombre]);
  }
}
