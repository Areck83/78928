import { TestBed } from '@angular/core/testing';

import { DatosChuncheService } from './datos-chunche.service';

describe('DatosChuncheService', () => {
  let service: DatosChuncheService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(DatosChuncheService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
