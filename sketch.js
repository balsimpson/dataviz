/*
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
*/

//example 4: Creating an array of objects using a class based on http://www.openprocessing.org/sketch/48960


int XSPEED = 4;
int YSPEED = 7;
int CIRCLESIZE = 15;

// create an empty array for 100 MovingCircle objects
MovingCircle[] myCircleArray = new MovingCircle[100];

function setup() {
  createCanvas(400, 400);
  background(120);

  // and actually create the objects and populate the
  // array with them
  for (int i=0; i < myCircleArray.length; i++) {
    myCircleArray[i] = new MovingCircle(random(0, width), random(0, height));
  }
}

function draw() {

  background(120);

  // iterate through every moving circle
  for (int i=0; i < myCircleArray.length; i++) {

    myCircleArray[i].update();
    myCircleArray[i].checkCollisions();
    myCircleArray[i].drawCircle();
  }
}

class MovingCircle {

  // any variable declared here will be properties of
  // objects of this type
  float x;
  float y;
  float xSpeed;
  float ySpeed;

  // Now the functions that are part of this object

  // this special function declaration has the same name
  // as the class (MovingCircle) and it has no return type. This
  // is known as the constructor and it's run when an
  // object of this type is created.

  MovingCircle(float xpos, float ypos) {
    x = xpos;
    y = ypos;
    xSpeed = XSPEED;
    ySpeed = YSPEED;
  }

  // Now come the functions (methods) of the object that
  // can be called. These functions describe what the
  // object can do:

  // update adds the speed to the position, making
  // our circle move around.
  // void update() {
  //   x += xSpeed;
  //   y += ySpeed;
  // }

  // this function checks to see if our circle has gone off
  // the edge of the screen, and if so reverses the speed
  void checkCollisions() {
    float r = CIRCLESIZE/2;

    if ( (x<r) || (x>width-r)){
      xSpeed = -xSpeed;
    }

    if( (y<r) || (y>height-r)) {
      ySpeed = -ySpeed;
    }
  }

  // This function finally draws the circle
  void drawCircle() {
    fill(255);
    ellipse(x, y, CIRCLESIZE, CIRCLESIZE);
  }
} // End of the MovingCircle class
