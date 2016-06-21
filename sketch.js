var num = 20;
var ball = [];

function preload() {
  data = loadJSON("runs.json");
}

function setup() {
  createCanvas(480, 120);
  background(204);
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
    var speed = map(rate, 0.0, 200.00, 0.0005, 1);
    //construct the ball
    ball[i] = new Balls(x, y, r, speed, batsman, rate);
  }
}

function draw() {
  background(204);
  for (var i = 0; i < ball.length; i++) {
    ball[i].move();
    ball[i].display();
    ball[i].check();
  }
}

function Balls(tempX, tempY, tempDiameter, tempS, tempB, tempR) {
  this.x = tempX;
  this.y = tempY;
  this.diameter = tempDiameter;
  this.speed = tempS;
  this.batsman = tempB
  this.rate = tempR

  this.move = function() {
    // this.x += random(-this.speed, this.speed);
    // this.y += random(-this.speed, this.speed);
    this.x += this.speed;
    //this.y += this.speed;
  };

  this.display = function() {
    fill(255,255,0);
    ellipse(this.x, this.y, this.diameter, this.diameter);
    if (this.diameter > 30) {
       fill(255,0,0);
       ellipse(this.x, this.y, this.diameter, this.diameter);
       fill(0,0,255);
       text(this.batsman, this.x-10, this.y);
       }
       fill(0,0,255);
       text(this.rate, this.x-10, this.y-10);
  };

  this.check = function() {
    if (this.x > width || this.x < 1.0) {
      this.x = random(-10,10);
      this.y = random(height);
    } else if (this.y > height || this.y < 1.0) {
      this.x = random(width);
      this.y = random(height);
    }
  }
}
