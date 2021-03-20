// Canvas and game variables
let canvasX = 400;
let canvasY = 400;
let fps = 30;

// Serial
let serial;

// Bar positioning
let margin = 10;
let barHeight = 40;
let barWidth = canvasX - 2 * margin;
let barX = margin;
let barY = (canvasY - barHeight) / 2;

// Bar percentage/rate of change
let barPercent = 0.30;
let barDelta = 0.0002;

// Stalling variables.
let chanceToStall = 0.01;   // Percent
let timeToStall = 180;       // Frames


// Other
let stalled = false;
let stalledCounter = 0;

function setup() {
  createCanvas(canvasX, canvasY);
  frameRate(fps);

  // serial shit
  serial = new p5.SerialPort();

  serial.list();
  serial.open('/dev/tty.usbmodem1411');

  /* Specify callback fcns for serial events */
  serial.on('connected', serverConnected);
  serial.on('list', gotList);
  serial.on('data', gotData);
  serial.on('error', gotError);
  serial.on('open', gotOpen);
  serial.on('close', gotClose);

}

function draw() {
  if(stalled) {
      ++stalledCounter;
      if(stalledCounter >= timeToStall) {
          stalled = false;
      }
      //console.log("hi");
      return;
  }

  if(random() < chanceToStall) {
    stalled = true;
    stalledCounter = 0;
  }

  barPercent += barDelta;
  barPercent = round(barPercent, 4) % 1;
  background("beige");
  
  textAlign("center");
  text("Downloading...", canvasX / 2, barHeight - 10);

  // Percent text
  text(barPercent, canvasX - 50, barY + barHeight + 2 * margin);

  // Bar and background
  fill("grey");
  rect(margin, (canvasY - barHeight) / 2, barWidth, barHeight);
  fill("green");
  rect(margin, (canvasY - barHeight) / 2, barWidth * barPercent, barHeight);
  
}

function stall() {
  
} 

function gotData() {
  let currentString = serial.readLine();
  trim(currentString);
  if (!currentString) return;
  //console.log(currentString);
  latestData = currentString;
}

function serverConnected() {
  print("Connected to Server");
}

function gotList(thelist) {
  print("List of Serial Ports:");

  for (let i = 0; i < thelist.length; i++) {
    print(i + " " + thelist[i]);
  }
}

function gotOpen() {
  print("Serial Port is Open");
}

function gotClose() {
  print("Serial Port is Closed");
  latestData = "Serial Port is Closed";
}

function gotError(theerror) {
  print(theerror);
}
