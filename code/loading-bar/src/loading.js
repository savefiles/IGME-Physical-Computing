// Canvas and game variables
let canvasX = window.innerWidth - 20;//400;
let canvasY = window.innerHeight - 20;//400;
let fps = 30;

// Serial
let serial;
let latestSound;
let latestKnock;
let latestKnock2;
let currentLum = 100;
let currentWalk = 100;

// Bar positioning
let margin = 10;
let barHeight = 40;
let barWidth = canvasX - 2 * margin;
let barX = margin;
let barY = (canvasY - barHeight) / 2;

// Bar percentage/rate of change
let barPercent = 0.00;
let barDelta = 0.0004;
let barTotalTime = (1/barDelta);
let barTimeRemaining = (1 - barPercent) * barTotalTime;

// Stalling variables.
let chanceToStall = 0.01;   // Percent
let timeToStall = fps * 60 ;       // Frames


// Other
let stalled = false;
let stalledCounter = 0;
let color = "";
let randomNum; 
let img;
let walkingimg;
let clapping;
let timer = false;
let timeNum = 0;

function preload() {
  img = loadImage('../images/cheering.png');
  walkingimg = loadImage('../images/walking.jpg');
  clapping = loadImage('../images/clapping.jpg');
}


function setup() {
  createCanvas(canvasX, canvasY);
  frameRate(fps);
  angleMode(DEGREES);

  // serial shit
  serial = new p5.SerialPort();

  serial.list();
  serial.open('COM4');

  /* Specify callback fcns for serial events */
  serial.on('connected', serverConnected);
  serial.on('list', gotList);
  serial.on('data', gotData);
  serial.on('error', gotError);
  serial.on('open', gotOpen);
  serial.on('close', gotClose);

  randomNum = random(0.60, 0.99);

  image(img, 0, 0);
  image(walkingimg, 0, 0);
  image(clapping, 0, 0);
  tint(255, 0);
}



//image.src = "https://www.pngjoy.com/pngm/315/5929841_cheering-crowd-transparent-cheering-group-clipart-hd-png.png";

function drawCrowd() {
  img.width = window.innerWidth - 20;
  img.height = window.innerHeight + 20;
  tint(225, currentLum);
  image(img, 0, window.innerHeight/3);
}

function drawWalking() {
  walkingimg.width = window.innerWidth - 20;
  walkingimg.height = window.innerHeight - 500;
  tint(225, currentWalk);
  image(walkingimg, 0, 0);
}


function draw() {

  if(timer == true)
  {
    timeNum++;
  }

  if(timeNum == 60)
  {
    barPercent = 0.00;
  }

  if(barPercent >= 0.99)
  {
    tint(225, 500);
    clapping.width = window.innerWidth - 20;
    clapping.height = window.innerHeight ;
    image(clapping, 0, 0);
    //noLoop();
    timer = true;
    image(clapping, 0, 0);
    return;
  }

  if(!latestSound) latestSound = 0;
  if(!latestKnock) latestKnock = 0;

  // Change background colors.
  let amt = 1/(fps * 2);
  currentLum = lerp(currentLum, latestSound, amt);
  currentWalk = lerp(currentWalk, latestKnock2, amt);
  color = `hsl(0, 100%, ${100 - (currentLum/3)}%)`;
  console.log(latestKnock2);
  background('rgb(34,139,34)');
  //tint(225, 0);
  drawCrowd();
  
  drawWalking();

  //console.log(color);
  //drawImage(image);

  // Stalling logic.
  if(stalled) {
    // Change time remaining and draw.
    let remainingFramesStalled = min(5184000, barTimeRemaining * pow(2.5, (floor(stalledCounter / (fps * 5)) + 1)));
    let barTimeRemainingDraw = convertToTime(remainingFramesStalled);
    textAlign("center");
    text(convertToTime(barTimeRemaining) + " remaining", canvasX / 2, canvasY - 4 * margin);

    // Check stall times
    if(latestKnock) {
      stalledCounter += 1000;
    }
    if(latestSound > 30) {
      stalledCounter += 100;
    }
    ++stalledCounter;
    if(stalledCounter >= timeToStall) {
        stalled = false;
    }
  }
  // Increase bar if it's not stalled.
  else{
    barPercent += barDelta;
    barPercent = round(barPercent, 4) % 1;

    // Draw time remaining
    barTimeRemaining = (1 - barPercent) * barTotalTime;
    textAlign("center");
    text(convertToTime(barTimeRemaining) + " remaining", canvasX / 2, canvasY - 4 * margin);

    // RNG if the bar should stall.
    if(random() < chanceToStall) {
      stalled = true;
      stalledCounter = 0;
      //let barTimeRemaining = (1 - barPercent) * barTotalTime;
    }
  }
  
  push();
  translate(-canvasX / 2 + canvasX * barPercent , 0);
  drawRunner(-(millis() % 240)/8, stalled);
  pop();
}
 

