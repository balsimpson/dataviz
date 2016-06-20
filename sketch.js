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
    var total = ipl[i].Runs;
    var rate = ipl[i].Strikerate;
    var x = random(width);
    var y = random(height);
    var r = map(total, 0, 1000, 0, 50);
    var speed = map(rate, 0.0, 400.00, 0.0005, 3);

    ball[i] = new Balls(random(width), random(height), r, speed);
  }
}

function draw() {
  for (var i = 0; i < ball.length; i++) {
    ball[i].move();
    ball[i].display();
    ball[i].check();
  }
}

function mousePressed() {
  
}

function Balls(tempX, tempY, tempDiameter, tempS) {
  this.x = tempX;
  this.y = tempY;
  this.diameter = tempDiameter;
  this.speed = tempS;

  this.move = function() {
    this.x += random(-this.speed, this.speed);
    this.y += random(-this.speed, this.speed);
  };

  this.display = function() {
    ellipse(this.x, this.y, this.diameter, this.diameter);
  };

  this.check = function() {
    if (this.x > width || this.x < 1.0) {
      this.x = random(width);
      this.y = random(height);
    } else if (this.y > height || this.y < 1.0) {
      this.x = random(width);
      this.y = random(height);
    }
  }
}
