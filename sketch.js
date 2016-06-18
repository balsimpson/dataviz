var circles = [];
var data;
var x;
var y;
var r;
var ipl;

// Declare and construct two objects (h1, h2) from the class HLine

Balls b = new Balls(x, y, r);

function preload() {
  data = loadJSON("runs.json");
}

function setup() {
  createCanvas(400,600);
  background(120);
  fill(255, 0, 175, 100);
  //noStroke();
  ipl = data.ipl;
  //random position
  for (var i = 0; i < ipl.length; i++) {
    var names  = ipl[i].Batsman;
    //map circle size to total runs
    radius = map(ipl[i].Runs, 0, 1000, 5, 100);
    //map speed to strikerate
    var speed = map(ipl[i].Strikerate, 0, 300, 0.05, 0.1);
    Balls b = new Balls(random(width-20), random(height-20), ipl[i].radius);
  }
}

function draw() {
  //background(120);
  //fill(120);
  b.update();
}

class Balls {
  var x,y,r;
  Balls (x, y, r) {
    x = random(width);
    y = random(height);
    r = random(10,20);
  }

  void update() {
    ellipse(x,y,r,r);
    //x += 0.1;
  }
}