function gotData() {
  let currentString = serial.readLine();
  trim(currentString);
  if (!currentString) return;
  //parse data
  let data = currentString.split(",");
  latestSound = data[0];
  latestKnock = (data[1] > 100);
  latestKnock2 = data[1];
  //console.log(`Sound: ${latestSound}, Knock: ${latestKnock}`);
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

function convertToTime(frames) {
  let totalTimeSeconds = floor(frames / fps);
  let hours = floor(totalTimeSeconds / 3600);
  let minutes = floor((totalTimeSeconds % 3600) / 60);
  let seconds = totalTimeSeconds % 60;
  if(hours != 0) { 
    return `${hours} hours, ${minutes} minutes, ${seconds} seconds`;
  }
  else {
    if(minutes != 0) {
      return `${minutes} minutes, ${seconds} seconds`;
    }
    else {
      return `${seconds} seconds`;
    }
  }  
}

function drawRunner(angleChange, stalled) {
  let middleX = canvasX/2;
  let middleY = canvasY/2;
  let angle;
  if(stalled) { angleChange = 0; }

  push();
  noStroke();
  //translate(canvasX/2, canvasY/2);
  //rotate(30);
  fill(0);
 

  // Head, Body
  push();
  rectMode(CENTER);
  
  angle = 30;
  translate(middleX, middleY);
  rotate(angle);
  translate(-middleX , -middleY);

  rect(middleX, middleY, 30, 100, 10);
  circle(middleX, middleY - 75, 40);
  pop();

  // Arm 1 inner
  push();
  rectMode(CORNER);

  angle = 60 + angleChange;
  translate(middleX + 20, middleY - 45);
  rotate(angle);
  translate(-middleX - 20, -middleY + 45);
  rect(middleX + 20, middleY - 45, 60, 25, 20);

  angle = 50 + angleChange;
  translate(middleX + 55, middleY - 30);
  rotate(angle);
  translate(-middleX - 55, -middleY + 30);
  rect(middleX + 55, middleY - 30, 25, -60, 20);
  pop();

  // Arm 2 inner
  push();
  rectMode(CORNER);

  angle = 20 + angleChange;
  translate(middleX + 20, middleY - 45);
  rotate(angle);
  translate(-middleX - 20, -middleY + 45);
  rect(middleX + 20, middleY - 45, -60, 25, 20);

  angle = 30 + angleChange;
  translate(middleX - 15, middleY - 30);
  rotate(angle);
  translate(-middleX + 15, -middleY + 30);
  rect(middleX - 15, middleY - 30, -25, 60, 20);
  pop();


  // Leg 1 inner
  push();
  rectMode(CORNER);

  angle = 20 + angleChange;
  translate(middleX - 35, middleY + 25);
  rotate(angle);
  translate(-middleX + 35, -middleY - 25);
  rect(middleX - 35, middleY + 25, 80, 25, 20);

  angle = 00 + angleChange;
  translate(middleX + 40, middleY + 35);
  rotate(angle);
  translate(-middleX - 40, -middleY - 35);
  rect(middleX + 40, middleY + 35, -25, 60, 20);
  pop();


  // Leg 2 inner
  push();
  rectMode(CORNER);

  angle = 00 + angleChange;
  translate(middleX - 15, middleY + 30);
  rotate(angle);
  translate(-middleX + 15, -middleY - 30);
  rect(middleX - 15, middleY + 30, -60, 25, 20);

  angle = 00 + angleChange;
  translate(middleX - 55, middleY + 50);
  rotate(angle);
  translate(-middleX + 55, -middleY - 50);
  rect(middleX - 55, middleY + 50, -25, -60, 20);
  pop();
  


  pop();
}

Restart = () =>{
  background(color);
  textSize(50);
  textAlign("center");
  text("FIle Corrupted Refresh the page to try again", canvasX / 2, barY - 70);
  textSize(30);
  // Percent text
  text(`${(barPercent * 100).toFixed(1)}%`, canvasX - 50, barY + barHeight + 4 * margin);

  // Bar and background
  fill("grey");
  rect(margin, (canvasY - barHeight) / 2, barWidth, barHeight);
  fill("green");
  rect(margin, (canvasY - barHeight) / 2, barWidth * barPercent, barHeight);
}