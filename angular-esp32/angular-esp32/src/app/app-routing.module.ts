import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { InicioComponent } from './componentes/inicio/inicio.component';
import { AmbienteComponent } from './componentes/ambiente/ambiente.component';
import { CommonModule } from '@angular/common';

const routes: Routes = [
  {path:'',component: InicioComponent},
  {path: 'inicio', component: InicioComponent},
  {path: 'ambiente', component: AmbienteComponent}
];

@NgModule({
  imports: [CommonModule, RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
