var data;

function preload() {
  data = loadJSON("runs.json");
}

function setup() {
  var ipl = data.ipl;
  for (var i = 0; i < ipl.length; i++) {
    createElement('h1', ipl[i].Batsman);
    createElement('h2', ipl[i].Highest);
    var total = ipl[i].Runs;
    createElement('h2', total);
  }
}
