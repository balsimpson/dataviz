var circles = [];
var data;
var x;
var y;
var radius;
var ipl;

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
    ellipse(random(width-20), random(height-20), radius, radius);
  }
}

function draw() {
  //background(120);
    //fill(120);

    x = x + speed;
    y = y + speed;
}
