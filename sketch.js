var data;

function preload() {
  data = loadJSON("runs.json");
}

function setup() {
  createCanvas(400,600);
  var ipl = data.ipl;
  for (var i = 0; i < ipl.length; i++) {
    createElement('h1', ipl[i].Batsman);
    var highest = map(ipl[i].Runs, 0, 1000, 5, 100);
    var total = ipl[i].Runs;
    var speed = ipl[i].Strikerate;
    var name  = ipl[i].Batsman;

    var x = random(width);
    var y = random(height);

    ellipse(x,y,highest,highest);
    text(total,x,y);
  }
}
