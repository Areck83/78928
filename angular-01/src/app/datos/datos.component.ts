import { Component } from '@angular/core';
import { DataService } from '../data.service';

@Component({
  selector: 'app-datos',
  templateUrl: './datos.component.html',
  styleUrls: ['./datos.component.css']
})
export class DatosComponent {
	constructor(private ds:DataService){}
	ngOnInit(){
		this.ds.salvar(["estatico"]);
		//this.ds.mostrar(); --debug
	}
}
