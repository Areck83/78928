import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class DatosChuncheService {

  constructor(private http: HttpClient) { }

  getData(){
    return this.http.get('https://api.openbrewerydb.org/v1/breweries?page=15&per_page=3');
  }
}
