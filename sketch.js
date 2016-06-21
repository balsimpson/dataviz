var num = 20;
var ball = [];

function preload() {
  data = loadJSON("runs.json");
}

function setup() {
  createCanvas(480, 120);
  background(24);
  var ipl = data.ipl;
  for (var i = 0; i < ipl.length; i++) {
    //assign variables
    var batsman = ipl[i].Batsman;
    var total = ipl[i].Runs;
    var rate = ipl[i].Strikerate;
    // var x = random(width);
    var x = -30;
    var y = random(height);
    var r = map(total, 0, 1000, 0, 50);
    var speed = map(ipl[i].Strikerate, 0.0, 200.00, 0.0005, 1);
    var look = map(rate, 0.0, 250.00, 0, 255);
    //construct the ball
    ball[i] = new Balls(x, y, r, speed, batsman, rate, look);
  }
}

function draw() {
  //background(0);
  strokeWeight(0);
  for (var i = 0; i < ball.length; i++) {
    ball[i].move();
    ball[i].display();
    ball[i].check();
  }
}

function Balls(tempX, tempY, tempDiameter, tempS, tempB, tempR, tempC) {
  this.x = tempX;
  this.y = tempY;
  this.diameter = tempDiameter;
  this.speed = tempS;
  this.batsman = tempB
  this.rate = tempR
  this.look = tempC         //setcolor

  this.move = function() {
    // this.x += random(-this.speed, this.speed);
    // this.y += random(-this.speed, this.speed);
    this.x += this.speed;
    //this.y += this.speed;
  };

  this.display = function() {
    //background(100);
//noStroke();
fill(255,200,0,this.look);
    ellipse(this.x, this.y, this.diameter, this.diameter);
    if (this.diameter > 20) {
       fill(255,0,0,this.look);
       //fill(this.look,this.look,this.look,100);
       ellipse(this.x, this.y, this.diameter, this.diameter);
       fill(this.look,this.look,this.look,100);
       //fill(0,0,255,100);
       //text(this.batsman, this.x-10, this.y);
       }
       else {

       }
       //  fill(0,0,255,100);
       //text(this.rate, this.x-10, this.y-10);
  };

  this.check = function() {
    if (this.x > width+10) {
      this.x = -20;
      this.y = random(height);
    } else if (this.y > height || this.y < 1.0) {
      this.x = random(width);
      this.y = random(height);
    }
  }
}
