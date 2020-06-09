import { Component } from '@angular/core';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'study!!!!!!!!!!!!!!';

  constructor() {
    console.log('Working On');
  }

  public clickAfterPrint() : void{
    console.log('Click');
  }
}
